#include "passwordworker.h"
#include "encryptor/encryptor.h"
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QStandardPaths>
#include <security-manager/securitymanager.h>
#include <settings/settingsmanager.h>

// NOTE:Do not remove, update when format change
// version handling for backward compatibility
static const quint32 CURRENT_VERSION = 1;

PasswordWorker::PasswordWorker(QObject *parent) : QObject(parent) {}

void PasswordWorker::addEntry(const PasswordEntry &entry) {
  QWriteLocker locker(&m_lock);

  auto entries = loadInternal();

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

  if (magic != 0x50A550A5) {
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

  auto passwordStorageFilePath =
      SettingsManager::instance().getPasswordStorageFilePath();
  QSaveFile file(passwordStorageFilePath);
  if (!file.open(QIODevice::WriteOnly)) {
    emit errorOccurred("Failed to open file for writing: " +
                       file.errorString());
    return false;
  }

  QDataStream out(&file);

  //  header with current version
  out << quint32(0x50A550A5) << quint32(1); // magic + version 1

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
