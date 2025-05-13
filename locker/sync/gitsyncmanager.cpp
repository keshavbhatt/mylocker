#include "gitsyncmanager.h"

#include <QDebug>
#include <QDir>

#include <QProcess>

GitSyncManager::GitSyncManager(const QString &lockerPath, QObject *parent)
    : QObject(parent), m_lockerPath(lockerPath) {}

bool GitSyncManager::isGitRepo() const {
  QDir gitDir(m_lockerPath + "/.git");
  return gitDir.exists();
}

bool GitSyncManager::initRepo() {
  if (isGitRepo())
    return true;

  if (!runGitCommand({"init"}))
    return false;

  // Add .gitattributes
  QFile attrFile(m_lockerPath + QDir::separator() + ".gitattributes");
  if (attrFile.open(QIODevice::WriteOnly | QIODevice::Truncate |
                    QIODevice::Text)) {
    QTextStream out(&attrFile);
    // Treat vault data files as binary — prevent diff/merge conflicts
    out << "*.dat binary\n";
    // Prevent .vault.meta from being altered by text filters
    out << ".vault.meta binary\n";
    // Ensure .ini settings stay as text
    out << "*.ini text\n";
    // Disable automatic line ending conversion (important for vault files)
    out << "* -text\n";
    attrFile.close();

    runGitCommand({"add", ".gitattributes"});
    runGitCommand({"commit", "-m", "Add .gitattributes"});
  }

  return true;
}

bool GitSyncManager::addRemote(const QString &remoteUrl) {
  return runGitCommand({"remote", "add", "origin", remoteUrl});
}

bool GitSyncManager::forceSync(const QString &commitMessage) {
  emit syncProgress("Staging local changes...");
  if (!runGitCommand({"add", "."}))
    return false;

  emit syncProgress("Committing local changes...");
  if (!runGitCommand({"commit", "-m", commitMessage})) {
    qDebug() << "Nothing to commit (probably)";
  }

  emit syncProgress("Fetching latest changes...");
  if (!runGitCommand({"fetch"})) {
    emit syncError("Failed to fetch changes.");
    return false;
  }

  emit syncProgress("Checking if remote branch exists...");
  if (runGitCommand({"rev-parse", "--verify", "origin/main"})) {
    emit syncProgress("Merging remote branch...");
    if (!runGitCommand({"merge", "origin/main"})) {
      emit syncError("Failed to merge remote changes.");
      return false;
    }
  } else {
    emit syncProgress("Remote branch doesn't exist yet, skipping merge.");
  }

  emit syncProgress("Pushing changes...");
  if (!runGitCommand({"push", "origin", "main"}))
    return false;

  emit syncProgress("Sync completed!");
  return true;
}

QString GitSyncManager::lastCommitHash() {
  QString output;
  if (runGitCommand({"rev-parse", "HEAD"}, &output))
    return output.trimmed();
  return QString();
}

bool GitSyncManager::runGitCommand(const QStringList &args, QString *output) {
  QProcess git;
  git.setWorkingDirectory(m_lockerPath);
  git.start("git", args);
  bool finished = git.waitForFinished();

  if (!finished || git.exitStatus() != QProcess::NormalExit ||
      git.exitCode() != 0) {
    auto error = "Git failed: " + git.readAllStandardError();
    emit syncError(error);
    return false;
  }

  if (output)
    *output = QString::fromUtf8(git.readAllStandardOutput());
  return true;
}
