# McStudio Theme Completion

日期：2026-06-30
状态：Implementation

## 目标

完成 McStudio 主题体系第一版：

1. 保留 FreeCAD 全局主题作为根主题。
2. McStudio 新增局部主题 token / policy。
3. SARibbon 使用官方主题作为基础。
4. McStudioSARibbon.qss 作为 overlay，只作用于 McStudio Ribbon 子树。
5. 不调用 `qApp->setStyleSheet()`，不污染其它 Workbench。

## 计划

- 新增 `Theme/McStudioTheme.h/.cpp`。
- 新增 `Resources/styles/McStudioSARibbon.qss`。
- 更新 `Resources/McStudio.qrc` 注册 overlay。
- 更新 `McStudioSARibbonBackend.cpp` 使用 McStudioTheme。
- 构建验证。

## 2026-06-30 执行记录

已完成 McStudio 主题体系第一版。

### 新增 Theme 层

新增：

```text
src/Mod/McStudio/Gui/Theme/McStudioThemeTokens.h
src/Mod/McStudio/Gui/Theme/McStudioTheme.h
src/Mod/McStudio/Gui/Theme/McStudioTheme.cpp
```

职责：

- 读取 McStudio UI 偏好：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/UI
  ThemeMode = FollowFreeCAD | Light | Dark
```

- 根据 Qt / FreeCAD palette 判断 dark/light。
- 生成 McStudio 第一版 design tokens。
- 为 SARibbon 选择内置主题：
  - Light -> `RibbonThemeOffice2021Blue`
  - Dark  -> `RibbonThemeOffice2021Dark`
- 加载 McStudio SARibbon overlay QSS。
- 将 overlay 追加到 SARibbonBar 的局部 stylesheet。

### 新增 SARibbon overlay

新增：

```text
src/Mod/McStudio/Gui/Resources/styles/McStudioSARibbon.qss
```

它只覆盖 SARibbon 子树：

- SARibbonBar
- SARibbonCategory
- SARibbonPanel
- SARibbonPanelLabel
- SARibbonTabBar
- SARibbonStackedWidget
- SARibbonButtonGroupWidget
- SARibbonMenu

使用 token 占位：

```text
@mc-bg-ribbon
@mc-bg-surface
@mc-bg-panel
@mc-bg-hover
@mc-bg-pressed
@mc-border-subtle
@mc-text-primary
@mc-text-secondary
@mc-text-disabled
@mc-accent
@mc-accent-hover
@mc-accent-pressed
@mc-panel-title-height
```

### 资源注册

更新：

```text
src/Mod/McStudio/Gui/Resources/McStudio.qrc
```

加入：

```xml
<file>styles/McStudioSARibbon.qss</file>
```

### CMake 注册

更新：

```text
src/Mod/McStudio/Gui/CMakeLists.txt
```

加入 Theme 源码和 overlay qss。

### SARibbonBackend 接入

更新：

```text
src/Mod/McStudio/Gui/RibbonBackends/McStudioSARibbonBackend.cpp
```

现在不再在 backend 内部直接选择主题，而是调用：

```cpp
applyMcStudioSARibbonTheme(_ribbon);
applyRibbonIconMetrics(_ribbon);
```

主题策略集中到 `McStudioTheme.cpp`。

### 作用范围

本轮仍然没有调用：

```cpp
qApp->setStyleSheet(...)
Gui::Application::setStyleSheet(...)
```

不会覆盖 FreeCAD 全局 UI。

### 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

仍然存在既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮主题体系无关。

## 后续

下一阶段可以做：

1. Home tab 命令瘦身，减少灰色 disabled command。
2. ThemeMode 偏好设置 UI。
3. McStudioTaskPanel / StartPage / CommandFinder 复用 Theme tokens。
4. 更细的 light/dark 识别和 accent color 参数。
