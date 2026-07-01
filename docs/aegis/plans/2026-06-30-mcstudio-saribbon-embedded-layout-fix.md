# McStudio SARibbon Embedded Layout Fix

日期：2026-06-30
状态：Implementation

## 问题

截图中 FreeCAD 经典菜单栏仍然存在，但 SARibbon category tab 行消失：

```text
Home / Features / Sketch / Surface / Evaluate / View / Setup / Output / Advanced
```

## 根因

上一轮使用：

```cpp
RibbonStyleCompactThreeRow
titleBarHeight = 0
tabOnTitle = false
```

这组配置不适合 SARibbon。

SARibbon 源码中 `setRibbonStyle(CompactThreeRow)` 会默认：

```cpp
setTabOnTitle(true)
```

并且 SARibbon 官方注释说明，在 `RibbonStyleCompact**` 模式下：

```text
tabbar height must be <= titlebar height, otherwise it displays abnormally
```

而 McStudio 嵌入式模式里：

```text
titleBarHeight = 0
```

因此 compact 模式和无 titlebar 嵌入模式冲突，导致 tab 行不可见或布局异常。

## 长期决策

McStudio 嵌入 FreeCAD TopDockHost 时使用：

```text
RibbonStyleLooseThreeRow
TitleBarHeight = 0
TabOnTitle = false
TabBarHeight = 26
CategoryHeight = compact explicit value
```

也就是说：

- 不使用 SARibbon 自己的 titlebar。
- tab 单独占一行。
- category / panel 区高度显式控制，避免 Loose 模式过高。
- 横向宽度仍通过 QSizePolicy::Ignored + minimumWidth=0 控制，不让 SARibbon 拉长主窗口。

## 2026-06-30 执行记录

已完成整体修复：

### 1. 不再使用 CompactThreeRow 作为无 titlebar 嵌入模式

从：

```cpp
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
```

改为：

```cpp
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
```

原因：Compact 模式默认将 tab 放入 title 区，而 McStudio 将 title 区高度设为 0。

### 2. 保留无 titlebar，但强制 tab 独立显示

```cpp
ribbon->setTitleBarHeight(tokens.titleBarHeight); // 0
ribbon->setTabOnTitle(false);
ribbon->setTabBarHeight(tokens.tabBarHeight);     // 26
```

### 3. 显式控制 category/panel 区高度

新增 token：

```cpp
int categoryHeight {88};
```

并应用：

```cpp
ribbon->setCategoryHeight(tokens.categoryHeight);
```

这样既避免 Loose 模式无限偏高，又保留稳定的 category tab 行。

### 4. 强制刷新 SARibbon 几何

```cpp
ribbon->updateRibbonGeometry();
```

确保 style / title / tab / category / panel 高度调整全部生效。

### 5. 横向宽度修复仍保留

SARibbonBar 仍然设置为横向可压缩：

```cpp
_ribbon->setMinimumWidth(0);
_ribbon->setMaximumWidth(QWIDGETSIZE_MAX);
_ribbon->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
```

所以不会重新把 FreeCAD 主窗口拉长。

### 6. 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮修复无关。
