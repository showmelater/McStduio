# McStudio Python Ribbon Workbench 第一版实施规划

日期：2026-06-27  
分支建议：`feature/mcstudio-ribbon-workbench`  
状态：调研完成，待实现  
ArchitectureReviewRequired: yes

## 1. 目标

第一版 McStudio 不是重写 FreeCAD 建模、草图或 CAM 内核，而是新增一个 McStudio 工作台，用 SolidWorks 风格的 Ribbon UI 重新组织现有命令。

目标结果：

```text
McStudio Ribbon
├── 项目
├── 草图
├── 建模
├── 参考几何
├── 装夹
├── 加工
├── 仿真
└── 输出
```

用户切换到 McStudio 工作台时显示顶部 Ribbon；离开 McStudio 工作台时隐藏 Ribbon，不影响其它 FreeCAD 工作台。

## 2. 现状证据

### 2.1 构建模块可用性

`build/debug/CMakeCache.txt` 中确认：

```text
BUILD_PART=ON
BUILD_PART_DESIGN=ON
BUILD_SKETCHER=ON
BUILD_CAM=ON
BUILD_DRAFT=ON
BUILD_BIM=ON
BUILD_TECHDRAW=ON
BUILD_MEASURE=ON
BUILD_MATERIAL=ON
```

所以第一版可以加载并复用：

```text
Part / PartGui
PartDesign / PartDesignGui
Sketcher / SketcherGui
Path / PathGui / PathCommands
```

### 2.2 FreeCAD 工作台加载机制

FreeCAD GUI 启动时扫描 `src/Mod/<Module>/InitGui.py`，源码位置：

```text
src/Gui/FreeCADGuiInit.py:292-318
```

因此新增 McStudio 工作台的入口是：

```text
src/Mod/McStudio/InitGui.py
```

### 2.3 工作台切换机制

`Application::activateWorkbench()` 的顺序：

1. 找到旧工作台。
2. 找到新工作台 Python handler。
3. 首次激活时调用新工作台 `Initialize()`。
4. `WorkbenchManager::activate()` 激活新 C++/Python Workbench。
5. 调用旧工作台 `Deactivated()`。
6. 调用新工作台 `Activated()`。
7. 更新 active document 的 workbench 名称。

关键源码：

```text
src/Gui/Application.cpp:1894-2038
src/Gui/WorkbenchManager.cpp:114-124
```

结论：McStudio Ribbon 的显示/隐藏必须是幂等的：

```python
Activated()   -> Ribbon.show_ribbon()
Deactivated() -> Ribbon.hide_ribbon()
```

避免重复创建 DockWidget，也避免离开工作台后残留 Ribbon。

### 2.4 工作台菜单/工具栏机制

工作台激活时，FreeCAD 会重建工具栏、Dock、菜单：

```text
src/Gui/Workbench.cpp:450-476
```

传统工具栏由：

```text
src/Gui/ToolBarManager.cpp
src/Gui/ToolBarManager.h
```

管理，本质是 `QToolBar`。

但是 Ribbon 第一版不应该修改 `ToolBarManager`，而应该在 McStudio 工作台中自建顶部 Ribbon 面板。

### 2.5 Python 模块可以访问主窗口

本地已有示例使用：

```python
FreeCADGui.getMainWindow()
QtGui.QDockWidget()
mw.addDockWidget(...)
```

参考：

```text
src/Mod/TemplatePyMod/PythonQt.py:13-28
src/Mod/TemplatePyMod/PythonQt.py:77-86
```

结论：McStudio 可以用 `QDockWidget + QTabWidget` 做顶部 Ribbon，不用改 `MainWindow.cpp`。

## 3. 第一版架构决策

### 决策

第一版采用：

```text
src/Mod/McStudio
  Python Workbench
  Python Ribbon UI
  Python command wrappers
```

不修改：

```text
src/Gui/MainWindow.cpp
src/Gui/ToolBarManager.cpp
src/Gui/MenuManager.cpp
src/Gui/Workbench.cpp
```

