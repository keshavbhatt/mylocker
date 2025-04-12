#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

class Palette {
public:
  // Primary colors
  static QColor primary();
  static QColor primaryDark();
  static QColor primaryLight();

  // Accent colors
  static QColor accent();
  static QColor success();
  static QColor warning();
  static QColor error();
  static QColor info();

  // Neutral tones
  static QColor background();
  static QColor surface();
  static QColor textPrimary();
  static QColor textSecondary();

  // Icon colors
  static QColor iconDefault();
  static QColor iconSuccess();
  static QColor iconWarning();
  static QColor iconError();
  static QColor iconInfo();
  static QColor iconPrimary();
  static QColor iconSecondary();
  static QColor iconAccent();
  static QColor iconNeutral();
  static QColor iconHighlight();
  static QColor iconMuted();
  static QColor iconLight();
  static QColor iconDark();
};
#endif // PALETTE_H
