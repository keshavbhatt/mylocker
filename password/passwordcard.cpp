#include "passwordcard.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "addpassworddialog.h"
#include "utils/layout.h"
#include <settings/settingsmanager.h>

PasswordCard::PasswordCard(const PasswordEntry &entry, QWidget *parent)
    : QFrame(parent), m_entry(entry) {
  setupUI();
}

void PasswordCard::setEntry(const PasswordEntry &entry) { m_entry = entry; }

void PasswordCard::refresh() {
  if (QLayout *oldLayout = this->layout()) {
    Utils::Layout::clearLayout(oldLayout);
    delete oldLayout;
  }

  setupUI();
}

void PasswordCard::setupUI() {
  auto *cardLayout = new QVBoxLayout(this);

  m_titleLabel = new QLabel("<b>" + m_entry.title + "</b>", this);
  m_userLabel = new QLabel("Username: " + m_entry.username, this);
  m_passwordLabel = new QLabel("Password: ••••••••", this);
  m_categoryLabel = new QLabel("Category: " + m_entry.category, this);

  m_hidePasswordTimer = new QTimer(this);
  m_hidePasswordTimer->setSingleShot(true);
  connect(m_hidePasswordTimer, &QTimer::timeout, this, [this]() {
    if (m_passwordLabel->text() != "Password: ••••••••") {
      m_passwordLabel->setText("Password: ••••••••");
      m_togglePassBtn->setText("Show Password");
    }
  });

  m_togglePassBtn = new QToolButton(this);
  m_togglePassBtn->setText("Show Password");
  connect(m_togglePassBtn, &QToolButton::clicked, this, [=]() {
    bool isHidden = m_passwordLabel->text() == "Password: ••••••••";
    m_passwordLabel->setText(isHidden ? "Password: " + m_entry.password
                                      : "Password: ••••••••");
    m_togglePassBtn->setText(isHidden ? "Hide Password" : "Show Password");

    if (isHidden) {
      m_hidePasswordTimer->start(8000);
    } else {
      m_hidePasswordTimer->stop();
    }
  });

  m_togglePassBtn->setPopupMode(QToolButton::MenuButtonPopup);
  m_togglePassBtn->setToolTip("Show/Hide Password");
  QMenu *togglePassBtnMenu = new QMenu(m_togglePassBtn);
  togglePassBtnMenu->addAction("Show Full Entry", this,
                               [this]() { showFullDetailsDialog(); });
  m_togglePassBtn->setMenu(togglePassBtnMenu);

  QToolButton *copyPasswordBtn = createCopyToolButton();

  QToolButton *openUrlBtn = createOpenUrlButton();

  QToolButton *editToolButton = new QToolButton(this);
  editToolButton->setText("Edit Entry");
  editToolButton->setToolTip("Edit this Entry");
  editToolButton->setPopupMode(QToolButton::MenuButtonPopup);

  connect(editToolButton, &QToolButton::clicked, this,
          &PasswordCard::showEditDialog);

  QMenu *editToolButtonMenu = new QMenu(editToolButton);
  editToolButtonMenu->addAction("Edit as New Entry", this,
                                &PasswordCard::showDuplicateDialog);
  editToolButtonMenu->addSeparator();
  editToolButtonMenu->addAction("Delete Entry", this,
                                [this]() { emit deleteRequested(m_entry.id); });

  editToolButton->setMenu(editToolButtonMenu);

  auto *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(m_togglePassBtn);
  buttonLayout->addWidget(copyPasswordBtn);

  if (openUrlBtn) {
    buttonLayout->addWidget(openUrlBtn);
  }

  buttonLayout->addStretch();
  buttonLayout->addWidget(editToolButton);

  cardLayout->addWidget(m_titleLabel);
  cardLayout->addWidget(m_userLabel);
  cardLayout->addWidget(m_passwordLabel);
  cardLayout->addWidget(m_categoryLabel);
  cardLayout->addLayout(buttonLayout);

  setLayout(cardLayout);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  setLineWidth(2);
}

PasswordEntry PasswordCard::entry() const { return m_entry; }

void PasswordCard::copyToClipboardSecurely(const QString &text) {
  QApplication::clipboard()->setText(text);

  int clearDelayMs = SettingsManager::instance().getClipboardClearDelay();

  QTimer::singleShot(150, this, [clearDelayMs]() {
    QString message =
        (clearDelayMs != 0)
            ? "Copied to clipboard (auto-clears in a few seconds)."
              "\nIf you use a clipboard manager, consider clearing it manually "
              "for better security."
            : "Copied to clipboard.";
    QToolTip::showText(QCursor::pos(), message);
  });

  if (clearDelayMs != 0) {
    QTimer::singleShot(clearDelayMs, this, [text]() {
      QClipboard *clipboard = QApplication::clipboard();
      if (clipboard->text() == text) {
        clipboard->clear(QClipboard::Clipboard);
      }
    });
  }
}

