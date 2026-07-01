# McStudio Ribbon 命令展示模型修复计划

日期：2026-06-30
状态：Implementation Plan

## 现象

Ribbon 中部分按钮文字被截断或遮挡，例如：

- `Create McStudio Reference Plane`
- `Dress-Up Features`
- `Create Setup Datum`

这类文字来自 FreeCAD/McStudio 命令的 `QAction::text()`，适合菜单项，不适合 Ribbon 大按钮下方显示。

## 根因

当前 `RibbonCommandDefinition` 只有：

```cpp
const char* command;
RibbonCommandSize size;
RibbonCommandStyle style;
```

缺少 `displayText` / `label` 字段。`RibbonWidget` 创建按钮时直接使用 `QAction::text()` 计算尺寸和显示文本。

菜单文本、tooltip、Ribbon 显示名是三种不同语义，不能混用。

## 统一展示规则

### 大按钮

- 用于主命令。
- 显示图标 + 短名称。
- 名称必须是 2~6 个中文字或短英文。
- 例：
  - `McStudio_CreateReferencePlane` -> `参考平面`
  - `McStudio_CompDressUpFeatures` -> `修饰特征`
  - `PartDesign_Revolution` -> `旋转`

### 小按钮带文字

- 用于辅助命令。
- 允许短文本。
- 不放长句子。

### 小图标无文字

- 用于高频且图标明确的命令。
- Tooltip 保留完整说明。

### 下拉按钮

- 显示短组名。
- 下拉菜单里保留完整命令文本。

## 实施策略

1. 扩展 `RibbonCommandDefinition` 增加 `const char* label`。
2. 增加重载 helper：
   - `largeCommand(command, label)`
   - `smallCommand(command, label)`
   - `iconCommand(command)`
   - `dropDownCommand(command, label)`
   - `smallDropDownCommand(command, label)`
3. `RibbonWidget` 创建按钮时：
   - `button->setDefaultAction(action)` 复用命令状态、图标、tooltip。
   - 如果 `label` 不为空，调用 `button->setText(label)` 覆盖 Ribbon 显示文本。
   - 尺寸计算使用显示文本，而不是 QAction 原始文本。
4. 更新 `RibbonModel.cpp` 中容易过长的大按钮/下拉按钮。

## 验证

1. 特征 tab 中长英文不再出现在按钮下方。
2. 大按钮文字完整显示。
3. 下拉菜单仍能显示完整命令项。
4. 构建通过。
