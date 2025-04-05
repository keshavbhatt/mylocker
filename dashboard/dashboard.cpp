#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent) : QWidget(parent), ui(new Ui::Dashboard) {
  ui->setupUi(this);

  ui->welcomeLabel->setText(APPLICATION_FULLNAME + QString(" Dashboard"));

  connect(ui->passwordManagerButton, &QPushButton::clicked, this,
          &Dashboard::openPasswordManager);
  connect(ui->secureNotesButton, &QPushButton::clicked, this,
          &Dashboard::openSecureNotes);
  connect(ui->lockNowButton, &QPushButton::clicked, this,
          &Dashboard::onLockNowClicked);
}

Dashboard::~Dashboard() { delete ui; }

void Dashboard::setAutoLockManager(AutoLockManager *manager) {
  ui->autoLockTimeoutLabel->setVisible(manager != nullptr);
  connect(manager, &AutoLockManager::remainingTimeUpdated, this,
          &Dashboard::updateAutoLockLabel);
}

void Dashboard::updateAutoLockLabel(int seconds) {
  ui->autoLockTimeoutLabel->setText(QString("Auto-lock in %1 %2")
                                        .arg(seconds)
                                        .arg(seconds > 1 ? "secs" : "sec"));
}
