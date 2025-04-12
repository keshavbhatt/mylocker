#include "iconpickerdialog.h"

#include "iconloader.h"
#include <QColorDialog>
#include <QDebug>
#include <QFileInfo>
#include <QPushButton>
#include <QToolTip>
#include <QVBoxLayout>

IconPickerDialog::IconPickerDialog(QWidget *parent) : QDialog(parent) {
  m_allIcons = Utils::IconLoader::getAvailableIcons();

  m_selectedColor = m_settings.value("iconPicker/lastColor", QColor("#9fa0a4"))
                        .value<QColor>();

  setupUi();
  populateList(m_allIcons);
}

void IconPickerDialog::setupUi() {
  setWindowTitle("Select Icon");
  resize(600, 500);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  initListView();

  QHBoxLayout *searchBarLayout = new QHBoxLayout();

  // Search box
  m_searchBox = new QLineEdit(this);
  m_searchBox->setPlaceholderText(
      QString("Search from %1 icons...").arg(m_allIcons.count()));
  m_filterDebouncer = new Utils::Debouncer(500, this);
  connect(m_searchBox, &QLineEdit::textChanged, this, [this]() {
    m_filterDebouncer->debounce([this]() { applyFilter(m_searchBox->text()); });
  });
  searchBarLayout->addWidget(m_searchBox);

  m_colorPreview = new QLabel();
  m_colorPreview->setFixedSize(24, 24);
  updateColorPreview();
  searchBarLayout->addWidget(m_colorPreview);

  m_colorButton = new QPushButton("Choose Color");
  connect(m_colorButton, &QPushButton::clicked, this,
          &IconPickerDialog::chooseColor);
  searchBarLayout->addWidget(m_colorButton);

  mainLayout->addLayout(searchBarLayout);
  mainLayout->addWidget(m_listWidget);

  // OK/Cancel buttons
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch();

  m_okButton = new QPushButton("OK");
  m_okButton->setEnabled(false);
  QPushButton *cancelButton = new QPushButton("Cancel");
  connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  buttonsLayout->addWidget(m_okButton);
  buttonsLayout->addWidget(cancelButton);

  mainLayout->addLayout(buttonsLayout);

  m_searchBox->setFocus();
}

void IconPickerDialog::populateList(const QStringList &icons) {
  m_listWidget->clear();

  for (const QString &iconPath : icons) {
    auto iconName = QFileInfo(iconPath).baseName();
    QIcon coloredIcon = Utils::IconLoader::loadColoredIcon(
        iconName, m_selectedColor, QSize(64, 64));
    QListWidgetItem *item = new QListWidgetItem(coloredIcon, iconName);
    item->setToolTip(iconName);
    item->setData(Qt::UserRole, iconPath);
    item->setTextAlignment(Qt::AlignHCenter);
    m_listWidget->addItem(item);
  }
}

void IconPickerDialog::initListView() {
  m_listWidget = new QListWidget(this);
  m_listWidget->setViewMode(QListView::IconMode);
  m_listWidget->setResizeMode(QListView::Adjust);
  m_listWidget->setIconSize(QSize(48, 48));
  m_listWidget->setGridSize(QSize(80, 80));
  m_listWidget->setSpacing(12);
  m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_listWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

  m_listWidget->setUniformItemSizes(true);

  m_listWidget->setDragEnabled(false);
  m_listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
  m_listWidget->setDropIndicatorShown(false);
  m_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_listWidget->setWrapping(true);
  m_listWidget->setFlow(QListView::LeftToRight);
  m_listWidget->setWordWrap(true);
  m_listWidget->setBatchSize(200);

  connect(m_listWidget, &QListWidget::itemClicked, this,
          [this](QListWidgetItem *item) {
            m_selectedIcon = item->text();
            m_okButton->setEnabled(true);
          });

  connect(m_listWidget, &QListWidget::itemEntered, this,
          [=](QListWidgetItem *item) {
            const QString iconName = item->text();
            QToolTip::showText(QCursor::pos(), iconName);
          });
  m_listWidget->setMouseTracking(true);
}

void IconPickerDialog::chooseColor() {
  QColor color = QColorDialog::getColor(m_selectedColor, this);
  if (color.isValid()) {
    m_selectedColor = color;
    m_settings.setValue("iconPicker/lastColor", m_selectedColor);
    updateColorPreview();
    updateIconColors();
  }
}

void IconPickerDialog::updateIconColors() {
  for (int i = 0; i < m_listWidget->count(); ++i) {
    QListWidgetItem *item = m_listWidget->item(i);
    QString iconName = item->text();
    QIcon newIcon = Utils::IconLoader::loadColoredIcon(
        iconName, m_selectedColor, QSize(64, 64));
    item->setIcon(newIcon);
  }
}

void IconPickerDialog::applyFilter(const QString &text) {
  const QString filterText = text.trimmed();
  bool anyVisible = false;

  for (int i = 0; i < m_listWidget->count(); ++i) {
    QListWidgetItem *item = m_listWidget->item(i);
    bool match = item->text().contains(filterText, Qt::CaseInsensitive);
    item->setHidden(!match);
    if (match)
      anyVisible = true;
  }

  if (!anyVisible) {
    QToolTip::showText(
        m_searchBox->mapToGlobal(QPoint(0, m_searchBox->height())),
        "No icons found", m_searchBox);
  }
}

QString IconPickerDialog::selectedIconName() const { return m_selectedIcon; }

QColor IconPickerDialog::selectedColor() const { return m_selectedColor; }

void IconPickerDialog::setSelectedIcon(const QString &newSelectedIcon) {
  for (int i = 0; i < m_listWidget->count(); ++i) {
    QListWidgetItem *item = m_listWidget->item(i);
    if (item->text() == newSelectedIcon) {
      m_listWidget->setCurrentItem(item);
      m_selectedIcon = newSelectedIcon;
      return;
    }
  }

  qWarning() << "Icon not found in list:" << newSelectedIcon;
}

void IconPickerDialog::setSelectedColor(const QColor &newSelectedColor) {
  if (!newSelectedColor.isValid()) {
    qWarning() << "Invalid color provided to setSelectedColor()";
    return;
  }

  m_selectedColor = newSelectedColor;

  updateColorPreview();
  updateIconColors();
}

void IconPickerDialog::updateColorPreview() {
  QPixmap pix(24, 24);
  pix.fill(m_selectedColor);
  m_colorPreview->setPixmap(pix);
}