### 原因

1. 最小可行路径满足 Ribbon 目标。
2. 不影响原 FreeCAD 工作台。
3. 容易合并 FreeCAD 上游。
4. 出问题时只需禁用 McStudio 模块。
5. 后续可以平滑升级为 C++ RibbonWidget。

## 4. 文件地图

第一版新增：

```text
src/Mod/McStudio/
├── CMakeLists.txt
├── Init.py
├── InitGui.py
├── Commands.py
├── Ribbon.py
└── Resources/
    ├── icons/
    └── styles/
        └── McStudioRibbon.qss
```

修改：

```text
src/Mod/CMakeLists.txt
```

添加：

```cmake
add_subdirectory(McStudio)
```

建议先不要增加全局 `BUILD_MCSTUDIO` 选项，第一版直接编译；后续产品化再加 CMake option。

## 5. 工作台类型选择

第一版使用：

```python
def GetClassName(self):
    return "Gui::PythonBlankWorkbench"
```

原因：

- 目标是 SolidWorks 风格 McStudio Ribbon。
- `Gui::PythonWorkbench` 会继承 FreeCAD 默认工具栏，上方会显得混乱。
- `Gui::PythonBlankWorkbench` 更适合清爽 UI。

需要补充的基础命令全部放进 Ribbon 的“项目”和“视图/快捷区”。

如果第一版验证发现 BlankWorkbench 缺失基础行为，再退回：

```python
return "Gui::PythonWorkbench"
```

## 6. 模块加载策略

### 6.1 必须加载

McStudio 工作台 `Initialize()` 中加载：

```python
import Part
import PartGui
import PartDesign
import PartDesignGui
import Sketcher
import SketcherGui
```

原因：

- PartDesign 命令由 `PartDesignGui` 注册。
- Sketcher 命令由 `SketcherGui` 注册。
- Part/PartGui 提供基础几何支持。

### 6.2 CAM 延迟加载

CAM 初始化较重，`src/Mod/CAM/InitGui.py` 中会加载：

```python
import Path
import PathScripts
import PathGui
import Path.GuiInit
cam_assets.setup()
CAMAssetMigrator().check_migration_needed()
```

所以 McStudio 第一版不要在工作台初始化时强制完整加载 CAM。建议策略：

```text
Ribbon 创建时显示“加工/仿真”Tab
点击 CAM 相关按钮时再 lazy_load_cam()
加载失败时弹出提示，不让 McStudio 工作台整体失败
```

示例逻辑：

```python
def ensure_cam_loaded():
    try:
        import Path
        import PathGui
        import PathCommands
        return True
    except Exception as exc:
        FreeCAD.Console.PrintError(f"McStudio: CAM load failed: {exc}\n")
        return False
```

CAM 命令按钮通过包装命令或 Ribbon callback 调用：

```python
if ensure_cam_loaded():
    FreeCADGui.runCommand("CAM_Job")
```

## 7. 命令映射

### 7.1 项目 Tab

| 显示名 | 命令 | 来源 | 第一版使用 |
|---|---|---|---|
| 新建 | `Std_New` | Gui | 是 |
| 打开 | `Std_Open` | Gui | 是 |
| 保存 | `Std_Save` | Gui | 是 |
| 另存为 | `Std_SaveAs` | Gui | 是 |
| 导入 | `Std_Import` | Gui | 是 |
| 导出 | `Std_Export` | Gui | 是 |
| 撤销 | `Std_Undo` | Gui | 快捷区 |
| 重做 | `Std_Redo` | Gui | 快捷区 |
| 参数设置 | `Std_DlgPreferences` | Gui | 是 |

### 7.2 草图 Tab

