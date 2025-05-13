#ifndef SYNCCONTROLLER_H
#define SYNCCONTROLLER_H

#include "gitsyncmanager.h"

#include <QObject>
#include <QString>

class SyncController : public QObject {
  Q_OBJECT
public:
  explicit SyncController(const QString &lockerPath, QObject *parent = nullptr);

  bool isSyncEnabled() const;
  QString remoteUrl() const;

  bool enableSync(const QString &remoteUrl);
  void disableSync(bool clearRemote = true);

  bool syncNow();

  bool isGitRepo() const;

signals:
  void syncProgress(const QString &message);
  void syncError(const QString &error);

private:
  QString m_lockerPath;
  bool m_syncEnabled = false;
  QString m_remoteUrl;

  GitSyncManager *m_gitSync = nullptr;

  void loadSettings();
  void saveSettings();
  void setupGitManager();
};

#endif // SYNCCONTROLLER_H
