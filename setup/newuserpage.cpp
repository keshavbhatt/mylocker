#include "newuserpage.h"
#include "ui_newuserpage.h"

#include <security-manager/securitymanager.h>

#include <QAction>
#include <QFileDialog>
#include <QHelpEvent>
#include <QMessageBox>
#include <QSettings>
#include <QToolTip>

#include <vault/vault.h>
#include <vault/vaultmanager.h>

#include <help/whatsthishoverfilter.h>

NewUserPage::NewUserPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::NewUserPage) {
  ui->setupUi(this);

  WhatsThisHoverFilter *hoverFilter = new WhatsThisHoverFilter(this);
  WhatsThisHoverFilter::installWhatsThisHoverFilter(this, hoverFilter);

  connect(ui->backToWelcomePageBtn, &QPushButton::clicked, this,
          [=]() { emit backToWelcomePageRequested(); });

  QAction *selectNewDataDirectoryAction = new QAction(this);
  selectNewDataDirectoryAction->setText("Click to Select new Locker Directory");
  selectNewDataDirectoryAction->setIcon(QIcon(":/icons/folder-open-line.png"));

  ui->newDataDirectoryLineEdit->installEventFilter(this);
  ui->newDataDirectoryLineEdit->setReadOnly(true);
  ui->newDataDirectoryLineEdit->addAction(selectNewDataDirectoryAction,
                                          QLineEdit::TrailingPosition);
  connect(selectNewDataDirectoryAction, &QAction::triggered, this,
          &NewUserPage::selectNewDataDirectoryActionClicked);

  connect(ui->confirmMasterPasswordLineEdit, &QLineEdit::returnPressed, this,
          &NewUserPage::validateAndContinueSetup);

  connect(ui->continuePushButton, &QPushButton::clicked, this,
          &NewUserPage::validateAndContinueSetup);
}

bool NewUserPage::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->newDataDirectoryLineEdit &&
      event->type() == QEvent::MouseButtonPress) {
    selectNewDataDirectoryActionClicked();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

NewUserPage::~NewUserPage() { delete ui; }

void NewUserPage::selectNewDataDirectoryActionClicked() {
  QString dir = QFileDialog::getExistingDirectory(
      this, tr("Select an empty Directory for new Locker"), QDir::homePath(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (!dir.isEmpty()) {
    QDir selectedDir(dir);
    QStringList contents =
        selectedDir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    if (!contents.isEmpty()) {
      QMessageBox::warning(this, tr("Directory Not Empty"),
                           tr("The selected directory is not empty. Please "
                              "select an empty directory to continue."));
      return;
    }

    ui->newDataDirectoryLineEdit->setText(dir);
  }
}

void NewUserPage::validateAndContinueSetup() {
  QString newDataDirectoryPath = ui->newDataDirectoryLineEdit->text();
  QString newVaultName = ui->newVaultNameLineEdit->text().trimmed();
  QString password = ui->masterPasswordLineEdit->text();
  QString confirmPassword = ui->confirmMasterPasswordLineEdit->text();

  // Validate directory
  if (newDataDirectoryPath.isEmpty()) {
    QMessageBox::warning(this, "Error",
                         "Please select a directory to store new Locker data.");
    return;
  }

  // Validate vault name
  if (newVaultName.isEmpty()) {
    QMessageBox::warning(this, "Error",
                         "Please enter a name for your initial Vault.");
    return;
  }

  // Validate password
  if (password.length() < 6) {
    QMessageBox::warning(this, "Error",
                         "Master Password must be at least 6 characters.");
    return;
  }

  if (password != confirmPassword) {
    QMessageBox::warning(this, "Error", "Passwords do not match.");
    return;
  }

  // Create Vault
  Vault newVault(newVaultName);
  if (!VaultManager::instance().openVault(newVault, newDataDirectoryPath)) {
    QMessageBox::critical(this, "Error",
                          QString("Failed to open Vault " + newVaultName));
    return;
  }

  // Save master password
  SecurityManager::setMasterPassword(password);

  // Path was created by VaultManager, save it
  QSettings settings;
  settings.setValue("lockerDataDirPath", newDataDirectoryPath);

  emit setupComplete();
}