| 显示名 | 命令 | 来源 | 说明 |
|---|---|---|---|
| 新建草图 | `PartDesign_NewSketch` | PartDesign | 推荐入口 |
| 编辑草图 | `Sketcher_EditSketch` | Sketcher | 需要选中草图 |
| 映射草图 | `Sketcher_MapSketch` | Sketcher | 需要合适选择 |
| 重定向草图 | `Sketcher_ReorientSketch` | Sketcher | 可放二级 |
| 校验草图 | `Sketcher_ValidateSketch` | Sketcher | 推荐 |
| 镜像草图 | `Sketcher_MirrorSketch` | Sketcher | 推荐 |
| 合并草图 | `Sketcher_MergeSketches` | Sketcher | 推荐 |
| 矩形 | `Sketcher_CreateRectangle` | Sketcher | 进入 Sketch 后有效 |
| 圆 | `Sketcher_CreateCircle` | Sketcher | 进入 Sketch 后有效 |
| 线 | `Sketcher_CreateLine` | Sketcher | 进入 Sketch 后有效 |
| 约束组 | `Sketcher_CompConstrainTools` | Sketcher | group command |
| 尺寸组 | `Sketcher_CompDimensionTools` | Sketcher | group command |

### 7.3 建模 Tab

| 显示名 | 命令 | 来源 |
|---|---|---|
| Body | `PartDesign_Body` | PartDesign |
| 拉伸 | `PartDesign_Pad` | PartDesign |
| 切除 | `PartDesign_Pocket` | PartDesign |
| 孔 | `PartDesign_Hole` | PartDesign |
| 旋转 | `PartDesign_Revolution` | PartDesign |
| 旋转切除 | `PartDesign_Groove` | PartDesign |
| 圆角 | `PartDesign_Fillet` | PartDesign |
| 倒角 | `PartDesign_Chamfer` | PartDesign |
| 拔模 | `PartDesign_Draft` | PartDesign |
| 壳 | `PartDesign_Thickness` | PartDesign |
| 镜像 | `PartDesign_Mirrored` | PartDesign |
| 线性阵列 | `PartDesign_LinearPattern` | PartDesign |
| 圆周阵列 | `PartDesign_PolarPattern` | PartDesign |
| 多重变换 | `PartDesign_MultiTransform` | PartDesign |
| 布尔 | `PartDesign_Boolean` | PartDesign |

### 7.4 参考几何 Tab

| 显示名 | 命令 | 来源 |
|---|---|---|
| 基准面 | `PartDesign_Plane` | PartDesign |
| 基准轴 | `PartDesign_Line` | PartDesign |
| 基准点 | `PartDesign_Point` | PartDesign |
| 坐标系 | `PartDesign_CoordinateSystem` | PartDesign |
| 形状绑定 | `PartDesign_ShapeBinder` | PartDesign |
| 子形状绑定 | `PartDesign_SubShapeBinder` | PartDesign |
| McStudio 参考平面 | `McStudio_CreateReferencePlane` | McStudio wrapper |

### 7.5 装夹 Tab

第一版以 McStudio wrapper 为主，因为 FreeCAD 原生没有完整“装夹业务流程”工作台。

| 显示名 | 命令 | 第一版行为 |
|---|---|---|
| 工件 | `McStudio_CreateWorkpiece` | wrapper / 占位提示 |
| 毛坯 | `McStudio_CreateStock` | wrapper / 占位提示 |
| 夹具 | `McStudio_CreateFixture` | wrapper / 占位提示 |
| 定位 | `McStudio_SetFixtureLocation` | wrapper / 占位提示 |
| 工件坐标系 | `McStudio_SetWorkCoordinateSystem` | 可调用 `PartDesign_CoordinateSystem` |
| 装夹基准 | `McStudio_CreateSetupDatum` | 可调用 `PartDesign_Plane` |

### 7.6 加工 Tab

第一版 lazy CAM。

