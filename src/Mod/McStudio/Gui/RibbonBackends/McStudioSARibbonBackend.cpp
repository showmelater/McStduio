// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"
#include "McStudioSARibbonBackend.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include <QAction>
#include <QColor>
#include <QCoreApplication>
#include <QMenu>
#include <QPointer>
#include <QRegularExpression>
#include <QSize>
#include <QSizePolicy>
#include <QTimer>
#include <QToolButton>
#include <QString>

#include <Base/Console.h>
#include <Gui/Action.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Selection/Selection.h>
#include <Gui/ViewProviderDocumentObject.h>

#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <SARibbonContextCategory.h>
#include <SARibbonPanel.h>

#include "../RibbonModel.h"
#include "../Theme/McStudioTheme.h"

namespace McStudioGui
{
namespace
{
constexpr const char* RibbonTranslationContext = "McStudioGui::Ribbon";

QString ribbonText(const char* text)
{
    return text ? QCoreApplication::translate(RibbonTranslationContext, text) : QString();
}

QString stableObjectName(const QString& prefix, const QString& text)
{
    QString name = text;
    name.replace(QRegularExpression(QStringLiteral("[^A-Za-z0-9_]+")), QStringLiteral("_"));
    name = name.trimmed();
    while (name.startsWith(QLatin1Char('_'))) {
        name.remove(0, 1);
    }
    while (name.endsWith(QLatin1Char('_'))) {
        name.chop(1);
    }
    if (name.isEmpty()) {
        name = QStringLiteral("Unnamed");
    }
    return prefix + QLatin1Char('_') + name;
}


void applyRibbonIconMetrics(SARibbonBar* ribbon)
{
    if (!ribbon) {
        return;
    }

    const McStudioThemeTokens tokens = currentMcStudioThemeTokens();
    ribbon->setPanelToolButtonIconSize(QSize(tokens.smallIconSize, tokens.smallIconSize),
                                       QSize(tokens.largeIconSize, tokens.largeIconSize));
}

Gui::Command* commandByName(const char* commandName)
{
    if (!commandName || !Gui::Application::Instance) {
        return nullptr;
    }
    return Gui::Application::Instance->commandManager().getCommandByName(commandName);
}

QAction* ensureFreeCADAction(const char* commandName, QWidget* initializer)
{
    Gui::Command* command = commandByName(commandName);
    if (!command) {
        Base::Console().warning("McStudio SARibbon: skipping unknown command '%s'.\n", commandName);
        return nullptr;
    }

    if (initializer && Gui::Application::Instance) {
        Gui::Application::Instance->commandManager().addTo(commandName, initializer);
    }

    Gui::Action* commandAction = command->getAction();
    QAction* action = commandAction ? commandAction->action() : nullptr;
    if (!action) {
        Base::Console().warning("McStudio SARibbon: command '%s' has no QAction.\n", commandName);
    }
    return action;
}

QAction* createProxyAction(QAction* source, const RibbonCommandDefinition& definition, QObject* parent)
{
    if (!source) {
        return nullptr;
    }

    const QString text = definition.label ? ribbonText(definition.label) : source->text();
    auto* proxy = new QAction(source->icon(), text, parent);
    proxy->setObjectName(stableObjectName(QStringLiteral("McStudioRibbonAction"),
                                          QString::fromLatin1(definition.command ? definition.command : "Unknown")));
    proxy->setToolTip(source->toolTip());
    proxy->setStatusTip(source->statusTip());
    proxy->setWhatsThis(source->whatsThis());
    proxy->setEnabled(source->isEnabled());
    proxy->setVisible(source->isVisible());
    proxy->setCheckable(source->isCheckable());
    proxy->setChecked(source->isChecked());
    proxy->setShortcutContext(source->shortcutContext());

    QObject::connect(proxy, &QAction::triggered, source, [source]() {
        source->trigger();
    });
    QObject::connect(source, &QAction::changed, proxy, [source, proxy, text]() {
        proxy->setIcon(source->icon());
        proxy->setText(text.isEmpty() ? source->text() : text);
        proxy->setToolTip(source->toolTip());
        proxy->setStatusTip(source->statusTip());
        proxy->setWhatsThis(source->whatsThis());
        proxy->setEnabled(source->isEnabled());
        proxy->setVisible(source->isVisible());
        proxy->setCheckable(source->isCheckable());
        proxy->setChecked(source->isChecked());
    });

    return proxy;
}

QMenu* createProxyMenu(QAction* source,
                       Gui::Action* commandAction,
                       const RibbonCommandDefinition& definition,
                       QWidget* parent)
{
    if (!source) {
        return nullptr;
    }

    const QString text = definition.label ? ribbonText(definition.label) : source->text();
    auto* menu = new QMenu(text, parent);
    menu->setObjectName(stableObjectName(QStringLiteral("McStudioRibbonMenu"),
                                         QString::fromLatin1(definition.command ? definition.command : "Unknown")));
    menu->setIcon(source->icon());
    menu->setToolTip(source->toolTip());
    menu->setStatusTip(source->statusTip());

    if (auto* actionGroup = dynamic_cast<Gui::ActionGroup*>(commandAction)) {
        const QList<QAction*> actions = actionGroup->actions();
        if (!actions.isEmpty()) {
            menu->addActions(actions);
        }
    }
    else if (QMenu* sourceMenu = source->menu()) {
        menu->addActions(sourceMenu->actions());
    }
    else {
        menu->addAction(source);
    }

    return menu;
}

void addCommandToPanel(SARibbonPanel* panel,
                       QWidget* initializer,
                       const RibbonCommandDefinition& definition)
{
    if (!panel || !definition.command) {
        return;
    }

    if (std::string(definition.command) == "Separator") {
        panel->addSeparator();
        return;
    }

    Gui::Command* command = commandByName(definition.command);
    QAction* source = ensureFreeCADAction(definition.command, initializer);
    if (!command || !source) {
        return;
    }

    Gui::Action* commandAction = command->getAction();
    const bool isDropdown = definition.style != RibbonCommandStyle::IconOnly
        && (dynamic_cast<Gui::ActionGroup*>(commandAction) || source->menu());

    if (isDropdown) {
        QMenu* menu = createProxyMenu(source, commandAction, definition, panel);
        if (!menu) {
            return;
        }
        if (definition.size == RibbonCommandSize::Large) {
            panel->addLargeMenu(menu, QToolButton::InstantPopup);
        }
        else if (definition.size == RibbonCommandSize::Medium) {
            panel->addMediumMenu(menu, QToolButton::InstantPopup);
        }
        else {
            panel->addSmallMenu(menu, QToolButton::InstantPopup);
        }
        return;
    }

    QAction* proxy = createProxyAction(source, definition, panel);
    if (!proxy) {
        return;
    }

    if (definition.style == RibbonCommandStyle::IconOnly) {
        panel->addSmallAction(proxy);
    }
    else if (definition.size == RibbonCommandSize::Large) {
        panel->addLargeAction(proxy);
    }
    else if (definition.size == RibbonCommandSize::Medium) {
        panel->addMediumAction(proxy);
    }
    else {
        panel->addSmallAction(proxy);
    }
}

void populateCategory(SARibbonCategory* category,
                      QWidget* initializer,
                      const RibbonTabDefinition& tab,
                      const QString& objectNamePrefix)
{
    if (!category) {
        return;
    }

    const QString categoryTitle = ribbonText(tab.title);
    category->setObjectName(stableObjectName(objectNamePrefix, categoryTitle));

    for (const RibbonGroupDefinition& group : tab.groups) {
        const QString panelTitle = ribbonText(group.title);
        SARibbonPanel* panel = category->addPanel(panelTitle);
        if (!panel) {
            continue;
        }
        panel->setObjectName(stableObjectName(QStringLiteral("McStudioRibbonPanel"),
                                              categoryTitle + QLatin1Char('_') + panelTitle));

        for (const RibbonCommandDefinition& command : group.commands) {
            addCommandToPanel(panel, initializer, command);
        }
    }
}

bool activeDocumentIsInEditMode()
{
    if (!Gui::Application::Instance) {
        return false;
    }

    Gui::Document* document = Gui::Application::Instance->activeDocument();
    return document && Gui::Application::Instance->isInEdit(document);
}

bool hasActiveSelection()
{
    return Gui::Selection().size() > 0;
}

struct ContextCategoryHandle
{
    QString id;
    SARibbonContextCategory* context {nullptr};
};

class SARibbonBackend final: public McStudioRibbonBackend
{
public:
    explicit SARibbonBackend(QWidget* parent)
        : McStudioRibbonBackend(parent)
    {
        _ribbon = new SARibbonBar(parent);
        _ribbon->setObjectName(QStringLiteral("McStudioSARibbonBar"));
        _ribbon->setMinimumWidth(0);
        _ribbon->setMaximumWidth(QWIDGETSIZE_MAX);
        _ribbon->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        applyMcStudioSARibbonTheme(_ribbon);
        applyRibbonIconMetrics(_ribbon);

        rebuild();
        connectContextSignals();
    }

