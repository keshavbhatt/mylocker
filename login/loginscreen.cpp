#include "loginscreen.h"
#include "ui_loginscreen.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <icons/iconloader.h>
#include <locker/managelocker.h>
#include <security-manager/securitymanager.h>
#include <theme/palette.h>
#include <vault/vaultmanager.h>

LoginScreen::LoginScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginScreen) {
  ui->setupUi(this);

  setProperty("isLoginScreen", true);

  resetUi();

  connect(ui->passwordInput, &QLineEdit::returnPressed, this,
          &LoginScreen::onUnlockClicked);

  ui->unlockButton->setIconSize(QSize(22, 22));
  ui->unlockButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "lock-unlock-fill", Palette::iconSuccess()));
  connect(ui->unlockButton, &QPushButton::clicked, this,
          &LoginScreen::onUnlockClicked);

  ui->manageLockerButton->setIconSize(QSize(22, 22));
  ui->manageLockerButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "folder-settings-fill", Palette::iconInfo()));
  connect(ui->manageLockerButton, &QPushButton::clicked, this,
          &LoginScreen::onManageLockerClicked);

  loadVaults();
}

LoginScreen::~LoginScreen() { delete ui; }

void LoginScreen::logout() {
  resetUi();
  loadVaults();
}

void LoginScreen::onUnlockClicked() {
  const QString password = ui->passwordInput->text().trimmed();
  if (password.isEmpty()) {
    showError("Please enter your master password.");
    return;
  }

  if (SecurityManager::validateMasterPassword(password)) {
    ui->passwordInput->clear();

    auto lockerDataDirPath =
        m_settings.value("lastOpenedLockerDirPath").toString();

    Vault newVault(lockerDataDirPath + QDir::separator() +
                   m_vaultListWidget->selectedVaultName());

    if (!VaultManager::instance().openVault(newVault)) {
      QMessageBox::critical(this, "Error", "Failed to open vault.");
      return;
    }

    QTimer::singleShot(200, this, [=]() { emit loginSuccessful(); });
  } else {
    showError("Invalid password. Try again.");
  }
}

void LoginScreen::onManageLockerClicked() {
  const QString password = ui->passwordInput->text().trimmed();

  if (password.isEmpty()) {
    showError("Please enter your master password.");
    return;
  }

  if (SecurityManager::validateMasterPassword(password)) {
    auto lockerDataDirPath =
        m_settings.value("lastOpenedLockerDirPath").toString();
    ui->passwordInput->clear();
    ui->errorLabel->hide();
    openManageLocker(lockerDataDirPath);
  } else {
    showError("Invalid password. Try again.");
  }
}

void LoginScreen::openManageLocker(const QString &lockerDataDirPath) {
  ManageLocker *manageLocker = new ManageLocker(lockerDataDirPath, this);
  manageLocker->setWindowTitle(QApplication::applicationName() +
                               " | Manage Locker");
  manageLocker->setAttribute(Qt::WA_DeleteOnClose);
  manageLocker->setWindowModality(Qt::ApplicationModal);
  manageLocker->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint |
                               Qt::WindowCloseButtonHint);
  manageLocker->resize(500, 300);

  connect(manageLocker, &QObject::destroyed, this,
          [=]() { this->loadVaults(); });

  manageLocker->show();
}

void LoginScreen::showError(const QString &message) {
  ui->errorLabel->setText(message);
  ui->errorLabel->setStyleSheet("color: red;");
  ui->errorLabel->show();
  ui->passwordInput->setFocus();
}

void LoginScreen::resetUi() {
  ui->errorLabel->hide();
  ui->passwordInput->setEnabled(false);
  ui->unlockButton->setEnabled(false);
  ui->passwordInput->clear();
  ui->vaultWidget->setVisible(false);
}

void LoginScreen::vaultSelectionChanged(const QString &selectedVaultName) {
  if (selectedVaultName.trimmed().isEmpty() == false) {
    ui->passwordInput->setEnabled(true);
    ui->unlockButton->setEnabled(true);
    ui->unlockButton->setVisible(true);
    ui->unlockButton->setText(QString("UnLock '%1'").arg(selectedVaultName));
    ui->passwordInput->setFocus();
  } else {
    ui->unlockButton->setVisible(false);
    ui->passwordInput->setEnabled(false);
    ui->unlockButton->setEnabled(false);
  }
}

void LoginScreen::loadVaults() {
  if (m_vaultListWidget == nullptr) {
    m_vaultListWidget = new VaultListWidget(this);
    ui->vaultWidget->layout()->addWidget(m_vaultListWidget);
    connect(m_vaultListWidget, &VaultListWidget::vaultSelectionChanged, this,
            &LoginScreen::vaultSelectionChanged);
  }
  auto lockerDataDirPath =
      m_settings.value("lastOpenedLockerDirPath").toString();

  m_vaultListWidget->loadFromDirectory(lockerDataDirPath);
  ui->vaultWidget->setVisible(true);

  Vault newVault(lockerDataDirPath + QDir::separator() +
                 m_vaultListWidget->selectedVaultName());

  // temporary open the vault to allow password varification
  if (!VaultManager::instance().openVault(newVault)) {
    QMessageBox::critical(this, "Error", "Failed to open vault.");
    return;
  }
}