| 显示名 | 命令 | 来源 | 说明 |
|---|---|---|---|
| 创建 Job | `CAM_Job` | CAM | lazy load |
| 工艺检查 | `CAM_Sanity` | CAM | lazy load |
| 刀具库 | `CAM_ToolBitLibraryOpen` | CAM | lazy load |
| 新建刀具 | `CAM_ToolBitCreate` | CAM | lazy load |
| 检查路径 | `CAM_Inspect` | CAM | lazy load |
| 选择轮廓 | `CAM_SelectLoop` | CAM | lazy load |
| 2D 轮廓 | `CAM_Profile` | CAM | 需要 Job/选择 |
| 型腔 | `CAM_Pocket_Shape` | CAM | 需要 Job/选择 |
| 面铣 | `CAM_MillFacing` | CAM | 需要 Job/选择 |
| 钻孔 | `CAM_Drilling` | CAM | 需要 Job/选择 |
| 后处理 | `CAM_Post` | CAM | lazy load |

### 7.7 仿真 Tab

| 显示名 | 命令 | 来源 |
|---|---|---|
| 仿真 GL | `CAM_SimulatorGL` | CAM |
| 仿真 | `CAM_Simulator` | CAM |
| 干涉/工艺检查 | `CAM_Sanity` | CAM |
| Camotics | `CAM_Camotics` | CAM，可选，依赖外部程序 |

### 7.8 输出 Tab

| 显示名 | 命令 | 来源 |
|---|---|---|
| 导出模型 | `Std_Export` | Gui |
| G 代码 | `CAM_Post` | CAM |
| 选中后处理 | `CAM_PostSelected` | CAM |
| 工艺报告 | `McStudio_GenerateProcessReport` | McStudio wrapper |
| 设置单导出 | `McStudio_ExportSetupSheet` | McStudio wrapper |
| BOM | `McStudio_GenerateBOM` | McStudio wrapper |

## 8. Ribbon UI 布局

### 8.1 顶部结构

建议 `Ribbon.py` 使用：

```text
QDockWidget("McStudioRibbonDock")
└── QWidget root
    ├── Quick Access Row
    │   ├── McStudio logo/name
    │   ├── Save / Undo / Redo
    │   └── Help / Settings
    └── QTabWidget("McStudioRibbonTabs")
        ├── 项目
        ├── 草图
        ├── 建模
        ├── 参考几何
        ├── 装夹
        ├── 加工
        ├── 仿真
        └── 输出
```

`QDockWidget` 设置：

```python
dock.setObjectName("McStudioRibbonDock")
dock.setFeatures(QtGui.QDockWidget.NoDockWidgetFeatures)
dock.setTitleBarWidget(QtGui.QWidget())
mw.addDockWidget(QtCore.Qt.TopDockWidgetArea, dock)
```

### 8.2 Tab 页面布局

每个 Tab：

```text
QWidget page
└── QHBoxLayout
    ├── RibbonGroup
    ├── RibbonGroup
    ├── RibbonGroup
    └── stretch
```

每个 Group：

```text
QGroupBox(title)
└── QGridLayout 或 QHBoxLayout
    ├── large button
    ├── medium button
    └── small button
```

按钮使用：

```python
QtGui.QToolButton
setToolButtonStyle(QtCore.Qt.ToolButtonTextUnderIcon)
```

### 8.3 白色工业风 QSS

第一版用 `Resources/styles/McStudioRibbon.qss`：

```css
QWidget#McStudioRibbonRoot {
    background: #f7f9fb;
}

QTabWidget#McStudioRibbonTabs::pane {
    border-top: 1px solid #d8dde3;
    background: #ffffff;
}

QTabBar::tab {
    padding: 8px 18px;
    background: #f7f9fb;
    color: #1f2933;
}

QTabBar::tab:selected {
    background: #ffffff;
    border-bottom: 3px solid #2f80ed;
    font-weight: 600;
}

QGroupBox#McStudioRibbonGroup {
    border: 1px solid #d8dde3;
    border-radius: 6px;
    margin-top: 8px;
    padding: 8px;
    background: #ffffff;
}

QToolButton#McStudioRibbonButton {
    border: 1px solid transparent;
    border-radius: 4px;
    padding: 4px;
    background: transparent;
}

QToolButton#McStudioRibbonButton:hover {
    border-color: #b6d4ff;
    background: #eef6ff;
}
```

