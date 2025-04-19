#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QAction>
#include <QLineEdit>

class PasswordLineEdit : public QLineEdit {
  Q_OBJECT

public:
  explicit PasswordLineEdit(QWidget *parent = nullptr);

  bool isPasswordVisible() const { return m_passwordVisible; }

private:
  QAction *m_toggleAction;
  bool m_passwordVisible = false;

  void togglePasswordVisibility();
  void updateActionIcon();
};

#endif // PASSWORDLINEEDIT_H
