# ADR: McStudio Ribbon 长期方案

日期：2026-06-30
状态：Accepted
决策类型：长期架构决策

## 决策

McStudio Ribbon 长期方案采用：

```text
SARibbon 作为第三方 Ribbon Framework
+ McStudio 自己的 FreeCAD Adapter / Backend 层
+ 继续使用 Top QDockWidget Host 宿主
```

不采用：

```text
1. 继续维护当前手写 QTabWidget/QScrollArea/QToolButton Ribbon
2. 从零仿写完整 Ribbon Framework
3. 把 FreeCAD Gui::MainWindow 改成 SARibbonMainWindow
```

最终目标结构：

```text
Gui::MainWindow
  TopDockWidgetArea
    McStudioRibbonDockHost(QDockWidget)
      McStudioSARibbonHost(QWidget/QFrame)
        SARibbonBar
          SARibbonCategory
            SARibbonPanel
              SARibbonToolButton
```

## 为什么选择 SARibbon 作为长期方案

### 1. Ribbon 是复杂 UI 框架，不是普通控件堆叠

McStudio 当前问题包括：

- File 下拉按钮视觉偏移。
- 大按钮文字遮挡。
- group caption 不固定底部。
- QScrollArea 出现隐藏竖向空间。
- 后续 1000+ 命令无法靠 QSS 局部修补维护。

这些问题本质上需要：

```text
RibbonBar
Category/Page
Panel/Group
PanelLayout
ToolButtonLayoutStrategy
Overflow
Customize/Search
```

SARibbon 已经提供成熟实现，而 McStudio 当前实现只是 widget 堆叠。

### 2. SARibbon 的模型和 McStudio 目标一致

SARibbon 核心模型：

```text
SARibbonBar
  SARibbonCategory
    SARibbonPanel
      SARibbonPanelLayout
        SARibbonToolButton
```

正好对应 McStudio 需要的：

```text
RibbonBar
  Tab
    Group
      Large/Small/Dropdown Button
```

### 3. SARibbon 已解决我们当前最痛的问题

SARibbon 已有：

- panel title fixed bottom geometry
- large / medium / small action row proportion
- custom SARibbonToolButton drawing
- iconRect / textRect / indicatorRect 分离
- word wrap / max aspect ratio
- category horizontal overflow
- quick access bar / app button / customization 基础

这些正是 McStudio 手写框架当前缺失的能力。

### 4. 直接仿写成本高、风险大

仿写意味着 McStudio 要自己实现：

- 自定义 panel layout
- 自绘 tool button
- indicator 布局
- word wrap / elide
- 横向 overflow
- QSS / 主题
- 高 DPI
- RTL / 国际化细节
- 后续自定义系统

这会把大量精力耗在 UI 框架造轮子上，而 McStudio 真正价值应在 CAD/CAM 工作流、命令组织、特征识别、加工流程。

### 5. 许可证兼容

SARibbon 为 MIT License。MIT 许可证适合作为第三方源码依赖进入 McStudio/FreeCAD 模块，前提是保留许可证文件和版权声明。

## 关键约束

### 不替换 FreeCAD MainWindow

不使用：

```cpp
SARibbonMainWindow
```

原因：FreeCAD `Gui::MainWindow` 已管理：

- Workbench
- MDI
- Dock/TaskPanel
- ToolBarManager
- StatusBar
- MenuBar
- Preferences
- Save/restore layout

替换 MainWindow 侵入过大，不可接受。

### 只把 SARibbonBar 当作 McStudio 顶部命令面板

SARibbon 只用于 McStudio 工作台的 Ribbon 内容区域：

```text
McStudioRibbonDockHost
  SARibbonBar
```

FreeCAD 其它 Workbench 不受影响。

## 第三方依赖管理方式

长期采用 **vendored source + CMake target**，不是直接拷贝 amalgamated `SARibbon.h/.cpp` 作为最终形态。

推荐路径：

```text
src/3rdParty/SARibbon/
```

或者项目根目录：

```text
3rdParty/SARibbon/
```

当前已下载在：

```text
/home/aa/Desktop/McStduio/SARibbon/
```

后续应整理为受控第三方目录，并保留：

```text
SARibbon LICENSE
版本号 / commit hash
本地 patch 记录
```

## 构建策略

长期构建方式：

```cmake
set(SARIBBON_BUILD_STATIC_LIBS ON)
set(_SARIBBON_USE_FRAMELESS_LIB OFF)
add_subdirectory(3rdParty/SARibbon/src/SARibbonBar)
target_link_libraries(McStudioGui PRIVATE SARibbonBar::SARibbonBar)
```

优先静态链接进 `McStudioGui`，避免运行期部署额外 `libSARibbonBar.so`。

需要注意：

- 链接 Qt Svg
- 静态资源初始化 `Q_INIT_RESOURCE(SARibbonResource)` 如有需要
- 关闭 frameless / QWindowKit
- 避免启用 SARibbonMainWindow 相关特性

## McStudio Adapter 层

不要在 `RibbonWidget.cpp` 里直接散落 SARibbon 调用。

新增：