QToolButton *PasswordCard::createCopyToolButton() {
  QToolButton *button = new QToolButton(this);
  button->setPopupMode(QToolButton::MenuButtonPopup);
  button->setText("Copy Password");
  button->setToolTip("Copy Password to Clipboard");

  QObject::connect(button, &QToolButton::clicked, button,
                   [=]() { copyToClipboardSecurely(m_entry.password); });

  QMenu *menu = new QMenu(button);

  if (!m_entry.title.isEmpty()) {
    QAction *copyTitle = menu->addAction("Copy Title");
    QObject::connect(copyTitle, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.title); });
  }

  if (!m_entry.username.isEmpty()) {
    QAction *copyUsername = menu->addAction("Copy Username");
    QObject::connect(copyUsername, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.username); });
  }

  if (!m_entry.password.isEmpty()) {
    QAction *copyPassword = menu->addAction("Copy Password");
    QObject::connect(copyPassword, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.password); });
  }

  if (!m_entry.url.isEmpty()) {
    QAction *copyURL = menu->addAction("Copy URL");
    QObject::connect(copyURL, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.url); });
  }

  if (!m_entry.notes.isEmpty()) {
    QAction *copyNotes = menu->addAction("Copy Notes");
    QObject::connect(copyNotes, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.notes); });
  }

  if (!m_entry.category.isEmpty()) {
    QAction *copyCategory = menu->addAction("Copy Category");
    QObject::connect(copyCategory, &QAction::triggered, button,
                     [=]() { copyToClipboardSecurely(m_entry.category); });
  }

  button->setMenu(menu);
  return button;
}

void PasswordCard::showEditDialog() {
  AddPasswordDialog dialog(m_entry, this);
  dialog.setWindowTitle("Edit Entry");
  if (dialog.exec() == QDialog::Accepted) {
    PasswordEntry updated = dialog.getPasswordEntry(m_entry.id);
    updated.timestamp = QDateTime::currentDateTime();

    // this will handle update of the card
    // in password manager
    emit entryEdited(updated);
  }
}

void PasswordCard::showDuplicateDialog() {
  AddPasswordDialog dialog(m_entry, this);
  dialog.setWindowTitle("Edit as New Entry");
  if (dialog.exec() == QDialog::Accepted) {
    PasswordEntry updated = dialog.getPasswordEntry();
    updated.timestamp = QDateTime::currentDateTime();

    emit entryDuplicated(updated);
  }
}

void PasswordCard::showFullDetailsDialog() {
  QDialog *dialog = new QDialog(this);
  dialog->setModal(true);
  dialog->setWindowTitle("Password Details");
  dialog->resize(400, 300);

  QScrollArea *scrollArea = new QScrollArea(dialog);
  QWidget *scrollWidget = new QWidget(scrollArea);
  QGridLayout *grid = new QGridLayout(scrollWidget);

  QLocale locale;
  QString formattedDate =
      locale.toString(m_entry.timestamp, QLocale::LongFormat);

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
  addRow(row++, "Site/Service", m_entry.title);
  addRow(row++, "Username", m_entry.username);
  addRow(row++, "Password", "••••••••");
  addRow(row++, "Category", m_entry.category);
  addRow(row++, "URL", m_entry.url);
  addRow(row++, "Notes", m_entry.notes);
  addRow(row++, "Added On", formattedDate);
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

QToolButton *PasswordCard::createOpenUrlButton() {
  QString urlToOpen = m_entry.url.trimmed();

  if (urlToOpen.isEmpty() && m_entry.title.contains('.')) {
    urlToOpen = m_entry.title.trimmed();
  }

  if (!urlToOpen.startsWith("http://") && !urlToOpen.startsWith("https://")) {
    urlToOpen = "https://" + urlToOpen.toLower();
  }

  QUrl url(urlToOpen);
  if (!url.isValid() || url.host().isEmpty()) {
    return nullptr;
  }

  QToolButton *openUrlBtn = new QToolButton(this);
  openUrlBtn->setIcon(QIcon(":/icons/svg/links-line.svg"));
  openUrlBtn->setToolTip(
      QString("Open '%1' in Web Browser").arg(url.toString()));
  openUrlBtn->setCursor(Qt::PointingHandCursor);

  connect(openUrlBtn, &QToolButton::clicked, this,
          [url]() { QDesktopServices::openUrl(url); });

  return openUrlBtn;
}
