# McStudio Ribbon Framework Layout Analysis

日期：2026-06-30
状态：Aegis Analysis / No code change yet

## 现象

截图中的问题包括：

1. `File` 下拉大按钮视觉上偏斜。
2. 部分按钮名称被遮挡。
3. group 底部标题跟随上方按钮高度漂移，没有固定在 group 底部。
4. 不同 group 的标题 baseline 不一致，导致整条 Ribbon 缺少统一节奏。

这些不是局部 QSS 问题，而是当前 Ribbon framework 没有定义完整的 layout contract。

## 当前实现结构

当前 `RibbonModel` 数据结构：

```cpp
struct RibbonCommandDefinition {
    const char* command;
    RibbonCommandSize size;
    RibbonCommandStyle style;
    const char* label;
};

struct RibbonGroupDefinition {
    const char* title;
    std::vector<RibbonCommandDefinition> commands;
};
```

当前 `RibbonWidget` 生成结构：

```text
QTabWidget
  QScrollArea page
    QWidget pageContent / QHBoxLayout
      QFrame group / QVBoxLayout
        QWidget commandRow / QHBoxLayout
          QWidget large strip
            QToolButton...
          QWidget secondaryColumn
            QWidget small strip
            QWidget icon strip
        QLabel groupLabel
```

## 根因

### 1. RibbonModel 只描述命令，不描述布局槽位

当前只知道一个命令是 Large / Small / IconOnly，不知道它属于：

- primary area
- secondary area
- quick access area
- split button area
- group menu area
- overflow area

因此 `RibbonWidget` 只能靠 size/style 临时分流，缺少长期可扩展的 layout contract。

### 2. group 标题没有底部锚定

当前 group 是：

```cpp
layout->addWidget(commandRow);
layout->addWidget(label);
```

这是顺序布局，不是 anchored layout。上方命令区高度变化时，底部标题自然跟着移动。

正确结构应该是：

```text
Group fixed height
  Command area: fixed/allocated region
  Group caption: fixed bottom region
```

也就是 group 内部要先分配固定区域，而不是让控件自然堆叠。

### 3. 大下拉按钮使用 QToolButton 默认 MenuButtonPopup

`MenuButtonPopup` 会给右侧 arrow sub-control 分配视觉空间，导致 icon/text 的视觉中心偏左，`File` 这类大下拉按钮看起来歪。

这不是单个 File 的问题，所有大下拉按钮都会有同类风险。

### 4. 按钮尺寸由文字动态决定，但没有全局 cell grid

当前每个按钮按自己的文字宽度计算 minimum size：

```cpp
fontMetrics.horizontalAdvance(displayText)
```

这样会导致：

- 每个按钮宽度不同。
- group 内部视觉不齐。
- 下拉箭头、短文字、长文字混在同一排时节奏不稳定。

Ribbon 应该先定义 cell grid，再把按钮放入 cell，而不是每个按钮自己决定版面。

### 5. QSS 仍保留局部 min-height/min-width

QSS 中仍有：

```qss
QWidget[ribbonRole="large"] QToolButton {
    min-width: 48px;
    min-height: 48px;
}
```

这类规则应该只是视觉兜底，不应该参与布局计算。布局尺寸应该由 framework metrics 统一决定。

## 结论

当前实现已经从 `QToolBar` 过渡到 `QWidget + QToolButton`，但还没有完成真正的 Ribbon framework。

下一步不应该继续局部修 `File` 或某个文字遮挡，而应该建立：

```text
Ribbon Metrics
Ribbon Layout Contract
Ribbon Button Archetypes
Ribbon Group Grid
Ribbon Overflow Strategy
```

## 推荐长期架构

### 1. 引入 RibbonMetricsV2

统一定义所有尺寸：

```cpp
struct RibbonMetrics {
    int tabBarHeight;
    int pageHeight;
    int groupHeight;
    int groupCaptionHeight;
    int commandAreaHeight;

    int largeButtonWidth;
    int largeButtonHeight;
    int smallButtonWidth;
    int smallButtonHeight;
    int iconButtonSize;

    int largeIconSize;
    int smallIconSize;
    int groupSpacing;
    int commandSpacing;
};
```

核心规则：

```text
groupHeight = commandAreaHeight + groupCaptionHeight
```

而不是让 group 根据内容自然增长。

### 2. 引入按钮原型 Archetype

固定 4 类按钮：

#### LargeButton

```text
[large icon]
[short label]
```

- 固定 cell width。
- 固定 cell height。
- label 最多 1 行或 2 行。
- 不允许长 QAction text 直接进入。

#### SmallButton

```text
[small icon] short label
```

