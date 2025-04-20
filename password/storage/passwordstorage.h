#ifndef PASSWORDSTORAGE_H
#define PASSWORDSTORAGE_H

#include <QObject>
#include <QThread>

#include <password/passwordworker.h>

class PasswordStorage : public QObject {
  Q_OBJECT
public:
  static PasswordStorage &instance();

  void addPasswordEntry(const PasswordEntry &entry);
  void deletePasswordEntry(QUuid id);
  void updatePasswordEntry(const PasswordEntry &entry);

  void loadPasswords();

signals:
  void passwordsLoaded(QVector<PasswordEntry> entries);
  void operationFailed(const QString &error);
  void passwordAdded(const PasswordEntry &newEntry);
  void passwordUpdated(const PasswordEntry &updatedEntry);
  void passwordDeleted(QUuid id);
  void deletionError(const QString &error);

private:
  PasswordStorage();
  ~PasswordStorage();

  PasswordWorker *m_worker;
  QThread *m_workerThread;
};

#endif // PASSWORDSTORAGE_H
