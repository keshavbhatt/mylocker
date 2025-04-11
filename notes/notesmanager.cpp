#include "notesmanager.h"
#include "ui_notesmanager.h"

#include <QDebug>
#include <QMouseEvent>

NotesManager::NotesManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::NotesManager) {
  ui->setupUi(this);

  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &NotesManager::goToDashboard);
  connect(ui->addNoteButton, &QPushButton::clicked, this,
          &NotesManager::addNoteClicked);
  connect(ui->filterEntriesInput, &QLineEdit::textChanged, this,
          [this](const QString &text) { filterEntries(text); });

  installEventFilter(this);
}

bool NotesManager::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() == Qt::BackButton) {
      emit goToDashboard();
      return true;
    }
  }
  return QWidget::eventFilter(watched, event);
}

void NotesManager::updateStackWidget() {
  bool hasEntries = !ui->notesContainer->findChildren<QFrame *>().isEmpty();
  ui->stackedWidget->setCurrentWidget(hasEntries ? ui->notesPage
                                                 : ui->noNotesPage);
}

void NotesManager::addNoteClicked() {
  // AddNoteDialog dialog(this);
  // if (dialog.exec() == QDialog::Accepted) {
  //     NoteEntry entry = dialog.getNoteEntry();
  //     NotesStorage::instance().addNoteEntry(entry);
  // }
}

void NotesManager::filterEntries(const QString &filterText) {
  // QLayout *layout = ui->notesContainer->layout();
  // for (int i = 0; i < layout->count(); ++i) {
  //   QWidget *widget = layout->itemAt(i)->widget();
  //   if (auto card = qobject_cast<NoteCard *>(widget)) {
  //     const auto &entry = card->entry();
  //     bool matches = entry.title.contains(filterText, Qt::CaseInsensitive) ||
  //                    entry.username.contains(filterText, Qt::CaseInsensitive)
  //                    || entry.url.contains(filterText, Qt::CaseInsensitive)
  //                    || entry.notes.contains(filterText, Qt::CaseInsensitive)
  //                    || entry.category.contains(filterText,
  //                    Qt::CaseInsensitive);

  //     card->setVisible(matches);
  //   }
  // }
}

NotesManager::~NotesManager() {
  qDebug() << "Deleted NotesManager";
  delete ui;
}