```text
src/Mod/McStudio/Gui/RibbonBackends/
  McStudioRibbonBackend.h
  McStudioSARibbonBackend.h/.cpp
  McStudioRibbonActionAdapter.h/.cpp
```

职责：

### McStudioRibbonBackend

抽象 Ribbon 后端：

```cpp
class McStudioRibbonBackend
{
public:
    virtual QWidget* widget() = 0;
    virtual void rebuild(const std::vector<RibbonTabDefinition>& model) = 0;
};
```

### McStudioSARibbonBackend

把 McStudio 的 `RibbonModel` 转成 SARibbon：

```text
RibbonTabDefinition    -> SARibbonCategory
RibbonGroupDefinition  -> SARibbonPanel
RibbonCommandDefinition-> SARibbonToolButton / QAction
```

### McStudioRibbonActionAdapter

负责从 FreeCAD CommandManager 获取 QAction，并应用 McStudio Ribbon 短 label：

```text
FreeCAD QAction 原始文本 -> 菜单/tooltip/状态
McStudio Ribbon label    -> button 显示短名
```

## RibbonModel 长期升级

当前：

```cpp
RibbonCommandDefinition {
    command;
    size;
    style;
    label;
}
```

长期升级为：

```cpp
enum class RibbonCommandArchetype {
    LargeButton,
    MediumButton,
    SmallButton,
    IconButton,
    DropDownLargeButton,
    DropDownSmallButton,
    Gallery,
    CustomWidget
};

enum class RibbonCommandPlacement {
    PrimaryLane,
    SecondaryLane,
    OverflowMenu
};

enum class RibbonCommandPriority {
    Primary,
    Secondary,
    Advanced,
    Hidden
};

struct RibbonCommandDefinition {
    const char* command;
    const char* label;
    RibbonCommandArchetype archetype;
    RibbonCommandPlacement placement;
    RibbonCommandPriority priority;
};
```

这样 1000+ 命令可以按：

```text
常驻大按钮
小按钮
下拉菜单
高级模式
Command Finder
```

分层，而不是全部堆到 Ribbon 上。

## 经典菜单迁移策略

长期界面目标：

```text
McStudio Ribbon 主导
经典菜单栏默认隐藏或极简
高级/开发者模式可恢复
```

但隐藏经典菜单前，必须确保：

- File/Edit/View/Tools/Macro/Window/Help 核心命令已进入 Ribbon / 应用菜单 / 高级 tab / Command Finder
- 有 `ShowClassicMenuBar` 偏好项

参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/UI
  ShowClassicMenuBar = false
```

## 实施路线

### Phase 1：引入 SARibbon 第三方 target

目标：构建通过，不替换 UI。

- 整理第三方目录。
- add_subdirectory 静态链接。
- 保留 LICENSE。
- 禁用 frameless。
- 确认 QtSvg / resource。

### Phase 2：新增 McStudioSARibbonBackend

目标：只做最小 Ribbon：

```text
Home
  File: New, Open, Save
  Edit: Undo, Redo, Refresh
  View: Fit All, Fit Selection, Draw Style

Features
  Main: Pad, Pocket, Hole
  Reference: Reference Plane
```

默认仍可使用旧 backend，或直接切到 SARibbon backend 进行验证。

### Phase 3：替换当前自写 RibbonWidget 内部实现

宿主保留：

```text
Top QDockWidget Host
```

内部改为：

```text
SARibbonBackend
```

移除当前：

```text
QTabWidget + QScrollArea + 手写 QToolButton strip
```

### Phase 4：迁移完整 RibbonModel

把现有 Home/Features/Sketch/Surface/Evaluate/View/Setup/Output/Advanced 转成 SARibbon Category/Panel。

所有显示文本保持：

```text
英文源文本 + QT_TR_NOOP + translate
```

### Phase 5：菜单栏产品化

- 应用菜单替代 File 菜单。
- Advanced tab 收纳 Tools/Macro/Window。
- Help 下拉收纳帮助入口。
- 增加 `ShowClassicMenuBar`。
- 默认隐藏或极简菜单。

### Phase 6：命令搜索 / Command Finder

1000+ 命令不全部进入 Ribbon。低频命令进入搜索。

## 回滚策略

保留旧 backend 一段时间：

```text
Backend = Custom
Backend = SARibbon
```

直到 SARibbon 后端稳定。

参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Backend = "SARibbon"
```

最终稳定后，删除 Custom backend。

## 验收标准

长期方案验收：

1. File / Help / dropdown 不再视觉偏移。
2. group caption 固定底部，所有 caption baseline 一致。
3. 大按钮文字不遮挡。
4. 不出现竖向隐藏滚动。
5. 横向 overflow 有明确处理。
6. FreeCAD QAction 状态同步正常。
7. 国际化走 Qt 翻译系统。
8. 不影响其它 FreeCAD Workbench。
9. 不替换 FreeCAD MainWindow。
10. 第三方 LICENSE / 构建 / 打包策略清晰。

## 最终结论

长期最终方案：

```text
正式引入 SARibbon 作为 McStudio Ribbon Framework，
通过 McStudioSARibbonBackend 适配 FreeCAD Workbench，
继续使用 Top QDockWidget Host，
不替换 FreeCAD MainWindow，
不再维护当前手写 Ribbon layout。
```

