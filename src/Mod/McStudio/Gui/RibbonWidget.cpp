// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"
#include "RibbonWidget.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <string>

#include <QAction>
#include <QCoreApplication>
#include <QDockWidget>
#include <QEvent>
#include <QFile>
#include <QFrame>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QPointer>
#include <QRect>
#include <QScrollArea>
#include <QScrollBar>
#include <QSize>
#include <QSizePolicy>
#include <QStyle>
#include <QTabWidget>
#include <QTextStream>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <App/Application.h>
#include <Base/Console.h>
#include <Base/Parameter.h>
#include <Gui/Action.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/MainWindow.h>
#include <Gui/Workbench.h>
#include <Gui/WorkbenchManager.h>

#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <SARibbonPanel.h>

#include "RibbonModel.h"
#include "RibbonBackends/McStudioSARibbonBackend.h"
#include "Theme/McStudioTheme.h"

namespace McStudioGui
{
namespace
{
constexpr const char* RibbonDockObjectName = "McStudioRibbonDockHost";
constexpr const char* RibbonHostObjectName = "McStudioRibbonHost";
constexpr const char* TopControlBarObjectName = "McStudioTopControlBar";
constexpr const char* BrandLabelObjectName = "McStudioBrandLabel";
constexpr const char* BrandLogoObjectName = "McStudioBrandLogo";
constexpr const char* WorkbenchSwitcherObjectName = "McStudioWorkbenchSwitcher";
constexpr const char* QuickAccessBarObjectName = "McStudioQuickAccessBar";
constexpr const char* TopSearchObjectName = "McStudioTopSearch";
constexpr const char* RibbonStyleSheetPath = ":/Mod/McStudio/styles/McStudioRibbon.qss";
constexpr const char* RibbonPreferencePath = "User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon";
constexpr const char* RibbonTranslationContext = "McStudioGui::Ribbon";
constexpr double DefaultRibbonScale = 1.0;
constexpr double MinRibbonScale = 0.70;
constexpr double MaxRibbonScale = 1.80;

struct RibbonMetrics
{
    double scale {DefaultRibbonScale};
    int largeIconSize {32};
    int smallIconSize {24};
    int groupMarginH {6};
    int groupMarginV {5};
    int pageMargin {6};
    int spacing {6};
    int topControlBarHeight {40};
    int ribbonMaxHeight {100};
    int pageMaxHeight {74};
};

int scaledInt(int value, double scale)
{
    return std::max(1, static_cast<int>(std::lround(static_cast<double>(value) * scale)));
}


QPoint mouseGlobalPosition(QMouseEvent* event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return event->globalPosition().toPoint();
#else
    return event->globalPos();
#endif
}

class TopControlDragFilter : public QObject
{
public:
    explicit TopControlDragFilter(QWidget* window, QObject* parent = nullptr)
        : QObject(parent)
        , _window(window)
    {}

protected:
    bool eventFilter(QObject* watched, QEvent* event) override
    {
        Q_UNUSED(watched);
        if (!_window || !event) {
            return QObject::eventFilter(watched, event);
        }

        if (event->type() == QEvent::MouseButtonDblClick) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                _window->isMaximized() ? _window->showNormal() : _window->showMaximized();
                return true;
            }
        }

        if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                _dragging = true;
                _pressGlobal = mouseGlobalPosition(mouseEvent);
                _windowTopLeft = _window->frameGeometry().topLeft();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            if (_dragging && (mouseEvent->buttons() & Qt::LeftButton)) {
                if (!_window->isMaximized() && !_window->isFullScreen()) {
                    _window->move(_windowTopLeft + mouseGlobalPosition(mouseEvent) - _pressGlobal);
                }
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            _dragging = false;
        }

        return QObject::eventFilter(watched, event);
    }

private:
    QPointer<QWidget> _window;
    bool _dragging {false};
    QPoint _pressGlobal;
    QPoint _windowTopLeft;
};

double ribbonScale()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(RibbonPreferencePath);
    const double scale = hGrp->GetFloat("Scale", DefaultRibbonScale);
    return std::clamp(scale, MinRibbonScale, MaxRibbonScale);
}


RibbonMetrics ribbonMetrics()
{
    RibbonMetrics metrics;
    metrics.scale = ribbonScale();
    const McStudioThemeTokens tokens = currentMcStudioThemeTokens();
    metrics.smallIconSize = scaledInt(tokens.smallIconSize, metrics.scale);
    metrics.largeIconSize = scaledInt(tokens.largeIconSize, metrics.scale);
    metrics.groupMarginH = scaledInt(tokens.panelSpacing, metrics.scale);
    metrics.groupMarginV = scaledInt(std::max(4, tokens.panelSpacing / 2), metrics.scale);
    metrics.pageMargin = scaledInt(tokens.panelSpacing, metrics.scale);
    metrics.spacing = scaledInt(tokens.panelSpacing, metrics.scale);
    metrics.pageMaxHeight = scaledInt(tokens.categoryHeight, metrics.scale);
    metrics.ribbonMaxHeight = metrics.pageMaxHeight + scaledInt(tokens.tabBarHeight, metrics.scale);
    metrics.topControlBarHeight = scaledInt(std::clamp(tokens.fontLineHeight + 38, 52, 62), metrics.scale);
    return metrics;
}

QFont scaledFont(const QFont& baseFont, double scale)
{
    QFont font = baseFont;
    if (scale == DefaultRibbonScale) {
        return font;
    }

    if (font.pointSizeF() > 0.0) {
        font.setPointSizeF(std::max(1.0, font.pointSizeF() * scale));
    }
    else if (font.pixelSize() > 0) {
        font.setPixelSize(scaledInt(font.pixelSize(), scale));
    }
    return font;
}

QString ribbonText(const char* text)
{
    return text ? QCoreApplication::translate(RibbonTranslationContext, text) : QString();
}

int preferredFixedHeight(QWidget* widget, int fallbackHeight)
{
    if (!widget) {
        return fallbackHeight;
    }

    // Hidden Qt/SARibbon widgets can report a very small sizeHint during the
    // first layout pass. Use the largest stable fixed-height candidate instead
    // of returning the first positive value. Do not include widget->height()
    // here: hidden widgets can still carry stale construction-time geometry.
    int best = fallbackHeight;
    const int candidates[] = {
        widget->sizeHint().height(),
        widget->minimumSizeHint().height(),
        widget->minimumHeight(),
    };

    for (int height : candidates) {
        if (height > 0 && height < QWIDGETSIZE_MAX) {
            best = std::max(best, height);
        }
    }

    return best;
}

int preferredRibbonBackendHeight(QWidget* widget, int fallbackHeight)
{
    int height = preferredFixedHeight(widget, fallbackHeight);
    if (auto* ribbon = qobject_cast<SARibbonBar*>(widget)) {
        const int explicitRibbonHeight = ribbon->titleBarHeight() + ribbon->tabBarHeight() + ribbon->categoryHeight();
        if (explicitRibbonHeight > 0 && explicitRibbonHeight < QWIDGETSIZE_MAX) {
            height = std::max(height, explicitRibbonHeight);
        }
    }
    return height;
}

void relaxHorizontalSize(QWidget* widget)
{
    if (!widget) {
        return;
    }

    widget->setMinimumWidth(0);
    widget->setMaximumWidth(QWIDGETSIZE_MAX);
}

QString ribbonStyleSheet()
{
    QFile file(QString::fromLatin1(RibbonStyleSheetPath));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        Base::Console().warning("McStudio Ribbon: cannot load stylesheet '%s'.\n",
                                RibbonStyleSheetPath);
        return {};
    }

    QTextStream stream(&file);
    return stream.readAll();
}

QString normalizedObjectName(const char* title, const char* suffix)
{
    QString name = QString::fromUtf8(title);
    name.remove(QLatin1Char(' '));
    name.remove(QLatin1Char('/'));
    name.append(QLatin1Char('_'));
    name.append(QString::fromLatin1(suffix));
    return name;
}

QWidget* createCommandStrip(QWidget* parent,
                            const QString& objectName,
                            const char* ribbonRole,
                            const RibbonMetrics& metrics)
{
    auto* strip = new QWidget(parent);
    strip->setObjectName(objectName);
    strip->setProperty("class", QStringLiteral("McStudioRibbonCommandStrip"));
    strip->setProperty("ribbonRole", QString::fromLatin1(ribbonRole));
    strip->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    strip->setFont(scaledFont(strip->font(), metrics.scale));

    auto* layout = new QHBoxLayout(strip);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(std::max(1, scaledInt(2, metrics.scale)));
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    return strip;
}

QFrame* createCommandSeparator(QWidget* parent)
{
    auto* separator = new QFrame(parent);
    separator->setObjectName(QStringLiteral("McStudioRibbonCommandSeparator"));
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    return separator;
}


QFrame* createTopSeparator(QWidget* parent)
{
    auto* separator = new QFrame(parent);
    separator->setObjectName(QStringLiteral("McStudioTopSeparator"));
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    return separator;
}

QAction* commandActionForTopBar(const char* commandName, QWidget* initializer)
{
    if (!commandName || !Gui::Application::Instance) {
        return nullptr;
    }

    Gui::CommandManager& commandManager = Gui::Application::Instance->commandManager();
    Gui::Command* guiCommand = commandManager.getCommandByName(commandName);
    if (!guiCommand) {
        Base::Console().warning("McStudio TopBar: skipping unknown command '%s'.\n", commandName);
        return nullptr;
    }

    commandManager.addTo(commandName, initializer);
    Gui::Action* commandAction = guiCommand->getAction();
    return commandAction ? commandAction->action() : nullptr;
}

