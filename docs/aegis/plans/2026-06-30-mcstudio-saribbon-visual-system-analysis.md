# McStudio SARibbon Visual System Analysis

日期：2026-06-30
状态：Aegis Visual Review / Design Plan
输入：用户截图 `SARibbon 已可显示，但布局和审美问题明显`

## 当前状态

SARibbon 已经可以在 McStudio 顶部显示，说明长期结构：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    SARibbonBar
```

已经跑通。当前问题不再是“能否渲染”，而是：

```text
视觉体系没有建立
命令密度没有控制
主题没有接入
McStudio 产品风格没有定义
```

## 截图暴露的问题

### 1. 裸 SARibbon 默认风格过强

当前 tab 像普通 `QPushButton`：

```text
Home / Features / Sketch / Surface...
```

每个 tab 都有明显边框和灰底，看起来像控件按钮，而不是 CAD Ribbon 的 tab。

根因：

- 当前代码只调用了 `setRibbonStyle()`。
- 没有调用 SARibbon 的主题系统 `SA::applyRibbonTheme()`。
- 没有建立 McStudio 自己的主题 overlay。

### 2. Ribbon 高度偏矮，垂直压迫感强

截图里大图标、文字、panel title 都挤在一个较低区域内。

根因：

- 当前使用 `RibbonStyleCompactThreeRow`。
- `panelTitleHeight = 18` 偏紧。
- 大图标尺寸直接由 FreeCAD toolbar icon size 派生，未按 SARibbon 的视觉节奏重新标定。
- DockHost 高度仍混合旧 custom ribbon 的 `metrics.ribbonMaxHeight`。

### 3. 首页命令密度过高

截图中 `Edit`、`Quick View`、`Structure` panel 里禁用命令很多，灰色按钮密集，视觉噪声很重。

根因：

- 当前直接把现有 `RibbonModel` 全量映射进 SARibbon。
- 没有按 `Primary / Secondary / Overflow / Advanced` 做过滤。
- 禁用命令仍然作为常驻按钮占用空间。

### 4. Panel 分隔线太强，整体像表格

当前各 panel 之间竖线较重，视觉被切得很碎。

根因：

- 默认主题/QSS 风格未调整。
- `panelSpacing = 2` 过小，panel 靠得太紧。
- 没有统一 CAD 产品级留白系统。

### 5. 大按钮和小按钮的语义仍不够清晰

例如 Home 中：

```text
File panel: New / Open / Save
Edit panel: Edit Tools + Undo/Redo/Refresh
Quick View: 多个灰色视图按钮
Structure: 多个灰色结构按钮
```

看起来像普通 toolbar 组合，而不是基于工作流组织的 Ribbon。

根因：

- RibbonModel 还停留在临时迁移阶段。
- 未定义：每个 tab 最多几个 primary command。
- 未定义：禁用命令是否进入 overflow。
- 未定义：panel 是否允许只显示灰色命令。

## SARibbon 提供的关键视觉 API

当前代码已使用：

```cpp
_ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
_ribbon->setEnableWordWrap(true);
_ribbon->setEnableShowPanelTitle(true);
_ribbon->setRibbonAlignment(SARibbonAlignment::AlignLeft);
_ribbon->setPanelSpacing(2);
_ribbon->setPanelTitleHeight(18);
_ribbon->setPanelToolButtonIconSize(...);
```

SARibbon 还提供但当前未使用：

```cpp
SA::applyRibbonTheme(widget, ribbonBar, SARibbonTheme::...);
setButtonMaximumAspectRatio(...);
setTabBarHeight(...);
setTitleBarHeight(...);
setCategoryHeight(...);
setPanelSpacing(...);
setPanelTitleHeight(...);
setEnableIconRightText(...);
```

内置主题包括：

```text
RibbonThemeWindows7
RibbonThemeOffice2013
RibbonThemeOffice2016Blue
RibbonThemeOffice2016Green
RibbonThemeOffice2016Dark
RibbonThemeOffice2021Blue
RibbonThemeOffice2021Green
RibbonThemeOffice2021Dark
RibbonThemeDark
RibbonThemeDark2
```

## 设计方向

### 1. 先接入 SARibbon 官方主题

优先试：

```cpp
SA::applyRibbonTheme(_ribbon, _ribbon, SARibbonTheme::RibbonThemeOffice2021Blue);
```

或：

```cpp
SA::applyRibbonTheme(_ribbon, _ribbon, SARibbonTheme::RibbonThemeOffice2013);
```

原因：

- 先让 SARibbon 回到它设计预期的视觉体系。
- 不要马上自己写大量 QSS。
- 先对比 Office2013 / Office2021Blue 哪个更接近 FreeCAD/McStudio。

### 2. McStudio 采用 CAD Ribbon 风格，而不是 Office 原样

最终视觉应接近：

```text
浅灰背景
细分隔线
低饱和蓝色选中 tab
较大命令区留白
panel title 低对比度居中
禁用命令弱化但不喧宾夺主
```

不建议：

```text
高饱和 Office 蓝
强按钮边框
厚重 panel 分隔线
过多灰色禁用按钮常驻
```

### 3. 首页要重新分层

Home 不应该直接展示太多灰色命令。

推荐 Home 最多 5 个 panel：

```text
Application
  File dropdown / New / Open

