#ifndef VAULTSELECTIONDIALOG_H
#define VAULTSELECTIONDIALOG_H

#include "vaultlistwidget.h"

#include <QDialog>
#include <QString>
#include <QStringList>

class QListWidget;
class QPushButton;

class VaultSelectionDialog : public QDialog {
  Q_OBJECT

public:
  explicit VaultSelectionDialog(const QString &appDataDir,
                                QWidget *parent = nullptr);
  QString selectedVaultName() const;

private:
  VaultListWidget *vaultListWidget = nullptr;

  QString m_lockerDataDirPath;
  QPushButton *openButton;
  QPushButton *cancelButton;
};

#endif // VAULTSELECTIONDIALOG_H
