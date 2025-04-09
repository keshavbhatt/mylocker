#include "vaultselectiondialog.h"

#include <QDir>
#include <QFileInfoList>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

VaultSelectionDialog::VaultSelectionDialog(const QString &appDataDir,
                                           QWidget *parent)
    : QDialog(parent), m_lockerDataDirPath(appDataDir) {
  setWindowTitle("Select a Vault");
  resize(400, 300);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  vaultListWidget = new VaultListWidget(this);
  vaultListWidget->loadFromDirectory(m_lockerDataDirPath);
  mainLayout->addWidget(vaultListWidget);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  openButton = new QPushButton("Open", this);
  cancelButton = new QPushButton("Cancel", this);
  buttonLayout->addStretch();
  buttonLayout->addWidget(openButton);
  buttonLayout->addWidget(cancelButton);
  mainLayout->addLayout(buttonLayout);

  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(openButton, &QPushButton::clicked, this, [=]() {
    if (vaultListWidget->selectedVaultName().isEmpty()) {
      QMessageBox::warning(this, "No Vault Selected",
                           "Please select a vault to open.");
      return;
    }
    accept();
  });
}

QString VaultSelectionDialog::selectedVaultName() const {
  return vaultListWidget->selectedVaultName();
}
