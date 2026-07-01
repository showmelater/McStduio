# McStudio C++ Ribbon Workbench 调研规划

日期：2026-06-27  
状态：调研完成，待决策  
ArchitectureReviewRequired: yes

## 1. 问题

当前 McStudio 第一版是：

```text
Python InitGui.py
Python Commands.py
Python Ribbon.py
Gui::PythonBlankWorkbench + 手动补菜单锚点
```

用户询问：如果写成 C++ 版本，应该怎么做。

## 2. 结论

C++ 版本可行，但不建议直接把当前 Python 原型全部一次性重写成 C++。

推荐分两步：

```text
Stage C1: C++ Workbench
  C++ 接管菜单树、工具栏树、工作台类型注册。
  Ribbon 暂时仍可用 Python 或简化 QToolBar。

Stage C2: C++ RibbonWidget
  C++ 实现 QToolBar + QTabWidget + QGroupBox + QToolButton。
  activated()/deactivated() 负责显示/隐藏 Ribbon。
```

最终结构：

```text
src/Mod/McStudio/
├── CMakeLists.txt
├── Init.py
├── InitGui.py
├── Commands.py                 # 可逐步废弃或只保留 Python wrapper
├── Resources/
│   ├── icons/
│   └── styles/
└── Gui/
    ├── CMakeLists.txt
    ├── AppMcStudioGui.cpp
    ├── Workbench.h
    ├── Workbench.cpp
    ├── RibbonWidget.h
    ├── RibbonWidget.cpp
    ├── Command.cpp             # C++ wrapper commands，可后续加入
    └── PreCompiled.h
```

## 3. 证据

### 3.1 C++ 工作台注册模式

PartDesign 模式：

```python
# src/Mod/PartDesign/InitGui.py
class PartDesignWorkbench(Workbench):
    def Initialize(self):
        import PartDesignGui
        import PartDesign

    def GetClassName(self):
        return "PartDesignGui::Workbench"

Gui.addWorkbench(PartDesignWorkbench())
```

C++ 模块入口：

```cpp
// src/Mod/PartDesign/Gui/AppPartDesignGui.cpp
PyMOD_INIT_FUNC(PartDesignGui)
{
    ...
    CreatePartDesignCommands();
    PartDesignGui::Workbench::init();
    ...
}
```

C++ 工作台：

```cpp
TYPESYSTEM_SOURCE(PartDesignGui::Workbench, Gui::StdWorkbench)
Gui::MenuItem* Workbench::setupMenuBar() const
Gui::ToolBarItem* Workbench::setupToolBars() const
```

结论：McStudio C++ 版仍保留 `InitGui.py`，但 `GetClassName()` 返回：

```python
return "McStudioGui::Workbench"
```

### 3.2 最小 C++ 工作台参考

Inspection 是较小样板：

```text
src/Mod/Inspection/InitGui.py
src/Mod/Inspection/Gui/AppInspectionGui.cpp
src/Mod/Inspection/Gui/Workbench.h
src/Mod/Inspection/Gui/Workbench.cpp
src/Mod/Inspection/Gui/CMakeLists.txt
```

其 `Workbench.cpp` 很小：

```cpp
TYPESYSTEM_SOURCE(InspectionGui::Workbench, Gui::StdWorkbench)

Gui::MenuItem* Workbench::setupMenuBar() const
{
    Gui::MenuItem* root = StdWorkbench::setupMenuBar();
    Gui::MenuItem* item = root->findItem("&Windows");
    Gui::MenuItem* insp = new Gui::MenuItem;
    root->insertItem(item, insp);
    insp->setCommand("Inspection");
    *insp << "Inspection_VisualInspection"
          << "Inspection_InspectElement";
    return root;
}
```

McStudio 可以参考 Inspection，而不是直接复制 PartDesign 的完整体量。

### 3.3 C++ 命令调用

Ribbon 按钮可以调用：

```cpp
Gui::Application::Instance->commandManager().runCommandByName("PartDesign_Pad");
```

该接口在：

```text
src/Gui/Command.h
src/Gui/Command.cpp
```

已有调用示例：

```text
src/Gui/MainWindow.cpp
src/Mod/Assembly/Gui/TaskAssemblyMessages.cpp
```

## 4. C++ 版两种路线

### 4.1 路线 A：C++ Workbench + Python Ribbon 混合过渡

改动：

```text
新增 src/Mod/McStudio/Gui/AppMcStudioGui.cpp
新增 src/Mod/McStudio/Gui/Workbench.h/cpp
新增 src/Mod/McStudio/Gui/CMakeLists.txt
InitGui.py 返回 McStudioGui::Workbench
Ribbon.py 暂时保留
```

C++ Workbench 负责：

```text
setupMenuBar()
setupToolBars()
activated()
deactivated()
```

Python Ribbon 负责：

```text
QToolBar + QTabWidget UI
```

优点：

```text
风险低
保留现有可运行 Python UI
先验证 C++ 工作台注册链路
```

缺点：

```text
仍有 Python Ribbon
不是最终纯 C++
```

