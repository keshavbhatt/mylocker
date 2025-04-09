#include "managelocker.h"
#include "ui_managelocker.h"

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

ManageLocker::ManageLocker(const QString &lockerDataDirPath, QWidget *parent)
    : QWidget(parent), m_lockerDataDirPath(lockerDataDirPath),
      ui(new Ui::ManageLocker) {
  ui->setupUi(this);

  vaultListWidget = new VaultListWidget(this);
  vaultListWidget->loadFromDirectory(m_lockerDataDirPath);
  ui->vaultsLayout->addWidget(vaultListWidget);

  connect(vaultListWidget, &VaultListWidget::vaultSelectionChanged, this,
          [=](const QString &vaultName) {
            this->updateVaultActionButtons(vaultName);
          });

  connect(ui->createNewVaultButton, &QPushButton::clicked, this,
          &ManageLocker::createNewVault);
  connect(ui->deleteSelectedVaultButton, &QPushButton::clicked, this,
          &ManageLocker::deleteSelectedVault);
}

void ManageLocker::updateVaultActionButtons(const QString &vaultName) {
  if (vaultName.trimmed().isEmpty()) {
    ui->deleteSelectedVaultButton->setEnabled(false);
  } else {
    ui->deleteSelectedVaultButton->setEnabled(true);
  }
}

ManageLocker::~ManageLocker() { delete ui; }

void ManageLocker::createNewVault() {
  bool ok;
  QString newVaultName = QInputDialog::getText(this, tr("Create New Vault"),
                                               tr("Enter vault name:"),
                                               QLineEdit::Normal, "", &ok);

  if (ok && !newVaultName.trimmed().isEmpty()) {
    QDir dir(m_lockerDataDirPath);
    QString newVaultPath = dir.filePath(newVaultName.trimmed());

    if (QDir(newVaultPath).exists()) {
      QMessageBox::warning(this, tr("Vault Exists"),
                           tr("A vault with this name already exists."));
      return;
    }

    if (dir.mkdir(newVaultName)) {
      vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
    } else {
      QMessageBox::critical(this, tr("Error"),
                            tr("Failed to create vault directory."));
    }
  }
}

void ManageLocker::deleteSelectedVault() {
  QString selectedVault = vaultListWidget->selectedVaultName();
  if (selectedVault.isEmpty()) {
    QMessageBox::warning(this, tr("No Vault Selected"),
                         tr("Please select a vault to delete."));
    return;
  }

  QMessageBox::StandardButton confirm = QMessageBox::question(
      this, tr("Delete Vault"),
      tr("Are you sure you want to delete the vault '%1'?").arg(selectedVault),
      QMessageBox::Yes | QMessageBox::No);

  if (confirm == QMessageBox::Yes) {
    QDir vaultDir(m_lockerDataDirPath + "/" + selectedVault);
    if (vaultDir.removeRecursively()) {
      vaultListWidget->loadFromDirectory(m_lockerDataDirPath); // Refresh list
    } else {
      QMessageBox::critical(this, tr("Error"),
                            tr("Failed to delete vault directory."));
    }
  }
}
