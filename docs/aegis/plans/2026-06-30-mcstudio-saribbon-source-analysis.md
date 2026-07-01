# McStudio SARibbon Source Analysis

日期：2026-06-30
状态：Aegis Research / Design Direction
源码位置：`/home/aa/Desktop/McStduio/SARibbon/`

## 目标

分析 SARibbon 是否可以解决 McStudio 当前 Ribbon 的框架性问题：

- `File` 等下拉大按钮视觉偏移。
- 大按钮文字和 group caption 互相挤压。
- group caption 没有固定在底部。
- 后续 1000+ 命令无法靠局部 QSS 调整维护。

## SARibbon 目录结论

SARibbon 是完整 Ribbon 框架，不是 demo。核心文件集中在：

```text
SARibbon/src/SARibbonBar/
  SARibbonBar.h/.cpp
  SARibbonCategory.h/.cpp
  SARibbonCategoryLayout.h/.cpp
  SARibbonPanel.h/.cpp
  SARibbonPanelLayout.h/.cpp
  SARibbonPanelItem.h/.cpp
  SARibbonToolButton.h/.cpp
  SARibbonButtonLayoutStrategy.h/.cpp
  SARibbonElementFactory.h/.cpp
  SARibbonQuickAccessBar.h/.cpp
  SARibbonApplicationButton.h/.cpp
  SARibbonMainWindow.h/.cpp
```

许可证是 MIT，可以作为 McStudio/FreeCAD 集成或参考实现的候选。

## SARibbon 的核心架构

SARibbon 的层级是：

```text
SARibbonBar
  SARibbonCategory      // Tab page
    SARibbonPanel       // Group / Panel
      SARibbonPanelLayout
        SARibbonPanelItem
          SARibbonToolButton
```

这和我们之前自己分析出的正确模型一致：

```text
RibbonBar -> Category -> Panel -> Button
```

而当前 McStudio 是：

```text
QDockWidget
  QTabWidget
    QScrollArea
      QFrame group
        QWidget command strip
          QToolButton
```

McStudio 当前只是 widget 堆叠，还没有真正 Ribbon layout engine。

## SARibbon 如何解决当前问题

### 1. Panel 标题固定底部

SARibbonPanelLayout 中有专门的：

```cpp
int mTitleHeight { 15 };
int mTitleSpace { 2 };
SARibbonPanelLabel* mTitleLabel { nullptr };
QRect mTitleLabelGeometry;
```

并在 `doLayout()` 中显式设置标题位置：

```cpp
mTitleLabel->setGeometry(mTitleLabelGeometry);
```

这说明 panel caption 不是普通 VBox 顺序堆叠，而是由 layout engine 统一计算 geometry。

对 McStudio 的启示：

```text
Group caption 必须是 fixed bottom slot，不能继续 layout->addWidget(commandRow); layout->addWidget(label) 自然堆叠。
```

### 2. PanelLayout 统一处理大/中/小按钮

SARibbonPanel 支持：

```cpp
addLargeAction(QAction*)
addMediumAction(QAction*)
addSmallAction(QAction*)
addLargeAction(QAction*, QToolButton::ToolButtonPopupMode)
addSmallAction(QAction*, QToolButton::ToolButtonPopupMode)
```

底层通过 `SARibbonPanelItem::RowProportion` 管理：

```text
Large  -> 大按钮占满命令区高度
Medium -> 中按钮占两行或一行
Small  -> 小按钮占一行
```

对 McStudio 的启示：

```text
当前 Large/Small/IconOnly 还不够，应该升级成 RowProportion / Archetype / Placement。
```

### 3. ToolButton 自己绘制 icon/text/indicator

SARibbon 不直接依赖 QToolButton 默认布局，而是：

```cpp
SARibbonToolButton::paintEvent()
  paintButton(...)
  paintIcon(...)
  paintText(...)
  paintIndicator(...)
```

按钮内部矩形由：

```cpp
SARibbonButtonLayoutStrategy
  SARibbonLargeButtonLayoutStrategy
  SARibbonSmallButtonLayoutStrategy
```