适合下一步。

### 4.2 路线 B：C++ Workbench + C++ RibbonWidget

改动：

```text
新增 RibbonWidget.h/cpp
Workbench::activated() 中创建/显示 RibbonWidget
Workbench::deactivated() 中隐藏 RibbonWidget
```

C++ RibbonWidget 结构：

```text
QToolBar("McStudio Ribbon")
└── QWidget action widget
    ├── quick access row
    └── QTabWidget
        ├── 项目
        ├── 草图
        ├── 建模
        ├── 参考几何
        ├── 装夹
        ├── 加工
        ├── 仿真
        └── 输出
```

按钮：

```cpp
auto* button = new QToolButton(parent);
button->setText(QStringLiteral("拉伸"));
button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
connect(button, &QToolButton::clicked, this, [] {
    Gui::Application::Instance->commandManager().runCommandByName("PartDesign_Pad");
});
```

优点：

```text
更稳定
更原生
后续可做状态同步、图标、QSS、性能优化
```

缺点：

```text
CMake 更复杂
编译成本更高
每次调整 UI 需要编译
```

适合第二步。

### 4.3 路线 C：改 FreeCAD 核心 ToolBarManager/MainWindow

不推荐第一阶段。

改动：

```text
src/Gui/MainWindow.cpp
src/Gui/ToolBarManager.cpp
src/Gui/Workbench.cpp
```

风险：

```text
影响所有工作台
上游合并成本高
破坏用户自定义 toolbar
容易引入全局 GUI 回归
```

## 5. C++ Workbench 继承选择

### 方案 1：继承 Gui::StdWorkbench

```cpp
class Workbench: public Gui::StdWorkbench
```

优点：

```text
自动有标准菜单 &Help / &Windows
不会触发 Start manipulator 空菜单崩溃
符合 PartDesign/Inspection 模式
```

缺点：

```text
默认菜单/工具栏较多，不够 blank
```

### 方案 2：继承 Gui::Workbench，自己补菜单

```cpp
class Workbench: public Gui::Workbench
```

需要自己实现：

```cpp
setupMenuBar()
setupToolBars()
```

必须补：

```text
&Help -> Std_FreeCADUserHub
&Windows
```

优点：

```text
最接近 BlankWorkbench
UI 最干净
```

缺点：

```text
容易漏全局 manipulator 依赖
要维护标准菜单锚点
```

### 推荐

第一版 C++ Workbench 先继承：

```cpp
Gui::StdWorkbench
```

等稳定后再改成：

```cpp
Gui::Workbench + 自建菜单锚点
```

## 6. C++ 模块加载依赖

`AppMcStudioGui.cpp` 中应在 init 时确保基础模块：

```cpp
Base::Interpreter().runString("import PartGui");
Base::Interpreter().runString("import PartDesignGui");
Base::Interpreter().runString("import SketcherGui");
```

CAM 仍然建议 lazy load，不在 C++ GUI 模块初始化时强制加载。

C++ Ribbon 中 CAM 按钮可先调用 Python：

```cpp
Gui::Command::doCommand(Gui::Command::Gui, "import Path, PathGui, PathCommands");
Gui::Command::doCommand(Gui::Command::Gui, "import Path.GuiInit; Path.GuiInit.Startup()");
Gui::Application::Instance->commandManager().runCommandByName("CAM_Job");
```

后续再封装 C++ `ensureCamLoaded()`。

## 7. CMake 最小结构

`src/Mod/McStudio/CMakeLists.txt`：

```cmake
if(BUILD_GUI)
    add_subdirectory(Gui)
endif()

set(McStudio_Scripts
    Init.py
    InitGui.py
    Commands.py
    Ribbon.py
)

fc_copy_sources(McStudio "${CMAKE_BINARY_DIR}/Mod/McStudio" ${McStudio_Scripts})
install(FILES ${McStudio_Scripts} DESTINATION Mod/McStudio)
```

`src/Mod/McStudio/Gui/CMakeLists.txt`：

```cmake
set(McStudioGui_LIBS
    FreeCADGui
)

set(McStudioGui_SRCS
    AppMcStudioGui.cpp
    Workbench.cpp
    Workbench.h
    RibbonWidget.cpp
    RibbonWidget.h
    PreCompiled.h
)

add_library(McStudioGui SHARED ${McStudioGui_SRCS})
target_include_directories(McStudioGui PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(McStudioGui ${McStudioGui_LIBS})

SET_BIN_DIR(McStudioGui McStudioGui /Mod/McStudio)
SET_PYTHON_PREFIX_SUFFIX(McStudioGui)
install(TARGETS McStudioGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
```

## 8. 需要新增的 Export 宏

建议新增：

```text
src/Mod/McStudio/McStudioGlobal.h
```

内容参考 `InspectionGlobal.h`：

```cpp
#pragma once

#include <FCGlobal.h>

#ifndef McStudioGuiExport
#  ifdef McStudioGui_EXPORTS
#    define McStudioGuiExport FREECAD_DECL_EXPORT
#  else
#    define McStudioGuiExport FREECAD_DECL_IMPORT
#  endif
#endif
```

