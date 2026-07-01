# McStudio Ribbon Oversized Width Refactor

日期：2026-06-30
状态：Implementation

## 问题纠正

上一轮误判为高度问题。用户确认问题是：

```text
长度很长，不是高度
```

这里的“长度”是水平最小宽度过长：SARibbon 接入后，顶部 Dock/Ribbon 可能把 FreeCAD 主窗口最小宽度撑得很大。

## 根因

SARibbonBarLayout 会计算自身最小宽度：

```cpp
int minWidth { 500 };
int maxMinWidth { 1000 };
maxMinWidth = screenGeometry.width() * 0.8;
```

`SARibbonBarLayout::sizeHint()` / `minimumSize()` 返回：

```cpp
return QSize(width, height);
```

其中 width 来自 tab/corner/title/quick access 等区域计算。

在独立 `SARibbonMainWindow` 中这合理；但在 FreeCAD `QDockWidget TopDockHost` 内嵌时，这个最小宽度会向上传递给：

```text
SARibbonBar -> McStudioRibbonHost layout -> QDockWidget -> Gui::MainWindow
```

导致整个主窗口水平最小宽度被拉长。

## 正确策略

SARibbon 作为内嵌 TopDock 控件时，不能把自己的 sizeHint width 当作主窗口最小宽度。

应改为：

```text
外层宽度由 FreeCAD MainWindow / DockArea 决定
SARibbon 横向可压缩
超出部分由 SARibbonCategoryLayout 的左右滚动按钮处理
```

## 修改计划

1. 新增 `relaxHorizontalSize(QWidget*)`。
2. 对 `SARibbonBar` 设置：
   - `minimumWidth = 0`
   - `maximumWidth = QWIDGETSIZE_MAX`
   - horizontal size policy = `QSizePolicy::Ignored`
   - vertical size policy = `QSizePolicy::Fixed`
3. 对 `McStudioRibbonHost` 设置：
   - `minimumWidth = 0`
   - `maximumWidth = QWIDGETSIZE_MAX`
4. 对 `McStudioRibbonDockHost` 设置：
   - `minimumWidth = 0`
   - `maximumWidth = QWIDGETSIZE_MAX`
5. 保留高度固定逻辑，不再动高度。

## 预期效果

- SARibbon 不再强行拉长 FreeCAD 主窗口最小宽度。
- 当窗口较窄时，SARibbon 内部 category/panel 使用自身滚动/overflow。
- 顶部 Dock 仍然横向填满主窗口，但不要求主窗口变得很宽。

## 2026-06-30 执行记录

已完成第一轮横向宽度修复。

### 1. RibbonWidget 横向约束放松

新增：

```cpp
void relaxHorizontalSize(QWidget* widget)
```

它会设置：

```cpp
widget->setMinimumWidth(0);
widget->setMaximumWidth(QWIDGETSIZE_MAX);
```

已应用到：

```text
McStudioRibbonHost
backendWidget / SARibbonBar
McStudioRibbonDockHost
已有 ribbonHost 重新激活路径
```

### 2. SARibbonBar 横向 sizePolicy 改为 Ignored

`McStudioSARibbonBackend.cpp` 中：

```cpp
_ribbon->setMinimumWidth(0);
_ribbon->setMaximumWidth(QWIDGETSIZE_MAX);
_ribbon->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
```

目的：阻止 `SARibbonBarLayout::sizeHint().width()` / `minimumSize().width()` 把 FreeCAD 主窗口最小宽度撑大。

SARibbon 内部如果内容超出可用宽度，应由 `SARibbonCategoryLayout` 的 scroll/overflow 机制处理。

### 3. 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮横向宽度修复无关。
