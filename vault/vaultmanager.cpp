#include "vaultmanager.h"

#include <QDir>
#include <QMessageBox>

#include <settings/settingsmanager.h>

VaultManager &VaultManager::instance() {
  static VaultManager instance;
  return instance;
}

bool VaultManager::openVault(const Vault &vault,
                             const QString lockerDataDirPath) {

  QDir dataDir(lockerDataDirPath);
  if (!dataDir.exists()) {
    if (!dataDir.mkpath(".")) {
      QMessageBox::critical(
          0, "Error", "Unable to Create/Open Application Data Directory.");
      return false;
    }
  }

  QDir vaultDir(dataDir.path() + QDir::separator() + vault.name());
  if (!vaultDir.exists()) {
    if (!vaultDir.mkpath(".")) {
      QMessageBox::critical(0, "Error",
                            "Unable to Create/Open Vault's Data Directory.");
      return false;
    }
  }

  m_lockerDataDirPath = lockerDataDirPath;
  m_currentVault = vault;

  if (hasOpenVault()) {
    QSettings settings;
    settings.setValue("lastOpenedVaultName", vault.name());
  }
  return true;
}

bool VaultManager::hasOpenVault() const {
  return m_currentVault.has_value() && !m_lockerDataDirPath.isEmpty();
}

const Vault &VaultManager::currentVault() const {
  if (!m_currentVault) {
    QMessageBox::warning(0, "Error", "No vault is currently open");
  }
  return *m_currentVault;
}

const QString VaultManager::currentVaultDir() const {
  if (!m_currentVault || !m_currentVault.has_value()) {
    QMessageBox::warning(0, "Error", "No vault is currently open");
  }
  return m_lockerDataDirPath + QDir::separator() + m_currentVault->name();
}

QString VaultManager::lockerDataDirPath() const {
  return m_lockerDataDirPath;
}
