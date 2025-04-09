#include "vault.h"

Vault::Vault(const QString &name) : m_name(name) {}

QString Vault::name() const { return m_name; }