- 固定行高。
- 最多 3 行 stack。
- 用于辅助命令。

#### IconButton

```text
[small icon]
```

- 固定 square size。
- 无 label。

#### DropDownButton

```text
[icon]
[label]
[arrow overlay]
```

- 不使用右侧 split 区域挤压内容。
- arrow 作为 overlay 绘制或 QSS sub-control 绝对定位。
- 推荐大下拉使用 InstantPopup 或 DelayedPopup，不使用 MenuButtonPopup 的 split 宽度。

### 3. 引入 group grid

每个 group 分两个固定区：

```text
┌ Group ────────────────────────┐
│ CommandArea fixed height       │
│   primary lane                 │
│   secondary lane               │
│                                │
│ Caption fixed bottom height    │
└────────────────────────────────┘
```

C++ 层实现：

```cpp
frame->setFixedHeight(metrics.groupHeight);

layout->addWidget(commandArea, 1);
layout->addWidget(caption, 0);

commandArea->setFixedHeight(metrics.commandAreaHeight);
caption->setFixedHeight(metrics.groupCaptionHeight);
```

caption baseline 由 `groupCaptionHeight` 统一控制，不再随命令变化。

### 4. RibbonModel 升级为版式模型

当前：

```cpp
largeCommand(...)
smallCommand(...)
iconCommand(...)
```

建议升级为：

```cpp
RibbonCommandDefinition {
    const char* command;
    const char* label;
    RibbonCommandArchetype archetype;
    RibbonCommandPlacement placement;
    RibbonCommandOverflowPolicy overflow;
};
```

例如：

```cpp
ribbonCommand("Std_New", QT_TR_NOOP("New"), LargeButton, PrimaryLane)
ribbonCommand("Std_Undo", nullptr, IconButton, SecondaryLane)
ribbonCommand("McStudio_CompApplicationMenu", QT_TR_NOOP("File"), DropDownLargeButton, PrimaryLane)
```

### 5. 不能让 QAction text 直接决定 Ribbon 布局

原则：

```text
QAction::text       -> menu / tooltip / command palette
Ribbon label        -> short UI label
Tooltip/status tip  -> full explanation
```

如果某命令要进入 Ribbon，必须有短 label 或明确声明使用 action text。

### 6. 建立 Overflow 策略

后续会有 1000+ 命令，不可能都铺在 Ribbon 上。

每个 group 需要：

- 常驻命令
- 下拉命令
- overflow menu
- command finder fallback

推荐：

```text
Primary commands: 常驻大按钮
Secondary commands: 小按钮 stack
Overflow commands: group menu
Hidden advanced commands: command finder
```

## 对当前问题的框架级解释

### File 歪

不是 File 自己歪，而是 DropDownButton 没有独立 archetype。当前使用普通 QToolButton + MenuButtonPopup，arrow 占据视觉空间。

框架级修法：定义 DropDownLargeButton archetype，arrow 作为 overlay，不参与 content center。

### 文字遮挡

不是某个 label 太长，而是 Button cell 没有统一高度，Group command area 和 caption area 没有硬隔离。

框架级修法：固定 groupHeight / commandAreaHeight / captionHeight，按钮只在 commandArea 内布局。

### group 小标题跟着上面走

不是 QLabel 的问题，而是 Group layout 没有 bottom caption slot。

框架级修法：caption 是固定 slot，不是自然堆叠控件。

## 建议实施阶段

### Phase 1：冻结局部修补，建立 RibbonMetricsV2

目标：所有高度、宽度、caption、cell 都由 metrics 统一计算。

### Phase 2：重写 createRibbonGroup 为固定 grid

目标：

- group fixed height
- command area fixed height
- caption fixed bottom
- page 不再有 vertical overflow

### Phase 3：重写 createCommandButton 为 archetype factory

目标：

- LargeButton
- SmallButton
- IconButton
- DropDownLargeButton
- DropDownSmallButton

每类按钮统一尺寸、popup 策略、text elide/wrap 策略。

### Phase 4：升级 RibbonModel schema

目标：命令显式声明 archetype/placement，而不是靠 size/style 反推。

### Phase 5：加 overflow / command finder

目标：1000+ 命令不直接挤进 Ribbon。

## 验收标准

1. 所有 group caption 在同一水平 baseline。
2. 大下拉按钮内容视觉居中，arrow 不造成偏移。
3. 大按钮文字不遮挡 caption。
4. 小按钮 stack 行高一致。
5. page 不可竖向滚动。
6. 添加新命令时，只需声明 archetype/placement，不需要局部调 QSS。
7. 源码 UI 文本仍保持英文源文本 + QT_TR_NOOP + translate。

