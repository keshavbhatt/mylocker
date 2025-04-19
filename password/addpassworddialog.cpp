#include "addpassworddialog.h"

AddPasswordDialog::AddPasswordDialog(const PasswordEntry &entry,
                                     QWidget *parent)
    : AddPasswordDialog(parent) {
  siteInput->setText(entry.title);
  usernameInput->setText(entry.username);
  passwordInput->setText(entry.password);
  categoryCombo->setCurrentText(entry.category);

  if (!entry.url.isEmpty() || !entry.notes.isEmpty()) {
    extendedGroup->setChecked(true);
    urlInput->setText(entry.url);
    notesInput->setText(entry.notes);
  }
}

AddPasswordDialog::AddPasswordDialog(QWidget *parent)
    : QDialog(parent), siteInput(new QLineEdit(this)),
      usernameInput(new QLineEdit(this)),
      passwordInput(new PasswordLineEdit(this)), urlInput(new QLineEdit(this)),
      notesInput(new QTextEdit(this)), categoryCombo(new QComboBox(this)),
      extendedGroup(new QGroupBox("More Details", this)),
      saveBtn(new QPushButton("Save", this)),
      cancelBtn(new QPushButton("Cancel", this)) {

  setModal(true);
  setWindowTitle("Add New Password");

  notesInput->setPlaceholderText("Plain text note related to this entry");
  notesInput->setAcceptRichText(Qt::PlainText);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Basic fields
  siteInput->setPlaceholderText("Enter Site/Service");
  usernameInput->setPlaceholderText("Enter Username");
  passwordInput->setPlaceholderText("Enter Password");
  passwordInput->setEchoMode(QLineEdit::Password);

  mainLayout->addWidget(new QLabel("Site/Service:"));
  mainLayout->addWidget(siteInput);
  mainLayout->addWidget(new QLabel("Username:"));
  mainLayout->addWidget(usernameInput);

  // Password field
  mainLayout->addWidget(new QLabel("Password:"));
  QHBoxLayout *passwordLayout = new QHBoxLayout();
  QToolButton *generateBtn = createPasswordGeneratorButton(passwordInput, this);
  generateBtn->setToolTip(
      "Generate a secure password (click arrow for more options)");
  passwordLayout->addWidget(passwordInput);
  passwordLayout->addWidget(generateBtn);
  mainLayout->addLayout(passwordLayout);

  // Categories
  QStringList categories =
      QStringList::fromVector(CategoryManager::instance().getCategories());
  categories.removeAll("Other");
  categoryCombo->addItem("Other");
  categoryCombo->insertSeparator(categoryCombo->count());
  categoryCombo->addItems(categories);
  mainLayout->addWidget(new QLabel("Category:"));
  mainLayout->addWidget(categoryCombo);

  // Extended section
  extendedGroup->setCheckable(true);
  extendedGroup->setChecked(false); // Initially collapsed
  QVBoxLayout *extendedLayout = new QVBoxLayout();

  urlInput->setPlaceholderText("https://example.com");

  extendedLayout->addWidget(new QLabel("URL:"));
  extendedLayout->addWidget(urlInput);
  extendedLayout->addWidget(new QLabel("Notes:"));
  extendedLayout->addWidget(notesInput);

  extendedGroup->setLayout(extendedLayout);
  mainLayout->addWidget(extendedGroup);

  // Save & Cancel buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(saveBtn);
  buttonLayout->addWidget(cancelBtn);
  mainLayout->addLayout(buttonLayout);

  connect(saveBtn, &QPushButton::clicked, this, [this]() {
    if (!siteInput->text().isEmpty() && !usernameInput->text().isEmpty() &&
        !passwordInput->text().isEmpty()) {
      accept();
    }
  });

  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QToolButton *
AddPasswordDialog::createPasswordGeneratorButton(QLineEdit *targetInput,
                                                 QWidget *parent) {
  QToolButton *generateBtn = new QToolButton(parent);
  generateBtn->setText("Generate");
  generateBtn->setToolTip(
      "Generate a secure password (click arrow for more options)");
  generateBtn->setPopupMode(QToolButton::MenuButtonPopup);

  QMenu *generateMenu = new QMenu(generateBtn);
  QAction *advancedAction = generateMenu->addAction("Advanced...");
  generateBtn->setMenu(generateMenu);

  // Quick generate action
  connect(generateBtn, &QToolButton::clicked, parent, [targetInput]() {
    PasswordGenerator gen;
    QString password = gen.generate(PasswordGenerator::defaultSpec());
    if (!password.isEmpty())
      targetInput->setText(password);
  });

  // Advanced dialog
  connect(advancedAction, &QAction::triggered, parent, [targetInput, parent]() {
    PasswordGeneratorDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
      QString generated = dialog.generatedPassword();
      if (!generated.isEmpty())
        targetInput->setText(generated);
    }
  });

  return generateBtn;
}

PasswordEntry
AddPasswordDialog::getPasswordEntry(const QUuid &existingId) const {
  PasswordEntry entry;
  entry.id = existingId.isNull() ? QUuid::createUuid() : existingId;
  entry.title = siteInput->text();
  entry.username = usernameInput->text();
  entry.password = passwordInput->text();
  entry.updatedAt = QDateTime::currentDateTime();
  entry.category = categoryCombo->currentText();

  if (extendedGroup->isChecked()) {
    entry.url = urlInput->text();
    entry.notes = notesInput->toPlainText();
  }

  return entry;
}