然后：

```cpp
class McStudioGuiExport Workbench: public Gui::StdWorkbench
```

## 9. 风险点

| 风险 | 说明 | 处理 |
|---|---|---|
| C++ 模块导入失败 | `import McStudioGui` 找不到 so | CMake `SET_BIN_DIR` / `SET_PYTHON_PREFIX_SUFFIX` 必须正确 |
| 命令未注册 | PartDesign/Sketcher 命令没加载 | `InitGui.py` 或 `AppMcStudioGui.cpp` import 对应 Gui 模块 |
| Blank 菜单崩溃 | 旧问题来自 `&Help` 缺失 | C++ 初版继承 StdWorkbench，先避开 |
| CAM 重依赖 | CAM 初始化复杂 | lazy load |
| UI 编译迭代慢 | C++ UI 每次要编译 | 先保留 Python Ribbon 或只迁移核心 Widget |
| 上游合并 | 新增 Mod/McStudio 风险低 | 不改 src/Gui 核心 |

## 10. 推荐下一步

### 最小 C++ 验证切片

不直接做完整 C++ Ribbon，先做：

```text
1. 新增 McStudioGui C++ 模块
2. 新增 McStudioGui::Workbench : Gui::StdWorkbench
3. setupMenuBar() 添加 &McStudio 菜单
4. setupToolBars() 添加 McStudio 工具栏
5. InitGui.py import McStudioGui，并返回 McStudioGui::Workbench
6. 先不迁移 RibbonWidget
```

验证：

```text
pixi run cmake -S . -B build/debug
pixi run cmake --build build/debug --target McStudioGui
启动 FreeCAD
工作台列表出现 McStudio
切换不崩溃
&McStudio 菜单出现
```

通过后再做：

```text
7. C++ RibbonWidget
8. C++ Workbench::activated/deactivated 管理 Ribbon
9. 删除 Python Ribbon.py 或保留为 fallback
```

## 11. Verdict

C++ 版值得做，但应分阶段。

推荐路线：

```text
当前 Python Ribbon 原型保留
下一步实现 McStudioGui::Workbench C++ 最小模块
再实现 C++ RibbonWidget
最后再考虑是否完全替代 Python Commands.py
```


## 12. Implementation Checkpoint 2026-06-27

已完成第一阶段 C++ 化：McStudio 工作台注册、菜单/工具栏树、激活/离开生命周期迁移到 C++。

新增/修改：

```text
src/Mod/McStudio/McStudioGlobal.h
src/Mod/McStudio/Gui/CMakeLists.txt
src/Mod/McStudio/Gui/AppMcStudioGui.cpp
src/Mod/McStudio/Gui/PreCompiled.h
src/Mod/McStudio/Gui/Workbench.h
src/Mod/McStudio/Gui/Workbench.cpp
src/Mod/McStudio/CMakeLists.txt
src/Mod/McStudio/InitGui.py
```

当前架构：

```text
InitGui.py
  import McStudioGui
  import Commands
  GetClassName() -> McStudioGui::Workbench

McStudioGui::Workbench : Gui::BlankWorkbench
  setupMenuBar()     C++ 创建 File/Edit/View/Tools/Macro/McStudio/Windows/Help
  setupToolBars()    C++ 创建 McStudio 项目/参考几何/建模工具栏
  activated()        C++ 调用 Python Ribbon.show_ribbon()
  deactivated()      C++ 调用 Python Ribbon.hide_ribbon()
```

兼容策略：

```text
继续继承 Gui::BlankWorkbench，但 C++ setupMenuBar() 手动提供 &Help、&Windows 等菜单锚点。
&Help 中保留 Std_FreeCADUserHub，避免 StartGui::Manipulator 崩溃。
Ribbon.py 暂时保留为过渡 UI，下一阶段再迁移为 C++ RibbonWidget。
Python Commands.py 暂时保留，给 McStudio wrapper 命令使用。
```

验证结果：

```text
python3 -m py_compile src/Mod/McStudio/*.py 通过
pixi run cmake -S . -B build/debug 通过
ninja McStudioGui 编译通过，生成 build/debug/Mod/McStudio/McStudioGui.so
build/debug/Mod/McStudio/InitGui.py 已复制并返回 McStudioGui::Workbench
```

构建备注：

```text
构建过程中出现 “Not a valid object name upstream/main”，来自 FreeCAD Version.h 生成脚本读取 git 上游引用，不是 McStudioGui 编译错误。
新增 C++ 模块后 Ninja 首次会触发较多基础库重编译。
```

GUI 待验证：

```text
启动 ./build/debug/bin/FreeCAD
切换 McStudio
确认工作台由 McStudioGui::Workbench 创建
确认 Ribbon 显示/隐藏
确认 Help/Windows/McStudio 菜单存在
确认不再出现 PythonBlankWorkbench 菜单崩溃
```


## 13. Full C++ Workbench / Native FreeCAD UI Checkpoint 2026-06-27

