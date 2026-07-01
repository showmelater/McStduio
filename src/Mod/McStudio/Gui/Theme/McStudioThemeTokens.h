// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QColor>
#include <QString>

namespace McStudioGui
{

enum class McStudioThemeMode
{
    FollowFreeCAD,
    Light,
    Dark,
};

struct McStudioThemeTokens
{
    McStudioThemeMode mode {McStudioThemeMode::FollowFreeCAD};
    bool dark {false};

    QColor bgRibbon;
    QColor bgSurface;
    QColor bgPanel;
    QColor bgHover;
    QColor bgPressed;
    QColor borderSubtle;
    QColor textPrimary;
    QColor textSecondary;
    QColor textDisabled;
    QColor accent;
    QColor accentHover;
    QColor accentPressed;

    int baseIconSize {24};
    int smallIconSize {24};
    int largeIconSize {40};
    int fontLineHeight {14};

    int tabBarHeight {32};
    int titleBarHeight {0};
    int categoryHeight {118};
    int panelSpacing {8};
    int panelTitleHeight {22};

    int panelMinWidth {88};
    int buttonMinWidth {56};
    int tabMinWidth {72};
    int tabHorizontalPadding {14};

    double buttonMaximumAspectRatio {1.65};
};

}  // namespace McStudioGui
