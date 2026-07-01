# McStudio Global UI Theme Architecture

日期：2026-06-30
状态：Aegis Architecture Analysis
主题：SARibbon 主题系统与 McStudio / FreeCAD 全局 UI 主题的关系

## 问题

现在 SARibbon 已经接入并可以显示。下一步如果调用：

```cpp
SA::applyRibbonTheme(_ribbon, _ribbon, SARibbonTheme::RibbonThemeOffice2021Blue);
```

可以快速改善 Ribbon 外观。但必须先明确：

```text
SARibbon 主题系统是否应该接管 McStudio 全局 UI？
McStudio 是否应该有自己的全局主题？
FreeCAD 现有 stylesheet / icon theme / preference pack 如何共存？
```

## 调研事实

### FreeCAD 已有全局主题机制

FreeCAD 启动流程中会设置 stylesheet 和 icon theme：

```text
src/Gui/StartupProcess.cpp
  setStyleSheetPaths()
  setThemePaths()
  setStyleSheet()
```

相关偏好路径包括：

```text
User parameter:BaseApp/Preferences/MainWindow
  StyleSheet
  TiledBackground

User parameter:BaseApp/Preferences/Bitmaps/Theme
  SearchPath
  Name
```

FreeCAD 主程序通过：

```cpp
Gui::Application::setStyleSheet(...)
Gui::Application::reloadStyleSheet()
```

设置全局 stylesheet。

因此，McStudio 不应该绕过 FreeCAD 直接设置 `qApp->setStyleSheet()` 或覆盖全局 style。

### FreeCAD 有全局 QSS 资源

核心 QSS：

```text
src/Gui/Stylesheets/defaults.qss
src/Gui/Stylesheets/FreeCAD.qss
src/Gui/Stylesheets/overlay/Freecad Overlay.qss
```

说明 FreeCAD 已有根级别 UI 样式系统。

### McStudio 当前只有 Ribbon 局部 QSS

当前 McStudio 资源：

```text
src/Mod/McStudio/Gui/Resources/styles/McStudioRibbon.qss
```

这个原先是手写 Ribbon 的局部样式。接入 SARibbon 后，它不应继续控制所有 Ribbon 细节，而应该演变为：

```text
McStudioSARibbon.qss       // SARibbon overlay
McStudioPanel.qss          // Task / model side panel overlay
McStudioGlobal.qss         // McStudio local tokens / global overrides, not qApp global
```

### SARibbon 自带主题系统

SARibbon 提供：

```cpp
#include <SARibbonThemeManager.h>

SA::applyRibbonTheme(QWidget* w, SARibbonBar* bar, SARibbonTheme theme);
```

内置主题：

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

SARibbon 的主题资源包含：

```text
resource/theme-base.qss
resource/templates/office2013.qss
resource/templates/office2016.qss
resource/templates/office2021.qss
resource/templates/dark.qss
resource/templates/dark2.qss
resource/templates/win7.qss
resource/palettes/*.json
```

## 核心决策

### 1. FreeCAD 全局主题仍然是根主题

McStudio 不接管整个 FreeCAD 的全局 UI。

不做：

```cpp
qApp->setStyleSheet(mcstudioGlobalQss);
```

不做：

```cpp
Gui::Application::setStyleSheet("McStudio.qss", ...);
```

除非用户明确选择了 McStudio 全局主题包。

原因：

- McStudio 是 FreeCAD 工作台/模块，不是独立应用。
- 其它 Workbench 仍应保持 FreeCAD 用户选择的主题。
- 全局改 qApp stylesheet 会影响 Tree、TaskPanel、PropertyView、Dialog、AddonManager 等所有 UI。

### 2. SARibbon 主题只应用在 McStudio Ribbon 子树

SARibbon 主题应用范围应限制在：

```text
McStudioRibbonHost / SARibbonBar subtree
```

推荐：

```cpp
SA::applyRibbonTheme(_ribbon, _ribbon, selectedTheme);
```

或：

```cpp
SA::applyRibbonTheme(hostWidget, _ribbon, selectedTheme);
```

前提：hostWidget 只包含 McStudio Ribbon，不包含 FreeCAD 全局 UI。

