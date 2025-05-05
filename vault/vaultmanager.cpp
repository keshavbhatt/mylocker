#include "vaultmanager.h"

#include <QDir>
#include <QMessageBox>

#include <settings/settingsmanager.h>

VaultManager &VaultManager::instance() {
  static VaultManager instance;
  return instance;
}

bool VaultManager::openVault(const Vault &vault) {
  QDir vaultDir(vault.path());
  if (!vaultDir.exists()) {
    if (!vaultDir.mkpath(".")) {
      QMessageBox::critical(nullptr, "Error",
                            "Unable to Create/Open Vault Directory.");
      return false;
    }
  }

  m_currentVault = vault;

  if (hasOpenVault()) {
    QSettings settings;
    settings.setValue("lastOpenedVaultName", vault.name());
  }
  return true;
}

bool VaultManager::hasOpenVault() const { return m_currentVault.has_value(); }

const Vault &VaultManager::currentVault() const {
  if (!m_currentVault) {
    QMessageBox::warning(nullptr, "Error", "No vault is currently open");
  }
  return *m_currentVault;
}

const QString VaultManager::currentVaultDir() const {
  if (!m_currentVault) {
    QMessageBox::warning(nullptr, "Error", "No vault is currently open");
    return QString();
  }
  return m_currentVault->path();
}