QToolButton* createTopCommandButton(QWidget* parent,
                                    const char* commandName,
                                    const QString& fallbackText,
                                    int iconSize)
{
    auto* button = new QToolButton(parent);
    button->setObjectName(QStringLiteral("McStudioTopCommandButton"));
    button->setProperty("mcStudioTopButton", true);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setIconSize(QSize(iconSize, iconSize));
    button->setFixedSize(std::max(40, iconSize + 16), std::max(40, iconSize + 16));
    button->setFocusPolicy(Qt::TabFocus);

    QAction* action = commandActionForTopBar(commandName, parent);
    if (action) {
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button->setIconSize(QSize(iconSize, iconSize));
        button->setToolTip(action->toolTip().isEmpty() ? action->text() : action->toolTip());
    }
    else {
        button->setText(fallbackText);
        button->setToolTip(fallbackText);
        button->setEnabled(false);
    }

    return button;
}

QToolButton* createWorkbenchSwitcher(QWidget* parent, const RibbonMetrics& metrics)
{
    auto* switcher = new QToolButton(parent);
    switcher->setObjectName(QString::fromLatin1(WorkbenchSwitcherObjectName));
    switcher->setProperty("mcStudioWorkbenchSwitcher", true);
    switcher->setAutoRaise(false);
    switcher->setPopupMode(QToolButton::InstantPopup);
    switcher->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    switcher->setText(QStringLiteral("CAD"));
    switcher->setToolTip(QObject::tr("Switch McStudio workbench mode"));
    switcher->setIcon(parent->style()->standardIcon(QStyle::SP_ComputerIcon));
    switcher->setIconSize(QSize(metrics.smallIconSize, metrics.smallIconSize));
    switcher->setMinimumWidth(scaledInt(122, metrics.scale));
    switcher->setFixedHeight(std::max(42, metrics.smallIconSize + 18));

    auto* menu = new QMenu(switcher);
    menu->setObjectName(QStringLiteral("McStudioWorkbenchSwitcherMenu"));

    QAction* cadAction = menu->addAction(QStringLiteral("CAD"));
    cadAction->setCheckable(true);
    cadAction->setChecked(true);
    cadAction->setToolTip(QObject::tr("CAD design workspace"));

    QAction* camAction = menu->addAction(QStringLiteral("CAM"));
    camAction->setCheckable(true);
    camAction->setToolTip(QObject::tr("CAM manufacturing workspace preview"));

    QAction* machineAction = menu->addAction(QStringLiteral("Machine"));
    machineAction->setCheckable(true);
    machineAction->setToolTip(QObject::tr("Machine control workspace preview"));

    auto applyMode = [switcher, cadAction, camAction, machineAction](const QString& mode) {
        switcher->setText(mode);
        cadAction->setChecked(mode == QLatin1String("CAD"));
        camAction->setChecked(mode == QLatin1String("CAM"));
        machineAction->setChecked(mode == QLatin1String("Machine"));

        ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(
            "User parameter:BaseApp/Preferences/Mod/McStudio/Shell"
        );
        hGrp->SetASCII("ActiveWorkbenchMode", mode.toUtf8().constData());
        Base::Console().message("McStudio TopBar: requested workbench mode '%s'.\n", mode.toUtf8().constData());
    };

    QObject::connect(cadAction, &QAction::triggered, switcher, [applyMode] { applyMode(QStringLiteral("CAD")); });
    QObject::connect(camAction, &QAction::triggered, switcher, [applyMode] { applyMode(QStringLiteral("CAM")); });
    QObject::connect(machineAction, &QAction::triggered, switcher, [applyMode] { applyMode(QStringLiteral("Machine")); });

    switcher->setMenu(menu);
    return switcher;
}

QWidget* createQuickAccessBar(QWidget* parent, const RibbonMetrics& metrics)
{
    auto* quick = new QWidget(parent);
    quick->setObjectName(QString::fromLatin1(QuickAccessBarObjectName));
    quick->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    auto* layout = new QHBoxLayout(quick);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(std::max(3, scaledInt(8, metrics.scale)));
    layout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    struct QuickCommand
    {
        const char* command;
        const char* label;
    };

    static constexpr QuickCommand commands[] = {
        {"Std_ViewHome", "Home"},
        {"Std_New", "New"},
        {"Std_Open", "Open"},
        {"Std_Save", "Save"},
        {"Std_Refresh", "Refresh"},
        {"Std_Undo", "Undo"},
        {"Std_Redo", "Redo"},
    };

    for (const QuickCommand& command : commands) {
        layout->addWidget(createTopCommandButton(quick,
                                                 command.command,
                                                 QCoreApplication::translate(RibbonTranslationContext, command.label),
                                                 metrics.smallIconSize));
    }

    return quick;
}

QToolButton* createWindowControlButton(QWidget* parent,
                                      const QString& objectName,
                                      QStyle::StandardPixmap icon,
                                      const QString& tooltip,
                                      const std::function<void()>& slot)
{
    auto* button = new QToolButton(parent);
    button->setObjectName(objectName);
    button->setProperty("mcStudioTopButton", true);
    button->setProperty("mcStudioWindowButton", true);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setIcon(parent->style()->standardIcon(icon));
    button->setToolTip(tooltip);
    button->setFocusPolicy(Qt::TabFocus);
    QObject::connect(button, &QToolButton::clicked, button, slot);
    return button;
}

QWidget* createTopRightBar(QWidget* parent, const RibbonMetrics& metrics)
{
    auto* right = new QWidget(parent);
    right->setObjectName(QStringLiteral("McStudioTopRightBar"));
    right->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    auto* layout = new QHBoxLayout(right);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(std::max(3, scaledInt(6, metrics.scale)));
    layout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    layout->addWidget(createTopCommandButton(right,
                                             "Std_DlgCustomize",
                                             QObject::tr("Customize"),
                                             metrics.smallIconSize));
    layout->addWidget(createTopCommandButton(right,
                                             "McStudio_CompHelp",
                                             QObject::tr("Help"),
                                             metrics.smallIconSize));
    layout->addWidget(createTopSeparator(right));

    QPointer<Gui::MainWindow> mainWindow(Gui::getMainWindow());
    auto* minimize = createWindowControlButton(right,
                                               QStringLiteral("McStudioWindowMinimizeButton"),
                                               QStyle::SP_TitleBarMinButton,
                                               QObject::tr("Minimize"),
                                               [mainWindow] {
                                                   if (mainWindow) {
                                                       mainWindow->showMinimized();
                                                   }
                                               });
    auto* maximize = createWindowControlButton(right,
                                               QStringLiteral("McStudioWindowMaximizeButton"),
                                               QStyle::SP_TitleBarMaxButton,
                                               QObject::tr("Maximize / Restore"),
                                               [mainWindow] {
                                                   if (!mainWindow) {
                                                       return;
                                                   }
                                                   mainWindow->isMaximized() ? mainWindow->showNormal()
                                                                             : mainWindow->showMaximized();
                                               });
    auto* close = createWindowControlButton(right,
                                            QStringLiteral("McStudioWindowCloseButton"),
                                            QStyle::SP_TitleBarCloseButton,
                                            QObject::tr("Close"),
                                            [mainWindow] {
                                                if (mainWindow) {
                                                    mainWindow->close();
                                                }
                                            });

    const int buttonSize = std::max(40, metrics.smallIconSize + 16);
    for (QToolButton* button : {minimize, maximize, close}) {
        button->setIconSize(QSize(metrics.smallIconSize, metrics.smallIconSize));
        button->setFixedSize(buttonSize, buttonSize);
        layout->addWidget(button);
    }

    return right;
}

QFrame* createTopControlBar(QWidget* parent, const RibbonMetrics& metrics)
{
    auto* bar = new QFrame(parent);
    bar->setObjectName(QString::fromLatin1(TopControlBarObjectName));
    bar->setProperty("class", QStringLiteral("McStudioTopControlBar"));
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    bar->setMinimumHeight(metrics.topControlBarHeight);
    bar->setMaximumHeight(metrics.topControlBarHeight);
    bar->setFont(scaledFont(bar->font(), metrics.scale));

    auto* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(scaledInt(28, metrics.scale), 0, scaledInt(14, metrics.scale), 0);
    layout->setSpacing(std::max(6, scaledInt(10, metrics.scale)));
    layout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    auto* logo = new QLabel(QStringLiteral("M"), bar);
    logo->setObjectName(QString::fromLatin1(BrandLogoObjectName));
    logo->setFixedSize(scaledInt(30, metrics.scale), scaledInt(30, metrics.scale));
    logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    logo->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);

    auto* brand = new QLabel(QStringLiteral("McStudio"), bar);
    brand->setObjectName(QString::fromLatin1(BrandLabelObjectName));
    brand->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    brand->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    layout->addWidget(brand);

    if (Gui::MainWindow* mainWindow = Gui::getMainWindow()) {
        auto* dragFilter = new TopControlDragFilter(mainWindow, bar);
        bar->installEventFilter(dragFilter);
        logo->installEventFilter(dragFilter);
        brand->installEventFilter(dragFilter);
    }

    layout->addWidget(createWorkbenchSwitcher(bar, metrics));
    layout->addWidget(createTopSeparator(bar));
    layout->addWidget(createQuickAccessBar(bar, metrics));

    auto* search = new QLineEdit(bar);
    search->setObjectName(QString::fromLatin1(TopSearchObjectName));
    search->setPlaceholderText(QObject::tr("Search command  (Ctrl+/)"));
    search->setClearButtonEnabled(true);
    search->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    search->setMaximumWidth(scaledInt(380, metrics.scale));
    search->setMinimumWidth(scaledInt(260, metrics.scale));
    search->setFixedHeight(std::max(38, metrics.smallIconSize + 14));
    layout->addStretch(1);
    layout->addWidget(search);
    layout->addWidget(createTopRightBar(bar, metrics));

    return bar;
}

