# McStudio Ribbon Layout Engine Analysis

日期：2026-06-30
状态：Analysis / Architecture Direction

## 背景

截图中出现：

- `File` 下拉大按钮视觉偏斜。
- 部分按钮文字仍被遮挡。
- Group 底部标题跟随上方命令内容漂移，而不是固定在底部基线。

这些不是单个按钮 padding、某个 QSS 数值、某个 label 长度的问题，而是当前 Ribbon 框架缺少统一的布局语义。

## 当前框架问题

当前 McStudio Ribbon 内部结构大致为：

```text
Ribbon Page
  QScrollArea
    content QWidget + QHBoxLayout
      Ribbon Group QFrame
        QVBoxLayout
          commandRow QWidget
            large strip / small strip / icon strip
          group QLabel
```

这个结构的问题是：

1. Group 只是普通 `QVBoxLayout` 顺序堆叠，标题没有底部锚定。
2. 命令区没有固定的 content well，高度由按钮自然 sizeHint 决定。
3. 大按钮、小按钮、图标按钮、下拉按钮没有统一的 role metrics。
4. 下拉按钮使用 Qt 默认 `QToolButton::MenuButtonPopup`，箭头区域参与按钮内部排版，会导致主图标/文字视觉偏心。
5. 页面高度、group 高度、按钮高度分别计算，没有统一的 Ribbon geometry contract。
6. 后续添加更多命令时，同类问题会重复出现。

## 结论

不能继续靠局部修补：

- 不应该只调 `File` 的 padding。
- 不应该单独给某个按钮改宽度。
- 不应该继续通过增加 Ribbon 总高度掩盖文本遮挡。

需要建立一个 Ribbon layout engine：先定义统一几何模型，再让所有按钮按同一套模型布局。

## 目标布局模型

Ribbon Page 高度分为：

```text
TabBar
Page
  Page top/bottom padding
  Group
    Command Well      固定高度 / 弹性填充
    Group Caption     固定高度，始终贴底
```

每个 Group 的内部结构应固定为：

```text
QFrame McStudioRibbonGroup
  QVBoxLayout
    QWidget CommandWell      stretch=1, fixed available height
      QHBoxLayout / role slots
    QLabel GroupCaption      fixed height, bottom aligned
```

Group caption 不再跟随命令内容漂移。

## 命令展示角色

所有 Ribbon 命令必须归入固定角色：

### LargeButton

```text
[large icon]
[short label]
```

- 主命令。
- 图标和文字居中。
- 文本使用 Ribbon short label，不直接使用 QAction 长菜单文本。
- 下拉箭头作为 overlay 或独立 hit region，不挤压主内容。

### SmallTextButton

```text
[small icon] short label
```

- 辅助命令。
- 横向布局。
- 不允许长句子。

### IconButton

```text
[small icon]
```

- 高频或图标明确命令。
- 无文字。
- tooltip 保留完整说明。

### LargeDropDown / SmallDropDown

- 显示短 group label。
- 菜单中显示完整 QAction 文本。
- 箭头不应改变主图标/文本中心线。

## 需要引入的核心数据结构

在 `RibbonMetrics` 中增加语义化尺寸：

```cpp
int groupHeight;
int commandWellHeight;
int groupCaptionHeight;
int largeButtonWidth;
int largeButtonHeight;
int smallButtonHeight;
int iconButtonSide;
int dropDownIndicatorSize;
```

这些值从 FreeCAD `ToolbarIconSize`、字体、scale 推导。

## 需要引入的核心类/函数

### RibbonGroupMetrics

负责计算 group 高度和 caption 区高度。

### RibbonCommandButton

长期建议新增 McStudio 自己的按钮子类，而不是裸用 `QToolButton`：

```cpp
class RibbonCommandButton : public QToolButton
```

职责：

- 按 role 返回统一 `sizeHint()` / `minimumSizeHint()`。
- 控制 icon/text/menu indicator 排版。
- 下拉箭头 overlay，不参与主内容居中计算。
- 统一处理 elide / wrap / tooltip。

短期可先用 `QToolButton` + 统一 helper，但长期应该收敛成子类。

### RibbonCommandWell

Group 内部的命令承载区，负责：

- LargeButton 横向排列。
- SmallTextButton 两行或三行堆叠。
- IconButton 横向 icon strip。
- 多余宽度只在 group 之间或 page 末尾扩展，不挤压按钮本体。

## 修复路线

### Phase 1：冻结几何模型

1. `RibbonMetrics` 增加 group/command/caption/button 语义尺寸。
2. `createRibbonGroup()` 改为固定 group 高度。
3. `group label` 改为 fixed height + bottom anchored。
4. `commandRow` 改名/重构为 `commandWell`，占用剩余高度。

### Phase 2：统一按钮角色

1. 建立 `RibbonCommandRole` 到尺寸/样式的映射。
2. 所有按钮通过 `configureRibbonButton(button, role, metrics)` 配置。
3. 禁止单个按钮自己决定最小高度。
4. 下拉按钮统一使用 overlay-like indicator 或统一 popup policy。

### Phase 3：解决下拉偏心

1. 对 LargeDropDown，箭头不参与主内容居中。
2. 若继续使用 QToolButton，需要统一 QSS：`QToolButton::menu-indicator` 贴右上角。
3. 更长期：自定义 `RibbonCommandButton::paintEvent()` 绘制箭头。

### Phase 4：文本策略

1. 所有大按钮必须使用 `RibbonCommandDefinition::label` 短标签。
2. 源码使用英文 + `QT_TR_NOOP`，显示使用 `QCoreApplication::translate`。
3. 超过 role 宽度时：优先 elide，而不是撑高/遮挡。
4. 完整命令名保留到 tooltip 和下拉菜单。

### Phase 5：添加布局测试页

新增一个 McStudio internal/debug tab 或测试函数，包含：

- Large normal button
- Large dropdown button
- Small text button
- Icon button
- 长英文 label
- 中文 label
- 不同 ToolbarIconSize 下截图验证

## 验证标准

1. 所有 group caption 在同一水平基线。
2. `File` / `Help` / `Edit Tools` 等下拉按钮不视觉偏心。
3. 大按钮文字不遮挡 group caption。
4. 切换 FreeCAD ToolbarIconSize 后布局仍稳定。
5. 添加新命令时，只需指定 role + label，不需要单独调 padding/height。

## 当前结论

这是 Ribbon 框架层问题，需要重构为“role based layout engine”。局部 QSS 修补只能暂时缓解，后续命令一多还会复发。
