#ifndef PASSWORDWORKER_H
#define PASSWORDWORKER_H

#include <QDateTime>
#include <QObject>
#include <QReadWriteLock>
#include <QUuid>
#include <QVector>

struct PasswordEntry;

Q_DECLARE_METATYPE(PasswordEntry)
Q_DECLARE_METATYPE(QVector<PasswordEntry>)

struct PasswordEntry {
  // basic fields
  QUuid id;
  QString title;
  QString username;
  QString password;
  QDateTime timestamp;

  // extended fields
  QString url;
  QString notes;
  uint32_t category = 0;
  uint32_t flags = 0;

  PasswordEntry() : id(QUuid::createUuid()) {}
};

class PasswordWorker : public QObject {
  Q_OBJECT
public:
  explicit PasswordWorker(QObject *parent = nullptr);

public slots:
  void addEntry(const QString &title, const QString &username,
                const QString &password);
  void loadEntries();

  void deleteEntry(QUuid id);
signals:
  void entriesLoaded(QVector<PasswordEntry> entries);
  void errorOccurred(QString message);
  void entryDeleted(QUuid id);
  void deletionFailed(const QString &error);
  void passwordAdded(PasswordEntry entry);

private:
  QReadWriteLock m_lock;

  QVector<PasswordEntry> loadInternal();
  bool saveInternal(const QVector<PasswordEntry> &entries);
};
#endif // PASSWORDWORKER_H
