#ifndef MANAGELOCKER_H
#define MANAGELOCKER_H

#include <QWidget>

#include "vaultcreationdialog.h"
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <icons/iconloader.h>
#include <icons/iconpickerdialog.h>
#include <theme/palette.h>
#include <vault/vaultlistwidget.h>

namespace Ui {
class ManageLocker;
}

class ManageLocker : public QWidget {
  Q_OBJECT

public:
  explicit ManageLocker(const QString &lockerDataDirPath,
                        QWidget *parent = nullptr);
  ~ManageLocker();

private slots:
  void deleteSelectedVault();
  void createNewVault();
  void editSelectedVault();
  void updateVaultActionButtons();

private:
  QString m_lockerDataDirPath;
  Ui::ManageLocker *ui;

  VaultListWidget *vaultListWidget = nullptr;
};

#endif // MANAGELOCKER_H
