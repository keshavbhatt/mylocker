#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <QString>

class PasswordGenerator {

public:
  struct PasswordSpec {
    int length = 16;
    bool useUppercase = true;
    bool useLowercase = true;
    bool useDigits = true;
    bool useSymbols = false;
    bool useExtended = false;
  };

  static QString generate(const PasswordSpec &spec);
  static int calculateStrength(const QString &password);
  static QString strengthDescription(int score);
  static PasswordSpec defaultSpec();
};

#endif // PASSWORDGENERATOR_H
