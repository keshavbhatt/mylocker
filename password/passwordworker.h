#ifndef PASSWORDWORKER_H
#define PASSWORDWORKER_H

#include "encryptor/encryptor.h"
#include "passwordentry.h"
#include <security-manager/securitymanager.h>
#include <settings/settingsmanager.h>

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QReadWriteLock>
#include <QSaveFile>
#include <QStandardPaths>
#include <QToolButton>

struct PasswordEntry;

Q_DECLARE_METATYPE(PasswordEntry)
Q_DECLARE_METATYPE(QVector<PasswordEntry>)

class PasswordWorker : public QObject {
  Q_OBJECT
public:
  explicit PasswordWorker(QObject *parent = nullptr);

public slots:
  void loadEntries();
  void addEntry(const PasswordEntry &entry);
  void deleteEntry(QUuid id);
  void updateEntry(const PasswordEntry &updatedEntry);

signals:
  void entriesLoaded(QVector<PasswordEntry> entries);
  void errorOccurred(QString message);
  void deletionFailed(const QString &error);
  void entryDeleted(QUuid id);
  void passwordAdded(PasswordEntry entry);
  void passwordUpdated(const PasswordEntry &updatedEntry);

private:
  QReadWriteLock m_lock;

  QVector<PasswordEntry> loadInternal();
  bool saveInternal(const QVector<PasswordEntry> &entries);
};
#endif // PASSWORDWORKER_H
