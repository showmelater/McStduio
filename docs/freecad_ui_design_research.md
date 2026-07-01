# FreeCAD UI 设计研究与 McStudio Ribbon 化设计指南

> 目标：系统梳理 FreeCAD 的 UI 组织方式、样式系统、图标系统、工具栏/工作台生命周期、参数配置方式，并给出 McStudio Ribbon 后续 UI 改造的推荐做法。
>
> 本文基于当前项目源码调研，重点服务于 McStudio 工作台后续 Ribbon UI 稳定化和美化工作。

---

## 1. 总体结论

FreeCAD 的 UI 设计不是每个模块独立硬编码一套界面样式，而是遵循以下层次：

```text
Qt 原生控件 / Layout
  ↓
FreeCAD Workbench / Command / ToolBar / Menu 生命周期
  ↓
FreeCAD 全局 QSS / Theme / StyleParameters
  ↓
图标主题 / BitmapFactory / QIcon theme
  ↓
Preference 参数控制尺寸和行为
  ↓
少量复杂控件自绘或特殊 widget
```

换句话说，FreeCAD 的主线设计是：

```text
C++ 负责结构
QSS 负责视觉
ParameterGrp 负责偏好
CommandManager 负责命令
BitmapFactory / QIcon theme 负责图标
Workbench / ToolBarManager / MenuManager 负责生命周期
```

McStudio Ribbon 当前已经完成了功能轮廓：

```text
C++ Workbench
C++ RibbonWidget
RibbonModel
GroupCommand 下拉组
标准工具栏命令迁入 Ribbon
Ribbon-only setupToolBars()
```

但是视觉上还比较粗糙，核心原因是：

```text
1. Ribbon 控件结构虽然存在，但 objectName / dynamic property 不够完整。
2. QFrame / QTabWidget / QToolBar 仍主要依赖 Qt 默认样式。
3. McStudio 没有模块级 QSS，无法精准控制 Ribbon 局部视觉。
4. 布局参数分散在 C++ metrics 中，样式层还没有建立。
5. 当前外层 QToolBar 会受到 FreeCAD 全局 QToolBar 样式影响。
```

后续正确方向不是继续在 C++ 里硬调颜色，而是建立 FreeCAD 风格的 McStudio UI 分层：

```text
RibbonWidget.cpp   只负责 widget 结构、objectName、property、参数读取
RibbonModel.cpp    只负责 Ribbon 数据结构和命令组织
McStudioRibbon.qss 负责 Ribbon 视觉样式
ParameterGrp       控制 Scale / CompactMode / IconSize / ShowGroupFrames
```

---

## 2. FreeCAD UI 启动与样式加载链路

### 2.1 QSS 搜索路径

相关源码：

```text
src/Gui/StartupProcess.cpp
```

关键函数：

```cpp
StartupProcess::setStyleSheetPaths()
```

该函数注册 Qt stylesheet 搜索路径：

```text
qss:
  用户目录 Gui/Stylesheets/
  资源目录 Gui/Stylesheets/
  Qt resource :/stylesheets

overlay:
  用户目录 Gui/Stylesheets/overlay
  资源目录 Gui/Stylesheets/overlay
```

也就是说 FreeCAD 支持从用户目录、安装资源目录和 Qt resource 中加载 QSS。

### 2.2 QSS 应用入口

相关源码：

```text
src/Gui/StartupProcess.cpp
```

关键函数：

```cpp
StartupPostProcess::setStyleSheet()
```

它读取参数：

```text
User parameter:BaseApp/Preferences/MainWindow
  StyleSheet
  TiledBackground
```

然后调用：

```cpp
guiApp.setStyleSheet(...)
```

真正应用样式的位置：

```text
src/Gui/Application.cpp
```

关键函数：

```cpp
Application::setStyleSheet(const QString& qssFile, bool tiledBackground)
```

它会加载：

```text
qss:defaults.qss
用户选择的 qss 文件，例如 FreeCAD.qss
```

然后：

```cpp
qApp->setStyleSheet(defaultStyleSheet + "\n" + styleSheetContent);
```

