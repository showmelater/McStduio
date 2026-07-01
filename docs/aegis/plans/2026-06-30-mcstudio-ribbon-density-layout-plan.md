# McStudio Ribbon 图标大小与组布局优化计划

日期：2026-06-30
状态：Implementation Plan

## 当前现象

截图显示 McStudio Ribbon 已经位于顶部，但内部布局存在问题：

1. 组之间被横向拉得很开。
2. 单个命令组被分散到左、中、右，右侧空白被误当成组间距。
3. 大图标按钮偏大，Ribbon 高度略显臃肿。
4. 组分隔线很长，视觉上像一条横向表格，而不是紧凑 Ribbon。

## 根因定位

主要在 `src/Mod/McStudio/Gui/RibbonWidget.cpp`：

```cpp
frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
layout->setAlignment(Qt::AlignTop);
page->setWidgetResizable(true);
content->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
```

问题：

- `QSizePolicy::Minimum` 允许 group 在水平方向被拉伸。
- page `setWidgetResizable(true)` 会让内容宽度跟随 viewport。
- layout 只有 `AlignTop`，没有明确 `AlignLeft`。
- 没有尾部 stretch 专门吸收剩余宽度，导致 group 自己参与扩展。

## 对标规则

仿 NX / SOLIDWORKS，Ribbon 内部应该满足：

1. 命令组左对齐。
2. 组宽度由内容决定，不平均分布。
3. 右侧允许空白，但空白不应该出现在组与组之间。
4. 大按钮用于主命令，小按钮/图标按钮用于辅助命令。
5. Ribbon 高度控制在约 88~96px。

## 修改策略

### Step 1：组不可横向扩展

将 group、command row、secondary column 的水平策略从 `Minimum/Preferred` 调整为 `Maximum`：

```cpp
frame->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
commandRow->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
secondaryColumn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
```

### Step 2：Page 内容左对齐

```cpp
layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
layout->addStretch(1);
```

尾部 stretch 专门吸收剩余宽度，避免 group 被拉开。

### Step 3：降低 Ribbon 密度

默认：

```cpp
largeIconSize = 30
smallIconSize = 20
pageMargin = 4
spacing = 5
ribbonMaxHeight = 92
pageMaxHeight = 66
```

### Step 4：QSS 同步收紧按钮

大按钮：

```qss
min-width: 50px;
min-height: 44px;
padding: 3px 6px;
```

小按钮：

```qss
min-height: 22px;
```

## 验证点

1. 评估 tab 下三个组连续左对齐。
2. 中间不再出现巨大空白。
3. 右侧可以留空，但只在最后一个 group 后面。
4. Ribbon 高度略低于当前截图。
5. 首页/特征/草图 tab 不出现横向滚动条，除非命令确实超过窗口宽度。
6. 构建通过。
