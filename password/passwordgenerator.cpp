#include "passwordgenerator.h"

#include <QRandomGenerator>
#include <QRegExp>
#include <cmath>

QString PasswordGenerator::generate(const PasswordSpec &spec) {
  QString chars;
  if (spec.useUppercase)
    chars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (spec.useLowercase)
    chars += "abcdefghijklmnopqrstuvwxyz";
  if (spec.useDigits)
    chars += "0123456789";
  if (spec.useSymbols)
    chars += "!@#$%^&*()-_=+[{]}|;:',<.>/?";
  if (spec.useExtended) {
    for (int ch = 0x80; ch <= 0xFF; ++ch)
      chars.append(QChar(ch));
  }

  if (chars.isEmpty())
    return QString();

  QString result;
  for (int i = 0; i < spec.length; ++i) {
    int index = QRandomGenerator::global()->bounded(chars.size());
    result += chars.at(index);
  }

  return result;
}

int PasswordGenerator::calculateStrength(const QString &password) {
  if (password.isEmpty())
    return 0;

  int length = password.length();
  int poolSize = 0;

  if (password.contains(QRegExp("[a-z]")))
    poolSize += 26;
  if (password.contains(QRegExp("[A-Z]")))
    poolSize += 26;
  if (password.contains(QRegExp("[0-9]")))
    poolSize += 10;
  if (password.contains(QRegExp("[^A-Za-z0-9\\x80-\\xFF]")))
    poolSize += 32;
  if (password.contains(QRegExp("[\\x80-\\xFF]")))
    poolSize += 128;

  double entropy = length * log2(poolSize);
  int score = static_cast<int>(entropy);
  return qMin(score, 100);
}

QString PasswordGenerator::strengthDescription(int score) {
  if (score < 30)
    return "Weak";
  if (score < 60)
    return "Moderate";
  if (score < 80)
    return "Strong";
  return "Very Strong";
}

PasswordGenerator::PasswordSpec PasswordGenerator::defaultSpec() {
  return PasswordSpec(); // already has default values
}