int topControlHeightForHost(QWidget* host, int fallbackHeight)
{
    if (!host) {
        return fallbackHeight;
    }
    QWidget* top = host->findChild<QWidget*>(QString::fromLatin1(TopControlBarObjectName), Qt::FindDirectChildrenOnly);
    return top ? preferredFixedHeight(top, fallbackHeight) : 0;
}

int preferredRibbonHostHeight(QWidget* host, const RibbonMetrics& metrics)
{
    if (!host) {
        return metrics.topControlBarHeight + metrics.ribbonMaxHeight;
    }

    const int topHeight = topControlHeightForHost(host, metrics.topControlBarHeight);
    auto* ribbon = host->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar"));
    const int ribbonHeight = ribbon ? preferredRibbonBackendHeight(ribbon, metrics.ribbonMaxHeight)
                                    : metrics.ribbonMaxHeight;
    return topHeight + ribbonHeight;
}

void refreshTopControlBar(QWidget* ribbonHost, const RibbonMetrics& metrics)
{
    if (!ribbonHost) {
        return;
    }

    QWidget* top = ribbonHost->findChild<QWidget*>(QString::fromLatin1(TopControlBarObjectName), Qt::FindDirectChildrenOnly);
    if (!top) {
        return;
    }

    top->setMinimumHeight(metrics.topControlBarHeight);
    top->setMaximumHeight(metrics.topControlBarHeight);
    top->setFont(scaledFont(top->font(), metrics.scale));

    const QList<QToolButton*> buttons = top->findChildren<QToolButton*>();
    for (QToolButton* button : buttons) {
        if (!button) {
            continue;
        }
        if (button->property("mcStudioTopButton").toBool()) {
            button->setIconSize(QSize(metrics.smallIconSize, metrics.smallIconSize));
            button->setFixedSize(std::max(40, metrics.smallIconSize + 16),
                                 std::max(40, metrics.smallIconSize + 16));
        }
    }
}

