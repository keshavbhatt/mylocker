#include "passwordworker.h"
#include "config.h"
#include "encryptor/encryptor.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QSaveFile>

PasswordWorker::PasswordWorker(QObject *parent) : QObject(parent) {}

void PasswordWorker::addEntry(const QString &title, const QString &username,
                              const QString &password) {
  QWriteLocker locker(&m_lock);

  auto entries = loadInternal();

  PasswordEntry entry;
  entry.title = title;
  entry.username = username;
  entry.password = password;
  entry.timestamp = QDateTime::currentDateTime();
  entries.append(entry);

  if (!saveInternal(entries)) {
    emit errorOccurred("Failed to save password entry");
  }

  emit passwordAdded(entry);
}

void PasswordWorker::deleteEntry(QUuid id) {
  QWriteLocker locker(&m_lock);

  auto entries = loadInternal();

  auto it =
      std::remove_if(entries.begin(), entries.end(),
                     [&id](const PasswordEntry &e) { return e.id == id; });

  if (it == entries.end()) {
    emit deletionFailed("Entry not found");
    return;
  }
  entries.erase(it, entries.end());

  if (saveInternal(entries)) {
    emit entryDeleted(id);
  } else {
    emit deletionFailed("Save failed after deletion");
  }
}

void PasswordWorker::loadEntries() {
  QReadLocker locker(&m_lock);
  emit entriesLoaded(loadInternal());
}

QVector<PasswordEntry> PasswordWorker::loadInternal() {
  QVector<PasswordEntry> entries;

  QFile file(Config::STORAGE_FILE);
  if (!file.open(QIODevice::ReadOnly)) {
    if (file.exists()) {
      emit errorOccurred("Failed to open password file: " + file.errorString());
    }
    return entries;
  }

  QDataStream in(&file);
  quint32 magic, version;
  in >> magic >> version;

  if (magic != 0x50A550A5) {
    emit errorOccurred("Invalid password file format");
    return entries;
  }

  // version handling for backward compatibility
  const quint32 CURRENT_VERSION =
      1; // NOTE:Do not remove, update when format change

  while (!in.atEnd()) {
    PasswordEntry entry;
    QByteArray encrypted;

    // base fields
    in >> entry.id >> entry.title >> entry.username >> encrypted >>
        entry.timestamp;

    // extended fields for version 1+
    if (version >= 1) {
      in >> entry.url >> entry.notes >> entry.category >> entry.flags;
    }

    entry.password = Encryptor::decrypt(encrypted, Config::ENCRYPTION_KEY,
                                        Config::ENCRYPTION_IV);

    // minimal required fields
    if (!entry.id.isNull() && !entry.title.isEmpty()) {
      entries.append(entry);
    } else {
      qWarning() << "Skipping invalid entry with empty title or null ID";
    }
  }

  return entries;
}

bool PasswordWorker::saveInternal(const QVector<PasswordEntry> &entries) {
  QSaveFile file(Config::STORAGE_FILE);
  if (!file.open(QIODevice::WriteOnly)) {
    emit errorOccurred("Failed to open file for writing: " +
                       file.errorString());
    return false;
  }

  QDataStream out(&file);

  //  header with current version
  out << quint32(0x50A550A5) << quint32(1); // magic + version 1

  for (const PasswordEntry &entry : entries) {

    QByteArray encrypted = Encryptor::encrypt(
        entry.password.toUtf8(), Config::ENCRYPTION_KEY, Config::ENCRYPTION_IV);

    out << entry.id << entry.title << entry.username << encrypted
        << entry.timestamp;

    out << entry.url << entry.notes << entry.category << entry.flags;
  }

  if (!file.commit()) {
    emit errorOccurred("Failed to save passwords: " + file.errorString());
    return false;
  }

  return true;
}
