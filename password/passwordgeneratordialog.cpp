#include "passwordgeneratordialog.h"
#include "passwordgenerator.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

PasswordGeneratorDialog::PasswordGeneratorDialog(QWidget *parent)
    : QDialog(parent) {
  setWindowTitle("Password Generator");
  setModal(true);
  resize(400, 300);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Length
  QHBoxLayout *lengthLayout = new QHBoxLayout();
  lengthLabel = new QLabel("Length: 16", this);
  lengthSlider = new QSlider(Qt::Horizontal, this);
  lengthSlider->setRange(4, 128);
  lengthSlider->setValue(16);
  lengthLayout->addWidget(lengthLabel);
  lengthLayout->addWidget(lengthSlider);
  mainLayout->addLayout(lengthLayout);

  // Options
  uppercaseCheck = new QCheckBox("Include Uppercase", this);
  lowercaseCheck = new QCheckBox("Include Lowercase", this);
  digitsCheck = new QCheckBox("Include Digits", this);
  symbolsCheck = new QCheckBox("Include Symbols", this);
  extendedCheck = new QCheckBox("Include Extended ASCII", this);

  uppercaseCheck->setChecked(true);
  lowercaseCheck->setChecked(true);
  digitsCheck->setChecked(true);

  mainLayout->addWidget(uppercaseCheck);
  mainLayout->addWidget(lowercaseCheck);
  mainLayout->addWidget(digitsCheck);
  mainLayout->addWidget(symbolsCheck);
  mainLayout->addWidget(extendedCheck);

  // Password output
  passwordEdit = new QLineEdit(this);
  passwordEdit->setReadOnly(true);
  mainLayout->addWidget(passwordEdit);

  // Strength
  strengthProgress = new QProgressBar(this);
  strengthProgress->setRange(0, 100);
  mainLayout->addWidget(strengthProgress);

  // Buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  copyButton = new QPushButton("Copy", this);
  okButton = new QPushButton("OK", this);
  cancelButton = new QPushButton("Cancel", this);
  buttonLayout->addWidget(copyButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  mainLayout->addLayout(buttonLayout);

  // Connections
  connect(lengthSlider, &QSlider::valueChanged, this, [=](int value) {
    lengthLabel->setText(QString("Length: %1").arg(value));
    updatePassword();
  });

  connect(uppercaseCheck, &QCheckBox::toggled, this,
          &PasswordGeneratorDialog::updatePassword);
  connect(lowercaseCheck, &QCheckBox::toggled, this,
          &PasswordGeneratorDialog::updatePassword);
  connect(digitsCheck, &QCheckBox::toggled, this,
          &PasswordGeneratorDialog::updatePassword);
  connect(symbolsCheck, &QCheckBox::toggled, this,
          &PasswordGeneratorDialog::updatePassword);
  connect(extendedCheck, &QCheckBox::toggled, this,
          &PasswordGeneratorDialog::updatePassword);
  connect(copyButton, &QPushButton::clicked, this,
          &PasswordGeneratorDialog::copyToClipboard);
  connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  updatePassword();
}

QString PasswordGeneratorDialog::generatedPassword() const {
  return m_generatedPassword;
}

void PasswordGeneratorDialog::updatePassword() {
  PasswordGenerator::PasswordSpec spec;
  spec.length = lengthSlider->value();
  spec.useUppercase = uppercaseCheck->isChecked();
  spec.useLowercase = lowercaseCheck->isChecked();
  spec.useDigits = digitsCheck->isChecked();
  spec.useSymbols = symbolsCheck->isChecked();
  spec.useExtended = extendedCheck->isChecked();

  m_generatedPassword = PasswordGenerator::generate(spec);
  passwordEdit->setText(m_generatedPassword);

  int strength = PasswordGenerator::calculateStrength(m_generatedPassword);
  QString strengthDesc = PasswordGenerator::strengthDescription(strength);
  strengthProgress->setValue(strength);
  strengthProgress->setFormat(strengthDesc);
  strengthProgress->setTextVisible(true);
}

void PasswordGeneratorDialog::copyToClipboard() {
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(m_generatedPassword);
}
