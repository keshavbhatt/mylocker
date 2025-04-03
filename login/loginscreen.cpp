#include "loginscreen.h"
#include "ui_loginscreen.h"

#include <QMessageBox>

#include <security-manager/securitymanager.h>

LoginScreen::LoginScreen(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginScreen) {
  ui->setupUi(this);
  resetUi();

  connect(ui->passwordInput, &QLineEdit::returnPressed, this,
          &LoginScreen::onUnlockClicked);
  connect(ui->unlockButton, &QPushButton::clicked, this,
          &LoginScreen::onUnlockClicked);
}

LoginScreen::~LoginScreen() { delete ui; }

void LoginScreen::onUnlockClicked() {
  QString password = ui->passwordInput->text().trimmed();

  if (password.isEmpty()) {
    showError("Please enter your master password.");
    return;
  }

  if (SecurityManager::validateMasterPassword(password)) {
    resetUi();
    emit loginSuccessful();
  } else {
    showError("Invalid password. Try again.");
  }
}

void LoginScreen::showError(const QString &message) {
  ui->errorLabel->setText(message);
  ui->errorLabel->setStyleSheet("color: red;");
  ui->errorLabel->show();
}

void LoginScreen::resetUi() {
  ui->errorLabel->hide();
  ui->passwordInput->clear();
  ui->passwordInput->setFocus();
}
