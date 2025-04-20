#ifndef NOTEWORKER_H
#define NOTEWORKER_H

#include "noteentry.h"

#include <QDataStream>
#include <QFile>
#include <QObject>
#include <QReadWriteLock>
#include <QSaveFile>
#include <QUuid>

struct NoteEntry;

Q_DECLARE_METATYPE(NoteEntry)
Q_DECLARE_METATYPE(QVector<NoteEntry>)

class NoteWorker : public QObject {
  Q_OBJECT
public:
  explicit NoteWorker(QObject *parent = nullptr);

public slots:
  void loadEntries();
  void addEntry(const NoteEntry &entry);
  void deleteEntry(QUuid id);
  void updateEntry(const NoteEntry &updatedEntry);

signals:
  void entriesLoaded(QVector<NoteEntry> entries);
  void errorOccurred(QString message);
  void deletionFailed(const QString &error);
  void entryDeleted(QUuid id);
  void noteAdded(NoteEntry entry);
  void noteUpdated(const NoteEntry &updatedEntry);

private:
  QReadWriteLock m_lock;

  QVector<NoteEntry> loadInternal();
  bool saveInternal(const QVector<NoteEntry> &entries);
};

#endif // NOTEWORKER_H
