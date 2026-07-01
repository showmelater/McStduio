# McStudio Ribbon Oversized Host Analysis

日期：2026-06-30
状态：Aegis Analysis

## 问题

用户反馈：SARibbon 可以显示，但总体窗口长度/高度太长。

结合当前截图和源码，问题主要指顶部 Ribbon/DockHost 的整体高度过长；横向宽度方面，McStudioRibbonDockHost 本来被设计为横跨 FreeCAD 顶部区域，这是预期行为，不是主要异常。

## 结论

当前过长的核心原因是：

```text
旧 Custom Ribbon 外层高度策略
+ SARibbon 自己的 setFixedHeight 主栏高度
+ LooseThreeRow 较高模式
+ panelTitleHeight = 21
```

共同把 `McStudioRibbonDockHost` 撑高了。

## 代码证据

### 1. 旧 RibbonMetrics 仍在控制 SARibbon 外层高度

`RibbonWidget.cpp` 中旧 metrics：

```cpp
metrics.pageMaxHeight = metrics.largeIconSize + scaledInt(70, metrics.scale);
metrics.ribbonMaxHeight = metrics.pageMaxHeight + scaledInt(30, metrics.scale);
```

如果 FreeCAD toolbar icon size 为 24：

```text
largeIconSize ≈ 32
pageMaxHeight ≈ 102
ribbonMaxHeight ≈ 132
```

这套高度原本是为了旧的 `QTabWidget + QScrollArea + QToolButton`，不应该继续强行约束 SARibbon。

### 2. Host 创建时先设置旧 maximumHeight

```cpp
host->setMaximumHeight(metrics.ribbonMaxHeight);
```

然后 SARibbon backend 创建后又根据 SARibbon sizeHint 调整：

```cpp
const int backendHeight = backendWidget->sizeHint().height();
if (backendHeight > 0) {
    host->setMinimumHeight(backendHeight);
    host->setMaximumHeight(std::max(metrics.ribbonMaxHeight, backendHeight));
}
```

这里的问题是：

```text
外层 maximumHeight = max(旧 ribbonMaxHeight, SARibbon sizeHint)
```

也就是说，即使 SARibbon 实际可以更矮，旧 `ribbonMaxHeight` 也会托底，让 Host 不会低于旧高度。

### 3. DockHost 也继续用旧 ribbonMaxHeight 托底

```cpp
int dockHeight = metrics.ribbonMaxHeight;
if (QWidget* content = dock->widget()) {
    const int contentHeight = content->sizeHint().height();
    if (contentHeight > 0) {
        dockHeight = std::max(dockHeight, contentHeight);
    }
}

dock->setMinimumHeight(dockHeight);
dock->setMaximumHeight(dockHeight);
```

这里同样是：

```text
dockHeight = max(旧 ribbonMaxHeight, SARibbon contentHeight)
```

这会导致 SARibbon 接入后，DockHost 仍然被旧手写 Ribbon 的高度系统撑住。

### 4. SARibbon 自己也会 setFixedHeight

SARibbon 源码 `SARibbonBarLayout.cpp`：

```cpp
int mainBarHeight = calcMainBarHeight(...);
ribbonBar->setFixedHeight(mainBarHeight);
```

高度计算公式：

```cpp
categoryHeight = font.lineSpacing() * 4.8 + panelTitleHeight;
mainBarHeight = tabBarHeight + titleBarHeight + categoryHeight;
```

如果使用 `RibbonStyleLooseThreeRow`，并且 `panelTitleHeight=21`，SARibbon 本身会比 Compact 模式更高。

### 5. 当前 McStudioTheme 选择了 LooseThreeRow

`McStudioTheme.cpp` 当前：

```cpp
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
ribbon->setPanelSpacing(tokens.panelSpacing);
ribbon->setPanelTitleHeight(tokens.panelTitleHeight); // 21
ribbon->setButtonMaximumAspectRatio(tokens.buttonMaximumAspectRatio);
```

LooseThreeRow 是为了改善拥挤感，但它确实会增加整体高度。

## 主因排序

### 第一主因：旧 Custom Ribbon 高度托底没有删除

