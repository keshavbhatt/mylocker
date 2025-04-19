#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent) : QWidget(parent), ui(new Ui::Dashboard) {
  ui->setupUi(this);

  ui->welcomeLabel->setText(
      QString("Opened Vault: %1")
          .arg(VaultManager::instance().currentVault().name()));

  ui->passwordManagerButton->setIconSize(QSize(22, 22));
  ui->passwordManagerButton->setIcon(
      Utils::IconLoader::loadColoredIcon("key-2-fill", Palette::iconPrimary()));
  connect(ui->passwordManagerButton, &QPushButton::clicked, this,
          &Dashboard::showPasswordManager);

  ui->secureNotesButton->setIconSize(QSize(22, 22));
  ui->secureNotesButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "sticky-note-fill", Palette::iconWarning()));
  connect(ui->secureNotesButton, &QPushButton::clicked, this,
          &Dashboard::showNotesManager);

  ui->lockNowButton->setIconSize(QSize(22, 22));
  ui->lockNowButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "logout-box-fill", Palette::iconError()));
  connect(ui->lockNowButton, &QPushButton::clicked, this,
          &Dashboard::lockApplicationRequested);

  ui->stackedWidget->setCurrentWidget(ui->mainPage);

  installEventFilter(this);
}

bool Dashboard::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() == Qt::BackButton) {
      emit lockApplicationRequested();
      return true;
    }
  }
  return QWidget::eventFilter(watched, event);
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
            [=]() { ui->stackedWidget->slideInWgt(ui->mainPage); });

    ui->stackedWidget->addWidget(passwordManager);
  }
  ui->stackedWidget->slideInWgt(passwordManager);
}

void Dashboard::showNotesManager() {
  if (!notesManager) {
    notesManager = new NotesManager(this);
    notesManager->layout()->setContentsMargins(0, 0, 0, 0);
    connect(notesManager, &NotesManager::goToDashboard, this,
            [=]() { ui->stackedWidget->slideInWgt(ui->mainPage); });

    ui->stackedWidget->addWidget(notesManager);
  }
  ui->stackedWidget->slideInWgt(notesManager);
}
