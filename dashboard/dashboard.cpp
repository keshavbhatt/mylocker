#include "dashboard.h"
#include "ui_dashboard.h"

#include <QDebug>
#include <vault/vaultmanager.h>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent), ui(new Ui::Dashboard) {
  ui->setupUi(this);

  ui->welcomeLabel->setText(
      QString("Opened Vault: %1")
          .arg(VaultManager::instance().currentVault().name()));

  connect(ui->passwordManagerButton, &QPushButton::clicked, this,
          &Dashboard::showPasswordManager);
  connect(ui->secureNotesButton, &QPushButton::clicked, this,
          &Dashboard::showNotesManager);
  connect(ui->lockNowButton, &QPushButton::clicked, this,
          &Dashboard::lockApplicationRequested);
}

Dashboard::~Dashboard() {
  qDebug() << "Deleted Dashboard";
  delete ui;
}

void Dashboard::setAutoLockManager(AutoLockManager *manager) {
  ui->autoLockTimeoutLabel->setVisible(manager != nullptr);
  connect(manager, &AutoLockManager::remainingTimeUpdated, this,
          &Dashboard::updateAutoLockLabel);
}

void Dashboard::updateAutoLockLabel(int seconds) {
  ui->autoLockTimeoutLabel->setText(QString("Auto-LogOut in %1 %2")
                                        .arg(seconds)
                                        .arg(seconds > 1 ? "secs" : "sec"));
}

void Dashboard::showPasswordManager() {
  if (!passwordManager) {
    passwordManager = new PasswordManager(this);
    passwordManager->layout()->setContentsMargins(0, 0, 0, 0);
    connect(passwordManager, &PasswordManager::goToDashboard, this,
            [=]() { ui->stackedWidget->setCurrentWidget(ui->mainPage); });

    ui->stackedWidget->addWidget(passwordManager);
  }
  ui->stackedWidget->setCurrentWidget(passwordManager);
}

void Dashboard::showNotesManager() {
  if (!notesManager) {
    notesManager = new NotesManager(this);
    notesManager->layout()->setContentsMargins(0, 0, 0, 0);
    connect(notesManager, &NotesManager::goToDashboard, this,
            [=]() { ui->stackedWidget->setCurrentWidget(ui->mainPage); });

    ui->stackedWidget->addWidget(notesManager);
  }
  ui->stackedWidget->setCurrentWidget(notesManager);
}
