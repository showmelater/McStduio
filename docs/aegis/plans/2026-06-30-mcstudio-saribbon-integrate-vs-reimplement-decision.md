# McStudio SARibbon 引用 vs 仿写决策分析

日期：2026-06-30
状态：Aegis Decision Analysis

## 问题

McStudio 当前 Ribbon 存在框架性问题：

- 下拉按钮视觉中心偏移，例如 File。
- 大按钮文字和 group caption 互相挤压。
- group caption 没有固定在底部。
- 后续 1000+ 命令无法靠局部 QSS 维护。

SARibbon 已经下载在：

```text
/home/aa/Desktop/McStduio/SARibbon/
```

需要判断：

```text
A. 直接引用 SARibbon 第三方库
B. 仿照 SARibbon 写 McStudio 自己的轻量 Ribbon framework
C. 两阶段：先引用验证，再决定是否仿写/沉淀
```

## 官方导入方式

SARibbon 官方文档提供：

### 1. 直接引入

将：

```text
SARibbon.h
SARibbon.cpp
```

直接加入工程。

如果不启用 qwk/frameless，默认不需要 QWindowKit。

### 2. CMake 第三方库方式

流程：

```cmake
set(SARibbonBar_DIR ".../lib/cmake/SARibbonBar")
find_package(SARibbonBar)
target_link_libraries(target PUBLIC SARibbonBar::SARibbonBar)
```

### 3. 静态库方式

如果静态编译并定义 `SA_RIBBON_BAR_NO_EXPORT`，需要手动初始化资源：

```cpp
Q_INIT_RESOURCE(SARibbonResource);
```

## 本地源码事实

本地 `SARibbon/src/SARibbonBar/CMakeLists.txt` 显示：

- 库名：`SARibbonBar`
- 可构建 STATIC 或 SHARED
- 依赖 Qt Core / Gui / Widgets / Svg
- 默认可不启用 frameless
- 内部包含 colorWidgets 源码
- 资源文件：`SARibbonResource.qrc`
- 提供 alias target：`SARibbonBar::SARibbonBar`

McStudio 当前 GUI CMake：

```cmake
set(McStudioGui_LIBS
    FreeCADGui
)

add_library(McStudioGui SHARED ...)
target_link_libraries(McStudioGui ${McStudioGui_LIBS})
```

也就是说，当前 McStudioGui 是 FreeCAD 的一个 GUI shared library，不能像独立 Qt app 一样接管 MainWindow。

## 方案 A：直接引用 SARibbon.h/.cpp 到 McStudioGui

### 做法

把 SARibbon 单文件版本加入 `McStudioGui_SRCS`：

```cmake
set(McStudioGui_SRCS
    ...
    ${CMAKE_SOURCE_DIR}/SARibbon/src/SARibbon.h
    ${CMAKE_SOURCE_DIR}/SARibbon/src/SARibbon.cpp
)
```

并增加 include path：

```cmake
target_include_directories(McStudioGui PRIVATE
    ${CMAKE_SOURCE_DIR}/SARibbon/src
)
```

如有 QtSvg 依赖，补：

```cmake
target_link_libraries(McStudioGui Qt${QT_VERSION_MAJOR}::Svg)
```

### 优点

- 最快。
- 不需要安装外部库。
- 不需要处理运行时 `libSARibbonBar.so` 路径。
- 适合做 prototype。
- 方便直接调试 SARibbon 源码。

### 风险

- `SARibbon.cpp` 是 amalgamated 大文件，编译会变慢。
- 第三方代码直接进入 `McStudioGui.so`，以后更新 SARibbon 需要手动替换。
- 可能和 FreeCAD 的 Qt 编译宏、PCH、warning policy 冲突。
- 静态资源初始化、宏定义需要验证。
- 不适合长期精细裁剪。

### 适用阶段

适合：Phase 0 原型验证。

不建议：直接作为最终架构。

## 方案 B：作为 CMake 第三方库 add_subdirectory / find_package 引用

### 做法 1：add_subdirectory

```cmake
set(SARIBBON_BUILD_STATIC_LIBS ON)
set(_SARIBBON_USE_FRAMELESS_LIB OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/SARibbon/src/SARibbonBar ${CMAKE_BINARY_DIR}/SARibbonBar)
target_link_libraries(McStudioGui SARibbonBar::SARibbonBar)
```

### 做法 2：find_package

先独立安装 SARibbon，再：

```cmake
set(SARibbonBar_DIR "...")
find_package(SARibbonBar REQUIRED)
target_link_libraries(McStudioGui SARibbonBar::SARibbonBar)
```

### 优点

- 第三方库边界清晰。
- 可升级、可独立构建。
- 符合官方 CMake 引入方式。
- 适合长期作为第三方依赖管理。

### 风险

- FreeCAD 主工程打包时要处理第三方库安装。
- 如果 SHARED，要处理运行时 `libSARibbonBar.so` 搜索路径。
- 如果 STATIC，要处理资源初始化。
- SARibbon CMake 可能定义 install/export/translation 逻辑，与 FreeCAD 大工程策略需要适配。
- 引入完整 SARibbon 可能带入 Customize/Dialog/Gallery/Frameless 等 McStudio 暂时不需要的模块。

### 适用阶段

适合：SARibbon 原型验证通过后，如果决定正式第三方依赖，再整理为 3rdparty target。

## 方案 C：仿照 SARibbon 写 McStudio 自己的轻量 Framework

### 做法

