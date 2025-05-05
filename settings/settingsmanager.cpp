#include "settingsmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <vault/vaultmanager.h>

SettingsManager::SettingsManager() { updateStoragePath(); }

SettingsManager::~SettingsManager() { delete settings; }

SettingsManager &SettingsManager::instance() {
  static SettingsManager instance;
  instance.updateStoragePath();
  return instance;
}

void SettingsManager::updateStoragePath() {

  QString newPath = VaultManager::instance().currentVault().lockerDirPath() +
                    QDir::separator() + ".settings.ini";

  if (storageFile != newPath) {
    storageFile = newPath;
    delete settings;
    settings = new QSettings(storageFile, QSettings::IniFormat);
  }
}

void SettingsManager::setValue(const QString &key, const QVariant &value) {
  QMutexLocker locker(&mutex);
  settings->setValue(key, value);
  settings->sync();
}

QVariant SettingsManager::getValue(const QString &key,
                                   const QVariant &defaultValue) const {
  QMutexLocker locker(&mutex);
  return settings->value(key, defaultValue);
}

void SettingsManager::setStorageFile(const QString &filePath) {
  QMutexLocker locker(&mutex);
  storageFile = filePath;
  delete settings;
  settings = new QSettings(storageFile, QSettings::IniFormat);
}

QString SettingsManager::getStorageFile() const {
  QMutexLocker locker(&mutex);
  return storageFile;
}

int SettingsManager::getClipboardClearDelay() const {
  return instance()
      .getValue("clipboard/clearDelay", 30000)
      .toInt(); // default 30 sec , 0 mean no autoclear
}

QString SettingsManager::getPasswordStorageFilePath() const {
  const QString baseDirPath = VaultManager::instance().currentVaultDir();
  QString fullPath = baseDirPath + QDir::separator() + "passwords.dat";
  return fullPath;
}

QString SettingsManager::getNoteStorageFilePath() const {
  const QString baseDirPath = VaultManager::instance().currentVaultDir();
  QString fullPath = baseDirPath + QDir::separator() + "notes.dat";
  return fullPath;
}
