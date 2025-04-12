#ifndef VAULTLISTWIDGET_H
#define VAULTLISTWIDGET_H

#include <QLabel>
#include <QListWidget>
#include <QString>
#include <QWidget>

class VaultListWidget : public QWidget {
  Q_OBJECT

signals:
  void vaultSelectionChanged(const QString &vaultName);

public:
  explicit VaultListWidget(QWidget *parent = nullptr,
                           const bool &embded = false);

  void loadFromDirectory(const QString &directoryPath);
  QString selectedVaultName() const;
  QLabel *m_vaultLabel = nullptr;

  bool hasItemSelected();
  private:
  QListWidget *vaultListWidget;
  bool m_isEmbeded;
  void selectCurrentListItemFromSettings();
};
#endif // VAULTLISTWIDGET_H
