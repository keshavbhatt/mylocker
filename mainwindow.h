#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDirIterator>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

#include <dashboard/dashboard.h>
#include <icons/iconpickerdialog.h>
#include <login/autolockmanager.h>
#include <login/loginscreen.h>
#include <security-manager/securitymanager.h>
#include <settings/settingsmanager.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void showDashboard();
  void checkForLogout();
  void lockApplication();

private:
  Ui::MainWindow *ui;
  AutoLockManager *autoLockManager;
  LoginScreen *loginScreen = nullptr;

  Dashboard *dashboard = nullptr;
  void closeAllSecondaryWindows();
};

#endif // MAINWINDOW_H
