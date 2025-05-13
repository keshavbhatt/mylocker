#include "synccontroller.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

SyncController::SyncController(const QString &lockerPath, QObject *parent)
    : QObject(parent), m_lockerPath(lockerPath) {
  loadSettings();

  if (m_syncEnabled)
    setupGitManager();
}

bool SyncController::isSyncEnabled() const { return m_syncEnabled; }

QString SyncController::remoteUrl() const { return m_remoteUrl; }

bool SyncController::enableSync(const QString &remoteUrl) {
  m_syncEnabled = true;
  m_remoteUrl = remoteUrl;
  saveSettings();
  setupGitManager();

  if (!m_gitSync->isGitRepo()) {
    if (!m_gitSync->initRepo())
      return false;

    if (!m_gitSync->addRemote(m_remoteUrl))
      return false;
  }

  return true;
}

bool SyncController::isGitRepo() const {
  if (m_gitSync) {
    return m_gitSync->isGitRepo();
  }
  return false;
}

void SyncController::disableSync(bool clearRemote) {
  m_syncEnabled = false;
  if (clearRemote) {
    m_remoteUrl.clear();
  }
  saveSettings();

  if (m_gitSync) {
    delete m_gitSync;
    m_gitSync = nullptr;
  }
}

bool SyncController::syncNow() {
  if (!m_syncEnabled || !m_gitSync) {
    emit syncError("Sync is not enabled.");
    return false;
  }

  if (!m_gitSync->isGitRepo()) {
    emit syncError(
        "Sync failed: locker is not initialized as a Git repository.");
    return false;
  }

  return m_gitSync->forceSync("MyLocker update");
}

void SyncController::loadSettings() {
  QSettings settings(m_lockerPath + QDir::separator() + ".settings.ini",
                     QSettings::IniFormat);

  m_syncEnabled = settings.value("sync/enabled", false).toBool();
  m_remoteUrl = settings.value("sync/remoteUrl").toString();
}

void SyncController::saveSettings() {
  QSettings settings(m_lockerPath + QDir::separator() + ".settings.ini",
                     QSettings::IniFormat);

  settings.setValue("sync/enabled", m_syncEnabled);
  settings.setValue("sync/remoteUrl", m_remoteUrl);
}

void SyncController::setupGitManager() {
  if (m_gitSync)
    return;

  m_gitSync = new GitSyncManager(m_lockerPath, this);

  connect(m_gitSync, &GitSyncManager::syncProgress, this,
          &SyncController::syncProgress);
  connect(m_gitSync, &GitSyncManager::syncError, this,
          &SyncController::syncError);
}