    ~SARibbonBackend() override
    {
        _inEditConnection.disconnect();
        _resetEditConnection.disconnect();
        _selectionConnection.disconnect();
    }

    SARibbonBackend(const SARibbonBackend&) = delete;
    SARibbonBackend& operator=(const SARibbonBackend&) = delete;
    SARibbonBackend(SARibbonBackend&&) = delete;
    SARibbonBackend& operator=(SARibbonBackend&&) = delete;

    QWidget* widget() override
    {
        return _ribbon;
    }

private:
    void rebuild()
    {
        if (!_ribbon) {
            return;
        }

        QWidget* initializer = _ribbon;
        for (const RibbonTabDefinition& tab : ribbonTabs()) {
            const QString categoryTitle = ribbonText(tab.title);
            SARibbonCategory* category = _ribbon->addCategoryPage(categoryTitle);
            if (!category) {
                continue;
            }
            populateCategory(category, initializer, tab, QStringLiteral("McStudioRibbonCategory"));
        }

        buildContextCategories(initializer);
        refreshContextCategories();

        const auto& tabs = ribbonTabs();
        for (int index = 0; index < static_cast<int>(tabs.size()); ++index) {
            if (tabs[static_cast<std::size_t>(index)].title
                && std::strcmp(tabs[static_cast<std::size_t>(index)].title, QT_TR_NOOP("Sketch")) == 0) {
                _ribbon->setCurrentIndex(index);
                Base::Console().message("McStudio SARibbon: default active tab set to Sketch at index %d.\n", index);
                break;
            }
        }
    }

