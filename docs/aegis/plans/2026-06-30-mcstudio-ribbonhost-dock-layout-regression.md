# McStudio RibbonHost Dock Layout Regression 调研

日期：2026-06-30
状态：Root cause identified

## 现象

切换到 McStudio 后，Ribbon 显示在右侧视图区上方，但左侧“模型/任务”面板仍然从窗口左侧直接占据整列。视觉结果是：

```text
菜单栏 / FreeCAD 顶栏
左侧 ComboView / TaskView | McStudio Ribbon
左侧 ComboView / TaskView | MDI 视图区
```

而目标是：

```text
菜单栏 / FreeCAD 顶栏
McStudio Ribbon 横跨工作区宽度
左侧 ComboView / TaskView | MDI 视图区
```

## 根因

当前长期架构补丁把 Ribbon 插到了 `QMainWindow::centralWidget()` 内：

```cpp
// src/Mod/McStudio/Gui/RibbonWidget.cpp
QWidget* originalCentral = mainWindow->takeCentralWidget();
...
layout->addWidget(createRibbonHost(centralHost, metrics), 0);
layout->addWidget(originalCentral, 1);
mainWindow->setCentralWidget(centralHost);
```

但 FreeCAD 左侧“模型/任务”面板不是 central widget 的子控件，而是 `QDockWidget`：

```cpp
// src/Gui/Workbench.cpp
root->addDockWidget("Std_ComboView", Qt::LeftDockWidgetArea, Gui::DockWindowOption::VisibleTabbed);
root->addDockWidget("Std_TaskView", Qt::RightDockWidgetArea, Gui::DockWindowOption::VisibleTabbed);
```

`Std_ComboView` 的实际 dock 创建位置：

```cpp
// src/Gui/MainWindow.cpp
_updateDockWidget("Std_ComboView", enable, show, Qt::LeftDockWidgetArea, ...);
```

DockWindowManager 最终通过 `QMainWindow::addDockWidget()` 加入 dock 区：

```cpp
// src/Gui/DockWindowManager.cpp
mw->addDockWidget(pos, dw);
```

因此 Qt 布局层级实际是：

```text
QMainWindow
  TopToolBarArea / MenuBar
  Dock layout
    LeftDockWidgetArea: Std_ComboView / TaskView
    CentralWidget: McStudioRibbonCentralHost
      McStudioRibbonHost
      QMdiArea
```

所以 `centralWidget` 包装方案只能控制中间 MDI 区，不能控制左/右/下 dock 区。

## 结论

`centralWidget` 包装方案不满足 McStudio Ribbon 的视觉目标。它解决了“脱离 QToolBar”的问题，但引入了“Ribbon 只覆盖中央区，不覆盖 dock 区”的布局回归。

## 正确架构选择

要让 Ribbon 横跨左侧 dock 与 MDI 上方，Ribbon 必须插入到 `QMainWindow` dock layout 之上，而不是 central widget 内部。

可选方案：

### A. Top QDockWidget Host

使用一个固定高度、无标题栏、不可关闭、不可浮动、不可移动的 `QDockWidget` 放到 `Qt::TopDockWidgetArea`：

```cpp
auto* dock = new QDockWidget(mainWindow);
dock->setObjectName("McStudioRibbonDockHost");
dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
dock->setTitleBarWidget(new QWidget(dock));
dock->setWidget(createRibbonHost(dock, metrics));
mainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
```

优点：

- 位于 central widget 和 left/right dock 之上。
- 比完整塞进 `QToolBar` 更符合 Qt 主窗口区域布局。
- 不需要修改 FreeCAD 核心 `MainWindow`。

风险：

- 仍属于 Qt dock 系统，需要禁用 close/move/float。
- 可能进入 View/Dock menu，需要隐藏 toggle action。
- 可能被 `saveState/restoreState` 影响，需要 objectName 稳定且每次激活修正 features/area。

### B. FreeCAD MainWindow Core Extension

在 `Gui::MainWindow` 增加正式 API：

```cpp
void setWorkbenchCommandSurface(QWidget* widget);
void clearWorkbenchCommandSurface(QWidget* widget);
```

MainWindow 内部自己维护一个位于 dock layout 之上的 command surface 区域。

优点：长期最干净。
缺点：侵入 FreeCAD 核心，改动范围大，rebase 上游风险高。

### C. 回到 QToolBar 但严格固定高度

可让 Ribbon 横跨全窗口，但语义仍是 toolbar，且会继续受 ToolBarManager、工具栏锁定、换行、拖拽体系影响。

不推荐作为长期方案。

## 推荐下一步

放弃当前 `centralWidget` wrapper 方向，改为 A：`Top QDockWidget Host`。先实现为 McStudio 私有 host，不接入 DockWindowManager，禁用全部 dock 特性，并隐藏 toggle action。

同时保留内部 `RibbonHost QFrame`，不要回到 `QToolBar` 外壳。

## 需要回滚/替换的代码点

当前需要替换掉：

```cpp
QWidget* ensureCentralHost(...)
mainWindow->takeCentralWidget();
mainWindow->setCentralWidget(centralHost);
```

改为：

```cpp
QDockWidget* ensureRibbonDockHost(...)
mainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
```

`hide()` 改为隐藏 dock host，而不是只隐藏 central 内部的 ribbon frame。

## 验证点

1. McStudio Ribbon 横跨左侧 ComboView 和 MDI 上方。
2. 左侧模型/任务面板从 Ribbon 下方开始，而不是顶到 Ribbon 左边。
3. 离开 McStudio 后 Ribbon dock 隐藏。
4. 再切回 McStudio 不重复创建 dock。
5. View/Dock 菜单不出现可关闭的 McStudio Ribbon。
6. 工具栏锁定、保存/恢复窗口状态后 Ribbon 仍能回到顶部。