bool addCommand(QWidget* strip,
                const RibbonCommandDefinition& command,
                int iconSize,
                Qt::ToolButtonStyle style,
                const RibbonMetrics& metrics)
{
    if (!strip || !command.command || !Gui::Application::Instance) {
        return false;
    }

    auto* layout = qobject_cast<QHBoxLayout*>(strip->layout());
    if (!layout) {
        return false;
    }

    if (std::string(command.command) == "Separator") {
        layout->addWidget(createCommandSeparator(strip));
        return true;
    }

    Gui::CommandManager& commandManager = Gui::Application::Instance->commandManager();
    Gui::Command* guiCommand = commandManager.getCommandByName(command.command);
    if (!guiCommand) {
        Base::Console().warning("McStudio Ribbon: skipping unknown command '%s'.\n", command.command);
        return false;
    }

    if (!commandManager.addTo(command.command, strip)) {
        Base::Console().warning("McStudio Ribbon: failed to add command '%s'.\n", command.command);
        return false;
    }

    Gui::Action* commandAction = guiCommand->getAction();
    QAction* action = commandAction ? commandAction->action() : nullptr;
    if (!action) {
        Base::Console().warning("McStudio Ribbon: command '%s' has no QAction.\n", command.command);
        return false;
    }

    auto* button = new QToolButton(strip);
    button->setObjectName(QStringLiteral("McStudioRibbonCommandButton"));
    button->setAutoRaise(true);
    button->setIconSize(QSize(iconSize, iconSize));
    button->setToolButtonStyle(style);
    button->setFont(scaledFont(button->font(), metrics.scale));
    button->setDefaultAction(action);

    const QString displayText = command.label ? ribbonText(command.label) : action->text();
    if (command.label) {
        button->setText(displayText);
    }

    const QFontMetrics fontMetrics(button->font());
    if (style == Qt::ToolButtonTextUnderIcon) {
        const int minWidth = std::max(iconSize + scaledInt(28, metrics.scale),
                                      fontMetrics.horizontalAdvance(displayText) + scaledInt(18, metrics.scale));
        const int minHeight = iconSize + fontMetrics.height() + scaledInt(24, metrics.scale);
        button->setMinimumSize(minWidth, minHeight);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    }
    else if (style == Qt::ToolButtonTextBesideIcon) {
        const int minWidth = iconSize + fontMetrics.horizontalAdvance(displayText) + scaledInt(24, metrics.scale);
        const int minHeight = std::max(iconSize + scaledInt(8, metrics.scale),
                                       fontMetrics.height() + scaledInt(10, metrics.scale));
        button->setMinimumSize(minWidth, minHeight);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    }
    else {
        const int minSide = iconSize + scaledInt(10, metrics.scale);
        button->setMinimumSize(minSide, minSide);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    if (auto* actionGroup = dynamic_cast<Gui::ActionGroup*>(commandAction)) {
        const QList<QAction*> actions = actionGroup->actions();
        if (!actions.isEmpty()) {
            auto* menu = new QMenu(button);
            menu->addActions(actions);
            button->setMenu(menu);
            button->setPopupMode(QToolButton::MenuButtonPopup);
        }
    }
    else if (action->menu()) {
        button->setMenu(action->menu());
        button->setPopupMode(QToolButton::MenuButtonPopup);
    }

    layout->addWidget(button);

    return true;
}

bool isLargeCommand(const RibbonCommandDefinition& command)
{
    return command.size == RibbonCommandSize::Large;
}

bool isSmallTextCommand(const RibbonCommandDefinition& command)
{
    return command.size == RibbonCommandSize::Small
        && command.style == RibbonCommandStyle::TextBesideIcon;
}

bool isIconOnlyCommand(const RibbonCommandDefinition& command)
{
    return command.size == RibbonCommandSize::Small && command.style == RibbonCommandStyle::IconOnly;
}

template<typename Predicate>
QWidget* createFilteredCommandStrip(QWidget* parent,
                                    const RibbonGroupDefinition& group,
                                    const RibbonMetrics& metrics,
                                    const char* objectSuffix,
                                    const char* ribbonRole,
                                    int iconSize,
                                    Qt::ToolButtonStyle style,
                                    Predicate predicate)
{
    auto* strip = createCommandStrip(parent,
                                     normalizedObjectName(group.title, objectSuffix),
                                     ribbonRole,
                                     metrics);

    int added = 0;
    for (const RibbonCommandDefinition& command : group.commands) {
        if (predicate(command) && addCommand(strip, command, iconSize, style, metrics)) {
            ++added;
        }
    }

    if (added == 0) {
        delete strip;
        return nullptr;
    }

    return strip;
}

QWidget* createRibbonGroup(QWidget* parent, const RibbonGroupDefinition& group, const RibbonMetrics& metrics)
{
    auto* frame = new QFrame(parent);
    frame->setObjectName(QStringLiteral("McStudioRibbonGroup"));
    frame->setProperty("class", QStringLiteral("McStudioRibbonGroup"));
    frame->setFrameShape(QFrame::NoFrame);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    frame->setMinimumHeight(metrics.pageMaxHeight - (2 * metrics.pageMargin));
    frame->setMaximumHeight(metrics.pageMaxHeight - (2 * metrics.pageMargin));

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(metrics.groupMarginH, metrics.groupMarginV, metrics.groupMarginH, metrics.groupMarginV);
    layout->setSpacing(std::max(1, metrics.groupMarginV));
    layout->setAlignment(Qt::AlignTop);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    auto* commandRow = new QWidget(frame);
    commandRow->setObjectName(QStringLiteral("McStudioRibbonCommandRow"));
    commandRow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    auto* commandLayout = new QHBoxLayout(commandRow);
    commandLayout->setContentsMargins(0, 0, 0, 0);
    commandLayout->setSpacing(metrics.spacing);
    commandLayout->setAlignment(Qt::AlignTop);

    if (auto* largeBar = createFilteredCommandStrip(commandRow,
                                                    group,
                                                    metrics,
                                                    "large",
                                                    "large",
                                                    metrics.largeIconSize,
                                                    Qt::ToolButtonTextUnderIcon,
                                                    isLargeCommand)) {
        commandLayout->addWidget(largeBar);
    }

    auto* secondaryColumn = new QWidget(commandRow);
    secondaryColumn->setObjectName(QStringLiteral("McStudioRibbonSecondaryColumn"));
    secondaryColumn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    auto* secondaryLayout = new QVBoxLayout(secondaryColumn);
    secondaryLayout->setContentsMargins(0, 0, 0, 0);
    secondaryLayout->setSpacing(std::max(1, metrics.groupMarginV));
    secondaryLayout->setAlignment(Qt::AlignTop);
    secondaryLayout->setSizeConstraint(QLayout::SetMinimumSize);

    int secondaryBars = 0;
    if (auto* smallBar = createFilteredCommandStrip(secondaryColumn,
                                                    group,
                                                    metrics,
                                                    "small",
                                                    "small",
                                                    metrics.smallIconSize,
                                                    Qt::ToolButtonTextBesideIcon,
                                                    isSmallTextCommand)) {
        secondaryLayout->addWidget(smallBar);
        ++secondaryBars;
    }

    if (auto* iconBar = createFilteredCommandStrip(secondaryColumn,
                                                   group,
                                                   metrics,
                                                   "icons",
                                                   "icon",
                                                   metrics.smallIconSize,
                                                   Qt::ToolButtonIconOnly,
                                                   isIconOnlyCommand)) {
        secondaryLayout->addWidget(iconBar);
        ++secondaryBars;
    }

    if (secondaryBars > 0) {
        commandLayout->addWidget(secondaryColumn);
    }
    else {
        delete secondaryColumn;
    }

    auto* label = new QLabel(ribbonText(group.title), frame);
    label->setObjectName(QStringLiteral("McStudioRibbonGroupLabel"));
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    label->setFont(scaledFont(label->font(), metrics.scale));

    layout->addWidget(commandRow);
    layout->addWidget(label);
    return frame;
}

QWidget* createRibbonPage(QWidget* parent, const RibbonTabDefinition& tab, const RibbonMetrics& metrics)
{
    auto* page = new QScrollArea(parent);
    page->setObjectName(QStringLiteral("McStudioRibbonPage"));
    page->setFrameShape(QFrame::NoFrame);
    page->setWidgetResizable(true);
    page->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    page->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (QScrollBar* verticalBar = page->verticalScrollBar()) {
        verticalBar->setEnabled(false);
        verticalBar->setRange(0, 0);
    }
    page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    page->setMinimumHeight(metrics.pageMaxHeight);
    page->setMaximumHeight(metrics.pageMaxHeight);

    auto* content = new QWidget(page);
    content->setObjectName(QStringLiteral("McStudioRibbonPageContent"));
    content->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    content->setMinimumHeight(metrics.pageMaxHeight);
    content->setMaximumHeight(metrics.pageMaxHeight);
    auto* layout = new QHBoxLayout(content);
    layout->setContentsMargins(metrics.pageMargin, metrics.pageMargin, metrics.pageMargin, metrics.pageMargin);
    layout->setSpacing(metrics.spacing);
    layout->setAlignment(Qt::AlignTop);

    for (const RibbonGroupDefinition& group : tab.groups) {
        layout->addWidget(createRibbonGroup(content, group, metrics));
    }

    page->setWidget(content);
    return page;
}

QWidget* createRibbonWidget(QWidget* parent)
{
    const RibbonMetrics metrics = ribbonMetrics();

    auto* tabs = new QTabWidget(parent);
    tabs->setObjectName(QStringLiteral("McStudioRibbonTabs"));
    tabs->setDocumentMode(true);
    tabs->setTabPosition(QTabWidget::North);
    tabs->setUsesScrollButtons(true);
    tabs->setElideMode(Qt::ElideRight);
    tabs->setMovable(false);
    tabs->setFont(scaledFont(tabs->font(), metrics.scale));
    tabs->setAccessibleName(QObject::tr("McStudio Ribbon tabs"));
    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tabs->setMaximumHeight(metrics.ribbonMaxHeight);

    int sketchIndex = -1;
    const auto& modelTabs = ribbonTabs();
    for (int index = 0; index < static_cast<int>(modelTabs.size()); ++index) {
        const RibbonTabDefinition& tab = modelTabs[static_cast<std::size_t>(index)];
        tabs->addTab(createRibbonPage(tabs, tab, metrics), ribbonText(tab.title));
        if (tab.title && std::strcmp(tab.title, QT_TR_NOOP("Sketch")) == 0) {
            sketchIndex = index;
        }
    }

    if (sketchIndex >= 0) {
        tabs->setCurrentIndex(sketchIndex);
    }

    return tabs;
}

QWidget* createRibbonHost(QWidget* parent, const RibbonMetrics& metrics)
{
    auto* host = new QFrame(parent);
    host->setObjectName(QString::fromLatin1(RibbonHostObjectName));
    host->setProperty("class", QStringLiteral("McStudioRibbonHost"));
    host->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    relaxHorizontalSize(host);
    host->setWindowTitle(QObject::tr("McStudio Ribbon"));
    host->setFont(scaledFont(host->font(), metrics.scale));
    host->setStyleSheet(ribbonStyleSheet());
    host->setAccessibleName(QObject::tr("McStudio Ribbon"));

    auto* layout = new QVBoxLayout(host);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QWidget* topControlBar = createTopControlBar(host, metrics);
    layout->addWidget(topControlBar);

    McStudioRibbonBackend* backend = createSARibbonBackend(host);
    QWidget* backendWidget = backend ? backend->widget() : nullptr;
    if (backendWidget) {
        relaxHorizontalSize(backendWidget);
        layout->addWidget(backendWidget);

        const int topHeight = preferredFixedHeight(topControlBar, metrics.topControlBarHeight);
        const int backendHeight = preferredRibbonBackendHeight(backendWidget, metrics.ribbonMaxHeight);
        const int hostHeight = topHeight + backendHeight;
        Base::Console().message(
            "McStudioDiag[createRibbonHost]: top height=%d backend height=%d host height=%d fallback=%d sizeHint=%d minSizeHint=%d minH=%d class=%s.\n",
            topHeight,
            backendHeight,
            hostHeight,
            metrics.ribbonMaxHeight,
            backendWidget->sizeHint().height(),
            backendWidget->minimumSizeHint().height(),
            backendWidget->minimumHeight(),
            backendWidget->metaObject() ? backendWidget->metaObject()->className() : "<null>"
        );
        host->setMinimumHeight(hostHeight);
        host->setMaximumHeight(hostHeight);
    }
    else {
        delete backend;
        QWidget* customRibbon = createRibbonWidget(host);
        layout->addWidget(customRibbon);
        const int hostHeight = preferredFixedHeight(topControlBar, metrics.topControlBarHeight) + metrics.ribbonMaxHeight;
        host->setMinimumHeight(hostHeight);
        host->setMaximumHeight(hostHeight);
    }

    return host;
}

QWidget* findRibbonHost(Gui::MainWindow* mainWindow)
{
    return mainWindow ? mainWindow->findChild<QWidget*>(QString::fromLatin1(RibbonHostObjectName)) : nullptr;
}

QDockWidget* findRibbonDockHost(Gui::MainWindow* mainWindow)
{
    return mainWindow ? mainWindow->findChild<QDockWidget*>(QString::fromLatin1(RibbonDockObjectName)) : nullptr;
}


const char* boolText(bool value)
{
    return value ? "true" : "false";
}

const char* toolBarAreaName(Qt::ToolBarArea area)
{
    switch (area) {
        case Qt::LeftToolBarArea:
            return "LeftToolBarArea";
        case Qt::RightToolBarArea:
            return "RightToolBarArea";
        case Qt::TopToolBarArea:
            return "TopToolBarArea";
        case Qt::BottomToolBarArea:
            return "BottomToolBarArea";
        case Qt::NoToolBarArea:
        default:
            return "NoToolBarArea";
    }
}

std::string objectNameForLog(const QObject* object)
{
    if (!object) {
        return std::string("<null>");
    }

    QString name = object->objectName();
    if (name.isEmpty()) {
        name = QStringLiteral("<unnamed>");
    }
    return name.toUtf8().constData();
}

std::string widgetTitleForLog(const QWidget* widget)
{
    if (!widget) {
        return std::string("<null>");
    }

    QString title = widget->windowTitle();
    if (title.isEmpty()) {
        title = QStringLiteral("<no-title>");
    }
    return title.toUtf8().constData();
}

std::string classNameForLog(const QObject* object)
{
    if (!object || !object->metaObject()) {
        return std::string("<null>");
    }
    return object->metaObject()->className();
}

std::string activeWorkbenchNameForLog()
{
    Gui::Workbench* activeWorkbench = Gui::WorkbenchManager::instance()->active();
    return activeWorkbench ? activeWorkbench->name() : std::string("<none>");
}


bool diagnosticsDumpAllRibbonWidgets()
{
    if (const char* env = std::getenv("MCSTUDIO_DIAG_VERBOSE")) {
        return env[0] != '\0' && std::strcmp(env, "0") != 0;
    }

    ParameterGrp::handle group = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/Mod/McStudio/Diagnostics"
    );
    return group->GetBool("DumpAllRibbonWidgets", false);
}

bool diagnosticsDumpInvisibleRibbonItems()
{
    if (const char* env = std::getenv("MCSTUDIO_DIAG_INVISIBLE")) {
        return env[0] != '\0' && std::strcmp(env, "0") != 0;
    }

    ParameterGrp::handle group = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/Mod/McStudio/Diagnostics"
    );
    return group->GetBool("DumpInvisibleRibbonItems", false);
}

// Interaction tracing wires heavy work (geometry recomputation + full ribbon
// state dumps) onto every tab switch, mode change, and command trigger. That is
// invaluable while debugging ribbon layout but noticeably slows normal use, so
// it is OFF unless explicitly enabled via env var or preference.
bool diagnosticsInteractionTracing()
{
    if (const char* env = std::getenv("MCSTUDIO_DIAG_INTERACTION")) {
        return env[0] != '\0' && std::strcmp(env, "0") != 0;
    }

    ParameterGrp::handle group = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/Mod/McStudio/Diagnostics"
    );
    return group->GetBool("TraceInteraction", false);
}