### 2.3 QSS token 替换

相关源码：

```text
src/Gui/Application.cpp
```

关键函数：

```cpp
Application::replaceVariablesInQss()
```

FreeCAD 的 QSS 中大量使用主题 token：

```text
@PrimaryColor
@AccentColor
@GeneralBorderColor
@TextForegroundColor
@ToolbarButtonsPadding
@TabbarBackgroundColor
@InputFieldBorderRadius
```

这些 token 会通过 StyleParameters 系统替换成具体颜色/尺寸。

相关目录：

```text
src/Gui/StyleParameters/
src/Gui/Stylesheets/parameters/
```

典型参数文件：

```text
src/Gui/Stylesheets/parameters/FreeCAD Dark.yaml
src/Gui/Stylesheets/parameters/FreeCAD Light.yaml
```

---

## 3. FreeCAD QSS 组织方式

### 3.1 样式文件位置

相关目录：

```text
src/Gui/Stylesheets/
```

主要文件：

```text
src/Gui/Stylesheets/defaults.qss
src/Gui/Stylesheets/FreeCAD.qss
src/Gui/Stylesheets/overlay/Freecad Overlay.qss
```

`defaults.qss` 用于 FreeCAD 自有控件的默认样式，例如：

```text
Gui--SplitButton
```

`FreeCAD.qss` 是主题级全局样式，覆盖：

```text
QMainWindow
QDockWidget
QToolBar
QToolButton
QTabWidget
QTabBar
QGroupBox
QFrame
QMenu
QComboBox
QScrollBar
QTreeView
QTableView
```

### 3.2 QToolBar 样式

相关位置：

```text
src/Gui/Stylesheets/FreeCAD.qss
```

典型样式对象：

```css
QToolBar
QToolBar:horizontal
QToolBar:vertical
QToolBar::handle
QToolBar::separator
```

FreeCAD 的 QToolBar 默认有背景、padding、handle、separator 等样式。

这对 McStudio Ribbon 很重要，因为 McStudio Ribbon 的外层也是：

```cpp
QToolBar* ribbon = new QToolBar(mainWindow);
```

因此如果 McStudio 不加局部 objectName/QSS，外层 Ribbon 会直接吃 FreeCAD 全局 QToolBar 样式。

### 3.3 QToolButton 样式

相关选择器：

```css
QToolButton
QToolBar QToolButton
QToolButton:hover
QToolButton:pressed
QToolButton:checked
QToolButton::menu-button
QToolButton::menu-arrow
QToolButton::menu-indicator
```

这说明 FreeCAD 对工具栏按钮和下拉按钮已有完整样式逻辑。McStudio 的下拉组使用 `Gui::GroupCommand` 后，本质还是 `QToolButton::MenuButtonPopup`，所以应该尽量保留这套行为，而不是自己重写按钮。

### 3.4 QTabWidget / QTabBar 样式

相关选择器：

```css
QTabWidget
QTabWidget::pane
QTabWidget::pane:top
QTabWidget::pane:bottom
QTabBar
QTabBar::tab
QTabBar::tab:selected
QTabBar::tab:bottom:selected
```

McStudio Ribbon 当前页签使用：

```cpp
tabs->setTabPosition(QTabWidget::South);
```

也就是 bottom tab。FreeCAD 全局 QSS 对 bottom tab 有样式，但它是为普通 QTabWidget 设计的，不是为 Ribbon 设计的。因此 McStudio 后续需要用：

```css
QTabWidget#McStudioRibbonTabs
QTabWidget#McStudioRibbonTabs::pane
QTabWidget#McStudioRibbonTabs QTabBar::tab
```

进行局部覆盖。

---

## 4. FreeCAD 图标系统

### 4.1 图标主题路径

相关源码：

```text
src/Gui/StartupProcess.cpp
```

关键函数：

```cpp
StartupProcess::setThemePaths()
```

它设置：

```cpp
QIcon::setThemeSearchPaths(...)
QIcon::setThemeName(...)
```

参数路径：

```text
User parameter:BaseApp/Preferences/Bitmaps/Theme
  SearchPath
  Name
```

