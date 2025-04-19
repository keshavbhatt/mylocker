#include "passwordlineedit.h"

#include <icons/iconloader.h>
#include <theme/palette.h>

PasswordLineEdit::PasswordLineEdit(QWidget *parent) : QLineEdit(parent) {
  setEchoMode(QLineEdit::Password);

  m_toggleAction = addAction(QIcon(), QLineEdit::TrailingPosition);
  m_toggleAction->setToolTip("Show password");
  updateActionIcon();

  connect(m_toggleAction, &QAction::triggered, this,
          &PasswordLineEdit::togglePasswordVisibility);
}

void PasswordLineEdit::togglePasswordVisibility() {
  m_passwordVisible = !m_passwordVisible;
  setEchoMode(m_passwordVisible ? QLineEdit::Normal : QLineEdit::Password);
  updateActionIcon();
}

void PasswordLineEdit::updateActionIcon() {
  QString iconName = m_passwordVisible ? "eye-off-fill" : "eye-fill";
  m_toggleAction->setIcon(
      Utils::IconLoader::loadColoredIcon(iconName, Palette::iconDefault()));
  m_toggleAction->setToolTip(m_passwordVisible ? "Hide password"
                                               : "Show password");
}
