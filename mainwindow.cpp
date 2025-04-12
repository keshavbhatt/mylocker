#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>

#include <security-manager/securitymanager.h>

#include <icons/iconpickerdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

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

    connect(dashboard, &Dashboard::lockApplicationRequested, this,
            &MainWindow::lockApplication);

    if (autoLockManager)
      dashboard->setAutoLockManager(autoLockManager);

    ui->stackedWidget->addWidget(dashboard);
  }
  ui->stackedWidget->slideInWgt(dashboard);
}

void MainWindow::checkForLogout() {

  QSettings settings;
  bool logoutRequired = settings.value("security/logout_required").toBool();

  if (logoutRequired) {
    QMessageBox::information(
        this, "Security", "Master password has been set. Please log in again.");
    ui->stackedWidget->slideInWgt(loginScreen);
    settings.setValue("security/logout_required", false);
  } else {
    ui->stackedWidget->slideInWgt(loginScreen);
  }
}

void MainWindow::lockApplication() {

  SecurityManager::clearSessionKey();

  closeAllSecondaryWindows();

  QTimer::singleShot(ui->stackedWidget->speed(), this, [this]() {
    loginScreen->logout();
    if (dashboard) {
      ui->stackedWidget->removeWidget(dashboard);
      delete dashboard;
      dashboard = nullptr;
    }
  });

  ui->stackedWidget->slideInWgt(loginScreen);

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
