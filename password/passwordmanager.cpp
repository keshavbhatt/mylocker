#include "passwordmanager.h"
#include "ui_passwordmanager.h"

#include <QClipboard>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include <password/storage/passwordstorage.h>

PasswordManager::PasswordManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::PasswordManager) {
  ui->setupUi(this);

  connect(&PasswordStorage::instance(), &PasswordStorage::passwordsLoaded, this,
          &PasswordManager::passwordsLoaded);
  connect(&PasswordStorage::instance(), &PasswordStorage::operationFailed, this,
          &PasswordManager::passwordError);
  connect(&PasswordStorage::instance(), &PasswordStorage::passwordAdded, this,
          &PasswordManager::passwordAdded);
  connect(&PasswordStorage::instance(), &PasswordStorage::passwordDeleted, this,
          &PasswordManager::handlePasswordDeleted);

  connect(ui->backToDashboardButton, &QPushButton::clicked, this,
          &PasswordManager::goToDashboard);
  connect(ui->addPasswordButton, &QPushButton::clicked, this,
          &PasswordManager::addPasswordClicked);

  ui->passwordsContainer->layout()->setAlignment(Qt::AlignTop);

  PasswordStorage::instance().loadPasswords();
}

void PasswordManager::addPasswordCardToUi(const PasswordEntry &entry) {
  QFrame *card = createPasswordCard(entry);
  card->setProperty("entryId", entry.id);
  card->setObjectName("card_" + entry.id.toString());
  card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  ui->passwordsContainer->layout()->addWidget(card);
}

QFrame *PasswordManager::createPasswordCard(const PasswordEntry &entry) {
  QFrame *card = new QFrame();
  QVBoxLayout *cardLayout = new QVBoxLayout(card);

  QLabel *titleLabel = new QLabel("<b>" + entry.title + "</b>");
  QLabel *userLabel = new QLabel("Username: " + entry.username);
  QLabel *hiddenPassLabel = new QLabel("••••••••");

  QPushButton *togglePassBtn = new QPushButton("Show");
  QPushButton *copyBtn = new QPushButton("Copy");
  QPushButton *deleteBtn = new QPushButton("Delete");

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(togglePassBtn);
  buttonLayout->addWidget(copyBtn);
  buttonLayout->addWidget(deleteBtn);

  cardLayout->addWidget(titleLabel);
  cardLayout->addWidget(userLabel);
  cardLayout->addWidget(hiddenPassLabel);
  cardLayout->addLayout(buttonLayout);
  card->setLayout(cardLayout);
  card->setFrameStyle(QFrame::StyledPanel);

  connect(togglePassBtn, &QPushButton::clicked, this,
          [hiddenPassLabel, entry, togglePassBtn]() {
            if (hiddenPassLabel->text() == "••••••••") {
              hiddenPassLabel->setText(entry.password);
              togglePassBtn->setText("Hide");
            } else {
              hiddenPassLabel->setText("••••••••");
              togglePassBtn->setText("Show");
            }
          });

  connect(copyBtn, &QPushButton::clicked, this, [entry]() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(entry.password);
  });

  connect(deleteBtn, &QPushButton::clicked, this,
          [=]() { confirmAndDeletePassword(entry.id); });

  return card;
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
  QDialog dialog(this);
  dialog.setWindowTitle("Add New Password");

  QVBoxLayout layout(&dialog);

  QLineEdit siteInput, usernameInput, passwordInput;
  siteInput.setPlaceholderText("Enter Site/Service");
  usernameInput.setPlaceholderText("Enter Username");
  passwordInput.setPlaceholderText("Enter Password");
  passwordInput.setEchoMode(QLineEdit::Password);

  QPushButton saveBtn("Save"), cancelBtn("Cancel");
  connect(&saveBtn, &QPushButton::clicked, this, [&]() {
    QString site = siteInput.text();
    QString username = usernameInput.text();
    QString password = passwordInput.text();

    if (!site.isEmpty() && !username.isEmpty() && !password.isEmpty()) {
      PasswordStorage::instance().addPasswordEntry(site, username, password);
      dialog.accept();
    }
  });

  connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

  layout.addWidget(new QLabel("Site/Service:"));
  layout.addWidget(&siteInput);
  layout.addWidget(new QLabel("Username:"));
  layout.addWidget(&usernameInput);
  layout.addWidget(new QLabel("Password:"));
  layout.addWidget(&passwordInput);
  layout.addWidget(&saveBtn);
  layout.addWidget(&cancelBtn);

  dialog.exec();
}
