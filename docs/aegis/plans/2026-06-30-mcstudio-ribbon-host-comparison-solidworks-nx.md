# McStudio Ribbon 宿主架构调研：对标 SOLIDWORKS 与 Siemens NX

日期：2026-06-30
状态：Research / Decision Pending
范围：McStudio Ribbon 在 FreeCAD/Qt 主窗口中的插入方式

## 问题

McStudio 需要一个类似 SOLIDWORKS CommandManager / Siemens NX Ribbon bar 的顶部命令区。当前已经试过两类方案：

1. `QToolBar` 外壳：能横跨窗口，也有拖拽能力，但会被 FreeCAD 工具栏系统拉伸、换行、持久化，视觉不稳定。
2. `centralWidget` wrapper：脱离工具栏系统，但只覆盖 MDI 中央区，无法覆盖左侧模型/任务 dock 面板上方。

现在需要判断哪种长期方案更适合：

- 类 SOLIDWORKS CommandManager
- 类 Siemens NX Ribbon bar
- Qt/FreeCAD 可维护实现

## 对标结论

### SOLIDWORKS CommandManager

核心特征：

- 是上下文敏感的命令工具区。
- 使用 tabs 切换不同工具集。
- 默认跟随文档类型和任务环境更新。
- 用于集中访问命令，并节省图形区空间。
- 可以浮动，也可以停靠到上方、左侧或右侧。
- 可以自定义 tabs、按钮、文字显示方式、分组。

适合 McStudio 借鉴的点：

1. 顶部命令区应该默认 dock 在顶部。
2. Ribbon/CommandManager 可以作为一个整体拖拽/浮动，但不应该像普通小 toolbar 一样参与每个按钮级别的自由换行。
3. 按任务上下文动态更新 tab 内容。
4. 允许用户自定义 tab、命令和显示文字。

不适合立即照搬的点：

- McStudio 当前没有完整 customize 系统。
- FreeCAD 当前 toolbar/dock 混合布局复杂，直接做可浮动 CommandManager 容易再次引起布局混乱。

### Siemens NX Ribbon bar

核心特征：

- Ribbon bar 由 tabs 和 groups 组织命令。
- NX 有 Resource bar，承载 Part Navigator、Roles 等导航面板。
- NX 有 Roles 概念，用于控制不同用户经验层级、行业、设备形态下显示哪些命令和布局。
- NX 有 Command Finder，用搜索来弥补 Ribbon 命令隐藏或过多的问题。
- NX 还有 Border bars，可放置高频命令，避免频繁切 tab。

适合 McStudio 借鉴的点：

1. Ribbon 顶部固定，左侧模型树/任务面板属于独立导航区。
2. 用 Role/Profile 控制命令复杂度，例如 Welcome / Essentials / Advanced。
3. 高级命令不一定全铺在 Ribbon 首页，可通过搜索、更多菜单、角色切换暴露。
4. Ribbon 不需要默认可拖拽；布局稳定和上下文效率优先。

不适合立即照搬的点：

- NX 的 Roles、Command Finder、Border bars 是完整 UI 体系，McStudio 当前不应一次性实现。

## FreeCAD/Qt 约束

### FreeCAD 主窗口布局

`Gui::MainWindow` 是 `QMainWindow`。

- 中央区是 `QMdiArea`。
- 左侧模型/任务面板是 `QDockWidget`。
- 顶部普通工具栏是 `QToolBar`。

因此：

```text
QMainWindow
  MenuBar / ToolBarArea
  Dock layout
    TopDockWidgetArea
    LeftDockWidgetArea
    RightDockWidgetArea
    CentralWidget(QMdiArea)
```

### centralWidget wrapper 不适合

把 Ribbon 放入 `centralWidget` 只能影响 `QMdiArea` 上方，不能影响左侧 dock 区上方。所以它会导致 Ribbon 只在右侧视图区上方显示，左侧模型/任务面板仍然顶到窗口上沿。

### QToolBar 不适合作为完整 Ribbon 外壳

`QToolBar` 的语义是普通工具栏。它会参与：

- toolbar 换行
- toolbar 拖拽
- toolbar lock
- toolbar menu
- ToolBarManager restore/save

