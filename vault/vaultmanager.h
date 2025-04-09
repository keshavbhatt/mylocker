#ifndef VAULTMANAGER_H
#define VAULTMANAGER_H

#include "vault.h"
#include <optional>

class VaultManager {
public:
  static VaultManager &instance();

  bool openVault(const Vault &vault, const QString lockerDataDirPath);
  bool hasOpenVault() const;

  const Vault &currentVault() const;
  const QString currentVaultDir() const;

  QString lockerDataDirPath() const;

private:
  VaultManager() = default;

  std::optional<Vault> m_currentVault;
  QString m_lockerDataDirPath;
};
#endif // VAULTMANAGER_H
