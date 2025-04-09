#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

#include <login/autolockmanager.h>

#include <password/passwordmanager.h>

#include <notes/notesmanager.h>

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget {
  Q_OBJECT

public:
  explicit Dashboard(QWidget *parent = nullptr);
  ~Dashboard();
  void setAutoLockManager(class AutoLockManager *manager);

signals:
  void lockApplicationRequested();

private:
  Ui::Dashboard *ui;
  PasswordManager *passwordManager = nullptr;
  NotesManager *notesManager = nullptr;

private slots:
  void updateAutoLockLabel(int seconds);
  void showPasswordManager();
  void showNotesManager();
};

#endif // DASHBOARD_H