void logMenuBarState(Gui::MainWindow* mainWindow, const char* stage)
{
    if (!mainWindow) {
        Base::Console().message("McStudioDiag[%s]: mainWindow=<null>; cannot inspect menuBar.\n", stage);
        return;
    }

    QMenuBar* menuBar = mainWindow->menuBar();
    if (!menuBar) {
        Base::Console().message("McStudioDiag[%s]: menuBar=<null>.\n", stage);
        return;
    }

    QWidget* leftCorner = menuBar->cornerWidget(Qt::TopLeftCorner);
    QWidget* rightCorner = menuBar->cornerWidget(Qt::TopRightCorner);
    const std::string menuName = objectNameForLog(menuBar);
    const std::string leftName = objectNameForLog(leftCorner);
    const std::string rightName = objectNameForLog(rightCorner);
    const std::string leftClass = classNameForLog(leftCorner);
    const std::string rightClass = classNameForLog(rightCorner);

    Base::Console().message(
        "McStudioDiag[%s]: menuBar name=%s visible=%s hidden=%s height=%d actions=%d "
        "leftCorner=%s/%s leftVisible=%s rightCorner=%s/%s rightVisible=%s.\n",
        stage,
        menuName.c_str(),
        boolText(menuBar->isVisible()),
        boolText(menuBar->isHidden()),
        menuBar->height(),
        menuBar->actions().size(),
        leftName.c_str(),
        leftClass.c_str(),
        boolText(leftCorner && leftCorner->isVisible()),
        rightName.c_str(),
        rightClass.c_str(),
        boolText(rightCorner && rightCorner->isVisible())
    );
}

void logToolBarsState(Gui::MainWindow* mainWindow, const char* stage)
{
    if (!mainWindow) {
        Base::Console().message("McStudioDiag[%s]: mainWindow=<null>; cannot inspect QToolBar list.\n", stage);
        return;
    }

    const QList<QToolBar*> toolBars = mainWindow->findChildren<QToolBar*>();
    Base::Console().message("McStudioDiag[%s]: QToolBar count=%d.\n", stage, toolBars.size());

    int index = 0;
    for (QToolBar* toolBar : toolBars) {
        if (!toolBar) {
            continue;
        }

        QObject* parent = toolBar->parent();
        QWidget* parentWidget = toolBar->parentWidget();
        QAction* toggle = toolBar->toggleViewAction();
        const std::string name = objectNameForLog(toolBar);
        const std::string title = widgetTitleForLog(toolBar);
        const std::string klass = classNameForLog(toolBar);
        const std::string parentName = objectNameForLog(parent);
        const std::string parentClass = classNameForLog(parent);
        const Qt::ToolBarArea area = mainWindow->toolBarArea(toolBar);

        Base::Console().message(
            "McStudioDiag[%s]: toolbar[%d] name=%s title=%s class=%s visible=%s hidden=%s "
            "floating=%s movable=%s enabled=%s area=%s parent=%s/%s parentVisible=%s "
            "toggleVisible=%s toggleEnabled=%s toggleChecked=%s actions=%d icon=%dx%d size=%dx%d pos=%d,%d.\n",
            stage,
            index,
            name.c_str(),
            title.c_str(),
            klass.c_str(),
            boolText(toolBar->isVisible()),
            boolText(toolBar->isHidden()),
            boolText(toolBar->isFloating()),
            boolText(toolBar->isMovable()),
            boolText(toolBar->isEnabled()),
            toolBarAreaName(area),
            parentName.c_str(),
            parentClass.c_str(),
            boolText(parentWidget && parentWidget->isVisible()),
            boolText(toggle && toggle->isVisible()),
            boolText(toggle && toggle->isEnabled()),
            boolText(toggle && toggle->isChecked()),
            toolBar->actions().size(),
            toolBar->iconSize().width(),
            toolBar->iconSize().height(),
            toolBar->width(),
            toolBar->height(),
            toolBar->pos().x(),
            toolBar->pos().y()
        );
        ++index;
    }
}

void logSARibbonDeepState(SARibbonBar* ribbon, const char* stage);
void installSARibbonInteractionDiagnostics(SARibbonBar* ribbon);
void forceStableSARibbonGeometry(SARibbonBar* ribbon, const char* reason);

void logRibbonDockState(Gui::MainWindow* mainWindow, const char* stage)
{
    if (!mainWindow) {
        Base::Console().message("McStudioDiag[%s]: mainWindow=<null>; cannot inspect McStudio ribbon dock.\n", stage);
        return;
    }

    QDockWidget* dock = findRibbonDockHost(mainWindow);
    QWidget* host = findRibbonHost(mainWindow);
    auto* ribbon = host ? host->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar")) : nullptr;

    if (!dock) {
        Base::Console().message("McStudioDiag[%s]: ribbonDock=<null>.\n", stage);
    }
    else {
        const std::string dockName = objectNameForLog(dock);
        Base::Console().message(
            "McStudioDiag[%s]: ribbonDock name=%s visible=%s floating=%s area=%s size=%dx%d minH=%d maxH=%d widget=%s.\n",
            stage,
            dockName.c_str(),
            boolText(dock->isVisible()),
            boolText(dock->isFloating()),
            toolBarAreaName(Qt::NoToolBarArea),
            dock->width(),
            dock->height(),
            dock->minimumHeight(),
            dock->maximumHeight(),
            boolText(dock->widget() != nullptr)
        );
    }

    if (!host) {
        Base::Console().message("McStudioDiag[%s]: ribbonHost=<null>.\n", stage);
    }
    else {
        const std::string hostName = objectNameForLog(host);
        Base::Console().message(
            "McStudioDiag[%s]: ribbonHost name=%s visible=%s size=%dx%d minH=%d maxH=%d layout=%s ribbon=%s.\n",
            stage,
            hostName.c_str(),
            boolText(host->isVisible()),
            host->width(),
            host->height(),
            host->minimumHeight(),
            host->maximumHeight(),
            boolText(host->layout() != nullptr),
            boolText(ribbon != nullptr)
        );
    }

    if (ribbon) {
        const std::string ribbonName = objectNameForLog(ribbon);
        Base::Console().message(
            "McStudioDiag[%s]: saribbon name=%s visible=%s size=%dx%d minH=%d maxH=%d titleH=%d tabH=%d categoryH=%d.\n",
            stage,
            ribbonName.c_str(),
            boolText(ribbon->isVisible()),
            ribbon->width(),
            ribbon->height(),
            ribbon->minimumHeight(),
            ribbon->maximumHeight(),
            ribbon->titleBarHeight(),
            ribbon->tabBarHeight(),
            ribbon->categoryHeight()
        );
    }
}


std::string actionTextForLog(const QAction* action)
{
    if (!action) {
        return std::string("<no-action>");
    }
    QString text = action->text();
    text.remove(QLatin1Char('&'));
    if (text.isEmpty()) {
        text = QStringLiteral("<no-text>");
    }
    return text.toUtf8().constData();
}

void logSARibbonButtonState(SARibbonBar* ribbon, const char* stage)
{
    if (!ribbon) {
        Base::Console().message("McStudioDiag[%s]: saribbonButtons ribbon=<null>.\n", stage);
        return;
    }

    const bool dumpAll = diagnosticsDumpAllRibbonWidgets();
    const bool dumpInvisible = diagnosticsDumpInvisibleRibbonItems();
    const QList<QToolButton*> buttons = ribbon->findChildren<QToolButton*>();
    int invisibleButtons = 0;
    int visibleButtons = 0;
    int visibleNullIconButtons = 0;
    int actionNullIconButtons = 0;
    int noActionNullIconButtons = 0;

    for (QToolButton* button : buttons) {
        if (!button) {
            continue;
        }

        QAction* action = button->defaultAction();
        const bool visible = button->isVisible();
        if (visible) {
            ++visibleButtons;
        }
        else {
            ++invisibleButtons;
        }

        if (visible && button->icon().isNull()) {
            ++visibleNullIconButtons;
        }
        if (action && action->icon().isNull()) {
            ++actionNullIconButtons;
        }
        if (!action && button->icon().isNull()) {
            ++noActionNullIconButtons;
        }
    }

    Base::Console().message(
        "McStudioDiag[%s]: saribbonButtons summary count=%d visible=%d invisible=%d "
        "visibleButtonIconNull=%d actionIconNull=%d noActionAndButtonIconNull=%d dumpAll=%s.\n",
        stage,
        buttons.size(),
        visibleButtons,
        invisibleButtons,
        visibleNullIconButtons,
        actionNullIconButtons,
        noActionNullIconButtons,
        boolText(dumpAll)
    );

    int index = 0;
    for (QToolButton* button : buttons) {
        if (!button) {
            continue;
        }

        QAction* action = button->defaultAction();
        const bool visible = button->isVisible();
        const bool buttonIconNull = button->icon().isNull();
        const bool actionIconNull = action && action->icon().isNull();
        const bool shouldLog = dumpAll
            || (dumpInvisible && !visible)
            || (visible && buttonIconNull)
            || actionIconNull;

        if (!shouldLog) {
            ++index;
            continue;
        }

        const std::string name = objectNameForLog(button);
        const std::string klass = classNameForLog(button);
        const std::string text = button->text().isEmpty() ? std::string("<no-text>") : button->text().toUtf8().constData();
        const std::string actionText = actionTextForLog(action);
        const std::string actionName = objectNameForLog(action);
        const QString actionData = action ? action->data().toString() : QString();
        const std::string actionDataText = actionData.isEmpty() ? std::string("<no-data>") : actionData.toUtf8().constData();

        Base::Console().message(
            "McStudioDiag[%s]: saribbonButton[%d] name=%s class=%s text=%s visible=%s enabled=%s "
            "size=%dx%d icon=%dx%d buttonIconNull=%s action=%s actionText=%s actionData=%s "
            "actionVisible=%s actionEnabled=%s actionIconNull=%s.\n",
            stage,
            index,
            name.c_str(),
            klass.c_str(),
            text.c_str(),
            boolText(visible),
            boolText(button->isEnabled()),
            button->width(),
            button->height(),
            button->iconSize().width(),
            button->iconSize().height(),
            boolText(buttonIconNull),
            actionName.c_str(),
            actionText.c_str(),
            actionDataText.c_str(),
            boolText(action && action->isVisible()),
            boolText(action && action->isEnabled()),
            boolText(!action || action->icon().isNull())
        );
        ++index;
    }
}

