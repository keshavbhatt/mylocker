#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QTimer>

#include <dashboard/dashboard.h>
#include <login/autolockmanager.h>
#include <login/loginscreen.h>
#include <notes/notesmanager.h>
#include <password/passwordmanager.h>
#include <settings/settingsmanager.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void showDashboard();
  void checkForLogout();
  void showPasswordManager();
  void showNotesManager();
  void lockApplication();

private:
  Ui::MainWindow *ui;
  AutoLockManager *autoLockManager;
  LoginScreen *loginScreen = nullptr;
  PasswordManager *passwordManager = nullptr;
  Dashboard *dashboard = nullptr;
  NotesManager *notesManager = nullptr;
  void closeAllSecondaryWindows();
};

#endif // MAINWINDOW_H
