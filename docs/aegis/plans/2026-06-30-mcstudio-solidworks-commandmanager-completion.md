# McStudio SolidWorks CommandManager Completion

日期：2026-06-30
状态：Implementation

## 目标

完成 SolidWorks CommandManager 化第一轮完整落地：

1. 增加 Medium command 模型，对齐 SARibbon/SolidWorks 的三行布局。
2. 常规 Category 继续使用 `SARibbonBar -> SARibbonCategory -> SARibbonPanel -> Action/Menu`。
3. 增加 ContextCategory 模型，对齐 SolidWorks 上下文标签逻辑。
4. SARibbon 后端构建 ContextCategory，并根据简单状态显示/隐藏。
5. 不替换 FreeCAD MainWindow，不使用 SARibbonMainWindow。

## 上下文策略

第一版上下文触发：

- `Sketch Tools`：FreeCAD active document 处于 edit mode 时显示。
- `Selection Tools`：存在选择对象时显示。
- `Manufacturing Context`：先构建但默认隐藏，后续接工艺对象/Setup 选择类型。

使用轻量 QTimer 周期刷新，避免本轮侵入 FreeCAD SelectionObserver / DocumentObserver 架构。

## 2026-06-30 执行记录

已完成 SolidWorks CommandManager 化完整第一轮。

### 1. 增加 Medium 命令模型

`RibbonCommandSize` 从：

```cpp
Large / Small
```

升级为：

```cpp
Large / Medium / Small
```

新增 helper：

```cpp
mediumCommand(command)
mediumCommand(command, label)
mediumDropDownCommand(command)
mediumDropDownCommand(command, label)
```

SARibbon 后端已映射：

```cpp
RibbonCommandSize::Large  -> addLargeAction / addLargeMenu
RibbonCommandSize::Medium -> addMediumAction / addMediumMenu
RibbonCommandSize::Small  -> addSmallAction / addSmallMenu
```

### 2. SolidWorks-like 命令密度调整

将常用但非主命令从 Small/IconOnly 提升为 Medium，以形成 SolidWorks CommandManager 三行布局：

- Save / Import / Export
- Undo / Redo / Refresh
- Body / Datum Geometry / Boolean
- Sketch Validate / Arc / Conic / Fillet / Curve Edit
- Sketch Relations 部分命令

### 3. 增加 ContextCategory 模型

新增：

```cpp
struct RibbonContextDefinition
```

并新增：

```cpp
const std::vector<RibbonContextDefinition>& ribbonContexts();
```

第一版上下文标签：

```text
Sketch Tools
Selection Tools
Manufacturing Context
```

### 4. SARibbon 后端构建 ContextCategory

`McStudioSARibbonBackend.cpp` 现在会构建：

```cpp
_ribbon->addContextCategory(...)
context->addCategoryPage(...)
category->addPanel(...)
```

上下文 category 默认隐藏。

### 5. ContextCategory 显示策略

第一版轻量触发：

```text
SketchContext    -> active document is in edit mode
SelectionContext -> current selection count > 0
Manufacturing    -> built but hidden; reserved for Setup/Process object context
```

使用 `QTimer` 每 500ms 刷新一次：

```cpp
refreshContextCategories()
```

这避免本轮侵入 FreeCAD SelectionObserver / DocumentObserver 架构，后续可替换为真正事件驱动。

### 6. 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

既有 version generation 输出：

```text
致命错误：Not a valid object name upstream/main
```

与本轮 Ribbon 完整化无关。

## 验证重点

1. 常规 Home / Features / Sketch tabs 正常显示。
2. Medium 按钮在 SARibbon 三行布局里显示合理。
3. 进入 Sketch/edit mode 时出现 Sketch Tools 上下文标签。
4. 选择对象时出现 Selection Tools 上下文标签。
5. 取消选择/退出编辑后上下文标签隐藏。
6. 横向宽度不再撑大主窗口。