用户要求：先依照 FreeCAD 的写法，完全变成 C++ 工作台，UI 样式先和 FreeCAD 一样。

已完成：

```text
1. McStudioGui::Workbench 改为继承 Gui::StdWorkbench。
2. setupMenuBar() 先调用 StdWorkbench::setupMenuBar()，再插入 &McStudio 菜单。
3. setupToolBars() 先调用 StdWorkbench::setupToolBars()，再追加 McStudio 原生 QToolBar 工具栏。
4. 删除运行时 Python Ribbon 依赖，不再创建自定义 QToolBar/QTabWidget/QSS Ribbon。
5. 删除运行时 Python Commands 依赖，McStudio wrapper 命令迁移到 C++ Command.cpp。
6. InitGui.py 只作为 FreeCAD 模块入口，GetClassName() 返回 McStudioGui::Workbench。
```

当前运行时文件：

```text
build/debug/Mod/McStudio/Init.py
build/debug/Mod/McStudio/InitGui.py
build/debug/Mod/McStudio/McStudioGui.so
```

当前源码结构：

```text
src/Mod/McStudio/CMakeLists.txt
src/Mod/McStudio/Init.py
src/Mod/McStudio/InitGui.py
src/Mod/McStudio/McStudioGlobal.h
src/Mod/McStudio/Gui/CMakeLists.txt
src/Mod/McStudio/Gui/AppMcStudioGui.cpp
src/Mod/McStudio/Gui/Command.cpp
src/Mod/McStudio/Gui/PreCompiled.h
src/Mod/McStudio/Gui/Workbench.h
src/Mod/McStudio/Gui/Workbench.cpp
```

C++ 命令已注册：

```text
McStudio_CreateReferencePlane
McStudio_CreateWorkpiece
McStudio_CreateStock
McStudio_CreateFixture
McStudio_SetFixtureLocation
McStudio_SetWorkCoordinateSystem
McStudio_CreateSetupDatum
McStudio_GenerateProcessReport
McStudio_ExportSetupSheet
McStudio_GenerateBOM
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
pixi run cmake -S . -B build/debug 通过
ninja McStudioGui 通过
ninja McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 生成成功
build/debug/Mod/McStudio 无 Commands.py、Ribbon.py、Resources/styles 运行时残留
```

GUI 待验证：

```text
启动 ./build/debug/bin/FreeCAD
切换 McStudio
确认样式为 FreeCAD 原生菜单/工具栏样式
确认无 Ribbon 样式面板
确认 McStudio 菜单位于 Windows 前
确认 McStudio 工具栏出现在 FreeCAD 原生工具栏区域
确认 McStudio_CreateReferencePlane 等命令可执行
```


## 14. Unknown Command Fix 2026-06-27

用户反馈切换 McStudio 时出现：

```text
CommandManager::addTo() try to add an unknown command (McStudio_CreateReferencePlane) to a widget!
CommandManager::addTo() try to add an unknown command (McStudio_CreateWorkpiece) to a widget!
...
```

根因：

```text
McStudioGui::Workbench::setupMenuBar() / setupToolBars() 已经执行，说明 C++ 工作台加载成功。
但 McStudio C++ wrapper 命令没有提前注册到 CommandManager。
AppMcStudioGui.cpp 只有 void CreateMcStudioCommands(); 声明，没有实际调用。
```

修复：

```cpp
CreateMcStudioCommands();
McStudioGui::Workbench::init();
```

调用位置：

```text
src/Mod/McStudio/Gui/AppMcStudioGui.cpp
```

验证：

```text
ninja McStudioGui 通过
build/debug/Mod/McStudio/McStudioGui.so 重新链接，时间戳更新到 14:26
strings McStudioGui.so 可看到 McStudio_CreateReferencePlane / McStudio_CreateWorkpiece 等命令字符串
运行时目录只剩 Init.py / InitGui.py / McStudioGui.so
```


## 15. Workbench Loading Flow Comparison 2026-06-27

对比对象：Inspection、PartDesign、Sketcher、Start、Measure、McStudio。

官方 C++ Workbench 加载流程：

```text
src/Mod/<Module>/InitGui.py
  class <Module>Workbench(Workbench)
  Initialize(): import <Module>Gui
  GetClassName(): return "<Module>Gui::Workbench"
  Gui.addWorkbench(...)

src/Mod/<Module>/Gui/App<Module>Gui.cpp
  PyMOD_INIT_FUNC(<Module>Gui)
  检查 Gui::Application::Instance
  import/load 依赖模块
  initModule()
  Create<Module>Commands()
  <Module>Gui::Workbench::init()
  ViewProvider / Resource / PrefPage init

src/Mod/<Module>/Gui/Workbench.cpp
  TYPESYSTEM_SOURCE(<Module>Gui::Workbench, Gui::StdWorkbench)
  setupMenuBar(): StdWorkbench::setupMenuBar() + 插入模块菜单
  setupToolBars(): StdWorkbench::setupToolBars() + 添加模块工具栏
```

McStudio 当前对齐情况：

