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
  explicit VaultListWidget(QWidget *parent = nullptr);

  void loadFromDirectory(const QString &directoryPath);
  QString selectedVaultName() const;
  QLabel *m_vaultLabel = nullptr;

private:
  QListWidget *vaultListWidget;
};
#endif // VAULTLISTWIDGET_H
