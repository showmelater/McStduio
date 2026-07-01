# McStudio SARibbon Phase 1 Implementation Plan

日期：2026-06-30
状态：Implementation

## 目标

按长期 ADR，将 SARibbon 作为 McStudio Ribbon Framework 引入，但不使用 SARibbonMainWindow，不替换 FreeCAD MainWindow。

第一阶段目标：

1. 将本地 `SARibbon/` 作为 third-party CMake target 接入。
2. 新增 `McStudioSARibbonBackend`。
3. `RibbonWidget` 在 TopDockHost 内创建 SARibbon backend。
4. 先使用最小命令集验证布局。
5. 保留当前 Custom backend 作为回滚路径。

## 设计约束

- 不改 FreeCAD `Gui::MainWindow` 基类。
- 不使用 `SARibbonMainWindow`。
- SARibbon 静态链接优先，减少运行时部署问题。
- 关闭 frameless / QWindowKit。
- FreeCAD QAction 仍来自 `Gui::CommandManager`。
- Ribbon 显示文本仍使用英文源文本 + Qt translate。

## 代码结构

新增：

```text
src/Mod/McStudio/Gui/RibbonBackends/
  McStudioRibbonBackend.h
  McStudioSARibbonBackend.h
  McStudioSARibbonBackend.cpp
```

职责：

- `McStudioRibbonBackend`：抽象接口。
- `McStudioSARibbonBackend`：将 `RibbonModel` 映射为 SARibbonBar/Category/Panel/Button。

## 构建策略

在 `src/Mod/McStudio/Gui/CMakeLists.txt` 中：

```cmake
set(SARIBBON_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(SARIBBON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SARIBBON_USE_FRAMELESS_LIB OFF CACHE BOOL "" FORCE)
set(_SARIBBON_USE_FRAMELESS_LIB OFF CACHE BOOL "" FORCE)
set(QT_VERSION_MAJOR ${FREECAD_QT_MAJOR_VERSION})
add_subdirectory(${CMAKE_SOURCE_DIR}/SARibbon/src/SARibbonBar ${CMAKE_BINARY_DIR}/src/3rdParty/SARibbonBar)
```

链接：

```cmake
target_link_libraries(McStudioGui PRIVATE SARibbonBar::SARibbonBar)
```

## 回滚

如果 SARibbon 构建或运行失败，保留旧 Custom backend，参数控制：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Backend = Custom | SARibbon
```


## 2026-06-30 执行记录

已完成第一轮长期方案落地：

### 构建接入

- 在 `src/Mod/McStudio/Gui/CMakeLists.txt` 中接入本地 `SARibbon/src/SARibbonBar`。
- 使用静态库方式：`SARIBBON_BUILD_STATIC_LIBS=ON`。
- 禁用 examples/tests/frameless。
- 链接 `SARibbonBar::SARibbonBar` 到 `McStudioGui`。
- 处理 SARibbon CMake 中 `CMAKE_DEBUG_POSTFIX` 未引用导致的 `set_target_properties` 参数错误。

### 后端接入

新增：

```text
src/Mod/McStudio/Gui/RibbonBackends/McStudioRibbonBackend.h
src/Mod/McStudio/Gui/RibbonBackends/McStudioSARibbonBackend.h
src/Mod/McStudio/Gui/RibbonBackends/McStudioSARibbonBackend.cpp
```

`McStudioSARibbonBackend` 负责：

- 创建 `SARibbonBar`。
- 将 `RibbonTabDefinition` 映射为 `SARibbonCategory`。
- 将 `RibbonGroupDefinition` 映射为 `SARibbonPanel`。
- 将 `RibbonCommandDefinition` 映射为 `addLargeAction/addSmallAction/addLargeMenu/addSmallMenu`。
- 使用代理 `QAction` 保持 Ribbon 短 label，不污染 FreeCAD 原始 QAction 文本。
- 对 `Gui::ActionGroup` 和带 menu 的 QAction 创建 SARibbon menu。

### 宿主接入

`RibbonWidget.cpp` 继续保留：

```text
Top QDockWidget Host
```

但内部优先创建：

```text
McStudioSARibbonBackend
  SARibbonBar
```

如果 SARibbon backend 未创建成功，保留旧 `createRibbonWidget()` 作为 fallback。

### 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：

```text
lib/libSARibbonBard.a built
Mod/McStudio/McStudioGui.so linked
exit code 0
```

仍然存在既有版本生成信息：

```text
致命错误：Not a valid object name upstream/main
```

但它不影响本轮 SARibbon / McStudioGui 构建。

### 待 GUI 验证

需要重启 FreeCAD 并切换到 McStudio，检查：

1. SARibbon 是否显示在 TopDockHost。
2. group caption 是否固定底部。
3. File / dropdown 是否不再偏移。
4. 大按钮文字是否不再遮挡。
5. 下拉菜单是否可展开。
6. FreeCAD QAction 是否可正常触发。
7. 切换离开 McStudio 后 Ribbon 是否隐藏。