```text
InitGui.py 返回 McStudioGui::Workbench，符合 Inspection / PartDesign 模式。
AppMcStudioGui.cpp 已调用 CreateMcStudioCommands()，再调用 McStudioGui::Workbench::init()。
Workbench 继承 Gui::StdWorkbench。
setupMenuBar() 先调用 StdWorkbench::setupMenuBar()，再插入 &McStudio。
setupToolBars() 先调用 StdWorkbench::setupToolBars()，再追加 McStudio 原生工具栏。
运行时不再依赖 Python Commands.py / Ribbon.py。
```

发现并已修复的问题：

```text
McStudio 覆盖 activated()/deactivated() 但没有调用基类。
Gui::Workbench::activated() 内部会执行 commandManager().signalPyCmdInitialized()。
PartDesign 的 activated() 明确调用 Gui::Workbench::activated()。
```

已修复为：

```cpp
void Workbench::activated()
{
    Gui::Workbench::activated();
    Base::Console().log("McStudioGui::Workbench activated.\n");
}

void Workbench::deactivated()
{
    Base::Console().log("McStudioGui::Workbench deactivated.\n");
    Gui::Workbench::deactivated();
}
```

命令验证：

```text
McStudio_CreateReferencePlane            OK
McStudio_CreateWorkpiece                 OK
McStudio_CreateStock                     OK
McStudio_CreateFixture                   OK
McStudio_SetFixtureLocation              OK
McStudio_SetWorkCoordinateSystem         OK
McStudio_CreateSetupDatum                OK
McStudio_GenerateProcessReport           OK
McStudio_ExportSetupSheet                OK
McStudio_GenerateBOM                     OK
```

依赖命令验证：

```text
PartDesign_NewSketch                FOUND
Sketcher_EditSketch                 FOUND
Sketcher_MapSketch                  FOUND
Sketcher_ValidateSketch             FOUND
PartDesign_Body                     FOUND
PartDesign_Pad                      FOUND
PartDesign_Pocket                   FOUND
PartDesign_Hole                     FOUND
PartDesign_Fillet                   FOUND
PartDesign_Chamfer                  FOUND
PartDesign_Plane                    FOUND
PartDesign_Line                     FOUND
PartDesign_Point                    FOUND
PartDesign_CoordinateSystem         FOUND
Std_New / Std_Open / Std_Save / Std_Import / Std_Export FOUND
```

剩余非阻塞差异：

```text
1. McStudio InitGui.py 目前用 FreeCADGui.addWorkbench，而官方多用 Gui.addWorkbench；两者通常可用，但后续可改成 Gui.addWorkbench 以完全贴近官方风格。
2. McStudio 还没有 Resources/icons/McStudioWorkbench.svg，工作台图标为空；不影响加载，但 UI 视觉不完整。
3. CMake 还没有安装 McStudioGlobal.h；目前仅编译使用，不影响运行。若后续其它模块 include McStudioGlobal.h，可补 install/header 分组。
4. 装夹/输出类命令目前是 placeholder，不是加载流程问题。
5. McStudio_NewSketch 尚未实现，因此 PartDesign_NewSketch 仍会按原生逻辑跳转 SketcherWorkbench。
```

验证：

```text
ninja McStudioGui 通过
build/debug/Mod/McStudio/McStudioGui.so 时间戳更新到 14:31
运行时目录仍只有 Init.py / InitGui.py / McStudioGui.so
```


## 16. Native C++ Visibility Slice 2026-06-27

用户反馈：McStudio 切换后日志显示 setupToolBars/setupMenuBar/activated 已执行，但 UI 仍不可见。

对比官方 C++ 工作台：

```text
Inspection:
  InitGui.py 使用 Gui.addWorkbench
  Initialize() 只 import InspectionGui
  setupMenuBar() 只插入一个 ASCII 顶层菜单 Inspection
  setupToolBars() 不新增工具栏

MeshPart:
  InitGui.py 使用 Gui.addWorkbench
  setupToolBars() 新增一个 ASCII 工具栏 MeshPart

PartDesign / Sketcher:
  setupMenuBar() 先 StdWorkbench::setupMenuBar()
  setupToolBars() 先 StdWorkbench::setupToolBars()
  菜单/工具栏内部 command 名均为英文/ASCII
```

McStudio 调整为官方最小可见切片：

```text
InitGui.py:
  删除 FreeCAD/FreeCADGui 显式 import
  Initialize() 只 import McStudioGui
  使用 Gui.addWorkbench(McStudioWorkbench())

Workbench.cpp:
  顶层菜单名改为 McStudio
  工具栏名改为 McStudio
  去掉中文菜单分组和多个中文工具栏
  只保留一个 ASCII 顶层菜单 + 一个 ASCII 工具栏
```

