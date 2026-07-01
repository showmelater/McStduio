# McStudio SARibbon Tab Row Disappeared Fix

日期：2026-06-30
状态：Implementation

## 问题

截图中 FreeCAD 经典菜单栏仍然存在：

```text
文件 / 编辑 / 视图 / 工具 / 宏 / McStudio / 窗口 / 帮助
```

消失的是 SARibbon category tab 行：

```text
Home / Features / Sketch / Surface / Evaluate / View / Setup / Output / Advanced
```

## 根因

上一轮为了降低嵌入式 SARibbon 高度，设置了：

```cpp
titleBarHeight = 0
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow)
```

SARibbon 的 `setRibbonStyle(CompactThreeRow)` 内部会自动调用：

```cpp
setTabOnTitle(isCompactStyle()); // compact => true
```

因此 tab 被放到 title 区。

但 title 区高度为 0：

```cpp
setTitleBarHeight(0)
```

所以 tab 行被压没。

## 决策

在 FreeCAD TopDockHost 嵌入模式下：

```text
titleBarHeight = 0
but tabOnTitle = false
```

也就是：

```cpp
ribbon->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
ribbon->setTitleBarHeight(0);
ribbon->setTabOnTitle(false);
ribbon->setTabBarHeight(26);
```

这样 SARibbon 不显示自己的 title bar，但 category tab 仍然单独显示。
