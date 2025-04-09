#ifndef VAULT_H
#define VAULT_H

#include <QString>

class Vault {
public:
  Vault(const QString &name);

  QString name() const;

private:
  QString m_name;
};
#endif // VAULT_H