计算。

核心结构：

```cpp
struct SARibbonButtonLayoutRects {
    QRect iconRect;
    QRect textRect;
    QRect indicatorRect;
};
```

这正是解决 `File` 歪的根本方式：

```text
下拉 indicator 有自己的绘制 rect，不应该让 QToolButton 默认 split 区域挤歪 icon/text 中心。
```

### 4. 支持文字换行和最大宽高比

SARibbonToolButton 中有：

```cpp
bool mEnableWordWrap { true };
qreal mButtonMaximumAspectRatio { 1.4 };
```

`SARibbonButtonLayoutStrategy::estimateTextWidth()` 会根据按钮高度、文本高度、最大宽高比估算文本宽度，并决定是否换行。

对 McStudio 的启示：

```text
按钮宽度不能纯粹由文本无限扩展，也不能靠 QSS 截断；应该有 max aspect ratio + word wrap / elide 策略。
```

### 5. CategoryLayout 处理横向 overflow

SARibbonCategoryLayout 有左右滚动按钮：

```cpp
SARibbonCategoryScrollButton* mLeftScrollBtn;
SARibbonCategoryScrollButton* mRightScrollBtn;
```

并在 `updateGeometryArr()` 中判断：

```cpp
bool needsScrolling = (total > categoryWidth);
```

然后显示左右滚动按钮或扩展 panel。

对 McStudio 的启示：

```text
后续 1000+ 命令不能靠 QScrollArea 隐藏滚动条；Category/Page 层应有明确横向 overflow 策略。
```

## 是否直接集成 SARibbon？

### 路线 A：直接把 FreeCAD MainWindow 替换为 SARibbonMainWindow

不推荐。

原因：

- FreeCAD `Gui::MainWindow` 已经是复杂 QMainWindow，管理 MDI、DockWindowManager、ToolBarManager、Workbench、状态栏、偏好设置。
- 替换基类为 `SARibbonMainWindow` 侵入太大。
- 上游 rebase 风险非常高。

结论：不作为 McStudio 当前路线。

### 路线 B：直接把 SARibbonBar 当作普通 widget 放进 McStudio Top QDockWidget Host

可行但需要验证。

原因：

- `SARibbonBar` 继承自 `QMenuBar`，理论上可以作为普通 QWidget 使用。
- 我们已经有 `Top QDockWidget Host`，可以放置一个 `SARibbonBar`。
- 通过 `addCategoryPage -> addPanel -> addLargeAction/addSmallAction` 构建 Ribbon。

风险：

- SARibbonBar 设计上通常由 `SARibbonMainWindow` 管理，部分 geometry / title / application button / quick access 逻辑可能默认假设 MainWindow 场景。
- 需要处理 FreeCAD 原生 menuBar 共存。
- 需要集成 CMake、资源、QSS、导出宏。
- 需要确认 Qt6 / FreeCAD 编译选项兼容。

结论：适合作为快速 prototype，不宜直接作为最终架构承诺。

### 路线 C：吸收 SARibbon 核心思想，重构 McStudio RibbonFramework v2

推荐。

保留 McStudio 当前宿主结构：

```text
QDockWidget McStudioRibbonDockHost
```

但重写内部 framework：

```text
McStudioRibbonBar
  McStudioRibbonCategory
    McStudioRibbonPanel
      McStudioRibbonPanelLayout
        McStudioRibbonButton
```

参考/移植 SARibbon 的关键点：

- PanelLayout 自定义 QLayout。
- Panel title fixed bottom geometry。
- Button layout strategy。
- Button rects: icon/text/indicator 分离。
- Large/Medium/Small row proportion。
- Category horizontal overflow。

优点：

- 不侵入 FreeCAD MainWindow。
- 不把完整第三方框架硬塞进 FreeCAD。
- 可以保持 McStudio 国际化、FreeCAD QAction、FreeCAD 参数系统、工作台切换逻辑。
- 后续更适合 CAD/CAM 专用改造。

结论：作为长期方案。

## 推荐实施路线

### Phase 0：SARibbon Prototype，不替换现有 UI

