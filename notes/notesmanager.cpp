#include "notesmanager.h"
#include "addnotedialog.h"
#include "notecard.h"
#include "ui_notesmanager.h"

NotesManager::NotesManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::NotesManager) {
  ui->setupUi(this);

  ui->scrollArea->verticalScrollBar()->setSingleStep(30);
  ui->notesContainer->layout()->setAlignment(Qt::AlignTop);

  connect(&NotesStorage::instance(), &NotesStorage::notesLoaded, this,
          &NotesManager::notesLoaded);
  connect(&NotesStorage::instance(), &NotesStorage::operationFailed, this,
          &NotesManager::noteError);
  connect(&NotesStorage::instance(), &NotesStorage::noteAdded, this,
          &NotesManager::noteAdded);
  connect(&NotesStorage::instance(), &NotesStorage::noteDeleted, this,
          &NotesManager::handleNoteDeleted);
  connect(&NotesStorage::instance(), &NotesStorage::noteUpdated, this,
          &NotesManager::handleNoteUpdated);

  connect(ui->filterEntriesInput, &QLineEdit::textChanged, this,
          [this](const QString &text) { filterEntries(text); });

  ui->backToDashboardButton->setIconSize(QSize(22, 22));
  ui->backToDashboardButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "arrow-left-circle-fill", Palette::iconDefault()));
  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &NotesManager::goToDashboard);

  ui->addNoteButton->setIconSize(QSize(22, 22));
  ui->addNoteButton->setIcon(Utils::IconLoader::loadColoredIcon(
      "add-circle-fill", Palette::iconSuccess()));
  connect(ui->addNoteButton, &QPushButton::clicked, this,
          &NotesManager::addNoteClicked);

  NotesStorage::instance().loadNotes();

  // stressTest();

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

NotesManager::~NotesManager() {
  qDebug() << "Deleted NotesManager";
  delete ui;
}

void NotesManager::stressTest() {
  QString currentVaultName = VaultManager::instance().currentVault().name();

  for (int i = 0; i < 500; ++i) {
    NoteEntry entry;
    entry.title = QString("Note %1 in %2").arg(i).arg(currentVaultName);
    entry.content =
        QString("This is the content of note #%1 created for testing purposes.")
            .arg(i);
    entry.createdAt = QDateTime::currentDateTime();
    entry.updatedAt = entry.createdAt;

    NotesStorage::instance().addNoteEntry(entry);
  }
}

void NotesManager::addNoteCardToUi(const NoteEntry &entry) {
  NoteCard *card = new NoteCard(entry, this);
  connect(card, &NoteCard::deleteRequested, this,
          &NotesManager::confirmAndDeleteNote);
  connect(card, &NoteCard::entryEdited, this,
          [=](const NoteEntry &updatedEntry) {
            NotesStorage::instance().updateNoteEntry(updatedEntry);
          });
  connect(card, &NoteCard::entryDuplicated, this,
          [=](const NoteEntry &newEntry) {
            NotesStorage::instance().addNoteEntry(newEntry);
          });

  card->setProperty("entryId", entry.id);
  card->setObjectName("note_card_" + entry.id.toString());
  card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  ui->notesContainer->layout()->addWidget(card);
}

void NotesManager::confirmAndDeleteNote(QUuid id) {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Confirm Deletion",
                                "Are you sure you want to delete this note?",
                                QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    NotesStorage::instance().deleteNoteEntry(id);
  }
}

void NotesManager::notesLoaded(QVector<NoteEntry> entries) {
  QLayout *layout = ui->notesContainer->layout();
  QLayoutItem *item;
  while ((item = layout->takeAt(0))) {
    delete item->widget();
    delete item;
  }

  for (const NoteEntry &entry : entries) {
    addNoteCardToUi(entry);
  }

  updateStackWidget();
}

void NotesManager::noteAdded(NoteEntry entry) {
  addNoteCardToUi(entry);
  updateStackWidget();
}

void NotesManager::handleNoteDeleted(QUuid id) {
  QLayout *layout = ui->notesContainer->layout();
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget && widget->property("entryId").toUuid() == id) {
      layout->removeWidget(widget);
      delete widget;
      break;
    }
  }
  updateStackWidget();
}

void NotesManager::handleNoteUpdated(const NoteEntry &updatedEntry) {
  QLayout *layout = ui->notesContainer->layout();
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget && widget->property("entryId").toUuid() == updatedEntry.id) {
      auto *card = qobject_cast<NoteCard *>(widget);
      if (card) {
        card->setEntry(updatedEntry);
        card->refresh();
      }
      break;
    }
  }
}

void NotesManager::noteError(const QString &error) {
  QMessageBox::warning(this, "Error", error);
}

void NotesManager::updateStackWidget() {
  bool hasEntries = !ui->notesContainer->findChildren<QFrame *>().isEmpty();
  ui->stackedWidget->slideInWgt(hasEntries ? ui->notesPage : ui->noNotesPage);
}

void NotesManager::addNoteClicked() {
  AddNoteDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    NoteEntry entry = dialog.getNoteEntry();
    NotesStorage::instance().addNoteEntry(entry);
  }
}

void NotesManager::filterEntries(const QString &filterText) {
  QLayout *layout = ui->notesContainer->layout();
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (auto card = qobject_cast<NoteCard *>(widget)) {
      const auto &entry = card->entry();
      bool matches = entry.title.contains(filterText, Qt::CaseInsensitive) ||
                     entry.content.contains(filterText, Qt::CaseInsensitive) ||
                     entry.category.contains(filterText, Qt::CaseInsensitive);

      card->setVisible(matches);
    }
  }
}
