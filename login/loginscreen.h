#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QSettings>
#include <QWidget>

#include <vault/vaultlistwidget.h>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget {
  Q_OBJECT

public:
  explicit LoginScreen(QWidget *parent = nullptr);
  ~LoginScreen();
  void logout();

signals:
  void loginSuccessful();

private slots:
  void onUnlockClicked();
  void onManageLockerClicked();
  void vaultSelectionChanged(const QString &selectedVaultName);

private:
  Ui::LoginScreen *ui;
  void showError(const QString &message);
  void resetUi();
  void loadVaults();
  QSettings m_settings;
  VaultListWidget *m_vaultListWidget = nullptr;
  void openManageLocker(const QString &lockerDataDirPath);
};

#endif // LOGINSCREEN_H