不直接依赖 SARibbon 代码，参考它的架构实现：

```text
McStudioRibbonBar
  McStudioRibbonCategory
    McStudioRibbonPanel
      McStudioRibbonPanelLayout
        McStudioRibbonButton
```

吸收以下机制：

- PanelLayout 自定义 QLayout。
- Panel caption 固定底部 geometry。
- Button layout strategy：iconRect/textRect/indicatorRect。
- Large/Medium/Small row proportion。
- Category 横向 overflow。

### 优点

- 完全适配 FreeCAD 工作台、QAction、Dock、参数、国际化。
- 体量可控，不引入大量不需要的 SARibbon 功能。
- 长期维护更清晰。
- UI 能按 CAD/CAM 专用需求定制。
- 避免第三方库打包和 ABI 问题。

### 风险

- 初期开发成本高。
- 需要实现和验证 layout engine。
- 短期不能快速看到完整效果。
- 容易重复造轮子，如果没有 SARibbon prototype 对照，方向可能走偏。

### 适用阶段

适合：中长期正式 McStudio RibbonFramework v2。

## 关键约束：不能直接替换 FreeCAD MainWindow

无论引用还是仿写，都不应该把 FreeCAD 主窗口改成 `SARibbonMainWindow`。

原因：

- FreeCAD `Gui::MainWindow` 管理 Workbench、Dock、MDI、TaskPanel、StatusBar、ToolBarManager。
- 替换基类侵入巨大。
- 上游维护和 rebase 风险高。

正确宿主仍然是现有方案：

```text
Gui::MainWindow
  TopDockWidgetArea
    McStudioRibbonDockHost
      SARibbonBar 或 McStudioRibbonBar
```

## 推荐决策

推荐使用 **两阶段路线**：

```text
先引用 SARibbon 做可切换 prototype；
验证通过后，再决定正式依赖 SARibbon，还是仿照其架构沉淀 McStudioRibbonFramework v2。
```

具体：

### Phase 0：直接引用 SARibbon.h/.cpp，做 Prototype

目标不是替换现有 Ribbon，而是验证：

- 能否编译进 FreeCAD/McStudio。
- 能否作为普通 widget 嵌入 TopDockHost。
- group caption 是否固定。
- File 下拉按钮是否不歪。
- 文字是否不遮挡。
- QAction enable/disable 是否同步。

新增参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Backend = "Custom" | "SARibbonPrototype"
```

默认：

```text
Backend = "Custom"
```

只有手动切换到 `SARibbonPrototype` 时才启用 SARibbon。

### Phase 1：如果 Prototype 通过，再做二选一

#### 选择 1：正式第三方依赖 SARibbon

条件：

- 嵌入 TopDockHost 高度稳定。
- 不依赖 SARibbonMainWindow。
- QAction / QMenu / 下拉 / 状态同步正常。
- 构建和资源无明显问题。
- 打包策略可接受。

做法：

- 从 `SARibbon.h/.cpp` 原型升级到 `add_subdirectory` 或 `find_package` 管理。
- 将 SARibbon 放入 `src/3rdParty` 或项目根 `3rdParty`。
- 固定版本和许可证说明。

#### 选择 2：仿照实现 McStudioRibbonFramework v2

条件：

- SARibbon 视觉证明方向正确。
- 但集成成本、打包、定制、MainWindow 假设太重。

做法：

- 参考 SARibbonPanelLayout / SARibbonToolButton / ButtonLayoutStrategy 结构。
- 实现 McStudio 自己的轻量 framework。
- 不复制整库，只借鉴架构。

## 当前建议

当前最稳建议：

```text
不要马上仿写；
不要马上正式依赖；
先直接引用 SARibbon.h/.cpp 做最小 prototype。
```

原因：

- 我们现在还没有验证 SARibbonBar 能否作为普通 widget 嵌进 FreeCAD TopDockHost。
- 如果可以，很多布局问题会立刻有对照答案。
- 如果不可以，也能知道哪些机制值得仿写。
- 直接引用原型成本最低，回滚也最容易。

## 最小 Prototype 范围

只做两个 Tab：

```text
Home
  File
    New
    Open
    Save
  Edit
    Undo
    Redo
    Refresh
  View
    Fit All
    Fit Selection
    Draw Style

Features
  Main Features
    Pad
    Pocket
    Hole
  Reference
    Reference Plane
```

不要一次迁移 1000+ 命令。

## 验收标准

Prototype 成功的标准：

1. 构建通过。
2. 切到 McStudio 后 SARibbon 显示在 TopDockHost。
3. 离开 McStudio 后隐藏。
4. group caption 固定底部。
5. File / Help / dropdown 不歪。
6. 大按钮文字不遮挡。
7. FreeCAD QAction 状态和触发正常。
8. 不影响 FreeCAD 其它 Workbench。

Prototype 失败的判断：

1. SARibbonBar 不能脱离 SARibbonMainWindow 稳定工作。
2. 高度/布局强依赖 menuBar/titleBar。
3. 与 FreeCAD MainWindow 事件或菜单冲突严重。
4. 构建和资源接入成本过高。

失败时转入：

```text
仿照 SARibbon 的 PanelLayout + ToolButtonLayoutStrategy 写 McStudioRibbonFramework v2。
```

## 最终一句话

技术上可以直接引用 SARibbon；
工程上不应该直接替换现有 UI；
最佳路线是：

```text
直接引用做 prototype -> 验证 -> 决定正式第三方依赖或仿照实现。
```