### 4.2 BitmapFactory

FreeCAD 命令图标通常不是直接加载文件，而是通过：

```cpp
Gui::BitmapFactory().iconFromTheme(...)
Gui::BitmapFactory().pixmap(...)
```

命令里设置：

```cpp
sPixmap = "PartDesign_Fillet";
```

然后 Action 创建时通过 BitmapFactory 找图标。

McStudio 当前做得正确的一点是：Ribbon 按钮仍然通过：

```cpp
Gui::Application::Instance->commandManager().addTo(commandName, toolbar)
```

添加命令，因此图标、文本、tooltip、enabled 状态仍然走 FreeCAD 命令系统。

---

## 5. FreeCAD 工具栏尺寸和偏好参数

相关源码：

```text
src/Gui/ToolBarManager.cpp
```

关键参数：

```text
User parameter:BaseApp/Preferences/General
  ToolbarIconSize
  StatusBarIconSize
  MenuBarIconSize
```

关键函数：

```cpp
ToolBarManager::setupConnection()
ToolBarManager::setupToolBarIconSize()
ToolBarManager::setToolBarIconSize(QToolBar*)
ToolBarManager::toolBarIconSize(QWidget*)
```

FreeCAD 的普通工具栏大小是全局参数控制的。

McStudio Ribbon 当前使用自己的参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Scale = 1.0
```

当前 Scale 影响：

```text
large icon size
small icon size
font scale
group margin
page margin
spacing
```

后续建议扩展为：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Scale
  LargeIconSize
  SmallIconSize
  CompactMode
  ShowGroupFrames
  TabPosition
```

这样和 FreeCAD 的 preference-driven 思路一致。

---

## 6. FreeCAD Workbench / ToolBar / Command 生命周期

### 6.1 Workbench 激活流程

相关源码：

```text
src/Gui/Workbench.cpp
```

核心流程：

```cpp
bool Workbench::activate()
{
    ToolBarItem* tb = setupToolBars();
    setupCustomToolbars(tb, "Toolbar");
    WorkbenchManipulator::changeToolBars(tb);
    ToolBarManager::getInstance()->setup(tb);
    delete tb;

    DockWindowItems* dw = setupDockWindows();
    DockWindowManager::instance()->setup(dw);
    delete dw;

    MenuItem* mb = setupMenuBar();
    addPermanentMenuItems(mb);
    MenuManager::getInstance()->setup(mb);
    delete mb;

    setupCustomShortcuts();
    return true;
}
```

也就是说 Workbench 激活时，FreeCAD 会依次构建：

```text
Toolbars
Dock windows
Menus
Shortcuts
```

### 6.2 标准工具栏来源

相关源码：

```text
src/Gui/Workbench.cpp
```

关键函数：

```cpp
StdWorkbench::setupToolBars()
```

它创建标准工具栏：

```text
File
  Std_New
  Std_Open
  Std_Save

Edit
  Std_Undo
  Std_Redo
  Std_Refresh

Workbench
  Std_Workbench

View
  Std_ViewFitAll
  Std_ViewFitSelection
  Std_ViewGroup
  Std_AlignToSelection
  Std_DrawStyle
  Std_Measure
  Std_MassProperties

Structure
  Std_Part
  Std_Group
  Std_LinkActions
  Std_VarSet

Help
  Std_WhatsThis
```

McStudio 之前并行显示标准工具栏的原因是：

```cpp
return StdWorkbench::setupToolBars();
```

现在已经改为 Ribbon-only：

```cpp
return new Gui::ToolBarItem();
```

因此标准 QToolBar 不再生成，标准命令改由 RibbonModel 暴露。

### 6.3 ToolBarManager 的作用

相关源码：

```text
src/Gui/ToolBarManager.cpp
```

核心职责：

```text
1. 将 ToolBarItem 声明树变成真实 QToolBar。
2. 通过 CommandManager::addTo() 加入命令。
3. 管理工具栏显示/隐藏状态。
4. 处理右键工具栏菜单。
5. 保存和恢复用户工具栏布局。
6. 根据 ToolbarIconSize 设置图标大小。
```

