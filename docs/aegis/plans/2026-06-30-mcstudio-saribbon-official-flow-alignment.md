# McStudio SARibbon Official Flow Alignment

日期：2026-06-30
状态：Implementation

## 目标

对照 SARibbon 官方 `创建Ribbon界面` 文档，确认 McStudio 是否使用 SARibbon 原生 Category 分类页，并把当前代码进一步对齐官方层级：

```text
SARibbonBar
  SARibbonCategory
    SARibbonPanel
      Action / Menu / Widget
```

## 审查结论

当前 McStudio 已经使用 SARibbon 官方分类页：

```cpp
SARibbonCategory* category = _ribbon->addCategoryPage(...);
```

并使用官方 Panel：

```cpp
SARibbonPanel* panel = category->addPanel(...);
```

命令通过：

```cpp
addLargeAction
addSmallAction
addLargeMenu
addSmallMenu
```

进入 Panel。

## 当前未接入的官方组件

暂时没有接入：

```text
Context Category
QuickAccessBar
RightButtonGroup
ApplicationButton
ApplicationWidget
Gallery
```

原因：McStudio 当前嵌入 FreeCAD TopDockHost，titlebar 为 0，不适合马上启用依赖 title 区域的 ApplicationButton / QuickAccessBar / RightButtonGroup。后续可以作为独立阶段接入。

## 本轮对齐修改

1. 为 Category 设置稳定 objectName。
2. 为 Panel 设置稳定 objectName。
3. 为 proxy QAction / QMenu 设置稳定 objectName。
4. 保持国际化显示文本不变。
