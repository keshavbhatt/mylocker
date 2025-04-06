#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <security-manager/securitymanager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), passwordManager(nullptr) {

  ui->setupUi(this);
  setWindowTitle(APPLICATION_FULLNAME);

  loginScreen = new LoginScreen(this);
  connect(loginScreen, &LoginScreen::loginSuccessful, this,
          &MainWindow::showDashboard);
  ui->stackedWidget->addWidget(loginScreen);

  checkForLogout();

  autoLockManager = new AutoLockManager(this);
  connect(autoLockManager, &AutoLockManager::timeout, this,
          &MainWindow::lockApplication);
  autoLockManager->start();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showDashboard() {
  if (!dashboard) {
    dashboard = new Dashboard(this);

    connect(dashboard, &Dashboard::onLockNowClicked, this,
            &MainWindow::lockApplication);
    connect(dashboard, &Dashboard::openPasswordManager, this,
            &MainWindow::showPasswordManager);
    connect(dashboard, &Dashboard::openSecureNotes, this,
            &MainWindow::showNotesManager);

    if (autoLockManager)
      dashboard->setAutoLockManager(autoLockManager);

    ui->stackedWidget->addWidget(dashboard);
  }
  ui->stackedWidget->setCurrentWidget(dashboard);
}

void MainWindow::showPasswordManager() {
  if (!passwordManager) {
    passwordManager = new PasswordManager(this);
    connect(passwordManager, &PasswordManager::goToDashboard, this,
            &MainWindow::showDashboard);

    ui->stackedWidget->addWidget(passwordManager);
  }
  ui->stackedWidget->setCurrentWidget(passwordManager);
}

void MainWindow::showNotesManager() {
  if (!notesManager) {
    notesManager = new NotesManager(this);
    connect(notesManager, &NotesManager::goToDashboard, this,
            &MainWindow::showDashboard);

    ui->stackedWidget->addWidget(notesManager);
  }
  ui->stackedWidget->setCurrentWidget(notesManager);
}

void MainWindow::checkForLogout() {
  bool logoutRequired =
      SettingsManager::instance().getValue("security/logout_required").toBool();

  if (logoutRequired) {
    QMessageBox::information(
        this, "Security", "Master password has been set. Please log in again.");
    ui->stackedWidget->setCurrentWidget(loginScreen);
    SettingsManager::instance().setValue("security/logout_required", false);
  } else {
    ui->stackedWidget->setCurrentWidget(loginScreen);
  }
}

void MainWindow::lockApplication() {
  closeAllSecondaryWindows();
  SecurityManager::clearSessionKey();
  ui->stackedWidget->setCurrentWidget(loginScreen);
  autoLockManager->reset();
}

void MainWindow::closeAllSecondaryWindows() {
  const auto topLevels = QApplication::topLevelWidgets();
  for (QWidget *w : topLevels) {
    if (w->isVisible() && !w->property("isLoginScreen").toBool()) {
      if (qobject_cast<QDialog *>(w) || qobject_cast<QMenu *>(w)) {
        w->close(); // Close dialogs and menus
      } else if (w != this) {
        w->hide(); // Hide any extra windows
      }
    }
  }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseMove || event->type() == QEvent::KeyPress ||
      event->type() == QEvent::MouseButtonPress ||
      event->type() == QEvent::Wheel) {
    autoLockManager->reset();
  }
  return QMainWindow::eventFilter(obj, event);
}