McStudio Ribbon 当前没有直接走 ToolBarManager 创建内部 Ribbon 结构，而是在：

```cpp
Workbench::activated()
  -> RibbonWidget::show()
```

里手动创建一个外层 QToolBar，再把 QTabWidget 放进去。

这是合理的，因为 Ribbon 不是普通 FreeCAD ToolBarItem 树能表达的结构。

---

## 7. FreeCAD 资源组织方式

FreeCAD 模块资源通常放在：

```text
src/Mod/<Module>/Gui/Resources/<Module>.qrc
src/Mod/<Module>/Gui/Resources/icons/*.svg
src/Mod/<Module>/Gui/Resources/translations/*.ts
```

例如：

```text
src/Mod/PartDesign/Gui/Resources/PartDesign.qrc
src/Mod/Sketcher/Gui/Resources/Sketcher.qrc
src/Mod/Surface/Gui/Resources/Surface.qrc
src/Mod/Measure/Gui/Resources/Measure.qrc
```

CMake 中通常使用：

```cmake
qt_add_resources(... Resources/<Module>.qrc ...)
```

McStudio 后续如果增加样式文件，建议采用类似结构：

```text
src/Mod/McStudio/Gui/Resources/McStudio.qrc
src/Mod/McStudio/Gui/Resources/styles/McStudioRibbon.qss
```

然后在：

```text
src/Mod/McStudio/Gui/CMakeLists.txt
```

加入：

```cmake
qt_add_resources(McStudioGui_QRC_SRCS Resources/McStudio.qrc)
```

并把资源加入 `McStudioGui_SRCS`。

---

## 8. McStudio 当前 UI 结构分析

当前 McStudio Ribbon 结构大致为：

```text
QToolBar McStudioRibbonToolBar
  QTabWidget McStudioRibbonTabs
    QWidget page
      QHBoxLayout
        QFrame group
          QVBoxLayout
            QWidget commandRow
              QHBoxLayout
                QToolBar large command bar
                QWidget secondaryColumn
                  QVBoxLayout
                    QToolBar small command bar
                    QToolBar icon-only command bar
            QLabel group title
```

当前优点：

```text
1. 完全 C++ 实现。
2. 命令仍走 FreeCAD CommandManager。
3. 下拉组走 FreeCAD Gui::GroupCommand。
4. 支持 Large / Small / IconOnly command metadata。
5. 支持 Scale 参数。
6. 已经支持 Ribbon-only 工具栏模式。
```

当前问题：

```text
1. QFrame 使用 StyledPanel，视觉较重且默认 Qt 感明显。
2. 页签 QTabWidget 是普通样式，没有 Ribbon 专属视觉。
3. group / label / command row / command toolbar 缺少稳定 objectName。
4. large/small/icon toolbar 缺少 ribbonRole property。
5. QToolBar 全局样式会影响 Ribbon 外层和内部 command bars。
6. 视觉结构没有统一的 QSS 管理。
7. 现在 UI 参数和视觉表达混在 C++ metrics 中。
```

---

## 9. McStudio Ribbon 推荐 UI 架构

### 9.1 设计原则

后续应遵循：

```text
结构在 C++
样式在 QSS
尺寸在 ParameterGrp
命令在 CommandManager
数据在 RibbonModel
生命周期在 Workbench
```

避免：

```text
1. qApp->setStyleSheet() 全局覆盖。
2. C++ 里硬编码大量颜色。
3. 手动创建自定义按钮替代 CommandManager。
4. 修改 FreeCAD 核心 ToolBarManager。
5. 把样式逻辑写进 RibbonModel。
```

### 9.2 推荐文件结构

```text
src/Mod/McStudio/Gui/
  RibbonModel.h
  RibbonModel.cpp
  RibbonWidget.h
  RibbonWidget.cpp
  Workbench.h
  Workbench.cpp
  Command.cpp
  Resources/
    McStudio.qrc
    styles/
      McStudioRibbon.qss
```

### 9.3 C++ 控件命名建议