目的：验证 SARibbonBar 是否能作为普通 widget 放入 McStudio Top Dock Host。

做法：

```text
新增 McStudioRibbonBackend = Custom / SARibbonPrototype
默认 Custom
通过参数切换 SARibbonPrototype
```

参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Backend = "Custom"
```

Prototype 中：

```cpp
auto* ribbon = new SARibbonBar(parent);
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
auto* home = ribbon->addCategoryPage(ribbonText("Home"));
auto* file = home->addPanel(ribbonText("File"));
file->addLargeAction(stdNewAction);
file->addLargeAction(stdOpenAction);
file->addSmallAction(stdSaveAction);
```

只做首页 2~3 个 panel，先验证：

- 能否编译。
- 能否嵌入 TopDock。
- 高度是否稳定。
- group caption 是否固定底部。
- 下拉按钮是否不歪。
- FreeCAD QAction 状态是否同步。

### Phase 1：建立 McStudio RibbonFramework v2 数据模型

当前：

```cpp
RibbonCommandSize
RibbonCommandStyle
```

升级为：

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
```

### Phase 2：实现 McStudioRibbonButton

参考：

```text
SARibbonToolButton
SARibbonButtonLayoutStrategy
```

重点：

```cpp
struct ButtonRects {
    QRect iconRect;
    QRect textRect;
    QRect indicatorRect;
};
```

自绘 icon/text/indicator，避免 QToolButton 默认 MenuButtonPopup 挤歪内容。

### Phase 3：实现 McStudioRibbonPanelLayout

参考：

```text
SARibbonPanelLayout
```

重点：

- fixed command area
- fixed caption area
- Large/Medium/Small row proportion
- title geometry 显式设置

### Phase 4：实现 Category overflow

参考：

```text
SARibbonCategoryLayout
```

重点：

- total panel width
- left/right scroll buttons
- optional expanding panel
- wheel horizontal scroll

### Phase 5：替换当前 RibbonWidget 内部实现

宿主仍然保留：

```text
Top QDockWidget Host
```

替换内部：

```text
QTabWidget/QScrollArea/QFrame group
```

为：

```text
McStudioRibbonBar/Category/Panel/Button
```

## McStudio 与 SARibbon 的映射表

| McStudio 当前 | SARibbon 对应 | 建议 |
|---|---|---|
| RibbonWidget host | SARibbonBar / custom RibbonBar | 保留 TopDock Host，替换内部 framework |
| RibbonTabDefinition | SARibbonCategory | 改名或映射为 Category |
| RibbonGroupDefinition | SARibbonPanel | 改为 Panel，固定 caption |
| RibbonCommandDefinition | SARibbonPanelItem + QAction | 增加 archetype/placement/overflow |
| QToolButton | SARibbonToolButton | 参考实现 McStudioRibbonButton |
| QHBox/QVBoxLayout | SARibbonPanelLayout | 改自定义 layout |
| QScrollArea 横向滚动 | SARibbonCategoryLayout | 改左右 scroll/overflow |

## 近期建议

不要马上大面积接入 SARibbon 源码到 McStudio 主构建。先做两个动作：

1. **写一个 SARibbon prototype 分支/开关**：验证它能不能作为普通 widget 嵌入当前 TopDockHost。
2. **基于 SARibbon 设计 McStudio RibbonFramework v2**：把我们当前 `RibbonWidget.cpp` 中的临时布局替换为可维护的 framework。

## 关键判断

SARibbon 已经证明：

```text
Ribbon 不能靠普通 QHBoxLayout/QVBoxLayout + QToolButton 默认布局长期维护。
```

它必须有：

```text
PanelLayout + ToolButtonLayoutStrategy + CategoryLayout
```

所以 McStudio 后续正确方向不是继续修局部，而是：

```text
要么嵌入 SARibbonBar prototype，
要么实现 McStudioRibbonFramework v2。
```

推荐最终路线：

```text
先 prototype SARibbonBar，验证视觉和兼容性；
再吸收其核心结构，做 McStudio 自己的轻量 RibbonFramework v2。
```
