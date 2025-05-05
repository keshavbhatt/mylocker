#include "vaultcreationdialog.h"

#include "theme/palette.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <icons/iconloader.h>
#include <icons/iconpickerdialog.h>

#include <vault/vault.h>

VaultCreationDialog::VaultCreationDialog(QWidget *parent)
    : QDialog(parent), m_selectedColor(Palette::iconSuccess()) {

  setWindowTitle("Create New Vault");

  m_nameEdit = new QLineEdit(this);
  m_nameEdit->setPlaceholderText("Enter vault name");

  m_iconPickerButton = new QPushButton("Choose Icon", this);
  m_iconPickerButton->setIconSize(QSize(22, 22));
  m_iconPickerButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "edit-2-fill", Palette::iconWarning()));
  connect(m_iconPickerButton, &QPushButton::clicked, this,
          &VaultCreationDialog::openIconPicker);

  m_iconPreviewLabel = new QLabel(this);
  m_iconPreviewLabel->setFixedSize(32, 32);

  m_selectedColor = Vault::defaultColor(); // default icon color
  m_selectedIcon = Vault::defaultIcon();   // default icon
  updateIconPreview();                     // default blank preview

  auto formLayout = new QVBoxLayout;
  formLayout->addWidget(new QLabel("Vault Name:"));
  formLayout->addWidget(m_nameEdit);

  formLayout->addWidget(new QLabel("Vault Icon:"));
  auto iconRow = new QHBoxLayout;
  iconRow->addWidget(m_iconPickerButton);
  iconRow->addWidget(m_iconPreviewLabel);
  formLayout->addLayout(iconRow);

  auto buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  formLayout->addWidget(buttonBox);

  setLayout(formLayout);
}

QString VaultCreationDialog::vaultName() const {
  return m_nameEdit->text().trimmed();
}

QString VaultCreationDialog::selectedIcon() const { return m_selectedIcon; }

QColor VaultCreationDialog::selectedColor() const { return m_selectedColor; }

void VaultCreationDialog::setVaultName(const QString &name) {
  m_nameEdit->setText(name);
}

void VaultCreationDialog::setIcon(const QString &iconName) {
  m_selectedIcon = iconName;
  updateIconPreview();
}

void VaultCreationDialog::setColor(const QColor &color) {
  m_selectedColor = color;
  updateIconPreview();
}

void VaultCreationDialog::setIconAndIconColor(const QColor &color,
                                              const QString &iconName) {
  m_selectedColor = color;
  m_selectedIcon = iconName;

  updateIconPreview();
}

void VaultCreationDialog::openIconPicker() {
  IconPickerDialog picker(this);
  picker.setSelectedIcon(m_selectedIcon);
  picker.setSelectedColor(m_selectedColor);

  if (picker.exec() == QDialog::Accepted) {
    m_selectedIcon = picker.selectedIconName();
    m_selectedColor = picker.selectedColor();
    updateIconPreview();
  }
}

void VaultCreationDialog::updateIconPreview() {
  if (!m_selectedIcon.isEmpty()) {
    QPixmap pix =
        Utils::IconLoader::loadColoredIcon(m_selectedIcon, m_selectedColor)
            .pixmap(32, 32);
    m_iconPreviewLabel->setPixmap(pix);
  } else {
    m_iconPreviewLabel->clear();
  }
}