不能把 `Gui::MainWindow` 或 `qApp` 作为 `applyRibbonTheme` 的目标。

### 3. McStudio 应该建立自己的 Design Token 层

不能让 SARibbon theme 直接成为 McStudio 全局规范。

McStudio 应定义一套跨组件 token：

```text
mc-bg-window
mc-bg-surface
mc-bg-ribbon
mc-bg-panel
mc-border-subtle
mc-border-strong
mc-text-primary
mc-text-secondary
mc-text-disabled
mc-accent
mc-accent-hover
mc-accent-pressed
mc-warning
mc-success
mc-radius-small
mc-radius-medium
mc-spacing-xs/s/m/l
mc-icon-small
mc-icon-large
```

这些 token 应用于：

```text
Ribbon
Task panel
Model tree adjacent UI
Command palette
Property shortcuts
Dialogs
Start / Home page
```

SARibbon 主题只是 Ribbon 组件对这些 token 的一个实现。

### 4. 主题层级采用四层架构

长期主题层级：

```text
Layer 0: Qt / OS native palette
Layer 1: FreeCAD global stylesheet + icon theme
Layer 2: McStudio design tokens / local stylesheet
Layer 3: Component overlays
        - McStudioSARibbon.qss
        - McStudioTaskPanel.qss
        - McStudioStartPage.qss
```

优先级：

```text
OS/Qt < FreeCAD < McStudio token < Component overlay
```

### 5. 不修改 SARibbon 源码主题文件

不改：

```text
SARibbon/src/SARibbonBar/resource/templates/*.qss
SARibbon/src/SARibbonBar/resource/palettes/*.json
```

McStudio 只新增 overlay：

```text
src/Mod/McStudio/Gui/Resources/styles/McStudioSARibbon.qss
```

原因：

- 保持 SARibbon 可升级。
- 减少第三方 patch。
- McStudio 风格与第三方库解耦。

## 推荐主题映射策略

### McStudio 主题模式

新增偏好：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/UI
  ThemeMode = "FollowFreeCAD" | "Light" | "Dark"
  AccentColor = "Blue" | "Green" | "Gray" | ...
