#include "noteworker.h"

#include <security-manager/securitymanager.h>

#include <settings/settingsmanager.h>

#include <encryptor/encryptor.h>

#include <categories/categorymanager.h>

// NOTE:Do not remove, update when format changes
static const quint32 NOTE_MAGIC = 0x4E0E4E0E;
static const quint32 NOTE_VERSION = 1;

NoteWorker::NoteWorker(QObject *parent) : QObject(parent) {}

void NoteWorker::addEntry(const NoteEntry &entry) {
  QWriteLocker locker(&m_lock);
  auto entries = loadInternal();

  NoteEntry newEntry = entry;

  if (newEntry.category.trimmed().isEmpty()) {
    newEntry.category = CategoryManager::instance().defaultCategory();
  }

  if (!newEntry.updatedAt.isValid()) {
    newEntry.updatedAt = newEntry.createdAt;
  }

  entries.append(newEntry);

  if (!saveInternal(entries)) {
    emit errorOccurred("Failed to save note entry");
    return;
  }

  emit noteAdded(newEntry);
}

void NoteWorker::deleteEntry(QUuid id) {
  QWriteLocker locker(&m_lock);
  auto entries = loadInternal();

  auto it = std::remove_if(entries.begin(), entries.end(),
                           [&id](const NoteEntry &e) { return e.id == id; });
  if (it == entries.end()) {
    emit deletionFailed("Note not found");
    return;
  }

  entries.erase(it, entries.end());

  if (!saveInternal(entries)) {
    emit deletionFailed("Save failed after deletion");
    return;
  }

  emit entryDeleted(id);
}

void NoteWorker::updateEntry(const NoteEntry &updatedEntry) {
  QWriteLocker locker(&m_lock);
  auto entries = loadInternal();

  bool found = false;
  for (auto &entry : entries) {
    if (entry.id == updatedEntry.id) {
      entry.title = updatedEntry.title;
      entry.content = updatedEntry.content;
      entry.updatedAt = QDateTime::currentDateTime();
      entry.category = updatedEntry.category;
      found = true;
      break;
    }
  }

  if (!found) {
    emit errorOccurred("Note not found for update");
    return;
  }

  if (!saveInternal(entries)) {
    emit errorOccurred("Failed to save updated note");
    return;
  }

  emit noteUpdated(updatedEntry);
}

void NoteWorker::loadEntries() {
  QReadLocker locker(&m_lock);
  emit entriesLoaded(loadInternal());
}

QVector<NoteEntry> NoteWorker::loadInternal() {
  QVector<NoteEntry> entries;

  QByteArray key, iv;
  if (!SecurityManager::getSessionKey(key, iv)) {
    emit errorOccurred("Decryption key not available");
    return entries;
  }

  QFile file(SettingsManager::instance().getNoteStorageFilePath());
  if (!file.open(QIODevice::ReadOnly)) {
    if (file.exists()) {
      emit errorOccurred("Failed to open note file: " + file.errorString());
    }
    return entries;
  }

  QDataStream in(&file);
  quint32 magic, version;
  in >> magic >> version;

  if (magic != NOTE_MAGIC) {
    emit errorOccurred("Invalid note file format");
    return entries;
  }

  while (!in.atEnd()) {
    QByteArray encrypted;
    in >> encrypted;

    QByteArray decryptedBytes = Encryptor::decrypt(encrypted, key, iv);
    QDataStream entryIn(&decryptedBytes, QIODevice::ReadOnly);

    NoteEntry entry;
    entryIn >> entry.id >> entry.title >> entry.content >> entry.createdAt >>
        entry.updatedAt >> entry.category;

    if (!entry.id.isNull() && !entry.title.isEmpty()) {
      entries.append(entry);
    }
  }

  return entries;
}

bool NoteWorker::saveInternal(const QVector<NoteEntry> &entries) {
  QByteArray key, iv;
  if (!SecurityManager::getSessionKey(key, iv)) {
    emit errorOccurred("Encryption key not available");
    return false;
  }

  QSaveFile file(SettingsManager::instance().getNoteStorageFilePath());
  if (!file.open(QIODevice::WriteOnly)) {
    emit errorOccurred("Failed to open file for writing: " +
                       file.errorString());
    return false;
  }

  QDataStream out(&file);
  out << NOTE_MAGIC << NOTE_VERSION;

  for (const NoteEntry &entry : entries) {
    QByteArray serialized;
    QDataStream entryOut(&serialized, QIODevice::WriteOnly);
    entryOut << entry.id << entry.title << entry.content << entry.createdAt
             << entry.updatedAt << entry.category;

    QByteArray encrypted = Encryptor::encrypt(serialized, key, iv);
    out << encrypted;
  }

  if (!file.commit()) {
    emit errorOccurred("Failed to save notes: " + file.errorString());
    return false;
  }

  return true;
}
