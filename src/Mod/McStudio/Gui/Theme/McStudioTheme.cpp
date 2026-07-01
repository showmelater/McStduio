// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"
#include "McStudioTheme.h"

#include <algorithm>
#include <cmath>

#include <QApplication>
#include <QFile>
#include <QFontMetrics>
#include <QPalette>
#include <QString>
#include <QTextStream>

#include <App/Application.h>
#include <Base/Console.h>
#include <Base/Parameter.h>
#include <Gui/MainWindow.h>
#include <Gui/ToolBarManager.h>

#include <SARibbonBar.h>
#include <SARibbonButtonGroupWidget.h>
#include <SARibbonQuickAccessBar.h>
#include <SARibbonThemeManager.h>

namespace McStudioGui
{
namespace
{
constexpr const char* McStudioUIPreferencePath = "User parameter:BaseApp/Preferences/Mod/McStudio/UI";
constexpr const char* SARibbonOverlayPath = ":/Mod/McStudio/styles/McStudioSARibbon.qss";

QString colorName(const QColor& color)
{
    return color.name(QColor::HexRgb);
}

bool applicationPaletteIsDark()
{
    const QPalette palette = QApplication::palette();
    const QColor window = palette.color(QPalette::Window);
    const QColor text = palette.color(QPalette::WindowText);
    return window.lightness() < text.lightness();
}

McStudioThemeMode themeModeFromPreference()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(McStudioUIPreferencePath);
    const std::string mode = hGrp->GetASCII("ThemeMode", "FollowFreeCAD");

    if (mode == "Light") {
        return McStudioThemeMode::Light;
    }
    if (mode == "Dark") {
        return McStudioThemeMode::Dark;
    }
    return McStudioThemeMode::FollowFreeCAD;
}

bool effectiveDark(McStudioThemeMode mode)
{
    switch (mode) {
        case McStudioThemeMode::Light:
            return false;
        case McStudioThemeMode::Dark:
            return true;
        case McStudioThemeMode::FollowFreeCAD:
        default:
            return applicationPaletteIsDark();
    }
}

int freeCADBaseToolbarIconSize()
{
    if (auto* manager = Gui::ToolBarManager::getInstance()) {
        return std::clamp(manager->toolBarIconSize(), 16, 64);
    }

    if (Gui::MainWindow* mainWindow = Gui::getMainWindow()) {
        return std::clamp(mainWindow->iconSize().width(), 16, 64);
    }

    return 24;
}

QString replaceToken(QString qss, const QString& token, const QColor& color)
{
    qss.replace(QStringLiteral("@%1").arg(token), colorName(color));
    return qss;
}

QString replaceToken(QString qss, const QString& token, int value)
{
    qss.replace(QStringLiteral("@%1").arg(token), QString::number(value));
    return qss;
}
}  // namespace

