#ifndef NOTESSTORAGE_H
#define NOTESSTORAGE_H

#include <QObject>
#include <notes/noteworker.h>

class NotesStorage : public QObject {
  Q_OBJECT
public:
  static NotesStorage &instance();

  void addNoteEntry(const NoteEntry &entry);
  void deleteNoteEntry(QUuid id);
  void updateNoteEntry(const NoteEntry &entry);

  void loadNotes();

signals:
  void notesLoaded(QVector<NoteEntry> entries);
  void operationFailed(const QString &error);
  void noteAdded(const NoteEntry &newEntry);
  void noteUpdated(const NoteEntry &updatedEntry);
  void noteDeleted(QUuid id);
  void deletionError(const QString &error);

private:
  NotesStorage();
  ~NotesStorage();

  NoteWorker *m_worker;
  QThread *m_workerThread;
};

#endif // NOTESSTORAGE_H
