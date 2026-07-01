# McStudio Ribbon Vertical Scroll Regression

日期：2026-06-30
状态：Root cause identified / Fix planned

## 现象

McStudio Ribbon 顶部面板已经显示在正确位置，但面板内部上下方向存在隐藏空间，鼠标滚轮/触控板可上下滑动，说明 Ribbon page 内容高度大于 viewport 高度。

## 根因

当前 Ribbon page 使用 `QScrollArea`：

```cpp
page->setWidgetResizable(true);
page->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
page->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
page->setMaximumHeight(metrics.pageMaxHeight);
```

`ScrollBarAlwaysOff` 只隐藏竖向滚动条，不等于禁止竖向滚动。只要内部内容比 viewport 高，`QScrollArea` 仍可能响应 wheel/trackpad 产生竖向位移。

内部内容高度主要来自：

- 大按钮 `QToolButton::TextUnderIcon`
- 图标尺寸来自 FreeCAD `ToolbarIconSize`
- group 上下 margin
- command row 与 group label 间距
- group label 本身高度
- QSS 中 QToolButton padding/min-height

当前公式：

```cpp
pageMaxHeight = largeIconSize + 48;
ribbonMaxHeight = pageMaxHeight + 28;
```

这个公式偏小，不能覆盖真实 `QToolButton + group label + margins` 的 sizeHint，导致竖向裁切。

## 修复原则

Ribbon 不应该有竖向滚动。只允许横向内容过多时横向滚动或后续折叠。

修复策略：

1. 用更保守的公式计算 page/ribbon 高度。
2. 给 page/content/group 设置 minimumHeight 与 maximumHeight，避免 viewport 小于内容高度。
3. 关闭 QScrollArea 的竖向滚动条对象，减少 wheel 导致竖向滚动的可能。
4. 如仍有问题，后续用普通 QWidget + 横向 QScrollArea 或重写 wheelEvent。

