# McStudio Ribbon 内部控件长期方案：QToolButton 替代 QToolBar

日期：2026-06-30
状态：Implementation Plan

## 背景

FreeCAD 普通工具栏尺寸由统一参数控制：

```text
User parameter:BaseApp/Preferences/General/ToolbarIconSize
```

并由 `ToolBarManager::setupToolBarIconSize()` 扫描 `MainWindow` 下所有 `QToolBar` 后执行：

```cpp
toolbar->setIconSize(QSize(s, s));
```

McStudio Ribbon 外层已经改成 `Top QDockWidget Host`，但内部命令容器仍然使用 `QToolBar`。这会导致：

1. McStudio 内部 command bar 被 FreeCAD 当作普通 toolbar 扫描。
2. large/small/icon 三种 Ribbon 尺寸被统一覆盖。
3. Ribbon 内部布局受 QToolBar sizeHint/ToolBarManager 影响，难以稳定仿 NX/SOLIDWORKS。

## 决策

长期方案：

```text
QDockWidget McStudioRibbonDockHost
  QFrame McStudioRibbonHost
    QTabWidget
      QScrollArea page
        QFrame group
          QWidget command strip
            QToolButton / QFrame separator
```

不再在 Ribbon 内部使用 `QToolBar`。

## 设计原则

1. 命令仍来自 FreeCAD `Gui::CommandManager`。
2. 通过 `Command::addTo(QWidget*)` 初始化命令 QAction。
3. 使用 `QToolButton::setDefaultAction(QAction*)` 复用 FreeCAD QAction。
4. 如果命令是 `Gui::ActionGroup`，用 `QMenu` 承载 group actions，模拟 toolbar drop-down。
5. 图标尺寸从 `ToolbarIconSize` 派生：
   - smallIconSize = toolbar base size
   - largeIconSize = max(base + 8, base * 1.35)
6. Ribbon 高度从图标尺寸派生，不再写死魔法高度。

## 验证

1. `findChildren<QToolBar*>` 不再包含 McStudio 内部 command bars。
2. Ribbon 命令点击仍正常。
3. 下拉组合命令仍可展开。
4. 更改 FreeCAD 工具栏图标尺寸后，重建 Ribbon 能跟随尺寸。
5. 构建通过。
