#include "palette.h"

QColor Palette::primary() { return QColor("#0078D4"); } // Blue
QColor Palette::primaryDark() { return QColor("#005A9E"); }
QColor Palette::primaryLight() { return QColor("#E5F1FB"); }

QColor Palette::accent() { return QColor("#FF4081"); }  // Pink
QColor Palette::success() { return QColor("#28A745"); } // Green
QColor Palette::warning() { return QColor("#FFC107"); } // Amber
QColor Palette::error() { return QColor("#DC3545"); }   // Red
QColor Palette::info() { return QColor("#17A2B8"); }    // Cyan

QColor Palette::background() { return QColor("#F5F5F5"); }
QColor Palette::surface() { return QColor("#FFFFFF"); }
QColor Palette::textPrimary() { return QColor("#212121"); }
QColor Palette::textSecondary() { return QColor("#757575"); }

QColor Palette::iconDefault() { return QColor("#424242"); } // Neutral gray
QColor Palette::iconSuccess() { return success(); }
QColor Palette::iconWarning() { return warning(); }
QColor Palette::iconError() { return error(); }
QColor Palette::iconInfo() { return QColor("#0288D1"); }      // Cyan Blue
QColor Palette::iconPrimary() { return QColor("#1976D2"); }   // Blue
QColor Palette::iconSecondary() { return QColor("#7B1FA2"); } // Purple
QColor Palette::iconAccent() { return QColor("#FF4081"); }    // Pink Accent
QColor Palette::iconNeutral() { return QColor("#607D8B"); }   // Blue Gray
QColor Palette::iconHighlight() { return QColor("#FF6F00"); } // Orange
QColor Palette::iconMuted() { return QColor("#9E9E9E"); }     // Muted Gray
QColor Palette::iconLight() { return QColor("#FAFAFA"); }     // Very light gray
QColor Palette::iconDark() { return QColor("#212121"); }      // Almost black
