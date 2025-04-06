#ifndef PASSWORDGENERATORDIALOG_H
#define PASSWORDGENERATORDIALOG_H

#include <QDialog>

class QLabel;
class QSlider;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QProgressBar;

class PasswordGeneratorDialog : public QDialog {
  Q_OBJECT

public:
  explicit PasswordGeneratorDialog(QWidget *parent = nullptr);
  QString generatedPassword() const;

private slots:
  void updatePassword();
  void copyToClipboard();

private:
  QSlider *lengthSlider;
  QLabel *lengthLabel;
  QCheckBox *uppercaseCheck;
  QCheckBox *lowercaseCheck;
  QCheckBox *digitsCheck;
  QCheckBox *symbolsCheck;
  QCheckBox *extendedCheck;

  QLineEdit *passwordEdit;
  QProgressBar *strengthProgress;

  QPushButton *copyButton;
  QPushButton *okButton;
  QPushButton *cancelButton;

  QString m_generatedPassword;
};

#endif // PASSWORDGENERATORDIALOG_H