McStudioThemeTokens currentMcStudioThemeTokens()
{
    McStudioThemeTokens tokens;
    tokens.mode = themeModeFromPreference();
    tokens.dark = effectiveDark(tokens.mode);

    if (tokens.dark) {
        tokens.bgRibbon = QColor(QStringLiteral("#25282d"));
        tokens.bgSurface = QColor(QStringLiteral("#2f3339"));
        tokens.bgPanel = QColor(QStringLiteral("#30343a"));
        tokens.bgHover = QColor(QStringLiteral("#3a4658"));
        tokens.bgPressed = QColor(QStringLiteral("#274767"));
        tokens.borderSubtle = QColor(QStringLiteral("#454b54"));
        tokens.textPrimary = QColor(QStringLiteral("#edf1f5"));
        tokens.textSecondary = QColor(QStringLiteral("#b8c0cc"));
        tokens.textDisabled = QColor(QStringLiteral("#767f8b"));
        tokens.accent = QColor(QStringLiteral("#4c8ed9"));
        tokens.accentHover = QColor(QStringLiteral("#5b9eea"));
        tokens.accentPressed = QColor(QStringLiteral("#3475bf"));
    }
    else {
        tokens.bgRibbon = QColor(QStringLiteral("#f4f6f8"));
        tokens.bgSurface = QColor(QStringLiteral("#ffffff"));
        tokens.bgPanel = QColor(QStringLiteral("#f8f9fb"));
        tokens.bgHover = QColor(QStringLiteral("#e7f0fb"));
        tokens.bgPressed = QColor(QStringLiteral("#d7e7f8"));
        tokens.borderSubtle = QColor(QStringLiteral("#d7dce2"));
        tokens.textPrimary = QColor(QStringLiteral("#202327"));
        tokens.textSecondary = QColor(QStringLiteral("#69717d"));
        tokens.textDisabled = QColor(QStringLiteral("#a5adb8"));
        tokens.accent = QColor(QStringLiteral("#2f7fd1"));
        tokens.accentHover = QColor(QStringLiteral("#3f8fe1"));
        tokens.accentPressed = QColor(QStringLiteral("#1f6fbd"));
    }

    const QFontMetrics fontMetrics(QApplication::font());
    tokens.baseIconSize = freeCADBaseToolbarIconSize();
    tokens.fontLineHeight = fontMetrics.lineSpacing();
    // Root-cause fix for the "small icon left-edge clipped" bug:
    // In LooseThreeRow the per-row height for small buttons is ~1/3 of the
    // panel content area (~27px with the compact categoryHeight). SARibbon's
    // calcSmallButtonDrawRects() shrinks the iconRect to fit that row height
    // (adjustIconSize by height), but paintIcon() still renders the pixmap at
    // the *unshrunk* realIconSize() and centers it — so an oversized icon
    // overflows the narrowed rect left/right and gets clipped. Keep the small
    // icon comfortably below the row height (cap at 20, and never exceed the
    // FreeCAD base size) so the icon always fits without height-driven shrink.
    tokens.smallIconSize = std::clamp(tokens.baseIconSize, 16, 20);
    // Compact large icon sizing to match SolidWorks (~28-36px), was 32-48.
    tokens.largeIconSize = std::clamp(
        static_cast<int>(std::lround(tokens.baseIconSize * 1.5)),
        28,
        36
    );

    tokens.tabBarHeight = std::clamp(tokens.fontLineHeight + 16, 28, 36);
    tokens.titleBarHeight = 0;
    // Thinner panel title row (was 20-26).
    tokens.panelTitleHeight = std::clamp(tokens.fontLineHeight + 4, 16, 22);
    // Tighter in-panel spacing (was 6-12).
    tokens.panelSpacing = std::clamp(tokens.baseIconSize / 4, 4, 8);
    // Shorter, more compact category area (was 104-144).
    tokens.categoryHeight = std::clamp(
        tokens.largeIconSize + tokens.panelTitleHeight + tokens.fontLineHeight * 3 + 12,
        96,
        128
    );

    tokens.panelMinWidth = std::clamp(tokens.largeIconSize * 2 + 8, 80, 120);
    // Narrower medium buttons (was 48-72).
    tokens.buttonMinWidth = std::clamp(tokens.smallIconSize * 2 + 8, 40, 60);
    tokens.tabMinWidth = std::clamp(tokens.fontLineHeight * 5, 60, 88);
    tokens.tabHorizontalPadding = std::clamp(tokens.fontLineHeight, 12, 18);
    tokens.buttonMaximumAspectRatio = tokens.baseIconSize >= 32 ? 1.75 : 1.65;
    return tokens;
}

SARibbonTheme ribbonThemeForTokens(const McStudioThemeTokens& tokens)
{
    return tokens.dark ? SARibbonTheme::RibbonThemeOffice2021Dark
                       : SARibbonTheme::RibbonThemeOffice2021Blue;
}