```

默认：

```text
ThemeMode = "FollowFreeCAD"
```

### SARibbon theme 映射

根据 McStudio ThemeMode 和 FreeCAD 当前 stylesheet 推断 SARibbon 主题：

```text
FollowFreeCAD + FreeCAD light  -> RibbonThemeOffice2021Blue 或 Office2013
FollowFreeCAD + FreeCAD dark   -> RibbonThemeOffice2021Dark 或 Dark2
Light                          -> RibbonThemeOffice2021Blue
Dark                           -> RibbonThemeOffice2021Dark
```

初始建议：

```text
Light: RibbonThemeOffice2021Blue
Dark:  RibbonThemeOffice2021Dark
```

如果 Office2021 视觉太强，再改：

```text
Light: RibbonThemeOffice2013
Dark:  RibbonThemeDark2
```

### FreeCAD 主题变化响应

FreeCAD 可 reload stylesheet。McStudio 后续应监听/刷新：

```text
BaseApp/Preferences/MainWindow/StyleSheet
```

或在 McStudio 激活时重新计算：

```cpp
applyMcStudioTheme();
```

第一阶段可以不实时监听，只在：

```text
Workbench activated
Ribbon rebuilt
```

时应用主题。

## 全局 UI 组件范围

McStudio 最终不只有 Ribbon。应该统一这些区域：

### Ribbon

使用 SARibbon + McStudioSARibbon overlay。

### 左侧 Model / Task 区域

当前仍是 FreeCAD 原生 panel。长期可增加 McStudio overlay：

```text
McStudioTaskPanel.qss
```

但只应作用于 McStudio 自己创建的 Task widgets，不污染 FreeCAD 所有 task panel。

### Start / Home 页面

当前中间区域有“新建文件”等页面。长期需要 McStudioStartPage 主题：

```text
McStudioStartPage.qss
```

### 命令搜索 / Command Finder

应使用同一 token 系统。

### Dialog / Preferences

McStudio 自己的设置页应使用同一 token，但不要覆盖 FreeCAD 全部 preferences dialog。

## 不推荐的方案

### 不推荐：SARibbon 主题作为全局 UI 主题

不应该：

```text
SARibbonThemeOffice2021Blue -> qApp stylesheet
```

原因：SARibbon QSS 是为 RibbonBar / Category / Panel / ToolButton 写的，不适合作为 Tree/Property/Task/Dialog 的全局样式。

### 不推荐：每个组件单独写 QSS

比如：

```text
Ribbon 一套蓝
TaskPanel 一套灰
StartPage 一套白
Dialog 一套暗
```

会导致 McStudio 产品碎片化。

必须先有 token，再有组件 overlay。

### 不推荐：直接改 FreeCAD 全局主题

McStudio 是工作台，不能默认改用户的 FreeCAD 全局偏好。

## 实施路线

### Phase T1：SARibbon 局部主题接入

目标：只改善 Ribbon。

- 在 `McStudioSARibbonBackend` 中引入 `SARibbonThemeManager.h`。
- 根据 `ThemeMode` 选择 SARibbonTheme。
- 调用 `SA::applyRibbonTheme(_ribbon, _ribbon, theme)`。
- 设置基础 metrics：RibbonStyle、panel spacing、title height、aspect ratio。

不影响 FreeCAD 其它 UI。

### Phase T2：McStudioSARibbon.qss overlay

新增：

```text
src/Mod/McStudio/Gui/Resources/styles/McStudioSARibbon.qss
```

加载顺序：

```text
SARibbon built-in theme qss
+ McStudioSARibbon overlay qss
```

只覆盖 McStudio Ribbon 视觉。

### Phase T3：McStudio Design Token 文件

新增：

```text
src/Mod/McStudio/Gui/Theme/McStudioTheme.h/.cpp
src/Mod/McStudio/Gui/Theme/McStudioThemeTokens.h
```

职责：

- 读取 FreeCAD 当前 StyleSheet / palette。
- 判断 light/dark。
- 输出 McStudio token。
- 为 SARibbon 选择 theme。
- 为 QSS overlay 替换变量。

### Phase T4：McStudio 子组件统一

对 McStudio 自己的 UI 子树逐步接入：

```text
Ribbon
Start page
Task widgets
Command finder
Preferences page
```

仍不接管 FreeCAD 全局 UI。

### Phase T5：可选 McStudio 全局主题包

如果最终要做独立产品化外观，可以提供用户显式选择：

```text
McStudio Product Theme
```

这时才通过 FreeCAD Preference Pack / stylesheet 机制设置全局主题，而不是工作台激活时偷偷覆盖。

## 近期最小实现建议

下一步先做 T1，不做全局大改：

```cpp
#include <SARibbonThemeManager.h>

SARibbonTheme chooseMcStudioRibbonTheme()
{
    // v1: simple light default
    return SARibbonTheme::RibbonThemeOffice2021Blue;
}

SA::applyRibbonTheme(_ribbon, _ribbon, chooseMcStudioRibbonTheme());
_ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
_ribbon->setPanelSpacing(8);
_ribbon->setPanelTitleHeight(21);
_ribbon->setButtonMaximumAspectRatio(1.3);
```

这一步只影响 Ribbon 子树。

## 验收标准

1. SARibbon 视觉改善，但 FreeCAD 左侧模型/任务面板不被错误套 SARibbon 样式。
2. 切换其它 Workbench 后 FreeCAD UI 不受 McStudio 主题影响。
3. McStudio Ribbon 能根据 light/dark 初步选择合适主题。
4. 后续 McStudio Task/Start/Command Finder 可复用同一 token，不各写各的。
5. 不修改 SARibbon 源码主题文件。
6. 不默认修改 FreeCAD 全局 stylesheet 偏好。

## 最终结论

SARibbon 主题系统应该作为：

```text
McStudio Ribbon component theme engine
```

而不是：

```text
McStudio / FreeCAD global UI theme engine
```

长期全局 UI 应由 McStudio 自己的 Design Token 层统一，SARibbon、TaskPanel、StartPage、CommandFinder 都只是 token 的不同组件实现。