Project
  Save / Import / Export / Recent

Edit
  Undo / Redo / Refresh / Delete / Edit Tools dropdown

View
  Fit All / Fit Selection / Standard View dropdown / Draw Style dropdown

Help
  Help dropdown
```

Structure 这类不应该放 Home，应该进入 Feature / Advanced / Structure 专用 tab。

### 4. 禁用命令策略

当前灰色按钮很多，非常影响审美。

长期策略：

```text
Primary 命令：即使禁用，也可以常驻
Secondary 命令：禁用时可以进入 overflow 或弱显示
Advanced 命令：不常驻，进入高级 tab 或 command finder
```

换句话说，不是所有目前 disabled 的 QAction 都应该直接出现在 Home。

### 5. SARibbon style 选择

当前：

```cpp
RibbonStyleCompactThreeRow
```

这个能省高度，但容易拥挤。

建议试验顺序：

1. `RibbonStyleLooseThreeRow`：验证标准视觉是否更舒服。
2. `RibbonStyleCompactThreeRow`：如果高度过高，再压缩。
3. `RibbonStyleCompactTwoRow`：只适合极简工具栏，不推荐当前阶段。

对 CAD/CAM 工作台，建议先用：

```cpp
RibbonStyleLooseThreeRow
```

稳定后再微调高度。

### 6. Panel spacing / title height 建议

当前：

```cpp
setPanelSpacing(2);
setPanelTitleHeight(18);
```

建议第一版改为：

```cpp
setPanelSpacing(6 或 8);
setPanelTitleHeight(20 或 22);
setButtonMaximumAspectRatio(1.25 ~ 1.35);
```

目标：

- panel 不再挤成表格。
- title baseline 更稳定。
- 大按钮文字不显得贴底。

### 7. 图标尺寸建议

当前：

```cpp
small = FreeCAD ToolbarIconSize
large = max(base + 8, 32)
```

建议改为产品级策略：

```text
smallIcon = clamp(base, 18, 24)
largeIcon = clamp(base * 1.5, 32, 40)
```

如果 FreeCAD toolbar icon 是 32，Ribbon large icon 不应无限变大；否则 Ribbon 高度会失控。

### 8. 主题 overlay 应该放在 McStudio，不要改 SARibbon 源码

新增：

```text
src/Mod/McStudio/Gui/Resources/styles/McStudioSARibbon.qss
```

用于覆盖少量 McStudio 风格：

- tab selected color
- panel divider opacity
- disabled action opacity
- host background
- panel title color

不要直接修改：

```text
SARibbon/src/SARibbonBar/resource/*.qss
```

## 推荐实施阶段

### Phase V1：接入官方主题 + 基础尺寸

修改 `McStudioSARibbonBackend`：

```cpp
#include <SARibbonThemeManager.h>

SA::applyRibbonTheme(_ribbon, _ribbon, SARibbonTheme::RibbonThemeOffice2021Blue);
_ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
_ribbon->setPanelSpacing(8);
_ribbon->setPanelTitleHeight(21);
_ribbon->setButtonMaximumAspectRatio(1.3);
```

目标：先让 SARibbon 看起来像成熟 Ribbon。

### Phase V2：重排 Home tab

把 `Structure` 从 Home 移出。

Home 只保留：

```text
Application / File / Edit / View / Help
```

灰色禁用命令减少一半以上。

### Phase V3：建立 McStudio SARibbon QSS overlay

新增：

```text
McStudioSARibbon.qss
```

仅覆盖 McStudio 产品风格，不改 SARibbon 原主题。

### Phase V4：命令分层模型升级

在 `RibbonCommandDefinition` 增加：

```cpp
RibbonCommandPriority
RibbonCommandVisibilityPolicy
```

用于控制 disabled 命令是否常驻显示。

## 当前截图的具体修复优先级

1. 先套主题，解决 tab 像普通按钮的问题。
2. 改 LooseThreeRow / panelSpacing / titleHeight，解决拥挤感。
3. 重排 Home，减少灰色 disabled 命令。
4. 加 McStudioSARibbon.qss，统一 CAD 风格。
5. 再做命令分层和 overflow。

## 验收标准

1. Tab 不再像普通 QPushButton。
2. Panel 分隔不再过强。
3. Home 中灰色 disabled 按钮明显减少。
4. Panel title baseline 清晰且统一。
5. 大按钮文字有呼吸感。
6. Ribbon 高度不遮挡模型区，但不显得压缩。
7. 风格更接近 CAD 产品，而不是裸 Qt 控件。

