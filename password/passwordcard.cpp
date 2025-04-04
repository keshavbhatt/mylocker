#include "passwordcard.h"

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

PasswordCard::PasswordCard(const PasswordEntry &entry, QWidget *parent)
    : QFrame(parent), m_entry(entry) {
  setupUI();
}

void PasswordCard::setupUI() {
  auto *cardLayout = new QVBoxLayout(this);

  QLabel *titleLabel = new QLabel("<b>" + m_entry.title + "</b>", this);
  QLabel *userLabel = new QLabel("Username: " + m_entry.username, this);
  m_passwordLabel = new QLabel("••••••••", this);

  QPushButton *togglePassBtn = new QPushButton("Show", this);
  connect(togglePassBtn, &QPushButton::clicked, this, [=]() {
    bool isHidden = m_passwordLabel->text() == "••••••••";
    m_passwordLabel->setText(isHidden ? m_entry.password : "••••••••");
    togglePassBtn->setText(isHidden ? "Hide" : "Show");
  });

  QPushButton *copyPasswordBtn = new QPushButton("Copy Password", this);
  connect(copyPasswordBtn, &QPushButton::clicked, this,
          [=]() { QApplication::clipboard()->setText(m_entry.password); });

  QToolButton *menuButton = new QToolButton(this);
  menuButton->setText("Detail");
  menuButton->setPopupMode(QToolButton::MenuButtonPopup);
  menuButton->setToolTip("Open in Detailed View");

  connect(menuButton, &QToolButton::clicked, this,
          [this]() { showFullDetailsDialog(); });

  QMenu *menu = new QMenu(menuButton);
  menu->addAction("Copy Password", this, [this]() {
    QApplication::clipboard()->setText(m_entry.password);
  });

  menu->addAction("Delete", this,
                  [this]() { emit deleteRequested(m_entry.id); });

  menuButton->setMenu(menu);

  auto *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(togglePassBtn);
  buttonLayout->addWidget(copyPasswordBtn);
  buttonLayout->addStretch();
  buttonLayout->addWidget(menuButton);

  cardLayout->addWidget(titleLabel);
  cardLayout->addWidget(userLabel);
  cardLayout->addWidget(m_passwordLabel);
  cardLayout->addLayout(buttonLayout);

  setLayout(cardLayout);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  setLineWidth(2);
}

void PasswordCard::showFullDetailsDialog() {
  QDialog *dialog = new QDialog(this);
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