把完整 `QTabWidget + QScrollArea + Ribbon groups` 放进去，会与 FreeCAD 工具栏系统冲突。

## 方案比较

### 方案 A：QToolBar 外壳

```text
QToolBar
  QTabWidget
    Ribbon pages
```

优点：

- 横跨主窗口。
- 原生支持拖拽、浮动、停靠。
- 实现简单。

缺点：

- 容易被拉长、换行、高度失控。
- 会进入普通工具栏管理逻辑。
- 与真正 Ribbon 语义不匹配。

结论：不推荐长期使用。

### 方案 B：centralWidget wrapper

```text
CentralWidget
  RibbonHost
  QMdiArea
```

优点：

- 脱离 toolbar 系统。
- 高度可控。
- 不影响 FreeCAD toolbar。

缺点：

- 不能覆盖左侧/右侧 dock 面板上方。
- 与 SOLIDWORKS/NX 的整体顶部命令区不一致。

结论：已经验证不合适，应回退。

### 方案 C：Top QDockWidget Host

```text
TopDockWidgetArea
  McStudioRibbonDockHost(QDockWidget, fixed, no title)
    McStudioRibbonHost(QFrame)
      QTabWidget
        Ribbon groups
```

优点：

- 能横跨 left dock + central widget 上方。
- 不使用 QToolBar 外壳。
- 不修改 FreeCAD 核心 `MainWindow`。
- 可选择默认固定，未来再开放可拖拽。
- 更接近 NX 的稳定顶部 Ribbon，也能兼容 SOLIDWORKS CommandManager 的“整体可停靠”方向。

缺点：

- 属于 Qt dock 系统，需要严格禁用 title/close/move/float。
- 需要隐藏 toggleViewAction，避免出现在 dock 菜单里。
- 需要验证 saveState/restoreState 后是否回到顶部。

结论：推荐作为当前长期方案第一版。

### 方案 D：修改 FreeCAD MainWindow，增加官方 CommandSurface API

```cpp
void MainWindow::setWorkbenchCommandSurface(QWidget*)
void MainWindow::clearWorkbenchCommandSurface(QWidget*)
```

优点：

- 架构最干净。
- 能在 MainWindow 内部精确插入 command surface。
- 可供未来其他 workbench 复用。

缺点：

- 侵入 FreeCAD 核心。
- rebase 上游风险大。
- 当前 McStudio 原型阶段成本过高。

结论：适合作为后续 ADR，不适合当前立即实现。

## 推荐决策

当前最适合 McStudio 的方案是：

```text
Top QDockWidget Host，默认固定，不可拖拽，不可关闭，不可浮动。
```

它更接近 NX 的稳定 Ribbon bar，也保留了未来向 SOLIDWORKS CommandManager “整体可拖拽/可浮动”演进的可能。

## 推荐行为设计

### 第一阶段：固定顶部 Ribbon

默认：

```cpp
dock->setAllowedAreas(Qt::TopDockWidgetArea);
dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
dock->setTitleBarWidget(hiddenZeroHeightWidget);
dock->toggleViewAction()->setVisible(false);
```

目标：

- 稳定。
- 横跨左侧模型/任务面板上方。
- 不进入普通工具栏系统。
- 不让用户误拖乱。

### 第二阶段：加入偏好项

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Movable = false
```

当 `Movable=true` 时：

```cpp
dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
```

这个更像 SOLIDWORKS CommandManager。

### 第三阶段：加入 Role/Profile

仿 NX：

```text
Welcome / Basic / Advanced / Manufacturing
```

控制 Ribbon tab 和 group 的复杂度。

### 第四阶段：Command Finder

仿 NX：

- 搜索 FreeCAD/McStudio 命令。
- 直接执行命令。
- 显示命令所在 tab/group。

## 执行建议

现在应该先把当前代码修正为方案 C，并保留内部 RibbonHost：

- 回退 `centralWidget wrapper`。
- 使用 `QDockWidget` 作为外层宿主。
- 内部继续使用 `QFrame#McStudioRibbonHost` 和现有 Ribbon tabs/groups。
- 默认固定顶部。

