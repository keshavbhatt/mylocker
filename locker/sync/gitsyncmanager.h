#ifndef GITSYNCMANAGER_H
#define GITSYNCMANAGER_H

#include <QObject>
#include <QString>

class GitSyncManager : public QObject {
  Q_OBJECT
public:
  explicit GitSyncManager(const QString &lockerPath, QObject *parent = nullptr);

  bool isGitRepo() const;
  bool initRepo();
  bool addRemote(const QString &remoteUrl);
  bool forceSync(const QString &commitMessage = "Update");
  QString lastCommitHash();

signals:
  void syncProgress(const QString &message);
  void syncError(const QString &error);

private:
  QString m_lockerPath;
  bool runGitCommand(const QStringList &args, QString *output = nullptr);
};

#endif // GITSYNCMANAGER_H