应增加稳定 objectName：

```cpp
ribbon->setObjectName("McStudioRibbonToolBar");
tabs->setObjectName("McStudioRibbonTabs");
page->setObjectName("McStudioRibbonPage");
frame->setObjectName("McStudioRibbonGroup");
label->setObjectName("McStudioRibbonGroupLabel");
commandRow->setObjectName("McStudioRibbonCommandRow");
```

对不同 command bar 增加 property：

```cpp
largeBar->setProperty("ribbonRole", "large");
smallBar->setProperty("ribbonRole", "small");
iconBar->setProperty("ribbonRole", "icon");
```

对 group frame 可增加：

```cpp
frame->setProperty("class", "ribbonGroup");
```

### 9.4 推荐 QSS 作用范围

不要写全局选择器：

```css
QToolButton { ... }
QFrame { ... }
QTabWidget { ... }
```

应写 McStudio 限定选择器：

```css
QToolBar#McStudioRibbonToolBar { ... }
QTabWidget#McStudioRibbonTabs { ... }
QWidget#McStudioRibbonPage { ... }
QFrame#McStudioRibbonGroup { ... }
QLabel#McStudioRibbonGroupLabel { ... }
QToolBar[ribbonRole="large"] QToolButton { ... }
QToolBar[ribbonRole="small"] QToolButton { ... }
QToolBar[ribbonRole="icon"] QToolButton { ... }
```

---

## 10. 推荐视觉设计方向

### 10.1 外层 Ribbon

目标：比普通 QToolBar 更像一个功能区容器。

建议：

```text
1. 去掉重渐变。
2. 去掉多余边框。
3. 保留底部分隔线。
4. 与 FreeCAD 主窗口颜色保持一致。
5. 不抢 3D 视图区视觉焦点。
```

QSS 方向：

```css
QToolBar#McStudioRibbonToolBar {
    border: none;
    background: palette(window);
    padding: 0px;
    spacing: 0px;
}
```

### 10.2 Ribbon 页签

目标：底部页签清晰但不笨重。

建议：

```text
1. 选中页签轻微高亮。
2. 非选中页签弱化。
3. 页签和 pane 视觉连接。
4. 底部页签高度不要太高。
```

QSS 方向：

```css
QTabWidget#McStudioRibbonTabs::pane {
    border: 1px solid palette(mid);
    border-radius: 3px;
    background: palette(window);
}

QTabWidget#McStudioRibbonTabs QTabBar::tab {
    padding: 3px 12px;
}
```

### 10.3 Ribbon 分组

目标：分组明显，但边框不要太重。

建议：

```text
1. group 用浅边框。
2. 标题居中，颜色弱化。
3. 组间距一致。
4. 不使用 QFrame::StyledPanel 的默认厚重外观。
```

C++ 方向：

```cpp
frame->setFrameShape(QFrame::NoFrame);
```

QSS 方向：

```css
QFrame#McStudioRibbonGroup {
    border: 1px solid palette(midlight);
    border-radius: 3px;
    background: palette(base);
}

QLabel#McStudioRibbonGroupLabel {
    color: palette(mid);
    padding-top: 2px;
}
```

### 10.4 大按钮

目标：主要操作明确，像 SolidWorks CommandManager 的主按钮。

建议：

```text
1. 图标 32px 保持。
2. 文字在图标下。
3. 按钮最小宽度略固定。
4. hover/pressed 交给 QSS。
```

QSS 方向：

```css
QToolBar[ribbonRole="large"] QToolButton {
    padding: 3px 6px;
    min-width: 48px;
}
```

### 10.5 小按钮

目标：小按钮可读，布局整齐。

建议：

```text
1. 图标 24px。
2. 文字在右。
3. 行高一致。
4. 用下拉合并低频同族命令。
```

QSS 方向：

```css
QToolBar[ribbonRole="small"] QToolButton {
    padding: 1px 4px;
    min-height: 26px;
}
```

### 10.6 下拉按钮

目标：保留 FreeCAD 原生行为。

建议：

