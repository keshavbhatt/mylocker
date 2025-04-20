#include "notesstorage.h"

#include <QThread>

NotesStorage &NotesStorage::instance() {
  static NotesStorage instance;
  return instance;
}

NotesStorage::NotesStorage() {
  static bool typesRegistered = false;
  if (!typesRegistered) {
    qRegisterMetaType<NoteEntry>();
    qRegisterMetaType<QVector<NoteEntry>>();
    typesRegistered = true;
  }

  m_worker = new NoteWorker;
  m_workerThread = new QThread;

  m_worker->moveToThread(m_workerThread);

  connect(m_worker, &NoteWorker::entriesLoaded, this,
          &NotesStorage::notesLoaded, Qt::QueuedConnection);

  connect(m_worker, &NoteWorker::errorOccurred, this,
          &NotesStorage::operationFailed, Qt::QueuedConnection);

  connect(m_worker, &NoteWorker::entryDeleted, this, &NotesStorage::noteDeleted,
          Qt::QueuedConnection);

  connect(m_worker, &NoteWorker::deletionFailed, this,
          &NotesStorage::deletionError, Qt::QueuedConnection);

  connect(m_worker, &NoteWorker::noteAdded, this, &NotesStorage::noteAdded,
          Qt::QueuedConnection);

  connect(m_worker, &NoteWorker::noteUpdated, this, &NotesStorage::noteUpdated,
          Qt::QueuedConnection);

  m_workerThread->start();
}

NotesStorage::~NotesStorage() {
  m_workerThread->quit();
  m_workerThread->wait();
  delete m_worker;
  delete m_workerThread;
}

void NotesStorage::addNoteEntry(const NoteEntry &entry) {
  QMetaObject::invokeMethod(m_worker, "addEntry", Qt::QueuedConnection,
                            Q_ARG(NoteEntry, entry));
}

void NotesStorage::deleteNoteEntry(QUuid id) {
  QMetaObject::invokeMethod(m_worker, "deleteEntry", Qt::QueuedConnection,
                            Q_ARG(QUuid, id));
}

void NotesStorage::updateNoteEntry(const NoteEntry &entry) {
  QMetaObject::invokeMethod(m_worker, "updateEntry", Qt::QueuedConnection,
                            Q_ARG(NoteEntry, entry));
}

void NotesStorage::loadNotes() {
  QMetaObject::invokeMethod(m_worker, "loadEntries", Qt::QueuedConnection);
}
