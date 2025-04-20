#include "passwordstorage.h"

PasswordStorage &PasswordStorage::instance() {
  static PasswordStorage instance;
  return instance;
}

PasswordStorage::PasswordStorage() {
  static bool typesRegistered = false;
  if (!typesRegistered) {
    qRegisterMetaType<PasswordEntry>();
    qRegisterMetaType<QVector<PasswordEntry>>();
    typesRegistered = true;
  }

  m_worker = new PasswordWorker;
  m_workerThread = new QThread;

  m_worker->moveToThread(m_workerThread);

  connect(m_worker, &PasswordWorker::entriesLoaded, this,
          &PasswordStorage::passwordsLoaded, Qt::QueuedConnection);

  connect(m_worker, &PasswordWorker::errorOccurred, this,
          &PasswordStorage::operationFailed, Qt::QueuedConnection);

  connect(m_worker, &PasswordWorker::entryDeleted, this,
          &PasswordStorage::passwordDeleted, Qt::QueuedConnection);

  connect(m_worker, &PasswordWorker::deletionFailed, this,
          &PasswordStorage::deletionError, Qt::QueuedConnection);

  connect(m_worker, &PasswordWorker::passwordAdded, this,
          &PasswordStorage::passwordAdded, Qt::QueuedConnection);

  connect(m_worker, &PasswordWorker::passwordUpdated, this,
          &PasswordStorage::passwordUpdated, Qt::QueuedConnection);

  m_workerThread->start();
}

PasswordStorage::~PasswordStorage() {
  m_workerThread->quit();
  m_workerThread->wait();
  delete m_worker;
  delete m_workerThread;
}

void PasswordStorage::addPasswordEntry(const PasswordEntry &entry) {
  QMetaObject::invokeMethod(m_worker, "addEntry", Qt::QueuedConnection,
                            Q_ARG(PasswordEntry, entry));
}

void PasswordStorage::deletePasswordEntry(QUuid id) {
  QMetaObject::invokeMethod(m_worker, "deleteEntry", Qt::QueuedConnection,
                            Q_ARG(QUuid, id));
}

void PasswordStorage::updatePasswordEntry(const PasswordEntry &entry) {
  QMetaObject::invokeMethod(m_worker, "updateEntry", Qt::QueuedConnection,
                            Q_ARG(PasswordEntry, entry));
}

void PasswordStorage::loadPasswords() {
  QMetaObject::invokeMethod(m_worker, "loadEntries", Qt::QueuedConnection);
}