void logSARibbonCategoryState(SARibbonBar* ribbon, const char* stage)
{
    if (!ribbon) {
        Base::Console().message("McStudioDiag[%s]: saribbonCategories ribbon=<null>.\n", stage);
        return;
    }

    const QList<SARibbonCategory*> allCategories = ribbon->categoryPages(true);
    const QList<SARibbonCategory*> visibleCategories = ribbon->categoryPages(false);
    const QList<int> contextIndexes = ribbon->currentVisibleContextCategoryTabIndexs();
    const bool dumpAll = diagnosticsDumpAllRibbonWidgets();
    const bool dumpInvisible = diagnosticsDumpInvisibleRibbonItems();
    Base::Console().message(
        "McStudioDiag[%s]: saribbonCategories summary currentIndex=%d all=%d visible=%d visibleContextTabs=%d dumpAll=%s.\n",
        stage,
        ribbon->currentIndex(),
        allCategories.size(),
        visibleCategories.size(),
        contextIndexes.size(),
        boolText(dumpAll)
    );

    int index = 0;
    for (SARibbonCategory* category : allCategories) {
        if (!category) {
            continue;
        }
        const std::string name = objectNameForLog(category);
        const std::string title = category->categoryName().isEmpty() ? std::string("<no-title>")
                                                                    : category->categoryName().toUtf8().constData();
        const QList<SARibbonPanel*> panels = category->panelList();
        const int tabIndex = ribbon->categoryIndex(category);
        const bool logCategory = dumpAll || category->isVisible() || (dumpInvisible && tabIndex >= 0);
        if (logCategory) {
            Base::Console().message(
                "McStudioDiag[%s]: saribbonCategory[%d] name=%s title=%s tabIndex=%d visible=%s context=%s "
                "size=%dx%d panelCount=%d.\n",
                stage,
                index,
                name.c_str(),
                title.c_str(),
                tabIndex,
                boolText(category->isVisible()),
                boolText(category->isContextCategory()),
                category->width(),
                category->height(),
                panels.size()
            );
        }

        int panelIndex = 0;
        for (SARibbonPanel* panel : panels) {
            if (!panel) {
                continue;
            }
            const std::string panelName = objectNameForLog(panel);
            const std::string panelTitle = panel->panelName().isEmpty() ? std::string("<no-title>")
                                                                        : panel->panelName().toUtf8().constData();
            const QList<QToolButton*> panelButtons = panel->findChildren<QToolButton*>();
            int nullIcons = 0;
            for (QToolButton* button : panelButtons) {
                QAction* action = button ? button->defaultAction() : nullptr;
                if (button && button->icon().isNull() && (!action || action->icon().isNull())) {
                    ++nullIcons;
                }
            }
            if (dumpAll || panel->isVisible() || nullIcons > 0 || (dumpInvisible && logCategory)) {
                Base::Console().message(
                    "McStudioDiag[%s]: saribbonPanel[%d.%d] name=%s title=%s visible=%s size=%dx%d buttons=%d nullIconButtons=%d.\n",
                    stage,
                    index,
                    panelIndex,
                    panelName.c_str(),
                    panelTitle.c_str(),
                    boolText(panel->isVisible()),
                    panel->width(),
                    panel->height(),
                    panelButtons.size(),
                    nullIcons
                );
            }
            ++panelIndex;
        }
        ++index;
    }
}

void logSARibbonDeepState(SARibbonBar* ribbon, const char* stage)
{
    if (!ribbon) {
        Base::Console().message("McStudioDiag[%s]: saribbonDeep ribbon=<null>.\n", stage);
        return;
    }

    logSARibbonCategoryState(ribbon, stage);
    logSARibbonButtonState(ribbon, stage);
}

void forceStableSARibbonGeometry(SARibbonBar* ribbon, const char* reason)
{
    if (!ribbon) {
        return;
    }

    const int requiredHeight = preferredRibbonBackendHeight(ribbon, ribbon->categoryHeight() + ribbon->tabBarHeight() + ribbon->titleBarHeight());
    ribbon->setMinimumHeight(requiredHeight);
    ribbon->setMaximumHeight(requiredHeight);
    ribbon->updateGeometry();
    ribbon->updateRibbonGeometry();
    ribbon->update();

    int hostHeight = requiredHeight;
    if (QWidget* host = ribbon->parentWidget()) {
        const int topHeight = topControlHeightForHost(host, 0);
        hostHeight = requiredHeight + topHeight;
        host->setMinimumHeight(hostHeight);
        host->setMaximumHeight(hostHeight);
        host->updateGeometry();
    }

    Base::Console().message(
        "McStudioDiag[%s]: forceStableSARibbonGeometry requiredHeight=%d hostHeight=%d ribbonSize=%dx%d.\n",
        reason,
        requiredHeight,
        hostHeight,
        ribbon->width(),
        ribbon->height()
    );
}

void installSARibbonInteractionDiagnostics(SARibbonBar* ribbon)
{
    if (!ribbon) {
        return;
    }

    if (ribbon->property("McStudioInteractionDiagnosticsInstalled").toBool()) {
        return;
    }
    ribbon->setProperty("McStudioInteractionDiagnosticsInstalled", true);

    const bool trace = diagnosticsInteractionTracing();

    // Layout-affecting signals: a tab / mode / style change genuinely alters the
    // ribbon's preferred height, so the geometry stabilization here is
    // functional and always runs. The full state dump is diagnostic and only
    // runs when interaction tracing is enabled.
    QObject::connect(ribbon,
                     &SARibbonBar::currentRibbonTabChanged,
                     ribbon,
                     [ribbon, trace](int index) {
                         const QString stage = QStringLiteral("saribbon-tab-changed:%1").arg(index);
                         const QByteArray bytes = stage.toUtf8();
                         forceStableSARibbonGeometry(ribbon, bytes.constData());
                         if (trace) {
                             Base::Console().message("McStudioDiag[%s]: signal currentRibbonTabChanged.\n", bytes.constData());
                             logSARibbonDeepState(ribbon, bytes.constData());
                         }
                     });

    QObject::connect(ribbon,
                     &SARibbonBar::ribbonModeChanged,
                     ribbon,
                     [ribbon, trace](SARibbonBar::RibbonMode mode) {
                         const QString stage = QStringLiteral("saribbon-mode-changed:%1").arg(static_cast<int>(mode));
                         const QByteArray bytes = stage.toUtf8();
                         forceStableSARibbonGeometry(ribbon, bytes.constData());
                         if (trace) {
                             Base::Console().message("McStudioDiag[%s]: signal ribbonModeChanged.\n", bytes.constData());
                             logSARibbonDeepState(ribbon, bytes.constData());
                         }
                     });

    QObject::connect(ribbon,
                     &SARibbonBar::ribbonStyleChanged,
                     ribbon,
                     [ribbon, trace](SARibbonBar::RibbonStyles style) {
                         const QString stage = QStringLiteral("saribbon-style-changed:%1").arg(static_cast<int>(style));
                         const QByteArray bytes = stage.toUtf8();
                         forceStableSARibbonGeometry(ribbon, bytes.constData());
                         if (trace) {
                             Base::Console().message("McStudioDiag[%s]: signal ribbonStyleChanged.\n", bytes.constData());
                             logSARibbonDeepState(ribbon, bytes.constData());
                         }
                     });

    // Triggering a command does NOT change ribbon layout, so no geometry
    // recomputation or state dump is needed here on the hot path. This purely
    // diagnostic hook is only wired up when interaction tracing is enabled.
    if (trace) {
        QObject::connect(ribbon,
                         &SARibbonBar::actionTriggered,
                         ribbon,
                         [ribbon](QAction* action) {
                             const std::string actionName = objectNameForLog(action);
                             const std::string actionText = actionTextForLog(action);
                             Base::Console().message(
                                 "McStudioDiag[saribbon-action-triggered]: action=%s text=%s iconNull=%s enabled=%s visible=%s.\n",
                                 actionName.c_str(),
                                 actionText.c_str(),
                                 boolText(!action || action->icon().isNull()),
                                 boolText(action && action->isEnabled()),
                                 boolText(action && action->isVisible())
                             );
                             logSARibbonDeepState(ribbon, "saribbon-action-triggered");
                         });

        Base::Console().message("McStudioDiag[installSARibbonInteractionDiagnostics]: installed with interaction tracing.\n");
        logSARibbonDeepState(ribbon, "installSARibbonInteractionDiagnostics");
    }
    else {
        Base::Console().message(
            "McStudioDiag[installSARibbonInteractionDiagnostics]: installed (layout stabilization only; "
            "set MCSTUDIO_DIAG_INTERACTION=1 to enable command/state tracing).\n");
    }
}