## 9. 冲突与处理

### 9.1 与其它工作台冲突

风险：离开 McStudio 后 Ribbon 残留。

处理：

```python
Deactivated() -> hide_ribbon()
mainWindowClosed -> destroy/remove ribbon
```

### 9.2 与 FreeCAD 原工具栏冲突

方案：第一版使用 `Gui::PythonBlankWorkbench`，减少原工具栏显示。Ribbon 不走 `ToolBarManager`，因此不干扰传统工具栏保存/恢复。

### 9.3 与菜单重建冲突

Ribbon 不插入 `QMenuBar`，而是 DockWidget，不受 `MenuManager::setup()` 每次清空菜单栏影响。

### 9.4 与 CAM 初始化冲突

CAM 初始化重且有外部依赖探测。处理：lazy load；失败不影响 McStudio 工作台。

### 9.5 与命令启用状态冲突

很多命令有 `IsActive()` 条件：没有文档、没有 Body、没有 Sketch、没有 CAM Job 时可能不可用。

第一版简化：按钮点击时直接 `FreeCADGui.runCommand()`，由 FreeCAD 命令自己决定是否执行。后续可加状态同步：

```text
按钮 enable/disable 与 CommandManager 状态同步
```

### 9.6 与图标资源冲突

第一版 wrapper 命令可以复用内置命令图标名；Ribbon 直接用按钮文字也可先跑通。后续再添加 McStudio 自己 SVG。

## 10. 任务拆分

### Task 1：建分支

```bash
git checkout mcstudio/dev
git pull
git checkout -b feature/mcstudio-ribbon-workbench
```

### Task 2：创建 McStudio 模块骨架

创建：

```text
src/Mod/McStudio/CMakeLists.txt
src/Mod/McStudio/Init.py
src/Mod/McStudio/InitGui.py
src/Mod/McStudio/Commands.py
src/Mod/McStudio/Ribbon.py
src/Mod/McStudio/Resources/styles/McStudioRibbon.qss
```

### Task 3：注册模块

修改：

```text
src/Mod/CMakeLists.txt
```

在 `#add_subdirectory(TemplatePyMod)` 后添加：

```cmake
add_subdirectory(McStudio)
```

### Task 4：实现 InitGui.py

职责：

- 注册 McStudioWorkbench。
- `Initialize()` 加载 PartDesign/Sketcher 基础模块。
- `Activated()` 调用 `Ribbon.show_ribbon()`。
- `Deactivated()` 调用 `Ribbon.hide_ribbon()`。
- `GetClassName()` 返回 `Gui::PythonBlankWorkbench`。

### Task 5：实现 Commands.py

先实现 wrapper/占位命令：

```text
McStudio_CreateReferencePlane
McStudio_CreateWorkpiece
McStudio_CreateStock
McStudio_CreateFixture
McStudio_SetWorkCoordinateSystem
McStudio_GenerateProcessReport
McStudio_ExportSetupSheet
McStudio_GenerateBOM
```

wrapper 命令内部可调用：

```python
FreeCADGui.runCommand("PartDesign_Plane")
FreeCADGui.runCommand("PartDesign_CoordinateSystem")
```

### Task 6：实现 Ribbon.py

职责：

- 建 `QDockWidget`。
- 建 `QTabWidget`。
- 建 group/button helper。
- 实现 `run_command()` 和 `run_cam_command()`。
- 实现 `show_ribbon()`、`hide_ribbon()`、`destroy_ribbon()`。

### Task 7：QSS 样式

实现白色工业风：

- 白底。
- 浅灰边框。
- 蓝色 active tab。
- 大按钮/中按钮。
- hover 轻蓝背景。

