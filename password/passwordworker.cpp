#include "passwordworker.h"

#include <categories/categorymanager.h>

// NOTE:Do not remove, update when format change
// version handling for backward compatibility
static const quint32 PASSWORD_MAGIC = 0x4E0E4E0E;
static const quint32 PASSWORD_VERSION = 1;

PasswordWorker::PasswordWorker(QObject *parent) : QObject(parent) {}

void PasswordWorker::addEntry(const PasswordEntry &entry) {
  QWriteLocker locker(&m_lock);
  auto entries = loadInternal();

  PasswordEntry newEntry = entry;

  if (newEntry.category.trimmed().isEmpty()) {
    newEntry.category = CategoryManager::instance().defaultCategory();
  }

  if (!newEntry.updatedAt.isValid()) {
    newEntry.updatedAt = newEntry.createdAt;
  }

  entries.append(newEntry);

  if (!saveInternal(entries)) {
    emit errorOccurred("Failed to save password entry");
  }

  emit passwordAdded(newEntry);
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

  if (!saveInternal(entries)) {
    emit deletionFailed("Save failed after deletion");
    return;
  }

  emit entryDeleted(id);
}

void PasswordWorker::updateEntry(const PasswordEntry &updatedEntry) {
  QWriteLocker locker(&m_lock);
  auto entries = loadInternal();

  bool found = false;
  for (auto &entry : entries) {
    if (entry.id == updatedEntry.id) {
      entry.title = updatedEntry.title;
      entry.username = updatedEntry.username;
      entry.password = updatedEntry.password;
      entry.updatedAt = QDateTime::currentDateTime();
      entry.url = updatedEntry.url;
      entry.notes = updatedEntry.notes;
      entry.category = updatedEntry.category;
      found = true;
      break;
    }
  }

  if (!found) {
    emit errorOccurred("Entry not found for update");
    return;
  }

  if (!saveInternal(entries)) {
    emit errorOccurred("Failed to save updated entry");
    return;
  }

  emit passwordUpdated(updatedEntry);
}

void PasswordWorker::loadEntries() {
  QReadLocker locker(&m_lock);
  emit entriesLoaded(loadInternal());
}

QVector<PasswordEntry> PasswordWorker::loadInternal() {
  QVector<PasswordEntry> entries;

  QByteArray key, iv;
  if (!SecurityManager::getSessionKey(key, iv)) {
    emit errorOccurred("Decryption key not available");
    return entries;
  }

  QFile file(SettingsManager::instance().getPasswordStorageFilePath());
  if (!file.open(QIODevice::ReadOnly)) {
    if (file.exists()) {
      emit errorOccurred("Failed to open password file: " + file.errorString());
    }
    return entries;
  }

  QDataStream in(&file);
  quint32 magic, version;
  in >> magic >> version;

  if (magic != PASSWORD_MAGIC) {
    emit errorOccurred("Invalid password file format");
    return entries;
  }

  while (!in.atEnd()) {
    QByteArray encrypted;
    in >> encrypted;

    QByteArray decryptedBytes = Encryptor::decrypt(encrypted, key, iv);
    QDataStream entryIn(&decryptedBytes, QIODevice::ReadOnly);

    PasswordEntry entry;
    entryIn >> entry.id >> entry.title >> entry.username >> entry.password >>
        entry.createdAt >> entry.updatedAt >> entry.url >> entry.notes >>
        entry.category;

    if (!entry.id.isNull() && !entry.title.isEmpty()) {
      entries.append(entry);
    }
  }

  return entries;
}

bool PasswordWorker::saveInternal(const QVector<PasswordEntry> &entries) {
  QByteArray key, iv;
  if (!SecurityManager::getSessionKey(key, iv)) {
    emit errorOccurred("Encryption key not available");
    return false;
  }

  QSaveFile file(SettingsManager::instance().getPasswordStorageFilePath());
  if (!file.open(QIODevice::WriteOnly)) {
    emit errorOccurred("Failed to open file for writing: " +
                       file.errorString());
    return false;
  }

  QDataStream out(&file);
  out << PASSWORD_MAGIC << PASSWORD_VERSION;

  for (const PasswordEntry &entry : entries) {
    QByteArray serialized;
    QDataStream entryOut(&serialized, QIODevice::WriteOnly);
    entryOut << entry.id << entry.title << entry.username << entry.password
             << entry.createdAt << entry.updatedAt << entry.url << entry.notes
             << entry.category;

    QByteArray encrypted = Encryptor::encrypt(serialized, key, iv);
    out << encrypted;
  }

  if (!file.commit()) {
    emit errorOccurred("Failed to save passwords: " + file.errorString());
    return false;
  }

  return true;
}
