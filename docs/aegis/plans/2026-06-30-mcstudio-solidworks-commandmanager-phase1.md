# McStudio SolidWorks CommandManager Phase 1

日期：2026-06-30
状态：Implementation

## 目标

将当前 SARibbon 命令组织向 SolidWorks CommandManager 风格收敛：

- Home 只保留常用入口，减少灰色 disabled 命令。
- 专业建模命令进入 Features / Sketch / Surface 等专用 Category。
- 大按钮只给主命令，小按钮和下拉承载二级命令。
- Structure 从 Home 移到 Advanced，避免首页噪声。
- 不引入 SARibbonMainWindow，不使用 titlebar 相关 ApplicationButton / QuickAccessBar。

## 对应关系

```text
SolidWorks CommandManager Tab -> SARibbonCategory
SolidWorks Toolbar Group       -> SARibbonPanel
Main Command                   -> addLargeAction / addLargeMenu
Secondary Command              -> addSmallAction / addSmallMenu
```

## 本轮范围

修改 `RibbonModel.cpp` 的命令组织，不改底层 SARibbon backend、不改 FreeCAD MainWindow、不改命令实现。

## 2026-06-30 执行记录

已完成 Phase 1：`RibbonModel.cpp` 命令组织重排。

### Home 瘦身

从旧结构：

```text
Application / File / Edit / Quick View / Structure / Help
```

调整为 SolidWorks-like 常用入口：

```text
Application / Documents / Edit / View / Help
```

变化：

- 删除 Home 里的 `Structure` panel。
- 删除 Home 里的 `Quick View` panel，合并到 `View` panel。
- `Documents` 保留 New / Open 大按钮，Save / Import / Export 小按钮。
- `View` 保留 Fit All / Fit Selection 大按钮，Standard Views / Display 下拉。

### Features 重排

调整为 SolidWorks Features Tab 风格：

```text
Create
Reference Geometry
Dress-Up
Pattern / Mirror
Advanced Features
```

变化：

- Pad / Pocket / Hole 保持主命令大按钮。
- Revolve / Groove / Sweep / Loft 从大按钮降为小按钮，减少视觉噪声。
- Reference Plane 短标签改为 Plane，更接近 CAD Ribbon 短文案。

### Sketch 重排

调整为 SolidWorks Sketch Tab 风格：

```text
Start
Draw
Modify
Dimensions
Relations
```

变化：

- Draw 只放线、矩形、圆弧、圆锥等创建命令。
- Modify 独立出来放 Fillet / Curve Edit。
- Dimensions 独立出来放 Dimension / Horizontal-Vertical。
- Relations 独立出来放 Coincident / Parallel / Tangent / Equal。

### Advanced 收纳 Structure

从 Home 移走的结构类命令进入 Advanced：

```text
Structure
  Part / Group / Link / Variable Set
```

### 保持不变

- SARibbon backend 不改。
- FreeCAD MainWindow 不改。
- Theme / layout 不改。
- 不启用 SARibbon ApplicationButton / QuickAccessBar / RightButtonGroup。

### 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮 RibbonModel 重排无关。
