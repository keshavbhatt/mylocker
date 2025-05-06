#include "addnotedialog.h"

AddNoteDialog::AddNoteDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Add Note");
  setModal(true);
  resize(400, 300);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Title
  QLabel *titleLabel = new QLabel("Title:", this);
  titleInput = new QLineEdit(this);

  // Content
  QLabel *contentLabel = new QLabel("Content:", this);
  contentInput = new QTextEdit(this);
  contentInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // Category
  QLabel *categoryLabel = new QLabel("Category:", this);
  categoryCombo = new QComboBox(this);
  QStringList categories =
      QStringList::fromVector(CategoryManager::instance().getCategories());
  categories.removeAll("Other");
  categoryCombo->addItem("Other");
  categoryCombo->insertSeparator(categoryCombo->count());
  categoryCombo->addItems(categories);

  // Buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  saveBtn = new QPushButton("Save", this);
  cancelBtn = new QPushButton("Cancel", this);

  connect(saveBtn, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

  buttonLayout->addStretch();
  buttonLayout->addWidget(saveBtn);
  buttonLayout->addWidget(cancelBtn);

  // Layout
  mainLayout->addWidget(titleLabel);
  mainLayout->addWidget(titleInput);
  mainLayout->addWidget(contentLabel);
  mainLayout->addWidget(contentInput, 1);
  mainLayout->addWidget(categoryLabel);
  mainLayout->addWidget(categoryCombo);
  mainLayout->addLayout(buttonLayout);
}

AddNoteDialog::AddNoteDialog(const NoteEntry &entry, QWidget *parent)
    : AddNoteDialog(parent) {
  titleInput->setText(entry.title);
  contentInput->setText(entry.content);
  int index = categoryCombo->findText(entry.category);
  if (index != -1)
    categoryCombo->setCurrentIndex(index);
}

NoteEntry AddNoteDialog::getNoteEntry(const QUuid &existingId) const {
  NoteEntry entry;
  entry.id = existingId.isNull() ? QUuid::createUuid() : existingId;
  entry.title = titleInput->text().trimmed();
  entry.content = contentInput->toPlainText().trimmed();
  entry.category = categoryCombo->currentText();
  entry.updatedAt = QDateTime::currentDateTime();

  if (entry.createdAt.isNull()) {
    entry.createdAt = entry.updatedAt;
  }

  return entry;
}
