# McStudio Ribbon Oversized Host Refactor

日期：2026-06-30
状态：Implementation

## 目标

修复 SARibbon 接入后顶部 Ribbon/DockHost 总体高度过长的问题。

## 审查结论

过高不是单个按钮问题，而是两套高度系统叠加：

```text
旧 Custom Ribbon metrics.ribbonMaxHeight
+ SARibbonBarLayout::setFixedHeight(mainBarHeight)
```

当前代码中 Host 和 DockHost 都使用：

```cpp
max(metrics.ribbonMaxHeight, saribbonHeight)
```

导致 SARibbon 不能按自己的高度收缩。

## 修改计划

1. 新增 helper：`preferredFixedHeight(widget, fallback)`。
2. SARibbon backend 存在时，Host 高度直接使用 backend widget 的 preferred height。
3. DockHost 高度直接使用 content widget 的 preferred height，不再用旧 metrics 托底。
4. Existing host 重新激活时，不再强制设置 `metrics.ribbonMaxHeight`。
5. McStudioTheme 改为嵌入模式：
   - `RibbonStyleCompactThreeRow`
   - `titleBarHeight = 0`
   - `tabBarHeight = 26`
   - `panelTitleHeight = 18`
   - `panelSpacing = 6`
6. 不改 SARibbon 源码。

## 2026-06-30 执行记录

已完成第一轮修复。

### 1. Host / DockHost 高度不再被旧 metrics 托底

新增 helper：

```cpp
int preferredFixedHeight(QWidget* widget, int fallbackHeight)
```

它按顺序读取：

```text
sizeHint.height
minimumSizeHint.height
minimumHeight
current height
fallback
```

SARibbon backend 存在时，`McStudioRibbonHost` 直接使用 backend widget 的 preferred height：

```cpp
host->setMinimumHeight(backendHeight);
host->setMaximumHeight(backendHeight);
```

不再使用：

```cpp
max(metrics.ribbonMaxHeight, backendHeight)
```

`McStudioRibbonDockHost` 也改为直接使用 content widget 的 preferred height：

```cpp
dockHeight = preferredFixedHeight(content, metrics.ribbonMaxHeight);
```

不再使用旧的：

```cpp
max(metrics.ribbonMaxHeight, contentHeight)
```

### 2. 修复配置顺序

`ensureRibbonDockHost()` 现在先确保 dock widget 存在，再调用：

```cpp
configureRibbonDockHost(dock, metrics)
```

避免已有 dock 但 widget 为空时先按旧 metrics 配置高度。

### 3. SARibbon 改为嵌入式紧凑模式

`McStudioThemeTokens` 新增：

```cpp
tabBarHeight = 26
titleBarHeight = 0
panelSpacing = 6
panelTitleHeight = 18
buttonMaximumAspectRatio = 1.25
```

`applyMcStudioSARibbonTheme()` 改为：

```cpp
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
ribbon->setTitleBarHeight(tokens.titleBarHeight);
ribbon->setTabBarHeight(tokens.tabBarHeight);
ribbon->setPanelTitleHeight(tokens.panelTitleHeight);
```

这样 SARibbon 作为 FreeCAD TopDockHost 内嵌控件，不再保留自己的 title bar 高度。

### 4. 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

仍有既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮高度修复无关。