目的：排除中文对象名、多个工具栏、用户布局状态、FreeCAD 翻译上下文等因素，先证明 McStudio 原生 UI 能被 Qt 创建并显示。

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 14:46
build/debug/Mod/McStudio/InitGui.py 已更新为 Gui.addWorkbench 风格
```

下一步测试：

```text
用 --user-cfg /tmp/mcstudio-user.cfg --log-file /tmp/mcstudio.log 启动
切换 McStudio
确认菜单栏是否有 McStudio
右键工具栏区域确认是否有 McStudio 工具栏勾选项
用 Python Console 打印 menuBar().actions() 和 QToolBar 列表
```


## 17. Toolbar Group Regression Fix 2026-06-27

用户反馈：点击草图后，草图图标也没有加进来，并怀疑之前排查时改乱了布局。

确认原因：

```text
第 16 步为了排查 McStudio UI 不显示，把 Workbench.cpp 退成了“官方最小可见切片”。
该切片只保留一个顶层 McStudio 菜单和一个 McStudio 工具栏。
因此原先的 McStudio 草图 / 建模 / 参考几何 / 装夹 独立工具栏分组被临时移除了。
这会直接导致草图图标不按独立分组显示。
```

修复：

```text
恢复多个 FreeCAD 原生工具栏分组，但使用英文/ASCII toolbar object name，避免中文对象名和用户布局参数的不确定性。
```

当前工具栏：

```text
McStudio Project
McStudio Sketch
McStudio Modeling
McStudio Reference
McStudio Fixture
```

其中 McStudio Sketch 包含：

```text
PartDesign_NewSketch
Sketcher_EditSketch
Sketcher_MapSketch
Sketcher_ValidateSketch
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 14:54
```


## 18. Startup Flow Audit / InitGui Registration Restore 2026-06-27

用户反馈：McStudio 工作台本身不显示，要求重新审核启动流程。

启动流程源码确认：

```text
FreeCADGuiInit.py 执行 Mod/McStudio/InitGui.py
  -> InitGui.py 调用 Gui/FreeCADGui.addWorkbench(handler)
  -> handler 被加入 Application::_pcWorkbenchDictionary
  -> 点击工作台时 Application::activateWorkbench(name)
  -> 调用 handler.GetClassName()
  -> 调用 handler.Initialize()
  -> Initialize() import McStudioGui
  -> PyMOD_INIT_FUNC(McStudioGui)
  -> CreateMcStudioCommands()
  -> McStudioGui::Workbench::init()
  -> WorkbenchManager::activate(name, "McStudioGui::Workbench")
  -> Workbench::setupToolBars/setupMenuBar
```

确认的风险：

```text
第 16 步为了贴近官方最小写法，把 InitGui.py 改成了只使用全局 Gui.addWorkbench。
官方文件中 Gui/FreeCAD/Workbench 由 FreeCADGuiInit.py 全局注入，理论可用；但为了减少 McStudio 调试变量，恢复为显式 import FreeCAD / import FreeCADGui，并使用 FreeCADGui.addWorkbench。
```

已恢复：

```python
import FreeCAD
import FreeCADGui