    void buildContextCategories(QWidget* initializer)
    {
        _contextCategories.clear();
        for (const RibbonContextDefinition& contextDefinition : ribbonContexts()) {
            const QString contextTitle = ribbonText(contextDefinition.title);
            const QString contextId = QString::fromLatin1(contextDefinition.id ? contextDefinition.id : "UnnamedContext");
            const QColor contextColor = contextDefinition.color ? QColor(QString::fromLatin1(contextDefinition.color))
                                                                : QColor();
            SARibbonContextCategory* context = _ribbon->addContextCategory(contextTitle, contextColor, contextId);
            if (!context) {
                continue;
            }
            context->setObjectName(stableObjectName(QStringLiteral("McStudioRibbonContext"), contextId));

            for (const RibbonTabDefinition& categoryDefinition : contextDefinition.categories) {
                const QString categoryTitle = ribbonText(categoryDefinition.title);
                SARibbonCategory* category = context->addCategoryPage(categoryTitle);
                if (!category) {
                    continue;
                }
                populateCategory(category,
                                 initializer,
                                 categoryDefinition,
                                 QStringLiteral("McStudioRibbonContextCategory"));
            }

            _ribbon->hideContextCategory(context);
            _contextCategories.push_back({contextId, context});
        }
    }

    // Context categories used to be refreshed by a 500ms polling timer that ran
    // forever, even while the McStudio workbench was inactive. Drive the refresh
    // from FreeCAD's edit-mode and selection signals instead. A zero-delay
    // single-shot timer coalesces bursts (e.g. multi-object selection) into one
    // relayout. The fastsignals connections are stored as members and severed in
    // the destructor, so no callback outlives this backend.
    void connectContextSignals()
    {
        _contextRefreshTimer = new QTimer(this);
        _contextRefreshTimer->setSingleShot(true);
        _contextRefreshTimer->setInterval(0);
        QObject::connect(_contextRefreshTimer, &QTimer::timeout, this, [this]() {
            refreshContextCategories();
        });

        QPointer<QTimer> timerGuard(_contextRefreshTimer);
        auto scheduleRefresh = [timerGuard]() {
            if (timerGuard) {
                timerGuard->start();
            }
        };

        if (Gui::Application::Instance) {
            _inEditConnection = Gui::Application::Instance->signalInEdit.connect(
                [scheduleRefresh](const Gui::ViewProviderDocumentObject&) { scheduleRefresh(); });
            _resetEditConnection = Gui::Application::Instance->signalResetEdit.connect(
                [scheduleRefresh](const Gui::ViewProviderDocumentObject&) { scheduleRefresh(); });
        }
        _selectionConnection = Gui::Selection().signalSelectionChanged.connect(
            [scheduleRefresh](const Gui::SelectionChanges&) { scheduleRefresh(); });
        // Initial visibility state is already established by rebuild()'s call to
        // refreshContextCategories() before connectContextSignals() runs.
    }

    void refreshContextCategories()
    {
        if (!_ribbon) {
            return;
        }

        const bool showSketchTools = activeDocumentIsInEditMode();
        const bool showSelectionTools = hasActiveSelection();

        for (const ContextCategoryHandle& handle : _contextCategories) {
            if (!handle.context) {
                continue;
            }

            bool visible = false;
            if (handle.id == QLatin1String("SketchContext")) {
                visible = showSketchTools;
            }
            else if (handle.id == QLatin1String("SelectionContext")) {
                visible = showSelectionTools;
            }
            else if (handle.id == QLatin1String("ManufacturingContext")) {
                visible = false;
            }

            if (_ribbon->isContextCategoryVisible(handle.context) != visible) {
                _ribbon->setContextCategoryVisible(handle.context, visible);
            }
        }
    }

private:
    SARibbonBar* _ribbon {nullptr};
    QTimer* _contextRefreshTimer {nullptr};
    std::vector<ContextCategoryHandle> _contextCategories;
    fastsignals::connection _inEditConnection;
    fastsignals::connection _resetEditConnection;
    fastsignals::connection _selectionConnection;
};
}  // namespace

McStudioRibbonBackend* createSARibbonBackend(QWidget* parent)
{
    return new SARibbonBackend(parent);
}

}  // namespace McStudioGui
