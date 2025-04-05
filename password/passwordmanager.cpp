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
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <password/storage/passwordstorage.h>

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

  PasswordStorage::instance().loadPasswords();
}

void PasswordManager::addPasswordCardToUi(const PasswordEntry &entry) {
  PasswordCard *card = new PasswordCard(entry, this);
  connect(card, &PasswordCard::deleteRequested, this,
          &PasswordManager::confirmAndDeletePassword);
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
        card->setEntry(updatedEntry); // update card data
        card->refresh();              // refresh UI (you'll implement this)
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
  ui->stackedWidget->setCurrentWidget(hasEntries ? ui->passwordsPage
                                                 : ui->noPasswordsPage);
}

PasswordManager::~PasswordManager() { delete ui; }

void PasswordManager::addPasswordClicked() {
  AddPasswordDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    PasswordEntry entry = dialog.getPasswordEntry();
    PasswordStorage::instance().addPasswordEntry(entry);
  }
}