```text
1. 不自己实现 QComboBox。
2. 不手写菜单逻辑。
3. 继续使用 Gui::GroupCommand。
4. QSS 只微调 padding/menu arrow。
```

---

## 11. 实施路线建议

### Phase 1：样式钩子

修改：

```text
src/Mod/McStudio/Gui/RibbonWidget.cpp
```

内容：

```text
1. 给 page / group / label / commandRow 加 objectName。
2. 给 large/small/icon command toolbar 加 ribbonRole property。
3. 把 group frame 从 StyledPanel 改为 NoFrame。
4. 保持当前布局不变。
```

风险低，优先做。

### Phase 2：模块 QSS

新增：

```text
src/Mod/McStudio/Gui/Resources/McStudio.qrc
src/Mod/McStudio/Gui/Resources/styles/McStudioRibbon.qss
```

修改：

```text
src/Mod/McStudio/Gui/CMakeLists.txt
src/Mod/McStudio/Gui/RibbonWidget.cpp
```

内容：

```text
1. 将 QSS 编进 McStudioGui 资源。
2. RibbonWidget 加载 qrc 中的 QSS。
3. 只对 McStudioRibbonToolBar 子树应用样式。
4. 选择器全部使用 objectName/property 限定。
```

### Phase 3：偏好参数扩展

新增参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Scale
  CompactMode
  LargeIconSize
  SmallIconSize
  ShowGroupFrames
```

目标：

```text
1. 大小可调。
2. 紧凑模式可调。
3. 分组框是否显示可调。
```

### Phase 4：布局策略

如果分组合并和样式优化后仍然横向过长，再考虑：

```text
1. QScrollArea 横向滚动。
2. CompactMode 隐藏部分文字。
3. 更多 GroupCommand 下拉合并。
4. 自定义 FlowLayout。
```

不建议现在立刻做 FlowLayout，因为复杂度较高，且会增加 Ribbon 高度。

---

## 12. McStudio 近期推荐任务

优先级：

```text
1. RibbonWidget 加 objectName/property 样式钩子。
2. 新增 McStudioRibbon.qss。
3. 改 group frame 视觉，从默认 StyledPanel 变成轻量 panel。
4. 调整首页页签，使标准命令区更像 Ribbon 首页。
5. 继续通过 GroupCommand 减少横向宽度。
```

最小可实施切片：

```text
Task 1: RibbonWidget 添加 objectName/property。
Task 2: 添加 Resources/McStudio.qrc 和 styles/McStudioRibbon.qss。
Task 3: RibbonWidget 加载局部 QSS。
Task 4: 编译验证。
Task 5: GUI 手动检查。
```

验证命令：

```bash
cd /home/aa/Desktop/McStduio
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py
cd build/debug && ninja McStudioGui McStudio
cd ../.. && python aegis-method-pack/scripts/aegis-workspace.py check --root .
```

GUI 验证：

```bash
cd /home/aa/Desktop/McStduio
rm -f /tmp/mcstudio-user.cfg /tmp/mcstudio.log
./build/debug/bin/FreeCAD --user-cfg /tmp/mcstudio-user.cfg --log-file /tmp/mcstudio.log
```

检查：

```text
1. 标准工具栏不再并行显示。
2. McStudio Ribbon 仍显示。
3. 首页 / 特征 / 草图 / 曲面 / 评估 / 装夹 / 输出 页签正常。
4. 下拉组正常。
5. 分组边框更轻。
6. 页签视觉更统一。
7. 日志无 skipping unknown command。
```

---

## 13. 对 McStudio 的最终建议

McStudio 不要走“快速堆样式”的路线，而应该走 FreeCAD 原生风格路线：

```text
Command 继续用 FreeCAD CommandManager
Dropdown 继续用 Gui::GroupCommand
Ribbon 数据继续用 RibbonModel
Ribbon 结构继续用 RibbonWidget
样式单独进入 McStudioRibbon.qss
尺寸进入 ParameterGrp
资源进入 Resources/McStudio.qrc
```

这样既能保持 FreeCAD 可维护性，又能逐步做出 SolidWorks 风格的 Ribbon UI。