void logNativeTopChromeState(Gui::MainWindow* mainWindow, const char* stage)
{
    if (!mainWindow) {
        Base::Console().message("McStudioDiag[%s]: mainWindow=<null>.\n", stage);
        return;
    }

    const std::string activeWorkbench = activeWorkbenchNameForLog();
    const std::string mainWindowName = objectNameForLog(mainWindow);
    Base::Console().message(
        "McStudioDiag[%s]: BEGIN mainWindow=%s visible=%s restoring=%s activeWorkbench=%s size=%dx%d.\n",
        stage,
        mainWindowName.c_str(),
        boolText(mainWindow->isVisible()),
        boolText(mainWindow->isRestoringWindowState()),
        activeWorkbench.c_str(),
        mainWindow->width(),
        mainWindow->height()
    );
    logMenuBarState(mainWindow, stage);
    logToolBarsState(mainWindow, stage);
    logRibbonDockState(mainWindow, stage);
    Base::Console().message("McStudioDiag[%s]: END.\n", stage);
}

void installNativeChromeDiagnostics(Gui::MainWindow* mainWindow)
{
    static fastsignals::connection connection;
    static QPointer<QTimer> diagnosticsTimer;
    static QString pendingReason;

    if (!mainWindow) {
        return;
    }

    if (!diagnosticsTimer) {
        diagnosticsTimer = new QTimer(mainWindow);
        diagnosticsTimer->setSingleShot(true);
        QObject::connect(diagnosticsTimer, &QTimer::timeout, [] {
            Gui::MainWindow* currentMainWindow = Gui::getMainWindow();
            if (!currentMainWindow) {
                return;
            }

            const QString reason = pendingReason.isEmpty() ? QStringLiteral("timer") : pendingReason;
            const QByteArray reasonBytes = reason.toUtf8();
            logNativeTopChromeState(currentMainWindow, reasonBytes.constData());
            pendingReason.clear();
        });
    }

    if (connection.connected()) {
        return;
    }

    ParameterGrp::handle mainWindowGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/MainWindow"
    );
    ParameterGrp::handle toolBarsGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/MainWindow/Toolbars"
    );
    ParameterGrp::handle menuBarLeftGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/MainWindow/MenuBarLeft"
    );
    ParameterGrp::handle menuBarRightGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/MainWindow/MenuBarRight"
    );
    ParameterGrp::handle statusBarGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/MainWindow/StatusBar"
    );
    ParameterGrp::handle generalGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/General"
    );

    connection = App::GetApplication().GetUserParameter().signalParamChanged.connect(
        [mainWindowGroup,
         toolBarsGroup,
         menuBarLeftGroup,
         menuBarRightGroup,
         statusBarGroup,
         generalGroup](ParameterGrp* param, ParameterGrp::ParamType, const char* name, const char*) {
            if (!param || !name || !diagnosticsTimer) {
                return;
            }

            QString reason;
            if (param == mainWindowGroup && std::strcmp(name, "WindowStateRestored") == 0) {
                reason = QStringLiteral("param:MainWindow/WindowStateRestored");
            }
            else if (param == toolBarsGroup) {
                reason = QStringLiteral("param:MainWindow/Toolbars/") + QString::fromLatin1(name);
            }
            else if (param == menuBarLeftGroup) {
                reason = QStringLiteral("param:MainWindow/MenuBarLeft/") + QString::fromLatin1(name);
            }
            else if (param == menuBarRightGroup) {
                reason = QStringLiteral("param:MainWindow/MenuBarRight/") + QString::fromLatin1(name);
            }
            else if (param == statusBarGroup) {
                reason = QStringLiteral("param:MainWindow/StatusBar/") + QString::fromLatin1(name);
            }
            else if (param == generalGroup
                     && (std::strcmp(name, "ToolbarIconSize") == 0
                         || std::strcmp(name, "MenuBarIconSize") == 0)) {
                reason = QStringLiteral("param:General/") + QString::fromLatin1(name);
            }

            if (!reason.isEmpty()) {
                pendingReason = reason;
                diagnosticsTimer->start(0);
            }
        }
    );

    Base::Console().message("McStudioDiag[install]: native top chrome diagnostics installed.\n");
}

QPointer<QMenuBar>& hiddenNativeMenuBar()
{
    static QPointer<QMenuBar> menuBar;
    return menuBar;
}

bool& hiddenNativeMenuBarWasVisible()
{
    static bool wasVisible = true;
    return wasVisible;
}

bool& hasHiddenNativeMenuBarState()
{
    static bool hasState = false;
    return hasState;
}


QPointer<Gui::MainWindow>& framelessNativeMainWindow()
{
    static QPointer<Gui::MainWindow> mainWindow;
    return mainWindow;
}

Qt::WindowFlags& savedNativeWindowFlags()
{
    static Qt::WindowFlags flags;
    return flags;
}

bool& hasSavedNativeWindowFlags()
{
    static bool hasState = false;
    return hasState;
}

void hideFreeCADNativeTitleBar(Gui::MainWindow* mainWindow)
{
    if (!mainWindow) {
        return;
    }

    if (!hasSavedNativeWindowFlags() || framelessNativeMainWindow() != mainWindow) {
        framelessNativeMainWindow() = mainWindow;
        savedNativeWindowFlags() = mainWindow->windowFlags();
        hasSavedNativeWindowFlags() = true;
    }

    if (mainWindow->windowFlags() & Qt::FramelessWindowHint) {
        return;
    }

    const bool wasVisible = mainWindow->isVisible();
    const bool wasMaximized = mainWindow->isMaximized();
    const bool wasFullScreen = mainWindow->isFullScreen();
    // Toggling FramelessWindowHint recreates the native window on X11, which
    // drops the normal (non-maximized) window position. Capture the current
    // normal geometry so it can be restored after the flag change; maximized /
    // fullscreen states are re-applied via their dedicated show* calls below.
    const QRect normalGeometry = mainWindow->normalGeometry();
    mainWindow->setWindowFlag(Qt::FramelessWindowHint, true);

    if (wasVisible) {
        if (wasFullScreen) {
            mainWindow->showFullScreen();
        }
        else if (wasMaximized) {
            mainWindow->showMaximized();
        }
        else {
            mainWindow->show();
            if (normalGeometry.isValid()) {
                mainWindow->setGeometry(normalGeometry);
            }
        }
    }

    Base::Console().message("McStudio Shell: native title bar hidden using Qt::FramelessWindowHint.\n");
}

void restoreFreeCADNativeTitleBar(Gui::MainWindow* mainWindow)
{
    if (!mainWindow) {
        return;
    }

    if (!hasSavedNativeWindowFlags() || framelessNativeMainWindow() != mainWindow) {
        return;
    }

    const bool wasVisible = mainWindow->isVisible();
    const bool wasMaximized = mainWindow->isMaximized();
    const bool wasFullScreen = mainWindow->isFullScreen();
    // Restoring the native frame also recreates the X11 window; preserve the
    // normal geometry across the flag change (see hideFreeCADNativeTitleBar).
    const QRect normalGeometry = mainWindow->normalGeometry();
    mainWindow->setWindowFlags(savedNativeWindowFlags());
    hasSavedNativeWindowFlags() = false;
    framelessNativeMainWindow().clear();

    if (wasVisible) {
        if (wasFullScreen) {
            mainWindow->showFullScreen();
        }
        else if (wasMaximized) {
            mainWindow->showMaximized();
        }
        else {
            mainWindow->show();
            if (normalGeometry.isValid()) {
                mainWindow->setGeometry(normalGeometry);
            }
        }
    }

    Base::Console().message("McStudio Shell: native title bar restored.\n");
}

void hideFreeCADNativeMenuBar(Gui::MainWindow* mainWindow)
{
    if (!mainWindow) {
        return;
    }

    logMenuBarState(mainWindow, "before-hide-menuBar");

    QMenuBar* menuBar = mainWindow->menuBar();
    if (!menuBar) {
        return;
    }

    if (!hasHiddenNativeMenuBarState() || hiddenNativeMenuBar() != menuBar) {
        hiddenNativeMenuBar() = menuBar;
        hiddenNativeMenuBarWasVisible() = menuBar->isVisible();
        hasHiddenNativeMenuBarState() = true;
    }

    menuBar->setVisible(false);
    logMenuBarState(mainWindow, "after-hide-menuBar");
}

