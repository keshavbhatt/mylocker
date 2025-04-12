#ifndef VAULTCREATIONDIALOG_H
#define VAULTCREATIONDIALOG_H

#include <QColor>
#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;
class QLabel;

class VaultCreationDialog : public QDialog {
  Q_OBJECT

public:
  explicit VaultCreationDialog(QWidget *parent = nullptr);

  QString vaultName() const;
  QString selectedIcon() const;
  QColor selectedColor() const;

  void setVaultName(const QString &name);
  void setIcon(const QString &iconName);
  void setColor(const QColor &color);
  void setIconAndIconColor(const QColor &color, const QString &iconName);

private slots:
  void openIconPicker();

private:
  void updateIconPreview();

  QLineEdit *m_nameEdit;
  QPushButton *m_iconPickerButton;
  QLabel *m_iconPreviewLabel;

  QString m_selectedIcon;
  QColor m_selectedColor;
};

#endif // VAULTCREATIONDIALOG_H