### Task 8：构建验证

```bash
pixi run cmake --build build/debug
```

检查输出目录：

```bash
find build/debug -path '*Mod/McStudio*' -type f
```

### Task 9：GUI 验证

```bash
./build/debug/bin/FreeCAD
```

验收：

1. 工作台列表出现 McStudio。
2. 切换 McStudio 后出现 Ribbon。
3. Ribbon Tab 包含：项目、草图、建模、参考几何、装夹、加工、仿真、输出。
4. 点击“新建/打开/保存”等基础命令正常。
5. 点击“Body/拉伸/切除/基准面”等命令能调用 FreeCAD 原生命令。
6. 切换到 PartDesign/Draft/CAM 后 Ribbon 隐藏。
7. 再切回 McStudio 不重复创建 Ribbon。
8. 控制台无 `Unknown command`。
9. CAM 加载失败时只提示，不导致 McStudio 工作台失败。

## 11. 风险

| 风险 | 影响 | 处理 |
|---|---|---|
| BlankWorkbench 过于干净，缺默认行为 | 高 | 退回 PythonWorkbench 或补齐项目/视图命令 |
| CAM 初始化重 | 中 | lazy load |
| Ribbon Dock 顶部位置和 Qt Dock 行为不完全像 SolidWorks | 中 | 第一版可接受，第二版 C++ RibbonWidget |
| 命令状态未同步 | 中 | 第一版由 FreeCAD 原命令处理，后续同步 QAction 状态 |
| 图标不统一 | 低 | 第一版先复用命令图标，后续自绘 McStudio SVG |
| 切换工作台残留 UI | 高 | 幂等 show/hide + destroy on mainWindowClosed |

## 12. 完成标准

计划完成不等于功能完成。第一版功能完成必须满足：

```text
ok: McStudio 工作台出现
ok: Ribbon 显示/隐藏正确
ok: 8 个 Tab 存在
ok: PartDesign/Sketcher 命令可调用
ok: CAM 命令 lazy load 不破坏工作台
ok: 切换其它工作台无残留
ok: 构建通过
ok: 控制台无 Unknown command
```

## 13. 后续演进

### 第二版

- Ribbon 按钮状态同步。
- McStudio 专属 SVG 图标。
- 装夹/工件/毛坯对象。
- TaskPanel 表单。
- CAM Job 创建向导。

### 第三版

- C++ RibbonWidget。
- McStudio C++ Gui 模块。
- 更深度的 MainWindow 统一品牌化。

### 第四版

只有在产品要求完全替换 FreeCAD 外壳时，才考虑改：

```text
src/Gui/MainWindow.cpp
src/Gui/ToolBarManager.cpp
src/Gui/Workbench.cpp
```

## 14. Implementation Checkpoint 2026-06-27

已完成最小骨架实现：

```text
src/Mod/McStudio/CMakeLists.txt
src/Mod/McStudio/Init.py
src/Mod/McStudio/InitGui.py
src/Mod/McStudio/Commands.py
src/Mod/McStudio/Ribbon.py
src/Mod/McStudio/Resources/styles/McStudioRibbon.qss
src/Mod/CMakeLists.txt
```

验证结果：

```text
python3 -m py_compile src/Mod/McStudio/*.py 通过
pixi run cmake -S . -B build/debug 通过
pixi run cmake --build build/debug --target McStudio 通过
build/debug/Mod/McStudio 文件复制成功
python aegis-method-pack/scripts/aegis-workspace.py check --root . 通过
```

尚未完成：

```text
GUI 手动验证：./build/debug/bin/FreeCAD
确认工作台列表出现 McStudio
确认切换 McStudio 后 Ribbon 显示
确认切换其它工作台后 Ribbon 隐藏
确认点击常用按钮无 Unknown command
```

## 15. Crash Debug Checkpoint 2026-06-27

用户反馈：点击 McStudio 工作台后 FreeCAD 秒退。

临时修复：

