# McStudio Top Dock Ribbon 实施计划

日期：2026-06-30
状态：Implementation Plan

## 目标

将 McStudio Ribbon 从错误的 `centralWidget wrapper` 方案修正为：

```text
QMainWindow
  TopDockWidgetArea
    McStudioRibbonDockHost(QDockWidget, 固定顶部、无标题栏)
      McStudioRibbonHost(QFrame)
        QTabWidget
          Ribbon pages/groups/commands

  LeftDockWidgetArea
    ComboView / TaskView / Model tree

  CentralWidget
    QMdiArea / 3D View
```

目标效果：

1. Ribbon 横跨左侧模型/任务面板和右侧 MDI 视图区上方。
2. 不再把完整 Ribbon 塞进 `QToolBar`。
3. 不再包装 `centralWidget`，避免 Ribbon 只覆盖右侧 MDI 区。
4. 默认行为仿 NX：固定顶部、稳定、不可关闭、不可浮动、不可拖拽。
5. 后续可通过偏好项演进成 SOLIDWORKS CommandManager 风格的整体可拖拽/可浮动。

## 设计决策

### 1. 外层使用 QDockWidget，不使用 QToolBar

原因：

- `QToolBar` 会参与 FreeCAD 工具栏换行、锁定、拖拽、持久化，导致布局被拉伸。
- `QDockWidget` 可放入 `TopDockWidgetArea`，在 Qt 主窗口布局中位于 left/right dock 与 central widget 之上。

### 2. 默认固定顶部

第一版：

```cpp
dock->setAllowedAreas(Qt::TopDockWidgetArea);
dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
dock->setFloating(false);
```

不让用户拖拽，优先保证稳定和接近 NX 的 Ribbon bar。

### 3. 隐藏 dock 标题栏和菜单入口

```cpp
auto* hiddenTitleBar = new QWidget(dock);
hiddenTitleBar->setFixedHeight(0);
dock->setTitleBarWidget(hiddenTitleBar);
dock->toggleViewAction()->setVisible(false);
dock->toggleViewAction()->setEnabled(false);
```

避免 McStudio Ribbon 出现在普通 dock 菜单里，被误关或误拖。

### 4. 设置 QMainWindow corner 策略

为了让顶部 dock 横跨左侧/右侧 dock 上方，需要明确：

```cpp
mainWindow->setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
mainWindow->setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
```

这一步很关键，否则某些 Qt 布局状态下 left/right dock 可能占据顶部角落。

## 修改步骤

### Step 1：清理错误方案

删除或替换：

```cpp
CentralHostObjectName
ensureCentralHost(...)
mainWindow->takeCentralWidget()
mainWindow->setCentralWidget(...)
```

### Step 2：新增 Top Dock Host

新增：

```cpp
RibbonDockObjectName = "McStudioRibbonDockHost"
findRibbonDockHost(...)
configureRibbonDockHost(...)
ensureRibbonDockHost(...)
```

### Step 3：show/hide 行为

`show()`：

1. 找 MainWindow。
2. 创建或复用 `McStudioRibbonDockHost`。
3. 强制它回到 `TopDockWidgetArea`。
4. 显示 dock 和内部 RibbonHost。

`hide()`：

1. 找 `McStudioRibbonDockHost`。
2. 隐藏 dock。

### Step 4：QSS 修正

外层选择器使用：

```qss
QDockWidget#McStudioRibbonDockHost
QWidget#McStudioRibbonDockTitleBar
QFrame#McStudioRibbonHost
```

### Step 5：验证

编译：

```bash
cmake --build build/debug --target McStudio McStudioGui
```

手动 GUI 验证：

1. 启动 `build/debug/bin/FreeCAD`。
2. 切换 McStudio。
3. Ribbon 是否横跨左侧模型/任务面板上方。
4. 左侧模型/任务面板是否从 Ribbon 下方开始。
5. 切换草图/特征 tab 是否高度稳定。
6. 离开 McStudio 后 Ribbon 是否隐藏。
7. 再切回 McStudio 是否不重复创建 dock。
8. View/Dock 菜单里是否没有 McStudio Ribbon 可关闭项。
9. 关闭/重开 FreeCAD 后布局是否不乱。

## 后续演进

第二阶段增加偏好项：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Movable = false
```

当 `Movable=true` 时允许：

```cpp
QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
```

这是向 SOLIDWORKS CommandManager 风格演进。

