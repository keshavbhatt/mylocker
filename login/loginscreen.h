#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget {
  Q_OBJECT

public:
  explicit LoginScreen(QWidget *parent = nullptr);
  ~LoginScreen();

signals:
  void loginSuccessful();

private slots:
  void onUnlockClicked();

private:
  Ui::LoginScreen *ui;
  void showError(const QString &message);
  void resetUi();
};

#endif // LOGINSCREEN_H
