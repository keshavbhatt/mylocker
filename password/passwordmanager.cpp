#include "passwordmanager.h"
#include "addpassworddialog.h"
#include "passwordcard.h"
#include "ui_passwordmanager.h"

#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <password/storage/passwordstorage.h>
#include <vault/vaultmanager.h>

#include <categories/categorymanager.h>

PasswordManager::PasswordManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::PasswordManager) {
  ui->setupUi(this);

  ui->scrollArea->verticalScrollBar()->setSingleStep(30);

  ui->passwordsContainer->layout()->setAlignment(Qt::AlignTop);

  connect(&PasswordStorage::instance(), &PasswordStorage::passwordsLoaded, this,
          &PasswordManager::passwordsLoaded);
  connect(&PasswordStorage::instance(), &PasswordStorage::operationFailed, this,
          &PasswordManager::passwordError);
  connect(&PasswordStorage::instance(), &PasswordStorage::passwordAdded, this,
          &PasswordManager::passwordAdded);
  connect(&PasswordStorage::instance(), &PasswordStorage::passwordDeleted, this,
          &PasswordManager::handlePasswordDeleted);
  connect(&PasswordStorage::instance(), &PasswordStorage::passwordUpdated, this,
          &PasswordManager::handlePasswordUpdated);

  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &PasswordManager::goToDashboard);
  connect(ui->addPasswordButton, &QPushButton::clicked, this,
          &PasswordManager::addPasswordClicked);
  connect(ui->filterEntriesInput, &QLineEdit::textChanged, this,
          [this](const QString &text) { filterEntries(text); });

  PasswordStorage::instance().loadPasswords();

  ui->backToDashboardButton->installEventFilter(this);
  // stressTest();

  installEventFilter(this);
}

bool PasswordManager::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() == Qt::BackButton) {
      emit goToDashboard();
      return true;
    }
  }
  return QWidget::eventFilter(watched, event);
}

void PasswordManager::stressTest() {
  QString currentVaultName = VaultManager::instance().currentVault().name();
  for (int i = 0; i < 500; ++i) {
    PasswordEntry entry;
    entry.category = "Other";
    entry.notes = QString("Note for user %1 password entry.").arg(i);
    entry.password = "assext";
    entry.title = QString("google %1 in %2").arg(i).arg(currentVaultName);
    entry.url = "gmail.com";
    entry.username = QString("user %1").arg(i);
    PasswordStorage::instance().addPasswordEntry(entry);
  }
}

void PasswordManager::addPasswordCardToUi(const PasswordEntry &entry) {
  PasswordCard *card = new PasswordCard(entry, this);
  connect(card, &PasswordCard::deleteRequested, this,
          &PasswordManager::confirmAndDeletePassword);
  connect(card, &PasswordCard::entryDuplicated, this,
          [=](const PasswordEntry &newEntry) {
            PasswordStorage::instance().addPasswordEntry(newEntry);
          });
  connect(card, &PasswordCard::entryEdited, this,
          [=](const PasswordEntry &entry) {
            PasswordStorage::instance().updatePasswordEntry(entry);
          });

  card->setProperty("entryId", entry.id);
  card->setObjectName("card_" + entry.id.toString());
  card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  ui->passwordsContainer->layout()->addWidget(card);
}

void PasswordManager::confirmAndDeletePassword(QUuid id) {
  QMessageBox::StandardButton reply;
  reply =
      QMessageBox::question(this, "Confirm Deletion",
                            "Are you sure you want to delete this password?",
                            QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    PasswordStorage::instance().deletePasswordEntry(id);
  }
}

void PasswordManager::passwordsLoaded(QVector<PasswordEntry> entries) {
  QLayout *layout = ui->passwordsContainer->layout();
  qDeleteAll(layout->children());

  for (const PasswordEntry &entry : entries) {
    addPasswordCardToUi(entry);
  }

  updateStackWidget();
}

void PasswordManager::passwordAdded(PasswordEntry entry) {
  addPasswordCardToUi(entry);
  updateStackWidget();
}

void PasswordManager::handlePasswordDeleted(QUuid id) {
  QLayout *layout = ui->passwordsContainer->layout();
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

void PasswordManager::handlePasswordUpdated(const PasswordEntry &updatedEntry) {
  QLayout *layout = ui->passwordsContainer->layout();
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget && widget->property("entryId").toUuid() == updatedEntry.id) {
      auto *card = qobject_cast<PasswordCard *>(widget);
      if (card) {
        card->setEntry(updatedEntry);
        card->refresh();
      }
      break;
    }
  }
}

void PasswordManager::passwordError(const QString &error) {
  QMessageBox::warning(this, "Error", error);
}

void PasswordManager::updateStackWidget() {
  bool hasEntries = !ui->passwordsContainer->findChildren<QFrame *>().isEmpty();
  ui->stackedWidget->slideInWgt(hasEntries ? ui->passwordsPage
                                           : ui->noPasswordsPage);
}

PasswordManager::~PasswordManager() {
  qDebug() << "Deleted PasswordManager";
  delete ui;
}

void PasswordManager::addPasswordClicked() {
  AddPasswordDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    PasswordEntry entry = dialog.getPasswordEntry();
    PasswordStorage::instance().addPasswordEntry(entry);
  }
}

void PasswordManager::filterEntries(const QString &filterText) {
  QLayout *layout = ui->passwordsContainer->layout();
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (auto card = qobject_cast<PasswordCard *>(widget)) {
      const auto &entry = card->entry();
      bool matches = entry.title.contains(filterText, Qt::CaseInsensitive) ||
                     entry.username.contains(filterText, Qt::CaseInsensitive) ||
                     entry.url.contains(filterText, Qt::CaseInsensitive) ||
                     entry.notes.contains(filterText, Qt::CaseInsensitive) ||
                     entry.category.contains(filterText, Qt::CaseInsensitive);

      card->setVisible(matches);
    }
  }
}