QString loadMcStudioSARibbonOverlay(const McStudioThemeTokens& tokens)
{
    QFile file(QString::fromLatin1(SARibbonOverlayPath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Base::Console().warning("McStudio theme: failed to load SARibbon overlay '%s'.\n", SARibbonOverlayPath);
        return QString();
    }

    QString qss = QString::fromUtf8(file.readAll());
    qss = replaceToken(qss, QStringLiteral("mc-bg-ribbon"), tokens.bgRibbon);
    qss = replaceToken(qss, QStringLiteral("mc-bg-surface"), tokens.bgSurface);
    qss = replaceToken(qss, QStringLiteral("mc-bg-panel"), tokens.bgPanel);
    qss = replaceToken(qss, QStringLiteral("mc-bg-hover"), tokens.bgHover);
    qss = replaceToken(qss, QStringLiteral("mc-bg-pressed"), tokens.bgPressed);
    qss = replaceToken(qss, QStringLiteral("mc-border-subtle"), tokens.borderSubtle);
    qss = replaceToken(qss, QStringLiteral("mc-text-primary"), tokens.textPrimary);
    qss = replaceToken(qss, QStringLiteral("mc-text-secondary"), tokens.textSecondary);
    qss = replaceToken(qss, QStringLiteral("mc-text-disabled"), tokens.textDisabled);
    qss = replaceToken(qss, QStringLiteral("mc-accent"), tokens.accent);
    qss = replaceToken(qss, QStringLiteral("mc-accent-hover"), tokens.accentHover);
    qss = replaceToken(qss, QStringLiteral("mc-accent-pressed"), tokens.accentPressed);
    qss = replaceToken(qss, QStringLiteral("mc-panel-title-height"), tokens.panelTitleHeight);
    qss = replaceToken(qss, QStringLiteral("mc-panel-min-width"), tokens.panelMinWidth);
    qss = replaceToken(qss, QStringLiteral("mc-button-min-width"), tokens.buttonMinWidth);
    qss = replaceToken(qss, QStringLiteral("mc-tab-min-width"), tokens.tabMinWidth);
    qss = replaceToken(qss, QStringLiteral("mc-tab-padding-h"), tokens.tabHorizontalPadding);
    return qss;
}

void applyMcStudioSARibbonTheme(SARibbonBar* ribbon)
{
    if (!ribbon) {
        return;
    }

    const McStudioThemeTokens tokens = currentMcStudioThemeTokens();

    SA::applyRibbonTheme(ribbon, ribbon, ribbonThemeForTokens(tokens));

    // FreeCAD embeds SARibbonBar inside a top QDockWidget, not inside SARibbonMainWindow.
    // Compact styles expect tabs on the title area and are unstable when titleBarHeight is 0.
    // Use loose mode with explicit compact heights: no SARibbon titlebar, visible category tabs,
    // and a bounded category/panel area.
    ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
    ribbon->setContentsMargins(0, 0, 0, 0);
    ribbon->setTitleBarHeight(tokens.titleBarHeight);
    ribbon->setTabOnTitle(false);
    ribbon->setTabBarHeight(tokens.tabBarHeight);
    ribbon->setCategoryHeight(tokens.categoryHeight);
    ribbon->setEnableWordWrap(true);
    ribbon->setEnableShowPanelTitle(true);
    ribbon->setRibbonAlignment(SARibbonAlignment::AlignLeft);
    ribbon->setPanelSpacing(tokens.panelSpacing);
    ribbon->setPanelTitleHeight(tokens.panelTitleHeight);
    ribbon->setButtonMaximumAspectRatio(tokens.buttonMaximumAspectRatio);
    ribbon->setTabBarBaseLineColor(QColor());
    ribbon->setTitleIconVisible(false);
    ribbon->setApplicationButton(nullptr);
    if (SARibbonQuickAccessBar* quickAccess = ribbon->quickAccessBar()) {
        quickAccess->hide();
        quickAccess->setFixedWidth(0);
    }
    if (SARibbonButtonGroupWidget* rightButtons = ribbon->rightButtonGroup()) {
        rightButtons->hide();
        rightButtons->setFixedWidth(0);
    }

    const QString overlay = loadMcStudioSARibbonOverlay(tokens);
    if (!overlay.isEmpty()) {
        ribbon->setStyleSheet(ribbon->styleSheet() + QLatin1Char('\n') + overlay);
    }
}

}  // namespace McStudioGui
