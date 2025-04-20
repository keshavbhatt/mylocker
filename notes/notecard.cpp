#include "notecard.h"

NoteCard::NoteCard(const NoteEntry &note, QWidget *parent)
    : QFrame(parent), m_note(note) {
  setupUI();
}

NoteEntry NoteCard::entry() const { return m_note; }

void NoteCard::setEntry(const NoteEntry &note) { m_note = note; }

void NoteCard::refresh() {
  if (QLayout *oldLayout = this->layout()) {
    Utils::Layout::clearLayout(oldLayout);
    delete oldLayout;
  }

  setupUI();
}

void NoteCard::setupUI() {
  auto *cardLayout = new QVBoxLayout(this);

  m_titleLabel = new QLabel("<b>" + m_note.title + "</b>", this);
  m_notePreviewLabel = new QLabel(m_note.content.left(100) + "...", this);
  m_notePreviewLabel->setWordWrap(true);
  m_categoryLabel = new QLabel("Category: " + m_note.category, this);

  QToolButton *showFullBtn = new QToolButton(this);
  showFullBtn->setText("Show Note");
  connect(showFullBtn, &QToolButton::clicked, this,
          &NoteCard::showFullDetailsDialog);

  QToolButton *editToolButton = new QToolButton(this);
  editToolButton->setText("Edit Note");
  editToolButton->setPopupMode(QToolButton::MenuButtonPopup);
  connect(editToolButton, &QToolButton::clicked, this,
          &NoteCard::showEditDialog);

  QMenu *menu = new QMenu(editToolButton);
  menu->addAction("Edit as New Note", this, &NoteCard::showDuplicateDialog);
  menu->addSeparator();
  menu->addAction("Delete Note", this,
                  [this]() { emit deleteRequested(m_note.id.toString()); });
  editToolButton->setMenu(menu);

  auto *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(showFullBtn);
  buttonLayout->addStretch();
  buttonLayout->addWidget(editToolButton);

  cardLayout->addWidget(m_titleLabel);
  cardLayout->addWidget(m_notePreviewLabel);
  cardLayout->addWidget(m_categoryLabel);
  cardLayout->addLayout(buttonLayout);

  setLayout(cardLayout);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  setLineWidth(2);
}

void NoteCard::showEditDialog() {
  AddNoteDialog dialog(m_note, this);
  dialog.setWindowTitle("Edit Note");
  if (dialog.exec() == QDialog::Accepted) {
    NoteEntry updated = dialog.getNoteEntry(m_note.id);
    updated.createdAt = m_note.createdAt;
    emit entryEdited(updated);
  }
}

void NoteCard::showDuplicateDialog() {
  AddNoteDialog dialog(m_note, this);
  dialog.setWindowTitle("Duplicate Note");
  if (dialog.exec() == QDialog::Accepted) {
    NoteEntry updated = dialog.getNoteEntry();
    emit entryDuplicated(updated);
  }
}

void NoteCard::showFullDetailsDialog() {
  QDialog *dialog = new QDialog(this);
  dialog->setModal(true);
  dialog->setWindowTitle("Note Details");
  dialog->resize(400, 300);

  QScrollArea *scrollArea = new QScrollArea(dialog);
  QWidget *scrollWidget = new QWidget(scrollArea);
  QGridLayout *grid = new QGridLayout(scrollWidget);

  QLocale locale;
  QString createdDateTime =
      locale.toString(m_note.createdAt, QLocale::LongFormat);
  QString updatedDateTime =
      locale.toString(m_note.updatedAt, QLocale::LongFormat);

  auto addRow = [&](int row, const QString &label, const QString &value) {
    if (value.isEmpty())
      return;

    QLabel *labelWidget = new QLabel("<b>" + label + ":</b>", scrollWidget);
    QLabel *valueWidget = new QLabel(value, scrollWidget);
    valueWidget->setTextFormat(Qt::PlainText);
    valueWidget->setWordWrap(true);

    grid->addWidget(labelWidget, row, 0, Qt::AlignTop | Qt::AlignRight);
    grid->addWidget(valueWidget, row, 1);
  };

  int row = 0;
  addRow(row++, "Title", m_note.title);
  addRow(row++, "Content", m_note.content);
  addRow(row++, "Category", m_note.category);
  addRow(row++, "Added On", createdDateTime);
  addRow(row++, "Updated On", updatedDateTime);
  grid->setRowStretch(row, 1);

  scrollWidget->setLayout(grid);
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);

  QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
  mainLayout->addWidget(scrollArea);

  QPushButton *closeBtn = new QPushButton("Close", dialog);
  connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
  mainLayout->addWidget(closeBtn, 0, Qt::AlignRight);

  dialog->setLayout(mainLayout);
  dialog->exec();
}

void NoteCard::copyToClipboardSecurely(const QString &text) {
  QApplication::clipboard()->setText(text);

  int delayMs = SettingsManager::instance().getClipboardClearDelay();

  QTimer::singleShot(150, this, [delayMs]() {
    QString message =
        (delayMs != 0) ? "Copied to clipboard (auto-clears in a few seconds)."
                       : "Copied to clipboard.";
    QToolTip::showText(QCursor::pos(), message);
  });

  if (delayMs != 0) {
    QTimer::singleShot(delayMs, this, [text]() {
      QClipboard *clipboard = QApplication::clipboard();
      if (clipboard->text() == text) {
        clipboard->clear(QClipboard::Clipboard);
      }
    });
  }
}
