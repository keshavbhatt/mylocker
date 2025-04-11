#ifndef ICONPICKERDIALOG_H
#define ICONPICKERDIALOG_H

#include <QColor>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QScrollArea>
#include <QSettings>
#include <QStringList>

#include <utils/debouncer.h>

class IconPickerDialog : public QDialog {
  Q_OBJECT

public:
  explicit IconPickerDialog(QWidget *parent = nullptr);

  QString selectedIconName() const;
  QColor selectedColor() const;

private slots:
  void chooseColor();
  void applyFilter(const QString &text);

private:
  void setupUi();
  void populateList(const QStringList &icons);
  void initListView();
  void updateIconColors();

  QColor m_selectedColor;
  QString m_selectedIcon;
  QStringList m_allIcons;
  QSettings m_settings;

  QListWidget *m_listWidget;
  QLabel *m_colorPreview;
  QPushButton *m_colorButton;
  QPushButton *m_okButton;
  Utils::Debouncer *m_filterDebouncer;
  QLineEdit *m_searchBox;
};

#endif // ICONPICKERDIALOG_H