```text
1. InitGui.py: Activated() 中用 QTimer.singleShot(0, Ribbon.show_ribbon) 延迟创建 Ribbon。
2. InitGui.py: GetClassName() 从 Gui::PythonBlankWorkbench 暂退回 Gui::PythonWorkbench。
3. Ribbon.py: Ribbon 宿主从 QDockWidget 改为 QToolBar + addWidget(ribbon_widget)。
4. Ribbon.py: 移除 dock.raise_()。
```

原因判断：

```text
QDockWidget 顶部插入发生在工作台切换期间，可能与 FreeCAD 正在重建 dock/menu/toolbar 状态冲突；
BlankWorkbench 也可能导致默认 UI 状态过空，先退回 PythonWorkbench 保障兼容。
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/*.py 通过
pixi run cmake --build build/debug --target McStudio 通过
```

下一步：重新启动 FreeCAD，点击 McStudio 工作台验证是否仍崩溃。

## 16. Crash Root-Cause Analysis 2026-06-27

用户提供的旧版崩溃栈显示：

```text
SIGSEGV
Gui::MenuItem::findItem(...)
StartGui::Manipulator::modifyMenuBar(Gui::MenuItem*)
Gui::WorkbenchManipulator::changeMenuBar(Gui::MenuItem*)
Gui::Workbench::activate()
Gui::WorkbenchManager::activate(...)
Gui::Application::activateWorkbench(...)
```

结论：崩溃发生在 Workbench 激活期间的菜单重建阶段，还未进入 McStudio 的 `Activated()`，因此不是 `Ribbon.show_ribbon()` 本身直接触发。

根因判断：

```text
McStudio 初版使用 Gui::PythonBlankWorkbench。
PythonBlankWorkbench 的菜单树为空。
Workbench::activate() 会调用 WorkbenchManipulator::changeMenuBar(mb)。
StartGui::Manipulator::modifyMenuBar() 无空值保护地执行 menuBar->findItem("&Help")，并继续 helpMenu->findItem(...）。
当 BlankWorkbench 没有 &Help 菜单时，Start manipulator 对空指针继续操作，导致段错误。
```

当前修复方向正确：

```text
GetClassName() 改回 Gui::PythonWorkbench。
保留标准菜单树，确保 &Help 存在。
Ribbon 继续由 McStudio 自己 show/hide。
```

后续如果仍想回到 BlankWorkbench，需要二选一：

```text
1. 在 McStudio BlankWorkbench 中手动补齐标准菜单，至少补 &Help / &Windows 等 Start manipulator 依赖项；或
2. 修改 StartGui::Manipulator::modifyMenuBar()，给 helpMenu / firstItem 增加空值保护。
```

第一版建议不改 FreeCAD 核心，继续使用 `Gui::PythonWorkbench`。

## 17. BlankWorkbench Menu Anchor Implementation 2026-06-27

用户决定采用方案 A：McStudio 继续使用 `Gui::PythonBlankWorkbench`，但由 McStudio 自己补齐全局 manipulator 依赖的标准菜单锚点。

已实现：

```text
InitGui.py::_setup_minimum_standard_menus()
```

该函数在 `Initialize()` 最开始执行，创建：

```text
&File
&Edit
&View
&Tools
&Macro
&Windows
&Help
```

关键兼容点：

```text
&Help 菜单包含 Std_FreeCADUserHub
```

原因：`StartGui::Manipulator::modifyMenuBar()` 会查找 `&Help`，并用 `Std_FreeCADUserHub` 作为插入 `Start_Start` 的锚点。

同时 `GetClassName()` 已切回：

```python
return "Gui::PythonBlankWorkbench"
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/*.py 通过
pixi run cmake --build build/debug --target McStudio 通过
```

GUI 仍需手动验证：

```text
启动 FreeCAD -> 切换 McStudio -> 不崩溃 -> 顶部 Ribbon 出现 -> Help 菜单中 Start Page 可被插入
```