class McStudioWorkbench(Workbench):
    MenuText = "McStudio"
    ToolTip = "McStudio integrated CAD/CAM workbench"
    Icon = ""

    def Initialize(self):
        FreeCAD.Console.PrintMessage("McStudio InitGui.Initialize: importing McStudioGui
")
        import McStudioGui
        FreeCAD.Console.PrintMessage("McStudio InitGui.Initialize: McStudioGui imported
")

    def GetClassName(self):
        return "McStudioGui::Workbench"

FreeCAD.Console.PrintMessage("McStudio InitGui: registering McStudioWorkbench
")
FreeCADGui.addWorkbench(McStudioWorkbench())
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudio 通过
build/debug/Mod/McStudio/InitGui.py 已同步
```

测试判断：

```text
启动日志应在模块初始化阶段出现：
McStudio InitGui: registering McStudioWorkbench

点击/切换 McStudio 时应出现：
McStudio InitGui.Initialize: importing McStudioGui
Loading GUI of McStudio module... done
McStudio InitGui.Initialize: McStudioGui imported
McStudioGui::Workbench setupToolBars.
McStudioGui::Workbench setupMenuBar.
McStudioGui::Workbench activated.
```


## 19. C++ Ribbon Host Implementation 2026-06-27

背景：用户确认 Python 版本已经初步实现 Ribbon，只是 C++ 版本没有显示出来。因此不再把 McStudio UI 继续限制为 FreeCAD 原生多工具栏，而是将 Python Ribbon 思路迁到 C++。

实现策略：

```text
不改 src/Gui/ToolBarManager.cpp
不改 src/Gui/MainWindow.cpp
在 McStudioGui::Workbench 激活时创建 McStudio 自己的顶部 Ribbon 宿主
外层：QToolBar objectName = McStudioRibbonToolBar
内部：QTabWidget objectName = McStudioRibbonTabs
每个页签内使用 QFrame + QToolBar + QLabel 组织命令组
命令按钮仍通过 Gui::Application::Instance->commandManager().addTo(command, toolbar) 创建
```

Ribbon 页签：

```text
Project
Sketch
Modeling
Reference / Fixture
Output
```

命令来源：

```text
Project: Std_New / Std_Open / Std_Save / Std_Import / Std_Export
Sketch: PartDesign_NewSketch / Sketcher_EditSketch / Sketcher_MapSketch / Sketcher_ValidateSketch
Modeling: PartDesign_Body / PartDesign_Pad / PartDesign_Pocket / PartDesign_Hole / PartDesign_Fillet / PartDesign_Chamfer
Reference: McStudio_CreateReferencePlane / PartDesign_Plane / PartDesign_Line / PartDesign_Point / PartDesign_CoordinateSystem
Fixture: McStudio_CreateWorkpiece / McStudio_CreateStock / McStudio_CreateFixture / McStudio_SetFixtureLocation / McStudio_SetWorkCoordinateSystem / McStudio_CreateSetupDatum
Output: McStudio_GenerateProcessReport / McStudio_ExportSetupSheet / McStudio_GenerateBOM
```

生命周期：

```text
Workbench::activated() -> showRibbon()
Workbench::deactivated() -> hideRibbon()
setupToolBars() 只返回 StdWorkbench::setupToolBars()，避免原生 McStudio 多工具栏和 Ribbon 重复显示
setupMenuBar() 保留 McStudio 菜单
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 15:21
McStudioGui.so 由约 1.6M 增至约 1.9M
```


## 20. Ribbon Architecture Split / Bottom Tabs 2026-06-27

用户反馈截图：C++ Ribbon 已初步显示，但样式简陋、命令少；要求：

```text
1. 上面的分类页签放到底部
2. 第一个分类改为“特征”
3. 第二个分类改为“草图”
4. 草图第一个命令为“退出草图”
5. 按推荐架构拆分 Ribbon 代码
```

调研确认：

```text
Sketcher 退出草图命令名为 Sketcher_LeaveSketch。
官方 Sketcher Workbench.cpp 也将 Sketcher_LeaveSketch 放入 sketch 工具栏。
Command.cpp 中 CmdSketcherLeaveSketch 注册为 Command("Sketcher_LeaveSketch")。
```

已新增文件：

```text
src/Mod/McStudio/Gui/RibbonModel.h
src/Mod/McStudio/Gui/RibbonModel.cpp
src/Mod/McStudio/Gui/RibbonWidget.h
src/Mod/McStudio/Gui/RibbonWidget.cpp
```

职责拆分：

```text
RibbonModel:
  定义 RibbonTabDefinition / RibbonGroupDefinition
  管理页签、分组、命令列表

RibbonWidget:
  创建 McStudioRibbonToolBar
  创建 McStudioRibbonTabs
  渲染 QTabWidget / QFrame / QToolBar / QLabel
  通过 CommandManager::addTo() 加载命令按钮
  show()/hide() 管理工作台激活生命周期

Workbench:
  只保留 activated/deactivated、setupMenuBar、setupToolBars
  activated() -> RibbonWidget::show()
  deactivated() -> RibbonWidget::hide()
```

当前 Ribbon 页签顺序：

```text
特征
草图
参考 / 装夹
输出
```

页签位置：

```cpp
tabs->setTabPosition(QTabWidget::South);
```

草图页第一个命令：

```text
Sketcher_LeaveSketch
```

特征页扩充命令：

```text
PartDesign_Body
PartDesign_Pad
PartDesign_Pocket
PartDesign_Hole
PartDesign_Fillet
PartDesign_Chamfer
PartDesign_Revolution
PartDesign_Groove
PartDesign_AdditiveLoft
PartDesign_SubtractiveLoft
PartDesign_AdditivePipe
PartDesign_SubtractivePipe
PartDesign_Mirrored
PartDesign_LinearPattern
PartDesign_PolarPattern
PartDesign_MultiTransform
PartDesign_Boolean
```

草图页扩充命令：

```text
Sketcher_LeaveSketch
PartDesign_NewSketch
Sketcher_EditSketch
Sketcher_MapSketch
Sketcher_ValidateSketch
Sketcher_CreateLine
Sketcher_CreatePolyline
Sketcher_CreateRectangle
Sketcher_CreateCircle
Sketcher_CreateArc
Sketcher_Trimming
Sketcher_Dimension
Sketcher_ConstrainHorizontal
Sketcher_ConstrainVertical
Sketcher_ConstrainCoincidentUnified
Sketcher_ConstrainDistance
Sketcher_ConstrainRadius
Sketcher_ConstrainAngle
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 15:37，大小约 2.2M
```


## 21. SolidWorks-Style Ribbon Organization 2026-06-27

用户要求：开始做计划并修改；随后要求按 SolidWorks 的组织方式先做。

实施原则：

```text
不再按 FreeCAD 模块堆命令。
按 SolidWorks CommandManager 的工作流组织：特征、草图、曲面、评估、装夹/工艺、输出。
本轮只放已确认存在的 FreeCAD/McStudio 命令，避免 unknown command。
```

新增依赖加载：

```cpp
import PartGui
import PartDesignGui
import SketcherGui
import SurfaceGui
import MeasureGui
```

原因：

```text
曲面页使用 Surface_* 命令，需要 SurfaceGui 注册。
评估页使用 Std_Measure / Std_MassProperties，需要 MeasureGui 注册。
Part_* 命令由 PartGui 注册。
PartDesign_* 命令由 PartDesignGui 注册。
Sketcher_* 命令由 SketcherGui 注册。
```

当前页签：

```text
特征
草图
曲面
评估
装夹 / 工艺
输出
```

特征页：

```text
主要特征: PartDesign_Body / PartDesign_Pad / PartDesign_Pocket / PartDesign_Hole
修改特征: PartDesign_Fillet / PartDesign_Chamfer / PartDesign_Draft / PartDesign_Thickness
旋转 / 扫掠 / 放样: PartDesign_Revolution / PartDesign_Groove / PartDesign_AdditivePipe / PartDesign_SubtractivePipe / PartDesign_AdditiveLoft / PartDesign_SubtractiveLoft
阵列 / 镜像: PartDesign_Mirrored / PartDesign_LinearPattern / PartDesign_PolarPattern / PartDesign_MultiTransform / PartDesign_Boolean
参考几何: McStudio_CreateReferencePlane / PartDesign_Plane / PartDesign_Line / PartDesign_Point / PartDesign_CoordinateSystem
```

草图页：

```text
草图: Sketcher_LeaveSketch / PartDesign_NewSketch / Sketcher_EditSketch / Sketcher_MapSketch / Sketcher_ValidateSketch
绘制: Sketcher_CreateLine / Sketcher_CreatePolyline / Sketcher_CreateRectangle / Sketcher_CreateCircle / Sketcher_CreateArc / Sketcher_Trimming
尺寸 / 约束: Sketcher_Dimension / Sketcher_ConstrainHorizontal / Sketcher_ConstrainVertical / Sketcher_ConstrainCoincidentUnified / Sketcher_ConstrainDistance / Sketcher_ConstrainRadius / Sketcher_ConstrainAngle
```

曲面页：

```text
曲面创建: Surface_Filling / Surface_GeomFillSurface / Surface_Sections / Part_Loft / Part_Sweep / Part_RuledSurface
曲面编辑: Surface_Cut / Surface_ExtendFace / Surface_BlendCurve / Part_Offset / Part_Thickness / Part_ProjectionOnSurface
```

评估页：

```text
测量: Std_Measure / Std_MassProperties / Part_ShapeInfo
检查: Part_CheckGeometry / Part_Section / Part_CrossSections / Part_SectionCut
视图: Std_ViewHome / Std_DrawStyle / Std_ToggleClipPlane / Std_ToggleTransparency
```

装夹 / 工艺页：

```text
工件: McStudio_CreateWorkpiece / McStudio_CreateStock
夹具: McStudio_CreateFixture / McStudio_SetFixtureLocation
坐标系: McStudio_SetWorkCoordinateSystem / McStudio_CreateSetupDatum
```

输出页：

```text
项目: Std_New / Std_Open / Std_Save / Std_Import / Std_Export
工艺文档: McStudio_GenerateProcessReport / McStudio_ExportSetupSheet / McStudio_GenerateBOM
模型处理: Part_Import / Part_Export / Part_SimpleCopy / Part_RefineShape
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 15:43，大小约 2.2M
```


## 22. Ribbon Scale Preference Parameter 2026-06-27

用户要求：调研 FreeCAD 原有参数写法，并给 McStudio Ribbon 加比例参数。

调研结论：

```text
FreeCAD C++ 代码通常通过 App::GetApplication().GetParameterGroupByPath("User parameter:...") 读取参数。
常见读取方式包括 hGrp->GetFloat(name, defaultValue)、hGrp->GetInt(name, defaultValue)。
例如 View3D / Navigation / Inspection 等模块均使用 ParameterGrp::handle。
```

McStudio 新增参数：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon
  Scale = 1.0
```

参数范围：

```text
默认：1.0
最小：0.70
最大：1.80
```

当前 Scale 影响：

```text
Ribbon 按钮图标尺寸：基础 24px * Scale
Ribbon 分组水平边距：基础 4px * Scale
Ribbon 分组垂直边距：基础 2px * Scale
Ribbon 页内边距：基础 4px * Scale
Ribbon 页内 spacing：基础 4px * Scale
Ribbon 外层 QToolBar 字体
Ribbon 页签字体
Ribbon 命令按钮字体
Ribbon 分组标题字体
```

实现位置：

```text
src/Mod/McStudio/Gui/RibbonWidget.cpp
```

关键函数：

```text
ribbonScale()
ribbonMetrics()
scaledFont()
```

热更新策略：

```text
RibbonWidget::show() 会读取当前 Scale。
如果已有 McStudioRibbonToolBar 的 McStudioRibbonScale property 与当前 Scale 不一致，则移除并重建 Ribbon。
用户改参数后切换离开/回到 McStudio 即可应用新比例。
```

验证：

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py 通过
ninja McStudioGui McStudio 通过
build/debug/Mod/McStudio/McStudioGui.so 更新时间 15:53，大小约 2.2M
```