`metrics.ribbonMaxHeight` 仍然用于 SARibbon Host/DockHost。

这是最大问题。

### 第二主因：SARibbon LooseThreeRow 本身偏高

SARibbon 的三行宽松布局自然更高。

### 第三主因：panelTitleHeight = 21 偏舒展

这对视觉有利，但进一步增加高度。

### 第四主因：titleBarHeight 可能没有归零

SARibbonBar 继承自类似 Ribbon 主栏体系，默认可能包含 titleBarHeight。作为嵌入 FreeCAD TopDockHost 的 Ribbon，不需要自己的 title area。

如果当前 SARibbon 没有把 titleBarHeight 压到 0，则高度公式会包含：

```text
tabBarHeight + titleBarHeight + categoryHeight
```

其中 titleBarHeight 默认可能在 25~30 像素。

## 推荐修复方向

### 1. 分离 Custom metrics 和 SARibbon metrics

不要让 `RibbonMetrics::ribbonMaxHeight` 继续控制 SARibbon。

新增：

```cpp
int effectiveRibbonHostHeight(QWidget* backendWidget, const RibbonMetrics& metrics)
{
    if (backendWidget && backendWidget->inherits("SARibbonBar")) {
        return backendWidget->sizeHint().height();
    }
    return metrics.ribbonMaxHeight;
}
```

或者更干净：Backend 提供 preferredHeight。

### 2. SARibbon 模式改成产品可配置

当前可以先改为：

```cpp
RibbonStyleCompactThreeRow
```

而不是 LooseThreeRow。

更长期：

```text
RibbonDensity = Comfortable | Compact
```

默认建议：

```text
CompactThreeRow
```

因为 FreeCAD 主窗口顶部空间宝贵。

### 3. 嵌入式 SARibbon 应关闭 title bar 高度

作为 TopDockHost 内部控件，SARibbon 不需要自己的 title bar。

建议在 `applyMcStudioSARibbonTheme()` 中加入：

```cpp
ribbon->setTitleBarHeight(0);
ribbon->setTabBarHeight(28);       // 或 26
ribbon->setPanelTitleHeight(18);   // 或 19
```

### 4. DockHost 高度以 SARibbon 实际 fixedHeight 为准

不要用：

```cpp
max(metrics.ribbonMaxHeight, contentHeight)
```

SARibbon backend 下应该使用：

```cpp
dockHeight = contentHeight;
```

并留少量边界：

```cpp
dockHeight = contentHeight + 0 或 2
```

### 5. 删除旧 Custom QSS 对 SARibbonHost 的高度影响

当前 Host 仍然：

```cpp
host->setStyleSheet(ribbonStyleSheet());
dock->setStyleSheet(ribbonStyleSheet());
```

`McStudioRibbon.qss` 是旧手写 Ribbon 的样式。SARibbon 进入后，旧 QSS 不应再控制 SARibbon 外层高度/边距。应拆分：

```text
McStudioRibbon.qss       -> only custom backend
McStudioSARibbon.qss     -> SARibbon overlay
McStudioRibbonHost.qss   -> host minimal style
```

## 推荐实施顺序

### Phase H1：先修高度托底

- SARibbon backend 下，Host/DockHost 高度使用 SARibbon `sizeHint().height()`。
- 不再 `max(metrics.ribbonMaxHeight, backendHeight)`。

### Phase H2：压缩 SARibbon 嵌入模式

- `setTitleBarHeight(0)`。
- `RibbonStyleCompactThreeRow`。
- `panelTitleHeight = 18/19`。
- `panelSpacing = 5/6`。

### Phase H3：拆旧 QSS

- 旧 `McStudioRibbon.qss` 只给 custom backend。
- SARibbon 只吃官方主题 + `McStudioSARibbon.qss` overlay。

## 预期效果

修复后：

```text
DockHost 高度 ≈ SARibbon 自己计算的 fixedHeight
```

而不是：

```text
DockHost 高度 = max(旧 Custom 高度, SARibbon 高度)
```

Ribbon 总高度应明显缩短，同时保留 SARibbon 的 panel title、tab、category 布局。
