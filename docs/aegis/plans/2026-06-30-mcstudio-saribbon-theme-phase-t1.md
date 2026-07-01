# McStudio SARibbon Theme Phase T1

日期：2026-06-30
状态：Implementation

## 目标

根据 `2026-06-30-mcstudio-global-ui-theme-architecture.md`，先实现局部 SARibbon 主题接入：

- 只作用于 McStudio Ribbon / SARibbonBar 子树。
- 不修改 FreeCAD 全局 stylesheet。
- 不调用 `qApp->setStyleSheet()`。
- 不修改 SARibbon 源码主题文件。
- 为后续 McStudio Design Token / overlay 预留扩展点。

## 实施内容

1. 在 `McStudioSARibbonBackend.cpp` 引入 SARibbon 主题 API。
2. 增加轻量主题选择函数。
3. 默认使用 `RibbonThemeOffice2021Blue`。
4. 根据 FreeCAD 当前 palette 简单判断 dark/light，dark 时选择 `RibbonThemeOffice2021Dark`。
5. 调用 `SA::applyRibbonTheme(_ribbon, _ribbon, theme)`。
6. 调整第一版视觉 metrics：
   - `RibbonStyleLooseThreeRow`
   - `panelSpacing = 8`
   - `panelTitleHeight = 21`
   - `buttonMaximumAspectRatio = 1.3`
   - 图标尺寸采用 Ribbon 专用 clamp 策略。

## 不做内容

- 不做全局 UI 接管。
- 不改 FreeCAD MainWindow stylesheet。
- 不新增 McStudio 全局主题包。
- 不改 SARibbon 内置 qss/json。
- 不重排 Home tab。

## 2026-06-30 执行记录

已完成 T1 第一版实现。

### 修改文件

```text
src/Mod/McStudio/Gui/RibbonBackends/McStudioSARibbonBackend.cpp
```

### 已实现

新增 SARibbon 主题 API：

```cpp
#include <SARibbonThemeManager.h>
```

新增局部主题选择函数：

```cpp
SARibbonTheme chooseRibbonTheme()
```

当前策略：

```text
FreeCAD/Qt palette dark -> RibbonThemeOffice2021Dark
否则                    -> RibbonThemeOffice2021Blue
```

新增局部视觉系统函数：

```cpp
void applyRibbonVisualSystem(SARibbonBar* ribbon)
```

应用内容：

```cpp
SA::applyRibbonTheme(ribbon, ribbon, chooseRibbonTheme());
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
ribbon->setEnableWordWrap(true);
ribbon->setEnableShowPanelTitle(true);
ribbon->setRibbonAlignment(SARibbonAlignment::AlignLeft);
ribbon->setPanelSpacing(8);
ribbon->setPanelTitleHeight(21);
ribbon->setButtonMaximumAspectRatio(1.3);
```

图标尺寸从旧逻辑：

```text
small = FreeCAD ToolbarIconSize
large = max(base + 8, 32)
```

改为 Ribbon 专用 clamp：

```cpp
smallIcon = clamp(freeCADIconSize, 18, 24)
largeIcon = clamp(freeCADIconSize * 1.5, 32, 40)
```

### 作用范围

本轮只调用：

```cpp
SA::applyRibbonTheme(ribbon, ribbon, theme)
```

因此主题只作用于 McStudio SARibbon 子树。

没有调用：

```cpp
qApp->setStyleSheet(...)
Gui::Application::setStyleSheet(...)
```

不会修改 FreeCAD 全局 UI。

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

与本轮主题接入无关。

## GUI 验证重点

重启 FreeCAD 并切换 McStudio 后检查：

1. Tab 是否不再像裸 QPushButton。
2. Office2021 light/dark 主题是否生效。
3. Ribbon 高度是否比之前更舒展。
4. Panel title 是否更清晰。
5. 其它 FreeCAD 工作台 UI 是否未被 SARibbon 主题污染。
6. 切换其它 Workbench 后样式是否正常。