void restoreFreeCADNativeMenuBar(Gui::MainWindow* mainWindow)
{
    if (!mainWindow) {
        return;
    }

    logMenuBarState(mainWindow, "before-restore-menuBar");

    QMenuBar* menuBar = mainWindow->menuBar();
    if (!menuBar) {
        return;
    }

    if (hasHiddenNativeMenuBarState() && hiddenNativeMenuBar() == menuBar) {
        menuBar->setVisible(hiddenNativeMenuBarWasVisible());
        hasHiddenNativeMenuBarState() = false;
        hiddenNativeMenuBar().clear();
        logMenuBarState(mainWindow, "after-restore-menuBar");
        return;
    }

    menuBar->setVisible(true);
    logMenuBarState(mainWindow, "after-restore-menuBar-forced-visible");
}

void refreshSARibbonBackend(QWidget* ribbonHost)
{
    if (!ribbonHost) {
        return;
    }

    auto* ribbon = ribbonHost->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar"));
    if (!ribbon) {
        return;
    }

    const McStudioThemeTokens tokens = currentMcStudioThemeTokens();
    applyMcStudioSARibbonTheme(ribbon);
    ribbon->setPanelToolButtonIconSize(QSize(tokens.smallIconSize, tokens.smallIconSize),
                                       QSize(tokens.largeIconSize, tokens.largeIconSize));
    ribbon->updateRibbonGeometry();
}

void installRibbonSizeRefresh(Gui::MainWindow* mainWindow)
{
    static fastsignals::connection connection;
    static QPointer<QTimer> refreshTimer;

    if (!mainWindow) {
        return;
    }

    if (!refreshTimer) {
        refreshTimer = new QTimer(mainWindow);
        refreshTimer->setSingleShot(true);
        QObject::connect(refreshTimer, &QTimer::timeout, [] {
            RibbonWidget::show();
        });
    }

    if (connection.connected()) {
        return;
    }

    ParameterGrp::handle generalGroup = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/General"
    );
    ParameterGrp::handle ribbonGroup = App::GetApplication().GetParameterGroupByPath(RibbonPreferencePath);

    connection = App::GetApplication().GetUserParameter().signalParamChanged.connect(
        [generalGroup, ribbonGroup](ParameterGrp* param, ParameterGrp::ParamType, const char* name, const char*) {
            if (!param || !name || !refreshTimer) {
                return;
            }

            const bool toolbarIconSizeChanged = param == generalGroup && std::strcmp(name, "ToolbarIconSize") == 0;
            const bool ribbonScaleChanged = param == ribbonGroup && std::strcmp(name, "Scale") == 0;
            if (toolbarIconSizeChanged || ribbonScaleChanged) {
                refreshTimer->start(100);
            }
        }
    );
}

void configureRibbonDockHost(QDockWidget* dock, const RibbonMetrics& metrics)
{
    if (!dock) {
        return;
    }

    dock->setWindowTitle(QObject::tr("McStudio Ribbon"));
    dock->setAccessibleName(QObject::tr("McStudio Ribbon"));
    dock->setAllowedAreas(Qt::TopDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setFloating(false);
    int dockHeight = metrics.topControlBarHeight + metrics.ribbonMaxHeight;
    if (QWidget* content = dock->widget()) {
        dockHeight = preferredFixedHeight(content, metrics.topControlBarHeight + metrics.ribbonMaxHeight);
    }

    dock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    relaxHorizontalSize(dock);
    dock->setMinimumHeight(dockHeight);
    dock->setMaximumHeight(dockHeight);
    dock->setStyleSheet(ribbonStyleSheet());

    if (QAction* toggleAction = dock->toggleViewAction()) {
        toggleAction->setVisible(false);
        toggleAction->setEnabled(false);
    }
}

QDockWidget* ensureRibbonDockHost(Gui::MainWindow* mainWindow, const RibbonMetrics& metrics)
{
    if (!mainWindow) {
        return nullptr;
    }

    // Let the top dock area own both top corners so the McStudio ribbon spans
    // above the left/right dock areas, matching CAD ribbon layouts such as NX.
    mainWindow->setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    mainWindow->setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);

    QDockWidget* dock = findRibbonDockHost(mainWindow);
    if (!dock) {
        dock = new QDockWidget(mainWindow);
        dock->setObjectName(QString::fromLatin1(RibbonDockObjectName));

        auto* hiddenTitleBar = new QWidget(dock);
        hiddenTitleBar->setObjectName(QStringLiteral("McStudioRibbonDockTitleBar"));
        hiddenTitleBar->setFixedHeight(0);
        dock->setTitleBarWidget(hiddenTitleBar);

        Base::Console().message("McStudioDiag[ensureRibbonDockHost]: creating McStudio ribbon dock.\n");
        dock->setWidget(createRibbonHost(dock, metrics));
        mainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
        Base::Console().message("McStudio Ribbon: installed top dock RibbonHost.\n");
        if (QWidget* createdHost = findRibbonHost(mainWindow)) {
            if (auto* createdRibbon = createdHost->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar"))) {
                installSARibbonInteractionDiagnostics(createdRibbon);
                forceStableSARibbonGeometry(createdRibbon, "after-create-ribbon-dock");
            }
        }
        logNativeTopChromeState(mainWindow, "after-create-ribbon-dock");
    }

    if (!dock->widget()) {
        // Lifecycle contract: normally the dock keeps its ribbon host (and the
        // SARibbon backend + bar parented under it) for the whole session; this
        // branch is only reached if the widget was detached elsewhere.
        // QDockWidget::setWidget does NOT delete a previously associated widget,
        // and a detached-but-still-child host would collide with the new one
        // (duplicate object names, leaked SARibbonBar + signal connections).
        // Destroy any leftover host before creating a fresh one.
        if (QWidget* staleHost = findRibbonHost(mainWindow)) {
            staleHost->setParent(nullptr);
            staleHost->deleteLater();
        }
        dock->setWidget(createRibbonHost(dock, metrics));
    }
    else if (QWidget* ribbonHost = findRibbonHost(mainWindow)) {
        Base::Console().message("McStudioDiag[ensureRibbonDockHost]: refreshing existing McStudio ribbon host.\n");
        refreshTopControlBar(ribbonHost, metrics);
        refreshSARibbonBackend(ribbonHost);
        if (auto* existingRibbon = ribbonHost->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar"))) {
            installSARibbonInteractionDiagnostics(existingRibbon);
            forceStableSARibbonGeometry(existingRibbon, "refresh-existing-ribbon-host");
        }
        relaxHorizontalSize(ribbonHost);
        const int hostHeight = preferredRibbonHostHeight(ribbonHost, metrics);
        ribbonHost->setMinimumHeight(hostHeight);
        ribbonHost->setMaximumHeight(hostHeight);
        ribbonHost->setFont(scaledFont(ribbonHost->font(), metrics.scale));
        ribbonHost->setStyleSheet(ribbonStyleSheet());
    }
    configureRibbonDockHost(dock, metrics);

    if (mainWindow->dockWidgetArea(dock) != Qt::TopDockWidgetArea) {
        mainWindow->removeDockWidget(dock);
        mainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
    }

    return dock;
}

}  // namespace

void RibbonWidget::show()
{
    Gui::MainWindow* mainWindow = Gui::getMainWindow();
    if (!mainWindow) {
        return;
    }

    Base::Console().message("McStudioDiag[RibbonWidget::show]: begin.\n");
    installNativeChromeDiagnostics(mainWindow);
    logNativeTopChromeState(mainWindow, "RibbonWidget::show/before-menu-hide");
    hideFreeCADNativeTitleBar(mainWindow);
    hideFreeCADNativeMenuBar(mainWindow);
    installRibbonSizeRefresh(mainWindow);

    const RibbonMetrics metrics = ribbonMetrics();
    QDockWidget* dock = ensureRibbonDockHost(mainWindow, metrics);
    if (!dock) {
        return;
    }

    QWidget* ribbonHost = findRibbonHost(mainWindow);
    if (!ribbonHost) {
        Base::Console().warning("McStudio Ribbon: dock host exists but ribbon host was not found.\n");
        return;
    }

    ribbonHost->show();
    dock->show();
    if (auto* ribbon = ribbonHost->findChild<SARibbonBar*>(QStringLiteral("McStudioSARibbonBar"))) {
        forceStableSARibbonGeometry(ribbon, "RibbonWidget::show/after-show");
        QPointer<SARibbonBar> ribbonGuard(ribbon);
        QTimer::singleShot(0, ribbon, [ribbonGuard] {
            if (!ribbonGuard) {
                return;
            }
            forceStableSARibbonGeometry(ribbonGuard, "RibbonWidget::show/deferred-0");
            logSARibbonDeepState(ribbonGuard, "RibbonWidget::show/deferred-0");
        });
    }
    logNativeTopChromeState(mainWindow, "RibbonWidget::show/end");
    Base::Console().message("McStudioDiag[RibbonWidget::show]: end.\n");
}

void RibbonWidget::hide()
{
    Gui::MainWindow* mainWindow = Gui::getMainWindow();
    if (!mainWindow) {
        return;
    }

    Base::Console().message("McStudioDiag[RibbonWidget::hide]: begin.\n");
    logNativeTopChromeState(mainWindow, "RibbonWidget::hide/before-menu-restore");
    restoreFreeCADNativeMenuBar(mainWindow);
    restoreFreeCADNativeTitleBar(mainWindow);

    QDockWidget* dock = findRibbonDockHost(mainWindow);
    if (!dock) {
        return;
    }

    dock->hide();
    logNativeTopChromeState(mainWindow, "RibbonWidget::hide/end");
    Base::Console().message("McStudioDiag[RibbonWidget::hide]: end.\n");
}

}  // namespace McStudioGui
