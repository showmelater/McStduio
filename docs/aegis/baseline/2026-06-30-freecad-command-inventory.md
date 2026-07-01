# FreeCAD / McStudio GUI 命令清单与 Ribbon 迁移基线

生成日期：2026-06-30

## 说明

- 本文档由源码自动扫描生成，覆盖 `src/**/*.cpp`、`src/**/*.h`、`src/**/*.py` 中的 GUI 命令定义、Workbench 菜单/工具栏引用、GroupCommand 引用和 McStudio Ribbon 引用。
- `已定义` 表示在源码中发现 `Command(...)`、`GroupCommand(...)`、`PythonCommand(...)` 或 `Gui.addCommand(...)`。
- `被引用` 表示在菜单、工具栏、组合命令或 Ribbon 中引用过，但是否在当前构建运行时可用仍需 FreeCAD 启动后验证。
- 本版过滤了明显非命令字符串，只保留已知 FreeCAD/McStudio 命令前缀。

## 总览

- 命令候选总数：**1184**
- 源码中已定义/注册：**1168**
- 仅被菜单/工具栏/组合命令引用：**16**
- 当前 McStudio Ribbon 已引用：**77**

### 按类别统计

| 类别 | 数量 | 已定义 | 当前 McStudio Ribbon 已引用 |
|---|---:|---:|---:|
| 00 FreeCAD 标准/全局命令 | 216 | 213 | 20 |
| 01 McStudio 当前工作台命令 | 26 | 21 | 26 |
| 10 PartDesign 特征建模 | 43 | 43 | 13 |
| 11 Sketcher 草图 | 138 | 135 | 14 |
| 12 Part 几何/实体 | 77 | 77 | 4 |
| 13 Surface 曲面 | 7 | 7 | 0 |
| 14 Mesh 网格 | 38 | 36 | 0 |
| 15 MeshPart 网格转实体 | 7 | 7 | 0 |
| 20 TechDraw 工程图 | 130 | 128 | 0 |
| 21 Draft 二维绘图 | 84 | 84 | 0 |
| 22 Arch 建筑 | 54 | 54 | 0 |
| 23 BIM | 85 | 85 | 0 |
| 24 Assembly 装配 | 31 | 31 | 0 |
| 30 FEM 有限元 | 102 | 102 | 0 |
| 31 CAM/Path 加工 | 49 | 49 | 0 |
| 32 Robot 机器人 | 15 | 15 | 0 |
| 40 Spreadsheet 表格 | 16 | 15 | 0 |
| 41 OpenSCAD | 15 | 15 | 0 |
| 42 Materials 材料 | 4 | 4 | 0 |
| 43 Points 点云 | 6 | 6 | 0 |
| 44 Reverse Engineering 逆向 | 12 | 12 | 0 |
| 45 Inspection 检测 | 2 | 2 | 0 |
| 46 IFC | 7 | 7 | 0 |
| 47 Import/导入导出 | 2 | 2 | 0 |
| 90 Start/模板/其它 | 1 | 1 | 0 |
| 91 Test/开发测试 | 11 | 11 | 0 |
| 92 Template/示例 | 6 | 6 | 0 |

## McStudio Ribbon 迁移视图

### 当前已经进入 McStudio Ribbon 的命令

| 命令 | Ribbon 短名/菜单名 | 类别 | 来源类型 |
|---|---|---|---|
| `Std_DlgCustomize` | Cu&stomize | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Std_DlgPreferences` | Prefere&nces | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `Std_DrawStyle` | &Draw Style | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, smallCommand |
| `Std_Export` | &Export… | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Std_Group` | New Group | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_Import` | &Import… | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Std_LinkActions` | Link Actions | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_New` | &New Document | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand, runCommandByName |
| `Std_Open` | &Open… | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand, runCommandByName |
| `Std_Part` | New Part | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, runCommandByName |
| `Std_Redo` | &Redo | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_Refresh` | Recompute | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, runCommandByName |
| `Std_Save` | &Save | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_ToggleTransparency` | Toggle Transparenc&y | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_Undo` | &Undo | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_VarSet` | Variable Set | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_ViewFitAll` | &Fit All | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, largeCommand, runCommandByName |
| `Std_ViewFitSelection` | Fit &Selection | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, largeCommand |
| `Std_ViewGroup` | Standard &Views | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand |
| `Std_ViewHome` | &Home | 00 FreeCAD 标准/全局命令 | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, smallCommand |
| `McStudio_CompAdvancedTools` | 高级工具 | 01 McStudio 当前工作台命令 | dropDownCommand |
| `McStudio_CompApplicationMenu` | 文件 | 01 McStudio 当前工作台命令 | dropDownCommand |
| `McStudio_CompDressUpFeatures` | 修饰特征 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompEditTools` | 编辑工具 | 01 McStudio 当前工作台命令 | smallDropDownCommand |
| `McStudio_CompEvaluateInspect` | Measure and Inspect | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompHelp` | 帮助 | 01 McStudio 当前工作台命令 | dropDownCommand |
| `McStudio_CompMacroTools` | 宏 / 自动化 | 01 McStudio 当前工作台命令 | dropDownCommand |
| `McStudio_CompPanelWindowTools` |  | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompSectionTools` | Section Tools | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompStandardViews` |  | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompSurfaceCreate` | Create Surface | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompSurfaceEdit` | Edit Surface | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompTransformFeatures` | 阵列 / 镜像 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompViewDisplay` |  | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompViewNavigate` |  | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CompViewVisibility` |  | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, dropDownCommand |
| `McStudio_CreateFixture` | 夹具 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_CreateReferencePlane` | 参考平面 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_CreateSetupDatum` | 加工基准 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_CreateStock` | 毛坯 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_CreateWorkpiece` | 工件 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_ExportSetupSheet` | 设置单 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_GenerateBOM` | BOM | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_GenerateProcessReport` | 工艺报告 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `McStudio_SetFixtureLocation` | Set Fixture Location | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, smallCommand |
| `McStudio_SetWorkCoordinateSystem` | 工件坐标系 | 01 McStudio 当前工作台命令 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_AdditiveLoft` | Additive Loft | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `PartDesign_AdditivePipe` | Additive Pipe | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `PartDesign_Body` | New Body | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName, smallCommand |
| `PartDesign_Boolean` | Boolean Operation | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `PartDesign_CompDatums` | Create Datum | 10 PartDesign 特征建模 | C++ Command/GroupCommand, dropDownCommand |
| `PartDesign_CompSketches` | Create Datum | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand |
| `PartDesign_Groove` | 旋转切除 | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_Hole` | 孔 | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_Pad` | 凸台 | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_Pocket` | 切除 | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_Revolution` | Revolve | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand |
| `PartDesign_SubtractiveLoft` | Subtractive Loft | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `PartDesign_SubtractivePipe` | Subtractive Pipe | 10 PartDesign 特征建模 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Sketcher_CompCreateArc` | Arc | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand |
| `Sketcher_CompCreateConic` | Conic | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand |
| `Sketcher_CompCreateFillets` | Fillet/Chamfer | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand |
| `Sketcher_CompCreateRectangles` | Rectangle | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand |
| `Sketcher_CompCurveEdition` | Edit Edges | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand |
| `Sketcher_CompDimensionTools` | Dimension | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand |
| `Sketcher_CompHorVer` | Horizontal/Vertical Constraint | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand |
| `Sketcher_CompLine` | Polyline | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand |
| `Sketcher_ConstrainCoincidentUnified` |  | 11 Sketcher 草图 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand |
| `Sketcher_ConstrainEqual` |  | 11 Sketcher 草图 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand |
| `Sketcher_ConstrainParallel` |  | 11 Sketcher 草图 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand |
| `Sketcher_ConstrainTangent` |  | 11 Sketcher 草图 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand |
| `Sketcher_LeaveSketch` | Leave Sketch | 11 Sketcher 草图 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, largeCommand |
| `Sketcher_ValidateSketch` | Validate Sketch | 11 Sketcher 草图 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Part_Export` | Export CAD File | 12 Part 几何/实体 | C++ Command/GroupCommand, smallCommand |
| `Part_Import` | Import CAD File | 12 Part 几何/实体 | C++ Command/GroupCommand, smallCommand |
| `Part_RefineShape` | Refine Shape | 12 Part 几何/实体 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |
| `Part_SimpleCopy` | Simple Copy | 12 Part 几何/实体 | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand |

### 仍需要人工评审的高优先级候选

| 命令 | 类别 | 菜单文字 | 定义状态 | 典型文件 |
|---|---|---|---|---|
| `Std_About` | 00 FreeCAD 标准/全局命令 | &About %1 | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_AboutQt` | 00 FreeCAD 标准/全局命令 | About &Qt | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_ActivateNextWindow` | 00 FreeCAD 标准/全局命令 | &Next | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ActivatePrevWindow` | 00 FreeCAD 标准/全局命令 | &Previous | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_AddonMgr` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/Dialogs/DlgMacroExecuteImp.cpp` |
| `Std_AlignToSelection` | 00 FreeCAD 标准/全局命令 | &Align to Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_Alignment` | 00 FreeCAD 标准/全局命令 | Ali&gn To… | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_AnnotationLabel` | 00 FreeCAD 标准/全局命令 | Annotation Label | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_AxisCross` | 00 FreeCAD 标准/全局命令 | Toggle A&xis Cross | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_BoxElementSelection` | 00 FreeCAD 标准/全局命令 | Bo&x Element Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_BoxSelection` | 00 FreeCAD 标准/全局命令 | &Box Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_CascadeWindows` | 00 FreeCAD 标准/全局命令 | &Cascade | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ClarifySelection` | 00 FreeCAD 标准/全局命令 | Clarify Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_CloseActiveWindow` | 00 FreeCAD 标准/全局命令 | &Close | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_CloseAllWindows` | 00 FreeCAD 标准/全局命令 | Close A&ll | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_CommandLine` | 00 FreeCAD 标准/全局命令 | Command &Line | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_Copy` | 00 FreeCAD 标准/全局命令 | &Copy | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Cut` | 00 FreeCAD 标准/全局命令 | Cu&t | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Delete` | 00 FreeCAD 标准/全局命令 | &Delete | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_DemoMode` | 00 FreeCAD 标准/全局命令 | View &Turntable | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DependencyGraph` | 00 FreeCAD 标准/全局命令 | Dependency Gra&ph | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_DescriptionMode` | 00 FreeCAD 标准/全局命令 | Des&cription | 已定义 | `src/Gui/WhatsThis.cpp` |
| `Std_DevHandbook` | 00 FreeCAD 标准/全局命令 | Developers Handbook | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_DlgMacroExecute` | 00 FreeCAD 标准/全局命令 | Ma&cros | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_DlgMacroExecuteDirect` | 00 FreeCAD 标准/全局命令 | &Execute Macro | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_DlgMacroRecord` | 00 FreeCAD 标准/全局命令 | Record &Macro | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_DlgParameter` | 00 FreeCAD 标准/全局命令 | E&dit Parameters | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_DockOverlay` | 00 FreeCAD 标准/全局命令 | Overlay Docked Panel | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayAll` | 00 FreeCAD 标准/全局命令 | Toggle Overl&ay for All Panels | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayMouseTransparent` | 00 FreeCAD 标准/全局命令 | Bypass &Mouse Events in Overlay Panels | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggle` | 00 FreeCAD 标准/全局命令 | Toggle &Overlay | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleBottom` | 00 FreeCAD 标准/全局命令 | Toggle &Bottom | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleLeft` | 00 FreeCAD 标准/全局命令 | Toggle &Left | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleRight` | 00 FreeCAD 标准/全局命令 | Toggle &Right | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleTop` | 00 FreeCAD 标准/全局命令 | Toggle &Top | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleTransparent` | 00 FreeCAD 标准/全局命令 | Toggle Tran&sparent Mode | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayTransparentAll` | 00 FreeCAD 标准/全局命令 | Toggle Tra&nsparent Panels | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_DockViewMenu` | 00 FreeCAD 标准/全局命令 | &Panels | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_DownloadOnlineHelp` | 00 FreeCAD 标准/全局命令 | Download Online Help | 已定义 | `src/Gui/NetworkRetriever.cpp` |
| `Std_DuplicateSelection` | 00 FreeCAD 标准/全局命令 | Duplicate Selecti&on | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Edit` | 00 FreeCAD 标准/全局命令 | Toggle &Edit Mode | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_ExportDependencyGraph` | 00 FreeCAD 标准/全局命令 | Export Dependency &Graph… | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Expressions` | 00 FreeCAD 标准/全局命令 | Expression Actions | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_FreeCADDonation` | 00 FreeCAD 标准/全局命令 | Donate to FreeCA&D | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_FreeCADForum` | 00 FreeCAD 标准/全局命令 | FreeCAD &Forum | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_FreeCADUserHub` | 00 FreeCAD 标准/全局命令 | &User Documentation | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_FreeCADWebsite` | 00 FreeCAD 标准/全局命令 | FreeCAD W&ebsite | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_FreezeViews` | 00 FreeCAD 标准/全局命令 | F&reeze Display | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_HideObjects` | 00 FreeCAD 标准/全局命令 | Hide All &Objects | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_HideSelection` | 00 FreeCAD 标准/全局命令 | &Hide Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_LinkImport` | 00 FreeCAD 标准/全局命令 | Import Links | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkImportAll` | 00 FreeCAD 标准/全局命令 | Import All Links | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkMake` | 00 FreeCAD 标准/全局命令 | Make Link | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkMakeGroup` | 00 FreeCAD 标准/全局命令 | Link Group | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkMakeRelative` | 00 FreeCAD 标准/全局命令 | Make Sub-Link | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkReplace` | 00 FreeCAD 标准/全局命令 | Replace With Link | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectActions` | 00 FreeCAD 标准/全局命令 | &Link Navigation | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectAllLinks` | 00 FreeCAD 标准/全局命令 | Select &All Links | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectLinked` | 00 FreeCAD 标准/全局命令 | &Go to Linked Object | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectLinkedFinal` | 00 FreeCAD 标准/全局命令 | Go to &Deepest Linked Object | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_LinkUnlink` | 00 FreeCAD 标准/全局命令 | Unlink | 已定义 | `src/Gui/CommandLink.cpp` |
| `Std_MDITest1` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_MDITest2` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_MDITest3` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_MacroAttachDebugger` | 00 FreeCAD 标准/全局命令 | &Attach to Remote Debugger | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStartDebug` | 00 FreeCAD 标准/全局命令 | &Debug Macro | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStepInto` | 00 FreeCAD 标准/全局命令 | Step &Into | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStepOver` | 00 FreeCAD 标准/全局命令 | Step &Over | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStopDebug` | 00 FreeCAD 标准/全局命令 | &Stop Debugging | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_Macro_` | 00 FreeCAD 标准/全局命令 |  | 仅引用 | `src/Gui/Command.cpp` |
| `Std_MainFullscreen` | 00 FreeCAD 标准/全局命令 | Fullscreen | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_MassProperties` | 00 FreeCAD 标准/全局命令 | Mass Properties | 已定义 | `src/Gui/Workbench.cpp` |
| `Std_Measure` | 00 FreeCAD 标准/全局命令 | &Measure | 已定义 | `src/Gui/Workbench.cpp` |
| `Std_MergeProjects` | 00 FreeCAD 标准/全局命令 | &Merge Document | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_OnlineHelp` | 00 FreeCAD 标准/全局命令 | &Help | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_OnlineHelpWebsite` | 00 FreeCAD 标准/全局命令 | Help Website | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_OpenMacrosFolder` | 00 FreeCAD 标准/全局命令 | Open Macro Folder | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_OrthographicCamera` | 00 FreeCAD 标准/全局命令 | Orthographic View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_Paste` | 00 FreeCAD 标准/全局命令 | &Paste | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_PerspectiveCamera` | 00 FreeCAD 标准/全局命令 | Perspective View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_Placement` | 00 FreeCAD 标准/全局命令 | P&lacement | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Print` | 00 FreeCAD 标准/全局命令 | &Print | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_PrintPdf` | 00 FreeCAD 标准/全局命令 | Export P&DF | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_PrintPreview` | 00 FreeCAD 标准/全局命令 | Print Previe&w | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_ProjectInfo` | 00 FreeCAD 标准/全局命令 | Doc&ument Information | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_ProjectUtil` | 00 FreeCAD 标准/全局命令 | Do&cument Utility | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_Properties` | 00 FreeCAD 标准/全局命令 | Propert&ies | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_PythonHelp` | 00 FreeCAD 标准/全局命令 | Python &Modules Documentation | 已定义 | `src/Gui/OnlineDocumentation.cpp` |
| `Std_Quit` | 00 FreeCAD 标准/全局命令 | E&xit | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_RandomColor` | 00 FreeCAD 标准/全局命令 | Random &Color | 已定义 | `src/Gui/CommandFeat.cpp` |
| `Std_RecallWorkingView` | 00 FreeCAD 标准/全局命令 | R&ecall Working View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_RecentFiles` | 00 FreeCAD 标准/全局命令 | Open &Recent | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_RecentMacros` | 00 FreeCAD 标准/全局命令 | &Recent Macros | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_Recompute` | 00 FreeCAD 标准/全局命令 | &Recompute | 已定义 | `src/Gui/CommandFeat.cpp` |
| `Std_ReloadStyleSheet` | 00 FreeCAD 标准/全局命令 | &Reload Stylesheet | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_ReportBug` | 00 FreeCAD 标准/全局命令 | Report an &Issue | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_RestartInSafeMode` | 00 FreeCAD 标准/全局命令 | Restart in Safe Mode | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_Revert` | 00 FreeCAD 标准/全局命令 | Rever&t | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_SaveAll` | 00 FreeCAD 标准/全局命令 | Sa&ve All | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_SaveAs` | 00 FreeCAD 标准/全局命令 | Save &As… | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_SaveCopy` | 00 FreeCAD 标准/全局命令 | Save a Cop&y… | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_SceneInspector` | 00 FreeCAD 标准/全局命令 | Scene I&nspector | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_SelBack` | 00 FreeCAD 标准/全局命令 | Selection &Back | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_SelBoundingBox` | 00 FreeCAD 标准/全局命令 | &Bounding Box | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_SelForward` | 00 FreeCAD 标准/全局命令 | Selection &Forward | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_SelectAll` | 00 FreeCAD 标准/全局命令 | Select &All | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_SelectVisibleObjects` | 00 FreeCAD 标准/全局命令 | &Select Visible Objects | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_SendToPythonConsole` | 00 FreeCAD 标准/全局命令 | &Send to Python Console | 已定义 | `src/Gui/CommandFeat.cpp` |
| `Std_SetAppearance` | 00 FreeCAD 标准/全局命令 | &Appearance | 已定义 | `src/Mod/Material/Gui/Command.cpp` |
| `Std_SetMaterial` | 00 FreeCAD 标准/全局命令 | &Material | 已定义 | `src/Mod/Material/Gui/Command.cpp` |
| `Std_ShowObjects` | 00 FreeCAD 标准/全局命令 | Show &All Objects | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ShowSelection` | 00 FreeCAD 标准/全局命令 | Sho&w Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_StoreWorkingView` | 00 FreeCAD 标准/全局命令 | St&ore Working View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_Test1` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test2` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test3` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test4` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test5` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test6` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_Test7` | 00 FreeCAD 标准/全局命令 |  | 仅引用 | `src/Gui/Workbench.cpp` |
| `Std_Test8` | 00 FreeCAD 标准/全局命令 |  | 仅引用 | `src/Gui/Workbench.cpp` |
| `Std_TestCmdFuncs` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestConsoleOutput` | 00 FreeCAD 标准/全局命令 | Test Console Output | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress1` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress2` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress3` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress4` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress5` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestQM` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TestReloadQM` | 00 FreeCAD 标准/全局命令 |  | 已定义 | `src/Gui/CommandTest.cpp` |
| `Std_TextDocument` | 00 FreeCAD 标准/全局命令 | Te&xt Document | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_TextureMapping` | 00 FreeCAD 标准/全局命令 | Text&ure Mapping | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TileWindows` | 00 FreeCAD 标准/全局命令 | &Tile | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ToggleBottomPanels` | 00 FreeCAD 标准/全局命令 | Toggle Bottom Panels | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToggleBreakpoint` | 00 FreeCAD 标准/全局命令 | Toggle &Breakpoint | 已定义 | `src/Gui/CommandMacro.cpp` |
| `Std_ToggleClipPlane` | 00 FreeCAD 标准/全局命令 | Clippin&g View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToggleFreeze` | 00 FreeCAD 标准/全局命令 | Toggle Freeze | 已定义 | `src/Gui/CommandFeat.cpp` |
| `Std_ToggleNavigation` | 00 FreeCAD 标准/全局命令 | Toggle Navigation/&Edit Mode | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToggleObjects` | 00 FreeCAD 标准/全局命令 | To&ggle All Objects | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToggleSelectability` | 00 FreeCAD 标准/全局命令 | Toggle Se&lectability | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToggleSkipRecompute` | 00 FreeCAD 标准/全局命令 | Skip Recomputes | 已定义 | `src/Gui/CommandFeat.cpp` |
| `Std_ToggleToolBarLock` | 00 FreeCAD 标准/全局命令 | Lock Toolbars | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ToggleVisibility` | 00 FreeCAD 标准/全局命令 | Toggle &Visibility | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ToolBarMenu` | 00 FreeCAD 标准/全局命令 | &Toolbars | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_Transform` | 00 FreeCAD 标准/全局命令 | Transform | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_TransformManip` | 00 FreeCAD 标准/全局命令 | Trans&form | 已定义 | `src/Gui/CommandDoc.cpp` |
| `Std_TreeCollapse` | 00 FreeCAD 标准/全局命令 | Collapse Selected Items | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeCollapseDocument` | 00 FreeCAD 标准/全局命令 | Collapse/E&xpand | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeDrag` | 00 FreeCAD 标准/全局命令 | Initiate &Dragging | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeExpand` | 00 FreeCAD 标准/全局命令 | Expand Selected Items | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeMultiDocument` | 00 FreeCAD 标准/全局命令 | &Multi Document | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreePreSelection` | 00 FreeCAD 标准/全局命令 | Preselection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeRecordSelection` | 00 FreeCAD 标准/全局命令 | Record Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSelectAllInstances` | 00 FreeCAD 标准/全局命令 | Select All Instances | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSelection` | 00 FreeCAD 标准/全局命令 | &Go to Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSingleDocument` | 00 FreeCAD 标准/全局命令 | &Single Document | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncPlacement` | 00 FreeCAD 标准/全局命令 | Sync Placement | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncSelection` | 00 FreeCAD 标准/全局命令 | Sync Selection | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncView` | 00 FreeCAD 标准/全局命令 | Sync View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_TreeViewActions` | 00 FreeCAD 标准/全局命令 | Tree View Actions | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_UnitsCalculator` | 00 FreeCAD 标准/全局命令 | &Units Converter | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_UserEditMode` | 00 FreeCAD 标准/全局命令 | Edit &Mode | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_UserInterface` | 00 FreeCAD 标准/全局命令 | Dock Views | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ViewBottom` | 00 FreeCAD 标准/全局命令 | Bottom | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewBoxZoom` | 00 FreeCAD 标准/全局命令 | &Box Zoom | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewCreate` | 00 FreeCAD 标准/全局命令 | New 3D View | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewDimetric` | 00 FreeCAD 标准/全局命令 | &Dimetric | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewDock` | 00 FreeCAD 标准/全局命令 | &Docked | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewDockUndockFullscreen` | 00 FreeCAD 标准/全局命令 | D&ocument Window | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewExample1` | 00 FreeCAD 标准/全局命令 | Inventor Example #1 | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewExample2` | 00 FreeCAD 标准/全局命令 | Inventor Example #2 | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewExample3` | 00 FreeCAD 标准/全局命令 | Inventor Example #3 | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewFront` | 00 FreeCAD 标准/全局命令 | Front | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewFullscreen` | 00 FreeCAD 标准/全局命令 | &Fullscreen | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewIsometric` | 00 FreeCAD 标准/全局命令 | &Isometric | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewIvIssueCamPos` | 00 FreeCAD 标准/全局命令 | Issue Camera &Position | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewLeft` | 00 FreeCAD 标准/全局命令 | Left | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewLoadImage` | 00 FreeCAD 标准/全局命令 | &Load Image… | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewRear` | 00 FreeCAD 标准/全局命令 | Rear | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewRestoreCamera` | 00 FreeCAD 标准/全局命令 | Restore Saved Camera | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewRight` | 00 FreeCAD 标准/全局命令 | Right | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewRotateLeft` | 00 FreeCAD 标准/全局命令 | Rotate &Left | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewRotateRight` | 00 FreeCAD 标准/全局命令 | Rotates &Right | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewSaveCamera` | 00 FreeCAD 标准/全局命令 | Save Current Camera | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewScreenShot` | 00 FreeCAD 标准/全局命令 | Save &Image… | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewStatusBar` | 00 FreeCAD 标准/全局命令 | Status Bar | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_ViewTop` | 00 FreeCAD 标准/全局命令 | Top | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewTrimetric` | 00 FreeCAD 标准/全局命令 | &Trimetric | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewUndock` | 00 FreeCAD 标准/全局命令 | &Undocked | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewVR` | 00 FreeCAD 标准/全局命令 | FreeCAD VR | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewZoomIn` | 00 FreeCAD 标准/全局命令 | Zoom &In | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_ViewZoomOut` | 00 FreeCAD 标准/全局命令 | Zoom &Out | 已定义 | `src/Gui/CommandView.cpp` |
| `Std_WhatsThis` | 00 FreeCAD 标准/全局命令 | &What's This? | 已定义 | `src/Gui/CommandStd.cpp` |
| `Std_Windows` | 00 FreeCAD 标准/全局命令 | Choose Open &Window | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_WindowsMenu` | 00 FreeCAD 标准/全局命令 | Activate Window | 已定义 | `src/Gui/CommandWindow.cpp` |
| `Std_Workbench` | 00 FreeCAD 标准/全局命令 | &Workbench | 已定义 | `src/Gui/CommandStd.cpp` |
| `PartDesign_AdditiveHelix` | 10 PartDesign 特征建模 | Additive Helix | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_Chamfer` | 10 PartDesign 特征建模 | Chamfer | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_Clone` | 10 PartDesign 特征建模 | Clone | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_CompPrimitiveAdditive` | 10 PartDesign 特征建模 | Additive Primitive | 已定义 | `src/Mod/PartDesign/Gui/CommandPrimitive.cpp` |
| `PartDesign_CompPrimitiveSubtractive` | 10 PartDesign 特征建模 | Subtractive Primitive | 已定义 | `src/Mod/PartDesign/Gui/CommandPrimitive.cpp` |
| `PartDesign_CoordinateSystem` | 10 PartDesign 特征建模 | Local Coordinate System | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_Draft` | 10 PartDesign 特征建模 | Draft | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_DuplicateSelection` | 10 PartDesign 特征建模 | Duplicate &Object | 已定义 | `src/Mod/PartDesign/Gui/CommandBody.cpp` |
| `PartDesign_Fillet` | 10 PartDesign 特征建模 | Fillet | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_InvoluteGear` | 10 PartDesign 特征建模 |  | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_Line` | 10 PartDesign 特征建模 | Datum Line | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_LinearPattern` | 10 PartDesign 特征建模 | Linear Pattern | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_Migrate` | 10 PartDesign 特征建模 | Migrate | 已定义 | `src/Mod/PartDesign/Gui/CommandBody.cpp` |
| `PartDesign_Mirrored` | 10 PartDesign 特征建模 | Mirror | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_MoveFeature` | 10 PartDesign 特征建模 | Move Object To… | 已定义 | `src/Mod/PartDesign/Gui/CommandBody.cpp` |
| `PartDesign_MoveFeatureInTree` | 10 PartDesign 特征建模 | Move Feature After… | 已定义 | `src/Mod/PartDesign/Gui/CommandBody.cpp` |
| `PartDesign_MoveTip` | 10 PartDesign 特征建模 | Set Tip | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_MultiTransform` | 10 PartDesign 特征建模 | Multi-Transform | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_NewSketch` | 10 PartDesign 特征建模 | New Sketch | 已定义 | `src/Mod/McStudio/Gui/Workbench.cpp` |
| `PartDesign_Plane` | 10 PartDesign 特征建模 | Datum Plane | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_Point` | 10 PartDesign 特征建模 | Datum Point | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_PolarPattern` | 10 PartDesign 特征建模 | Polar Pattern | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_Scaled` | 10 PartDesign 特征建模 | Scale | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_ShapeBinder` | 10 PartDesign 特征建模 | Shape Binder | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_Sprocket` | 10 PartDesign 特征建模 |  | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_SubShapeBinder` | 10 PartDesign 特征建模 | Sub-Shape Binder | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_SubtractiveHelix` | 10 PartDesign 特征建模 | Subtractive Helix | 已定义 | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_Thickness` | 10 PartDesign 特征建模 | Thickness | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `PartDesign_WizardShaft` | 10 PartDesign 特征建模 |  | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_WizardShaftCallBack` | 10 PartDesign 特征建模 |  | 已定义 | `src/Mod/PartDesign/WizardShaft/WizardShaft.py` |
| `Sketcher_ArcOverlay` | 11 Sketcher 草图 | Toggle Circular Helper for Arcs | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplineComb` | 11 Sketcher 草图 | Toggle B-Spline Curvature Comb | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplineConvertToNURBS` | 11 Sketcher 草图 | Geometry to B-Spline | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineDecreaseDegree` | 11 Sketcher 草图 | Decrease B-Spline Degree | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineDecreaseKnotMultiplicity` | 11 Sketcher 草图 | Decrease Knot Multiplicity | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineDegree` | 11 Sketcher 草图 | Toggle B-Spline Degree | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplineIncreaseDegree` | 11 Sketcher 草图 | Increase B-Spline Degree | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineIncreaseKnotMultiplicity` | 11 Sketcher 草图 | Increase Knot Multiplicity | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineInsertKnot` | 11 Sketcher 草图 | Insert Knot | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_BSplineKnotMultiplicity` | 11 Sketcher 草图 | Toggle B-Spline Knot Multiplicity | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplinePoleWeight` | 11 Sketcher 草图 | Toggle B-Spline Control Point Weight | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplinePolygon` | 11 Sketcher 草图 | Toggle B-Spline Control Polygon | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_CancelSketch` | 11 Sketcher 草图 | Cancel Editing | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_CarbonCopy` | 11 Sketcher 草图 | Carbon Copy | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_ChangeDimensionConstraint` | 11 Sketcher 草图 | Edit Value | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_CompBSplineShowHideGeometryInformation` | 11 Sketcher 草图 | Toggle B-Spline Information Layer | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_CompConstrainRadDia` | 11 Sketcher 草图 | Radius/Diameter Dimension | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_CompConstrainTools` | 11 Sketcher 草图 | Constrain | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_CompCopy` | 11 Sketcher 草图 | Clone | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_CompCreateBSpline` | 11 Sketcher 草图 | B-Spline | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CompCreateRegularPolygon` | 11 Sketcher 草图 | Polygon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CompExternal` | 11 Sketcher 草图 | External Geometry | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CompModifyKnotMultiplicity` | 11 Sketcher 草图 | Modify Knot Multiplicity | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_CompSlot` | 11 Sketcher 草图 | Slot | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CompToggleConstraints` | 11 Sketcher 草图 | Toggle Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainAngle` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainBlock` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainCoincident` | 11 Sketcher 草图 |  | 仅引用 | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainDiameter` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainDistance` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainDistanceX` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainDistanceY` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainGroup` | 11 Sketcher 草图 | Group Constraint | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainHorVer` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainHorizontal` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainLock` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainPerpendicular` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainPointOnObject` | 11 Sketcher 草图 |  | 仅引用 | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainRadiam` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainRadius` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainSnellsLaw` | 11 Sketcher 草图 | Refraction Constraint | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainSymmetric` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ConstrainVertical` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_CopyClipboard` | 11 Sketcher 草图 | C&opy Elements | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Create3PointArc` | 11 Sketcher 草图 | Arc From 3 Points | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_Create3PointCircle` | 11 Sketcher 草图 | Circle From 3 Points | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateArc` | 11 Sketcher 草图 | Arc From Center | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateArcOfEllipse` | 11 Sketcher 草图 | Elliptical Arc | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateArcOfHyperbola` | 11 Sketcher 草图 | Hyperbolic Arc | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateArcOfParabola` | 11 Sketcher 草图 | Parabolic Arc | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateArcSlot` | 11 Sketcher 草图 | Arc Slot | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateBSpline` | 11 Sketcher 草图 | B-Spline | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateBSplineByInterpolation` | 11 Sketcher 草图 | B-Spline From Knots | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateChamfer` | 11 Sketcher 草图 | Chamfer | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateCircle` | 11 Sketcher 草图 | Circle From Center | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateDraftLine` | 11 Sketcher 草图 |  | 仅引用 | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateEllipseBy3Points` | 11 Sketcher 草图 | Ellipse From 3 Points | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateEllipseByCenter` | 11 Sketcher 草图 | Ellipse From Center | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateFillet` | 11 Sketcher 草图 | Fillet | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateHeptagon` | 11 Sketcher 草图 | Heptagon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateHexagon` | 11 Sketcher 草图 | Hexagon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateLine` | 11 Sketcher 草图 | Line | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateOblong` | 11 Sketcher 草图 | Rounded Rectangle | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateOctagon` | 11 Sketcher 草图 | Octagon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePentagon` | 11 Sketcher 草图 | Pentagon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePeriodicBSpline` | 11 Sketcher 草图 | Periodic B-Spline | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePeriodicBSplineByInterpolation` | 11 Sketcher 草图 | Periodic B-Spline From Knots | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePoint` | 11 Sketcher 草图 | Point | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePolyline` | 11 Sketcher 草图 | Polyline | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreatePolylineLegacy` | 11 Sketcher 草图 | Polyline | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateRectangle` | 11 Sketcher 草图 | Rectangle | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateRectangle_Center` | 11 Sketcher 草图 | Centered Rectangle | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateRegularPolygon` | 11 Sketcher 草图 | Polygon | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateSlot` | 11 Sketcher 草图 | Slot | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateSquare` | 11 Sketcher 草图 | Square | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateText` | 11 Sketcher 草图 | Text | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateTriangle` | 11 Sketcher 草图 | Triangle | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_Cut` | 11 Sketcher 草图 | C&ut Elements | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_DeleteAllConstraints` | 11 Sketcher 草图 | Delete All Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_DeleteAllGeometry` | 11 Sketcher 草图 | Delete All Geometry | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Dimension` | 11 Sketcher 草图 | Dimension | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_EditSketch` | 11 Sketcher 草图 | Edit Sketch | 已定义 | `src/Mod/McStudio/Gui/Workbench.cpp` |
| `Sketcher_Extend` | 11 Sketcher 草图 | Extend Edge | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_Grid` | 11 Sketcher 草图 | Toggle Grid | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_Intersection` | 11 Sketcher 草图 | External Intersection | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_JoinCurves` | 11 Sketcher 草图 | Join Curves | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp` |
| `Sketcher_LeaveGroup` | 11 Sketcher 草图 | Leave | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_MapSketch` | 11 Sketcher 草图 | Attach Sketch | 已定义 | `src/Mod/McStudio/Gui/Workbench.cpp` |
| `Sketcher_MergeSketches` | 11 Sketcher 草图 | Merge Sketches | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `Sketcher_MirrorSketch` | 11 Sketcher 草图 | Mirror Sketch | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `Sketcher_NewSketch` | 11 Sketcher 草图 | New Sketch | 已定义 | `src/Mod/Part/Gui/Workbench.cpp` |
| `Sketcher_Offset` | 11 Sketcher 草图 | Offset | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Paste` | 11 Sketcher 草图 | P&aste Elements | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_ProfilesHexagon1` | 11 Sketcher 草图 |  | 已定义 | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Projection` | 11 Sketcher 草图 | External Projection | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_RectangularArray` | 11 Sketcher 草图 | Rectangular Array | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_RemoveAxesAlignment` | 11 Sketcher 草图 | Remove Axes Alignment | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_RenderingOrder` | 11 Sketcher 草图 | Rendering Order | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_ReorientSketch` | 11 Sketcher 草图 | Reorient Sketch | 已定义 | `src/Mod/PartDesign/Gui/Workbench.cpp` |
| `Sketcher_RestoreInternalAlignmentGeometry` | 11 Sketcher 草图 | Toggle Internal Geometry | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Rotate` | 11 Sketcher 草图 | Rotate / Polar Transform | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Scale` | 11 Sketcher 草图 | Scale | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectConflictingConstraints` | 11 Sketcher 草图 | Select Conflicting Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectConstraints` | 11 Sketcher 草图 | Select Associated Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectElementsAssociatedWithConstraints` | 11 Sketcher 草图 | Select Associated Geometry | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectElementsWithDoFs` | 11 Sketcher 草图 | Select Under-Constrained Elements | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectHorizontalAxis` | 11 Sketcher 草图 | Select Horizontal Axis | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectMalformedConstraints` | 11 Sketcher 草图 | Select Malformed Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectOrigin` | 11 Sketcher 草图 | Select Origin | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectPartiallyRedundantConstraints` | 11 Sketcher 草图 | Select Partially Redundant Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectRedundantConstraints` | 11 Sketcher 草图 | Select Redundant Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_SelectVerticalAxis` | 11 Sketcher 草图 | Select Vertical Axis | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Snap` | 11 Sketcher 草图 | Toggle Snap | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_Split` | 11 Sketcher 草图 | Split Edge | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_StopOperation` | 11 Sketcher 草图 | Stop Operation | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_SwitchVirtualSpace` | 11 Sketcher 草图 | Switch Virtual Space | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherVirtualSpace.cpp` |
| `Sketcher_Symmetry` | 11 Sketcher 草图 | Mirror | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_ToggleActiveConstraint` | 11 Sketcher 草图 | Toggle Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_ToggleConstruction` | 11 Sketcher 草图 | Toggle Construction Geometry | 已定义 | `src/Mod/Sketcher/Gui/CommandAlterGeometry.cpp` |
| `Sketcher_ToggleDrivingConstraint` | 11 Sketcher 草图 | Toggle Driving/Reference Constraints | 已定义 | `src/Mod/Sketcher/Gui/CommandConstraints.cpp` |
| `Sketcher_Translate` | 11 Sketcher 草图 | Move / Array Transform | 已定义 | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_Trimming` | 11 Sketcher 草图 | Trim Edge | 已定义 | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_ViewSection` | 11 Sketcher 草图 | Toggle Section View | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_ViewSketch` | 11 Sketcher 草图 | Align View to Sketch | 已定义 | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Part_Boolean` | 12 Part 几何/实体 | Boolean Operation | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_BooleanFragments` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/SplitFeatures.py` |
| `Part_Box` | 12 Part 几何/实体 | Cube | 已定义 | `src/Mod/Part/Gui/CommandParametric.cpp` |
| `Part_Box2` | 12 Part 几何/实体 | Box Fix 1 | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Box3` | 12 Part 几何/实体 | Box Fix 2 | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_BoxSelection` | 12 Part 几何/实体 | Box Selection | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Builder` | 12 Part 几何/实体 | Shape Builder | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Chamfer` | 12 Part 几何/实体 | Chamfer | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CheckGeometry` | 12 Part 几何/实体 | Check Geometry | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_ColorPerFace` | 12 Part 几何/实体 | Appearance per &Face | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Common` | 12 Part 几何/实体 | Intersection | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CompCompoundTools` | 12 Part 几何/实体 | Compound Tools | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CompJoinFeatures` | 12 Part 几何/实体 | Join Shapes | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CompOffset` | 12 Part 几何/实体 | Offset | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CompSplitFeatures` | 12 Part 几何/实体 | Split Shapes | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Compound` | 12 Part 几何/实体 | Compound | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CompoundFilter` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/CompoundTools/_CommandCompoundFilter.py` |
| `Part_Cone` | 12 Part 几何/实体 | Cone | 已定义 | `src/Mod/Part/Gui/CommandParametric.cpp` |
| `Part_CoordinateSystem` | 12 Part 几何/实体 | Coordinate System | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CrossSections` | 12 Part 几何/实体 | Cross-Sections | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_Cut` | 12 Part 几何/实体 | Cut | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Cylinder` | 12 Part 几何/实体 | Cylinder | 已定义 | `src/Mod/Part/Gui/CommandParametric.cpp` |
| `Part_DatumLine` | 12 Part 几何/实体 | Datum Line | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_DatumPlane` | 12 Part 几何/实体 | Datum Plane | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_DatumPoint` | 12 Part 几何/实体 | Datum Point | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Datums` | 12 Part 几何/实体 | Datums | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Defeaturing` | 12 Part 几何/实体 | Defeaturing | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_EdgeSelection` | 12 Part 几何/实体 | Edge Selection | 已定义 | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_EditAttachment` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/AttachmentEditor/Commands.py` |
| `Part_ElementCopy` | 12 Part 几何/实体 | Shape Element Copy | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_ExplodeCompound` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/CompoundTools/_CommandExplodeCompound.py` |
| `Part_Extrude` | 12 Part 几何/实体 | Extrude | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_FaceSelection` | 12 Part 几何/实体 | Face Selection | 已定义 | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_Fillet` | 12 Part 几何/实体 | Fillet | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Fuse` | 12 Part 几何/实体 | Union | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_ImportCurveNet` | 12 Part 几何/实体 | Import Curve Network | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_ImportStep` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Import/Gui/Command.cpp` |
| `Part_JoinConnect` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/JoinFeatures.py` |
| `Part_JoinCutout` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/JoinFeatures.py` |
| `Part_JoinEmbed` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/JoinFeatures.py` |
| `Part_Loft` | 12 Part 几何/实体 | Loft | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_MakeFace` | 12 Part 几何/实体 | Face From Wires | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_MakeSolid` | 12 Part 几何/实体 | Convert to Solid | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Mirror` | 12 Part 几何/实体 | Mirror | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_NewDoc` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Offset` | 12 Part 几何/实体 | 3D Offset | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_Offset2D` | 12 Part 几何/实体 | 2D Offset | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_PickCurveNet` | 12 Part 几何/实体 | Pick Curve Network | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_PointsFromMesh` | 12 Part 几何/实体 | Points From Shape | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_Primitives` | 12 Part 几何/实体 | Primitive | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_ProjectionOnSurface` | 12 Part 几何/实体 | Project on Surface | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_RemoveSelectionGate` | 12 Part 几何/实体 | No Selection Filters | 已定义 | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_ReverseShape` | 12 Part 几何/实体 | Reverse Shapes | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Revolve` | 12 Part 几何/实体 | Revolve | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_RuledSurface` | 12 Part 几何/实体 | Ruled Surface | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_Scale` | 12 Part 几何/实体 | Scale | 已定义 | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Section` | 12 Part 几何/实体 | Section | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_SectionCut` | 12 Part 几何/实体 | Persiste&nt Section Cut | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_SelectFilter` | 12 Part 几何/实体 | Selection Filter | 已定义 | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_ShapeFromMesh` | 12 Part 几何/实体 | Shape From Mesh | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_ShapeInfo` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_SimpleCylinder` | 12 Part 几何/实体 | Cylinder | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_Slice` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/SplitFeatures.py` |
| `Part_SliceApart` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/SplitFeatures.py` |
| `Part_Sphere` | 12 Part 几何/实体 | Sphere | 已定义 | `src/Mod/Part/Gui/CommandParametric.cpp` |
| `Part_Sweep` | 12 Part 几何/实体 | Sweep | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_Thickness` | 12 Part 几何/实体 | Thickness | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Part_ToleranceSet` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/ToleranceFeatures.py` |
| `Part_Torus` | 12 Part 几何/实体 | Torus | 已定义 | `src/Mod/Part/Gui/CommandParametric.cpp` |
| `Part_TransformedCopy` | 12 Part 几何/实体 | Transformed Copy | 已定义 | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_Tube` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BasicShapes/CommandShapes.py` |
| `Part_VertexSelection` | 12 Part 几何/实体 | Vertex Selection | 已定义 | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_XOR` | 12 Part 几何/实体 |  | 已定义 | `src/Mod/Part/BOPTools/SplitFeatures.py` |
| `Surface_BlendCurve` | 13 Surface 曲面 | Blend Curve | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Surface_CurveOnMesh` | 13 Surface 曲面 | Curve on Mesh | 已定义 | `src/Mod/Surface/Gui/Command.cpp` |
| `Surface_Cut` | 13 Surface 曲面 | Surface Cut | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Surface_ExtendFace` | 13 Surface 曲面 | Extend Face | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Surface_Filling` | 13 Surface 曲面 | Filling | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Surface_GeomFillSurface` | 13 Surface 曲面 | Fill Boundary Curves | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Surface_Sections` | 13 Surface 曲面 | Sections | 已定义 | `src/Mod/McStudio/Gui/Command.cpp` |
| `Mesh_AddFacet` | 14 Mesh 网格 | Add Triangle | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_BoundingBox` | 14 Mesh 网格 | Bounding Box Info | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_BuildRegularSolid` | 14 Mesh 网格 | Regular Solid | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_CrossSections` | 14 Mesh 网格 | Cross-Sections | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_CurvatureInfo` | 14 Mesh 网格 | Curvature Info | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Decimating` | 14 Mesh 网格 | Decimate | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Demolding` | 14 Mesh 网格 |  | 仅引用 | `src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Difference` | 14 Mesh 网格 | Difference | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_EvaluateFacet` | 14 Mesh 网格 | Face Info | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_EvaluateSolid` | 14 Mesh 网格 | Evaluate Solid | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Evaluation` | 14 Mesh 网格 | Evaluate and Repair | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Export` | 14 Mesh 网格 | Export Mesh… | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FillInteractiveHole` | 14 Mesh 网格 | Close Hole | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FillupHoles` | 14 Mesh 网格 | Fill Holes | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FlipNormals` | 14 Mesh 网格 | Flip Normals | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FromGeometry` | 14 Mesh 网格 | Mesh From Geometry | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FromPartShape` | 14 Mesh 网格 | Mesh From Shape | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_HarmonizeNormals` | 14 Mesh 网格 | Harmonize Normals | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Import` | 14 Mesh 网格 | Import Mesh… | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_Intersection` | 14 Mesh 网格 | Intersection | 已定义 | `src/Mod/Mesh/Gui/Command.cpp` |

> 高优先级候选共 956 条，此处显示前 450 条；完整清单见后文各类别章节。

## 完整分类清单

### 00 FreeCAD 标准/全局命令（216）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Std_About` | &About %1 | Displays information about %1 | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_AboutQt` | About &Qt | Displays information about Qt | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandStd.cpp` |
| `Std_ActivateNextWindow` | &Next | Activates the next window | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ActivatePrevWindow` | &Previous | Switches to the previously active window | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_AddonMgr` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Gui/Dialogs/DlgMacroExecuteImp.cpp`<br>`src/Gui/Dialogs/DlgPreferencePackManagementImp.cpp`<br>+4 more |
| `Std_AlignToSelection` | &Align to Selection | Aligns the camera view to the selected elements in the 3D view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Alignment` | Ali&gn To… | Aligns the selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_AnnotationLabel` | Annotation Label | Creates a new annotation label at the picked location in the 3D view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_AxisCross` | Toggle A&xis Cross | Toggles the axis cross at the origin | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_BoxElementSelection` | Bo&x Element Selection | Activates box element selection | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_BoxSelection` | &Box Selection | Activates the box selection tool | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_CascadeWindows` | &Cascade | Tiles pragmatic | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ClarifySelection` | Clarify Selection |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_CloseActiveWindow` | &Close | Closes the active window | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_CloseAllWindows` | Close A&ll | Closes all windows | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_CommandLine` | Command &Line | Opens a command line interface in the console | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandStd.cpp` |
| `Std_Copy` | &Copy | Copies the selection to the clipboard | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Cut` | Cu&t | Removes the selection and copies it to the clipboard | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Delete` | &Delete | Deletes the selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_DemoMode` | View &Turntable | Opens a turntable view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DependencyGraph` | Dependency Gra&ph | Shows the dependency graph of the objects in the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DescriptionMode` | Des&cription | Long description of commands | 已定义 | C++ Command/GroupCommand | `src/Gui/WhatsThis.cpp` |
| `Std_DevHandbook` | Developers Handbook | Opens the FreeCAD developers handbook | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DlgCustomize` | Cu&stomize | Opens a dialog to edit toolbars, shortcuts, and macros | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_DlgMacroExecute` | Ma&cros | Opens a dialog to execute a recorded macro | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandMacro.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DlgMacroExecuteDirect` | &Execute Macro | Executes the macro in the editor | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandMacro.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DlgMacroRecord` | Record &Macro | Opens a dialog to record a macro | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandMacro.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DlgParameter` | E&dit Parameters | Opens a dialog to edit the parameters | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DlgPreferences` | Prefere&nces | Opens a dialog to edit the preferences | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_DockOverlay` | Overlay Docked Panel | Sets the docked panel in overlay mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DockOverlayAll` | Toggle Overl&ay for All Panels | Toggles overlay mode for all docked panels | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayMouseTransparent` | Bypass &Mouse Events in Overlay Panels | Bypasses all mouse events in docked overlay panels | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggle` | Toggle &Overlay | Toggles overlay mode for the docked window under the cursor | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleBottom` | Toggle &Bottom | Toggles the visibility of the bottom overlay panel | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleLeft` | Toggle &Left | Toggles the visibility of the left overlay panel | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleRight` | Toggle &Right | Toggles the visibility of the right overlay panel | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleTop` | Toggle &Top | Toggles the visibility of the top overlay panel | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayToggleTransparent` | Toggle Tran&sparent Mode |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockOverlayTransparentAll` | Toggle Tra&nsparent Panels |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_DockViewMenu` | &Panels | Lists available dock panels | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_DownloadOnlineHelp` | Download Online Help | Downloads %1's online help | 已定义 | C++ Command/GroupCommand | `src/Gui/NetworkRetriever.cpp` |
| `Std_DrawStyle` | &Draw Style | Changes the draw style of the objects | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, smallCommand | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_DuplicateSelection` | Duplicate Selecti&on | Duplicates the selected objects to the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Edit` | Toggle &Edit Mode | Toggles the selected object's edit mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Export` | &Export… | Exports an object in the active document | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ExportDependencyGraph` | Export Dependency &Graph… | Exports the dependency graph as a Graphviz (.gv) file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Expressions` | Expression Actions | Actions that apply to expressions | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Tree.cpp` |
| `Std_FreeCADDonation` | Donate to FreeCA&D | Opens the FreeCAD donation page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_FreeCADForum` | FreeCAD &Forum | Opens the FreeCAD forum to find help from other users | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_FreeCADUserHub` | &User Documentation | Opens the documentation for users | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_FreeCADWebsite` | FreeCAD W&ebsite | Navigates to the official FreeCAD website | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_FreezeViews` | F&reeze Display | Freezes the current view position | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Group` | New Group |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandStructure.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_HideObjects` | Hide All &Objects | Hides all objects in the document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_HideSelection` | &Hide Selection | Hides all selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Import` | &Import… | Imports a file into the active document | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_LinkActions` | Link Actions | Commands that operate on link objects | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandLink.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_LinkImport` | Import Links | Imports selected external links | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkImportAll` | Import All Links | Imports all links of the active document | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkMake` | Make Link |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandLink.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_LinkMakeGroup` | Link Group | Creates a group of links | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandLink.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_LinkMakeRelative` | Make Sub-Link | Creates a sub-object or sub-element link | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkReplace` | Replace With Link | Replaces the selected objects with links | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectActions` | &Link Navigation | Link navigation actions | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandLink.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_LinkSelectAllLinks` | Select &All Links | Selects all links to the current selected object | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectLinked` | &Go to Linked Object | Selects the linked object and switches to its original document | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkSelectLinkedFinal` | Go to &Deepest Linked Object |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_LinkUnlink` | Unlink | Unlinks the object by placing it directly in the container | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandLink.cpp` |
| `Std_MDITest1` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_MDITest2` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_MDITest3` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_MacroAttachDebugger` | &Attach to Remote Debugger | Attaches to a remotely running debugger | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandMacro.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_MacroStartDebug` | &Debug Macro | Starts the debugging of macros | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStepInto` | Step &Into | Steps to the next line executed | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStepOver` | Step &Over | Steps to the next line in this file | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_MacroStopDebug` | &Stop Debugging | Stops the debugging of macros | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_Macro_` |  |  | 被引用 | Menu/Toolbar item | `src/Gui/Command.cpp` |
| `Std_MainFullscreen` | Fullscreen | Displays the main window in fullscreen mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_MassProperties` | Mass Properties | Calculates mass properties of selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/Workbench.cpp`<br>`src/Mod/McStudio/Gui/Command.cpp`<br>+1 more |
| `Std_Measure` | &Measure | Measures a feature | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/Workbench.cpp`<br>`src/Mod/McStudio/Gui/Command.cpp`<br>+1 more |
| `Std_MergeProjects` | &Merge Document | Merges another FreeCAD document into the active one | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_New` | &New Document | Creates a new empty document | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand, runCommandByName | `src/Gui/CommandDoc.cpp`<br>`src/Gui/MainWindow.cpp`<br>+4 more |
| `Std_OnlineHelp` | &Help | Opens the Help documentation | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_OnlineHelpWebsite` | Help Website | Opens the help documentation | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandStd.cpp` |
| `Std_Open` | &Open… | Opens a document or imports files | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand, runCommandByName | `src/Gui/Command.cpp`<br>`src/Gui/CommandDoc.cpp`<br>+4 more |
| `Std_OpenMacrosFolder` | Open Macro Folder | Opens the macros folder in the system file manager | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_OrthographicCamera` | Orthographic View | Switches to orthographic view mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Part` | New Part |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, runCommandByName | `src/Gui/CommandStructure.cpp`<br>`src/Gui/Workbench.cpp`<br>+2 more |
| `Std_Paste` | &Paste | Pastes the contents of the clipboard | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_PerspectiveCamera` | Perspective View | Switches to perspective view mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Placement` | P&lacement |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Print` | &Print | Prints the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_PrintPdf` | Export P&DF | Exports the active document as a PDF file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_PrintPreview` | Print Previe&w | Previews the active document before printing | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ProjectInfo` | Doc&ument Information | Shows information about the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ProjectUtil` | Do&cument Utility | Extracts or creates document files | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Properties` | Propert&ies |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Tree.cpp`<br>+1 more |
| `Std_PythonHelp` | Python &Modules Documentation | Opens the Python Modules documentation | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/OnlineDocumentation.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Quit` | E&xit | Quits the application | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_RandomColor` | Random &Color | Assigns random diffuse colors for the selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandFeat.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_RecallWorkingView` | R&ecall Working View | Recalls a previously stored temporary working view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_RecentFiles` | Open &Recent | Displays the list of recently opened files | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_RecentMacros` | &Recent Macros | Displays the list of recently used macros | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Recompute` | &Recompute | Recomputes a feature or document | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandFeat.cpp` |
| `Std_Redo` | &Redo | Redoes a previously undone action | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_Refresh` | Recompute | Recomputes the active document | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, runCommandByName | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+2 more |
| `Std_ReloadStyleSheet` | &Reload Stylesheet | Reloads the current stylesheet | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandStd.cpp` |
| `Std_ReportBug` | Report an &Issue | Opens the bugtracker to report an issue | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_RestartInSafeMode` | Restart in Safe Mode | Starts FreeCAD without any modules or plugins loaded | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Revert` | Rever&t | Reverts to the saved version of this file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Save` | &Save | Saves the active document | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+2 more |
| `Std_SaveAll` | Sa&ve All | Saves all open documents | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SaveAs` | Save &As… | Saves the active document under a new file name | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SaveCopy` | Save a Cop&y… | Saves a copy of the active document under a new file name | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SceneInspector` | Scene I&nspector | Opens the scene inspector | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SelBack` | Selection &Back |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_SelBoundingBox` | &Bounding Box | Shows selection bounding box | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SelForward` | Selection &Forward |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_SelectAll` | Select &All | Selects all objects in the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SelectVisibleObjects` | &Select Visible Objects | Selects all visible objects in the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SendToPythonConsole` | &Send to Python Console | Sends the selected object to the Python console | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandFeat.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_SetAppearance` | &Appearance | Sets the display properties of the selected object | 已定义 | C++ Command/GroupCommand | `src/Mod/Material/Gui/Command.cpp`<br>`src/Mod/Material/Gui/WorkbenchManipulator.cpp` |
| `Std_SetMaterial` | &Material | Sets the material of the selected object | 已定义 | C++ Command/GroupCommand | `src/Mod/Material/Gui/Command.cpp`<br>`src/Mod/Material/Gui/WorkbenchManipulator.cpp` |
| `Std_ShowObjects` | Show &All Objects | Shows all objects in the document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ShowSelection` | Sho&w Selection | Shows all selected objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_StoreWorkingView` | St&ore Working View | Stores a temporary working view for the current document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test1` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test2` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test3` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test4` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test5` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test6` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandTest.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Test7` |  |  | 被引用 | Menu/Toolbar item | `src/Gui/Workbench.cpp` |
| `Std_Test8` |  |  | 被引用 | Menu/Toolbar item | `src/Gui/Workbench.cpp` |
| `Std_TestCmdFuncs` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestConsoleOutput` | Test Console Output | Runs test cases to verify console messages | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress1` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress2` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress3` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress4` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestProgress5` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestQM` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TestReloadQM` |  |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandTest.cpp` |
| `Std_TextDocument` | Te&xt Document | Adds a text document to the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_TextureMapping` | Text&ure Mapping | Maps textures to shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_TileWindows` | &Tile | Tiles the windows | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToggleBottomPanels` | Toggle Bottom Panels | Toggles the bottom dock panels | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Gui/CommandView.cpp`<br>`src/Gui/MainWindow.cpp`<br>+1 more |
| `Std_ToggleBreakpoint` | Toggle &Breakpoint | Adds or removes a breakpoint at this position | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandMacro.cpp` |
| `Std_ToggleClipPlane` | Clippin&g View | Toggles clipping of the active view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ToggleFreeze` | Toggle Freeze |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandFeat.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ToggleNavigation` | Toggle Navigation/&Edit Mode | Toggles between navigation and edit mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToggleObjects` | To&ggle All Objects | Toggles the visibility of all objects in the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToggleSelectability` | Toggle Se&lectability | Toggles the property of the objects to get selected in the 3D view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToggleSkipRecompute` | Skip Recomputes |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandFeat.cpp` |
| `Std_ToggleToolBarLock` | Lock Toolbars | Locks toolbars so they are no longer moveable | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToggleTransparency` | Toggle Transparenc&y |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ToggleVisibility` | Toggle &Visibility | Toggles the visibility of the selection | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ToolBarMenu` | &Toolbars | Toggles this window | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Transform` | Transform | Transforms the selected object | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandDoc.cpp` |
| `Std_TransformManip` | Trans&form | Transforms the selected object in the 3D view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_TreeCollapse` | Collapse Selected Items | Collapses the currently selected tree items | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeCollapseDocument` | Collapse/E&xpand | Expands the active document and collapses all others | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeDrag` | Initiate &Dragging | Initiates dragging of the currently selected tree items | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeExpand` | Expand Selected Items | Expands the currently selected tree items | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeMultiDocument` | &Multi Document | Displays all documents in the tree view | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreePreSelection` | Preselection |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeRecordSelection` | Record Selection | Records the selection in the tree view in order to go back/forward using the navigation buttons | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeSelectAllInstances` | Select All Instances | Selects all instances of the currently selected object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_TreeSelection` | &Go to Selection | Scrolls to the first selected item | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_TreeSingleDocument` | &Single Document | Displays only the active document in the tree view | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncPlacement` | Sync Placement | Adjusts the placement on drag-and-drop of objects across coordinate systems (e.g. in part contain… | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncSelection` | Sync Selection |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeSyncView` | Sync View |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_TreeViewActions` | Tree View Actions | Tree view behavior options and actions | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Undo` | &Undo | Undoes the previous action | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandDoc.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_UnitsCalculator` | &Units Converter | Starts the units converter | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_UserEditMode` | Edit &Mode | Defines behavior when editing an object from the tree view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_UserInterface` | Dock Views | Docks all top-level views | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandWindow.cpp` |
| `Std_VarSet` | Variable Set | Creates a variable set, which is an object that maintains a set of properties to be used as varia… | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandStructure.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ViewBottom` | Bottom | Sets the camera to the bottom view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewBoxZoom` | &Box Zoom | Activates the box zoom tool | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewCreate` | New 3D View | Opens a new 3D view window for the active document | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewDimetric` | &Dimetric | Sets the camera to the dimetric view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewDock` | &Docked |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_ViewDockUndockFullscreen` | D&ocument Window |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewExample1` | Inventor Example #1 | Shows a 3D texture with manipulator | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewExample2` | Inventor Example #2 | Shows spheres and drag-lights | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewExample3` | Inventor Example #3 | Shows an animated texture | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewFitAll` | &Fit All | Fits all content into the 3D view | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, largeCommand, runCommandByName | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+3 more |
| `Std_ViewFitSelection` | Fit &Selection | Fits the selected content into the 3D view | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, largeCommand | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ViewFront` | Front | Sets the camera to the front view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewFullscreen` | &Fullscreen |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_ViewGroup` | Standard &Views | Changes to a standard view | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ViewHome` | &Home | Sets the camera to the default home view | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, iconCommand, smallCommand | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ViewIsometric` | &Isometric | Sets the camera to the isometric view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewIvIssueCamPos` | Issue Camera &Position |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewLeft` | Left | Sets the camera to the left view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewLoadImage` | &Load Image… | Loads an image | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewRear` | Rear | Sets the camera to the rear view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewRestoreCamera` | Restore Saved Camera | Restores the saved camera settings | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_ViewRight` | Right | Sets the camera to the right view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewRotateLeft` | Rotate &Left | Rotates the view by 90\xc2\xb0 counter-clockwise | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewRotateRight` | Rotates &Right | Rotates the view by 90\xc2\xb0 clockwise | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewSaveCamera` | Save Current Camera | Saves the current camera settings | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_ViewScreenShot` | Save &Image… | Creates a screenshot of the active view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewStatusBar` | Status Bar | Toggles the status bar | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewTop` | Top | Sets the camera to the top view | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, runCommandByName | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp`<br>+1 more |
| `Std_ViewTrimetric` | &Trimetric | Sets the camera to the trimetric view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewUndock` | &Undocked |  | 已定义 | C++ Command/GroupCommand | `src/Gui/CommandView.cpp` |
| `Std_ViewVR` | FreeCAD VR | Extends the FreeCAD 3D Window to a VR device | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewZoomIn` | Zoom &In | Increases the zoom factor by a fixed amount | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_ViewZoomOut` | Zoom &Out | Decreases the zoom factor by a fixed amount | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandView.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_WhatsThis` | &What's This? | Opens the documentation for the selected command | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Windows` | Choose Open &Window | Displays the open windows | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_WindowsMenu` | Activate Window | Activates this window | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandWindow.cpp`<br>`src/Gui/Workbench.cpp` |
| `Std_Workbench` | &Workbench | Switches between workbenches | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Gui/CommandStd.cpp`<br>`src/Gui/Workbench.cpp` |

### 01 McStudio 当前工作台命令（26）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `McStudio_CompAdvancedTools` | 高级工具 |  | McStudio Ribbon | dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompApplicationMenu` | 文件 |  | McStudio Ribbon | dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompDressUpFeatures` | 修饰特征 | Create fillet, chamfer, draft, or thickness features | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompEditTools` | 编辑工具 |  | McStudio Ribbon | smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompEvaluateInspect` | Measure and Inspect | Measure model properties and inspect geometry | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompHelp` | 帮助 |  | McStudio Ribbon | dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompMacroTools` | 宏 / 自动化 |  | McStudio Ribbon | dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompPanelWindowTools` |  |  | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompSectionTools` | Section Tools | Create sections, cross sections, section cuts, or clipping planes | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompStandardViews` |  |  | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompSurfaceCreate` | Create Surface | Create filling, fill surface, sections, loft, sweep, or ruled surface features | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompSurfaceEdit` | Edit Surface | Cut, extend, blend, offset, thicken, or project surfaces | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompTransformFeatures` | 阵列 / 镜像 | Create mirror, linear pattern, polar pattern, or multi-transform features | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompViewDisplay` |  |  | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompViewNavigate` |  |  | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CompViewVisibility` |  |  | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_CreateFixture` | 夹具 | Create a McStudio fixture placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_CreateReferencePlane` | 参考平面 | Create a reference plane using the Part Design datum plane command | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_CreateSetupDatum` | 加工基准 | Create a setup datum using the Part Design datum plane command | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_CreateStock` | 毛坯 | Create a McStudio stock placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_CreateWorkpiece` | 工件 | Create a McStudio workpiece placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_ExportSetupSheet` | 设置单 | Export a McStudio setup sheet placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_GenerateBOM` | BOM | Generate a McStudio BOM placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_GenerateProcessReport` | 工艺报告 | Generate a McStudio process report placeholder | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |
| `McStudio_SetFixtureLocation` | Set Fixture Location | Set fixture location placeholder | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, smallCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp` |
| `McStudio_SetWorkCoordinateSystem` | 工件坐标系 | Create a coordinate system using the Part Design datum coordinate system command | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/McStudio/Gui/RibbonModel.cpp`<br>+1 more |

### 10 PartDesign 特征建模（43）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `PartDesign_AdditiveHelix` | Additive Helix |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_AdditiveLoft` | Additive Loft |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_AdditivePipe` | Additive Pipe |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Body` | New Body | Creates a new body and activates it | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+3 more |
| `PartDesign_Boolean` | Boolean Operation |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Chamfer` | Chamfer | Applies a chamfer to the selected edges or faces | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Clone` | Clone | Copies a solid object parametrically as the base feature of a new body | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_CompDatums` | Create Datum | Creates a datum object or local coordinate system | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_CompPrimitiveAdditive` | Additive Primitive | Creates an additive primitive | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/CommandPrimitive.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_CompPrimitiveSubtractive` | Subtractive Primitive | Creates a subtractive primitive | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/CommandPrimitive.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_CompSketches` | Create Datum | Creates a datum object or local coordinate system | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_CoordinateSystem` | Local Coordinate System | Creates a new local coordinate system | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_Draft` | Draft | Applies a draft to the selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_DuplicateSelection` | Duplicate &Object | Duplicates the selected object and adds it to the active body | 已定义 | C++ Command/GroupCommand | `src/Mod/PartDesign/Gui/CommandBody.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_Fillet` | Fillet | Applies a fillet to the selected edges or faces | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Groove` | 旋转切除 |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Hole` | 孔 | Creates holes in the active body at the center points of circles or arcs of the selected sketch o… | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+2 more |
| `PartDesign_InvoluteGear` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/InitGui.py` |
| `PartDesign_Line` | Datum Line | Creates a new datum line | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_LinearPattern` | Linear Pattern |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Migrate` | Migrate | Migrates the document to the modern Part Design workflow | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/PartDesign/Gui/CommandBody.cpp`<br>`src/Mod/PartDesign/Gui/WorkflowManager.cpp` |
| `PartDesign_Mirrored` | Mirror | Mirrors the selected features or active body | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_MoveFeature` | Move Object To… | Moves the selected object to another body | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/CommandBody.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_MoveFeatureInTree` | Move Feature After… | Moves the selected feature after another feature in the same body | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/CommandBody.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_MoveTip` | Set Tip | Moves the tip of the body to the selected feature | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/CommandBody.cpp`<br>+1 more |
| `PartDesign_MultiTransform` | Multi-Transform |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_NewSketch` | New Sketch | Creates a new sketch | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Pad` | 凸台 | Extrudes the selected sketch or profile and adds it to the body | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+2 more |
| `PartDesign_Plane` | Datum Plane | Creates a new datum plane | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_Pocket` | 切除 | Extrudes the selected sketch or profile and removes it from the body | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+2 more |
| `PartDesign_Point` | Datum Point | Creates a new datum point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_PolarPattern` | Polar Pattern |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Revolution` | Revolve |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Scaled` | Scale | Scales the selected features or the active body | 已定义 | C++ Command/GroupCommand | `src/Mod/PartDesign/Gui/Command.cpp` |
| `PartDesign_ShapeBinder` | Shape Binder | Creates a new shape binder | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_Sprocket` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/InitGui.py` |
| `PartDesign_SubShapeBinder` | Sub-Shape Binder |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_SubtractiveHelix` | Subtractive Helix |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Workbench.cpp` |
| `PartDesign_SubtractiveLoft` | Subtractive Loft |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_SubtractivePipe` | Subtractive Pipe |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_Thickness` | Thickness | Applies thickness and removes the selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+1 more |
| `PartDesign_WizardShaft` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/WizardShaft/WizardShaft.py` |
| `PartDesign_WizardShaftCallBack` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/PartDesign/WizardShaft/WizardShaft.py` |

### 11 Sketcher 草图（138）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Sketcher_ArcOverlay` | Toggle Circular Helper for Arcs | Toggles the visibility of the circular helpers for all arcs | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_BSplineComb` | Toggle B-Spline Curvature Comb | Toggles the visibility of the curvature comb for all B-splines | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplineConvertToNURBS` | Geometry to B-Spline | Converts the selected geometry to B-splines | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_BSplineDecreaseDegree` | Decrease B-Spline Degree | Decreases the degree of the B-spline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_BSplineDecreaseKnotMultiplicity` | Decrease Knot Multiplicity | Decreases the multiplicity of the selected knot of a B-spline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_BSplineDegree` | Toggle B-Spline Degree | Toggles the visibility of the degree for all B-splines | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplineIncreaseDegree` | Increase B-Spline Degree | Increases the degree of the B-spline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_BSplineIncreaseKnotMultiplicity` | Increase Knot Multiplicity | Increases the multiplicity of the selected knot of a B-spline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_BSplineInsertKnot` | Insert Knot |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_BSplineKnotMultiplicity` | Toggle B-Spline Knot Multiplicity | Toggles the visibility of the knot multiplicity for all B-splines | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplinePoleWeight` | Toggle B-Spline Control Point Weight | Toggles the visibility of control point weights for all B-splines | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_BSplinePolygon` | Toggle B-Spline Control Polygon | Toggles the visibility of the control polygons for all B-splines | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp` |
| `Sketcher_CancelSketch` | Cancel Editing | Leaves 'edit' mode and reverts any changes | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/Command.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CarbonCopy` | Carbon Copy | Copies the geometry of another sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ChangeDimensionConstraint` | Edit Value | Edits the value of a dimensional constraint | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp` |
| `Sketcher_CompBSplineShowHideGeometryInformation` | Toggle B-Spline Information Layer | Toggles the visibility of the information layer for all B-splines | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherOverlay.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompConstrainRadDia` | Radius/Diameter Dimension | Constrains the radius or diameter of an arc or a circle | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompConstrainTools` | Constrain | Constrain tools | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp` |
| `Sketcher_CompCopy` | Clone | Creates a clone of the geometry taking as reference the last selected point | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_CompCreateArc` | Arc | Creates an arc | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompCreateBSpline` | B-Spline | Creates a B-spline curve defined by control points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompCreateConic` | Conic | Creates a conic | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompCreateFillets` | Fillet/Chamfer | Creates a fillet or chamfer between 2 lines | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompCreateRectangles` | Rectangle | Creates a rectangle | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompCreateRegularPolygon` | Polygon | Creates a regular polygon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompCurveEdition` | Edit Edges | Edge editing tools | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompDimensionTools` | Dimension | Dimension tools | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+2 more |
| `Sketcher_CompExternal` | External Geometry |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompHorVer` | Horizontal/Vertical Constraint | Constrains the selected elements either horizontally or vertically | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallDropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+1 more |
| `Sketcher_CompLine` | Polyline | Creates a continuous polyline | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, dropDownCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>+1 more |
| `Sketcher_CompModifyKnotMultiplicity` | Modify Knot Multiplicity | Modifies the multiplicity of the selected knot of a B-spline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompSlot` | Slot | Slot tools | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CompToggleConstraints` | Toggle Constraints | Toggle constrain tools | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainAngle` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainBlock` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ConstrainCoincident` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainCoincidentUnified` |  |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+2 more |
| `Sketcher_ConstrainDiameter` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainDistance` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainDistanceX` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainDistanceY` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainEqual` |  |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+2 more |
| `Sketcher_ConstrainGroup` | Group Constraint |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainHorVer` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ConstrainHorizontal` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ConstrainLock` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainParallel` |  |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+2 more |
| `Sketcher_ConstrainPerpendicular` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ConstrainPointOnObject` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainRadiam` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainRadius` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainSnellsLaw` | Refraction Constraint | Constrains the selected elements based on the refraction law (Snell's Law) | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_ConstrainSymmetric` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ConstrainTangent` |  |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>+2 more |
| `Sketcher_ConstrainVertical` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CopyClipboard` | C&opy Elements | Copies the selected geometries and constraints to the clipboard | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Create3PointArc` | Arc From 3 Points | Creates an arc defined by 2 end points and 1 point on the arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Create3PointCircle` | Circle From 3 Points | Creates a circle from 3 perimeter points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateArc` | Arc From Center | Creates an arc defined by a center point and an end point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateArcOfEllipse` | Elliptical Arc | Creates an elliptical arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateArcOfHyperbola` | Hyperbolic Arc | Creates a hyperbolic arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateArcOfParabola` | Parabolic Arc | Creates a parabolic arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateArcSlot` | Arc Slot | Creates an arc slot | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateBSpline` | B-Spline | Creates a B-spline curve defined by control points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateBSplineByInterpolation` | B-Spline From Knots | Creates a B-spline from knots, i.e. from interpolation | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateChamfer` | Chamfer | Creates a chamfer between 2 selected lines or at coincident points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateCircle` | Circle From Center | Creates a circle from a center and rim point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateDraftLine` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateEllipseBy3Points` | Ellipse From 3 Points | Creates an ellipse from 3 points on its perimeter | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateEllipseByCenter` | Ellipse From Center | Creates an ellipse from a center and rim point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateFillet` | Fillet | Creates a fillet between 2 selected lines or at coincident points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateHeptagon` | Heptagon | Creates a heptagon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateHexagon` | Hexagon | Creates a hexagon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateLine` | Line | Creates a line | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateOblong` | Rounded Rectangle | Creates a rounded rectangle from 2 corner points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateOctagon` | Octagon | Creates an octagon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreatePentagon` | Pentagon | Creates a pentagon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreatePeriodicBSpline` | Periodic B-Spline | Creates a periodic B-spline curve defined by control points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreatePeriodicBSplineByInterpolation` | Periodic B-Spline From Knots | Creates a periodic B-spline defined by knots using interpolation | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreatePoint` | Point | Creates a point | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreatePolyline` | Polyline | Creates a polyline in the sketch. M key cycles through segment modes. | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreatePolylineLegacy` | Polyline |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp` |
| `Sketcher_CreateRectangle` | Rectangle | Creates a rectangle from 2 corner points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_CreateRectangle_Center` | Centered Rectangle | Creates a centered rectangle from a center and a corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateRegularPolygon` | Polygon | Creates a regular polygon from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateSlot` | Slot | Creates a slot | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateSquare` | Square | Creates a square from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateText` | Text |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_CreateTriangle` | Triangle | Creates an equilateral triangle from a center and corner point | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Cut` | C&ut Elements | Cuts the selected geometries and constraints to the clipboard | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_DeleteAllConstraints` | Delete All Constraints | Deletes all constraints in the sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_DeleteAllGeometry` | Delete All Geometry |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Dimension` | Dimension | Constrains contextually based on the selection. The type can be changed with the M key. | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_EditSketch` | Edit Sketch | Opens the selected sketch for editing | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+3 more |
| `Sketcher_Extend` | Extend Edge | Extends an edge with respect to the selected position | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Grid` | Toggle Grid | Toggles the grid display in the active sketch | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_Intersection` | External Intersection | Creates the intersection of external geometry with the sketch plane | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_JoinCurves` | Join Curves | Joins 2 curves at selected end points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherBSpline.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_LeaveGroup` | Leave | Leaves the sketch editing mode | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_LeaveSketch` | Leave Sketch | Finishes editing the active sketch. Press Escape to exit. | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, largeCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+3 more |
| `Sketcher_MapSketch` | Attach Sketch |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Workbench.cpp`<br>`src/Mod/PartDesign/Gui/Command.cpp`<br>+3 more |
| `Sketcher_MergeSketches` | Merge Sketches | Creates a new sketch by merging at least 2 selected sketches | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/Sketcher/Gui/Command.cpp`<br>+1 more |
| `Sketcher_MirrorSketch` | Mirror Sketch |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/Sketcher/Gui/Command.cpp`<br>+1 more |
| `Sketcher_NewSketch` | New Sketch | Creates a new sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Workbench.cpp`<br>`src/Mod/Sketcher/Gui/Command.cpp`<br>+1 more |
| `Sketcher_Offset` | Offset | Adds an equidistant closed contour around selected geometry: positive values offset outward, nega… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Paste` | P&aste Elements | Pastes the geometries and constraints from the clipboard into the sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ProfilesHexagon1` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Sketcher/Gui/Workbench.cpp`<br>`src/Mod/Sketcher/Profiles.py` |
| `Sketcher_Projection` | External Projection | Creates the projection of external geometry in the sketch plane | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_RectangularArray` | Rectangular Array |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp` |
| `Sketcher_RemoveAxesAlignment` | Remove Axes Alignment |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_RenderingOrder` | Rendering Order | Reorders items in the rendering order | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_ReorientSketch` | Reorient Sketch |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/PartDesign/Gui/Workbench.cpp`<br>`src/Mod/Sketcher/Gui/Command.cpp`<br>+1 more |
| `Sketcher_RestoreInternalAlignmentGeometry` | Toggle Internal Geometry | Toggles the visibility of all internal geometry | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Rotate` | Rotate / Polar Transform | Rotates the selected geometry by creating 'n' copies, enabling circular pattern creation | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Scale` | Scale | Scales the selected geometries | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_SelectConflictingConstraints` | Select Conflicting Constraints | Selects all conflicting constraints | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/TaskSketcherMessages.cpp`<br>+1 more |
| `Sketcher_SelectConstraints` | Select Associated Constraints | Selects the constraints associated with the selected geometrical elements | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_SelectElementsAssociatedWithConstraints` | Select Associated Geometry | Selects the geometrical elements associated with the selected constraints | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_SelectElementsWithDoFs` | Select Under-Constrained Elements |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/TaskSketcherMessages.cpp`<br>+1 more |
| `Sketcher_SelectHorizontalAxis` | Select Horizontal Axis | Selects the local horizontal axis of the sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_SelectMalformedConstraints` | Select Malformed Constraints | Selects all malformed constraints | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/TaskSketcherMessages.cpp` |
| `Sketcher_SelectOrigin` | Select Origin | Selects the local origin point of the sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_SelectPartiallyRedundantConstraints` | Select Partially Redundant Constraints | Selects all partially redundant constraints | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/TaskSketcherMessages.cpp` |
| `Sketcher_SelectRedundantConstraints` | Select Redundant Constraints | Selects all redundant constraints | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/TaskSketcherMessages.cpp`<br>+1 more |
| `Sketcher_SelectVerticalAxis` | Select Vertical Axis | Selects the local vertical axis of the sketch | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Snap` | Toggle Snap | Toggles snapping | 已定义 | C++ Command/GroupCommand | `src/Mod/Sketcher/Gui/Command.cpp` |
| `Sketcher_Split` | Split Edge | Splits an edge into 2 segments while preserving constraints | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_StopOperation` | Stop Operation | Stops the active operation while in edit mode | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/Command.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_SwitchVirtualSpace` | Switch Virtual Space |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherVirtualSpace.cpp`<br>`src/Mod/Sketcher/Gui/Workbench.cpp` |
| `Sketcher_Symmetry` | Mirror | Creates a mirrored copy of the selected geometry | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ToggleActiveConstraint` | Toggle Constraints | Toggles the state of the selected constraints | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ToggleConstruction` | Toggle Construction Geometry | Toggles between defining geometry and construction geometry modes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandAlterGeometry.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ToggleDrivingConstraint` | Toggle Driving/Reference Constraints | Toggles between driving and reference mode of the selected constraints and commands | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandConstraints.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Translate` | Move / Array Transform | Translates the selected geometries and enables the creation of 'i' * 'j' copies | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandSketcherTools.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_Trimming` | Trim Edge | Trims an edge with respect to the selected position | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/Sketcher/Gui/CommandCreateGeo.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ValidateSketch` | Validate Sketch |  | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/McStudio/Gui/Workbench.cpp`<br>+3 more |
| `Sketcher_ViewSection` | Toggle Section View | Toggles between section view and full view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/Command.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |
| `Sketcher_ViewSketch` | Align View to Sketch | Aligns the camera orientation perpendicular to the active sketch plane | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Sketcher/Gui/Command.cpp`<br>`src/Mod/Sketcher/Gui/ViewProviderSketch.cpp`<br>+1 more |

### 12 Part 几何/实体（77）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Part_Boolean` | Boolean Operation | Applies a boolean operation with the selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_BooleanFragments` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/SplitFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Box` | Cube | Creates a solid cube | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandParametric.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Box2` | Box Fix 1 | Creates a solid box | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Box3` | Box Fix 2 | Creates a solid box | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp` |
| `Part_BoxSelection` | Box Selection | Selects elements in the 3D view using a box selection | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Builder` | Shape Builder | Advanced utility to create shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Chamfer` | Chamfer | Chamfers the selected edges of a shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CheckGeometry` | Check Geometry | Analyzes the selected shapes for errors | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+2 more |
| `Part_ColorPerFace` | Appearance per &Face | Sets the appearance of individual faces of the selected object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp`<br>+1 more |
| `Part_Common` | Intersection | Intersects the selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CompCompoundTools` | Compound Tools | Compound tools for working with multiple shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CompJoinFeatures` | Join Shapes | Joins the selected walled shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CompOffset` | Offset | Tools to offset shapes (construct parallel shapes) | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CompSplitFeatures` | Split Shapes | Shape splitting and compsolid creation tools | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Compound` | Compound | Compounds the selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CompoundFilter` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/CompoundTools/_CommandCompoundFilter.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Cone` | Cone | Creates a solid cone | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandParametric.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_CoordinateSystem` | Coordinate System | Creates a coordinate system that can be attached to other objects | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/Part/Gui/Command.cpp` |
| `Part_CrossSections` | Cross-Sections | Creates cross-sections | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Cut` | Cut | Cuts 2 selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Cylinder` | Cylinder | Creates a solid cylinder | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandParametric.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_DatumLine` | Datum Line | Creates a datum line that can be attached to other objects | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/Part/Gui/Command.cpp` |
| `Part_DatumPlane` | Datum Plane | Creates a datum plane that can be attached to other objects | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/Part/Gui/Command.cpp` |
| `Part_DatumPoint` | Datum Point | Creates a datum point that can be attached to other objects | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Datums` | Datums | Creates a datum object (coordinate system, plane, line, or point) that can be attached to other o… | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/WorkbenchManipulator.cpp` |
| `Part_Defeaturing` | Defeaturing | Removes the selected features from a shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandSimple.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_EdgeSelection` | Edge Selection | Only allows the selection of edges | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_EditAttachment` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Part/AttachmentEditor/Commands.py`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_ElementCopy` | Shape Element Copy | Creates a non-parametric copy of the selected shape element | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandSimple.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_ExplodeCompound` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/CompoundTools/_CommandExplodeCompound.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Export` | Export CAD File | Exports to a CAD file | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Part/Gui/Command.cpp` |
| `Part_Extrude` | Extrude | Extrudes the selected sketch or profile | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_FaceSelection` | Face Selection | Only allows the selection of faces | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_Fillet` | Fillet | Fillets the selected edges of a shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Fuse` | Union | Unites the selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Import` | Import CAD File | Imports a CAD file | 已定义 / McStudio Ribbon | C++ Command/GroupCommand, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Part/Gui/Command.cpp` |
| `Part_ImportCurveNet` | Import Curve Network | Imports a curve network | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp` |
| `Part_ImportStep` |  |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Import/Gui/Command.cpp` |
| `Part_JoinConnect` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/JoinFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_JoinCutout` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/JoinFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_JoinEmbed` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/JoinFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Loft` | Loft | Lofts the selected profiles | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_MakeFace` | Face From Wires | Creates a face from the selected wires (e.g. from a sketch) | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_MakeSolid` | Convert to Solid | Converts the selected shell or compound to a solid | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Mirror` | Mirror | Mirrors the selected shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_NewDoc` |  |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp` |
| `Part_Offset` | 3D Offset | Offsets shapes in 3D | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item, runCommandByName | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Offset2D` | 2D Offset | Offsets planar shapes in 2D | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_PickCurveNet` | Pick Curve Network | Picks a curve network | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/Command.cpp` |
| `Part_PointsFromMesh` | Points From Shape | Creates distributed points from the selected shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandSimple.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Primitives` | Primitive | Creates solid geometric primitives parametrically | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_ProjectionOnSurface` | Project on Surface |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_RefineShape` | Refine Shape | Creates a refined copy of the selected shapes | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Part/Gui/CommandSimple.cpp`<br>+1 more |
| `Part_RemoveSelectionGate` | No Selection Filters | Clears all selection filters | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_ReverseShape` | Reverse Shapes | Reverses the orientation of the selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Revolve` | Revolve | Revolves the selected shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_RuledSurface` | Ruled Surface | Creates a ruled surface between 2 selected wires | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Scale` | Scale | Scales the selected shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Section` | Section | Sections 2 selected shapes | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_SectionCut` | Persiste&nt Section Cut |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_SelectFilter` | Selection Filter | Changes the selection filter | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/CommandFilter.cpp`<br>`src/Mod/Part/Gui/WorkbenchManipulator.cpp` |
| `Part_ShapeFromMesh` | Shape From Mesh | Creates a shape from the selected mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandSimple.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_ShapeInfo` |  |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp` |
| `Part_SimpleCopy` | Simple Copy | Creates a simple non-parametric copy of the selected shapes | 已定义 / 被引用 / McStudio Ribbon | C++ Command/GroupCommand, Menu/Toolbar item, smallCommand | `src/Mod/McStudio/Gui/RibbonModel.cpp`<br>`src/Mod/Part/Gui/CommandSimple.cpp`<br>+1 more |
| `Part_SimpleCylinder` | Cylinder | Creates a solid cylinder | 已定义 | C++ Command/GroupCommand | `src/Mod/Part/Gui/CommandSimple.cpp` |
| `Part_Slice` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/SplitFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_SliceApart` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/SplitFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Sphere` | Sphere | Creates a solid sphere | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandParametric.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Sweep` | Sweep | Sweeps profiles along a wire | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_Thickness` | Thickness |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |
| `Part_ToleranceSet` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Part/BOPTools/ToleranceFeatures.py`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Torus` | Torus | Creates a solid torus | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandParametric.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_TransformedCopy` | Transformed Copy |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Part/Gui/CommandSimple.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_Tube` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Part/BasicShapes/CommandShapes.py`<br>`src/Mod/Part/Gui/Workbench.cpp` |
| `Part_VertexSelection` | Vertex Selection | Only allows the selection of vertices | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Part/Gui/CommandFilter.cpp` |
| `Part_XOR` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Part/BOPTools/SplitFeatures.py`<br>`src/Mod/Part/Gui/Command.cpp`<br>+1 more |

### 13 Surface 曲面（7）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Surface_BlendCurve` | Blend Curve | Joins 2 edges with continuity | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |
| `Surface_CurveOnMesh` | Curve on Mesh |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Surface/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Workbench.cpp` |
| `Surface_Cut` | Surface Cut | Cuts one shape using another | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |
| `Surface_ExtendFace` | Extend Face |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |
| `Surface_Filling` | Filling |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |
| `Surface_GeomFillSurface` | Fill Boundary Curves | Creates a surface from 2, 3, or 4 boundary edges | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |
| `Surface_Sections` | Sections | Creates a surface from a series of sectional edges | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/McStudio/Gui/Command.cpp`<br>`src/Mod/Surface/Gui/Command.cpp`<br>+1 more |

### 14 Mesh 网格（38）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Mesh_AddFacet` | Add Triangle | Adds a triangle manually to a mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_BoundingBox` | Bounding Box Info | Shows the bounding box coordinates of the selected mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_BuildRegularSolid` | Regular Solid | Builds a regular solid | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_CrossSections` | Cross-Sections | Creates cross-sections of the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_CurvatureInfo` | Curvature Info | Displays information about the curvature | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp`<br>+1 more |
| `Mesh_Decimating` | Decimate | Decimates a mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Demolding` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Difference` | Difference | Creates a boolean difference of the selected meshes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_EvaluateFacet` | Face Info | Displays information about the selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_EvaluateSolid` | Evaluate Solid | Checks whether the mesh is a solid | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Evaluation` | Evaluate and Repair | Opens a dialog to analyze and repair a mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Export` | Export Mesh… | Exports a mesh to a file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_FillInteractiveHole` | Close Hole | Closes a hole interactively in the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_FillupHoles` | Fill Holes | Fills holes in the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_FlipNormals` | Flip Normals | Flips the normals of the selected mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_FromGeometry` | Mesh From Geometry | Creates a mesh from the selected geometry | 已定义 | C++ Command/GroupCommand | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_FromPartShape` | Mesh From Shape | Tessellates the selected shape to a mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_HarmonizeNormals` | Harmonize Normals | Harmonizes the normals of the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Import` | Import Mesh… | Imports a mesh from a file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Intersection` | Intersection | Creates a boolean intersection from the selected meshes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Merge` | Merge | Merges selected meshes into one | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_PolyCut` | Cut | Cuts the mesh with a selected polygon | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_PolySegm` | Segment | Creates a mesh segment | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_PolySplit` | Split | Splits a mesh into 2 meshes | 已定义 | C++ Command/GroupCommand | `src/Mod/Mesh/Gui/Command.cpp` |
| `Mesh_PolyTrim` | Trim | Trims a mesh with a selected polygon | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_RemeshGmsh` | Refinement | Refines an existing mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp`<br>+1 more |
| `Mesh_RemoveCompByHand` | Remove Components Manually | Marks a component to remove it from the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_RemoveComponents` | Remove Components | Removes topologically independent components from the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Scale` | Scale | Scales the selected mesh objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_SectionByPlane` | Section From Plane | Sections the mesh with the selected plane | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Segmentation` | Segmentation | Creates new mesh segments from the mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_SegmentationBestFit` | Segmentation From Best-Fit Surfaces | Creates new mesh segments from the best-fit surfaces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Smoothing` | Smooth | Smoothes the selected meshes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_SplitComponents` | Split by Components | Splits the selected mesh into its components | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Transform` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_TrimByPlane` | Trim With Plane | Trims a mesh by removing faces on one side of a selected plane | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_Union` | Union | Unifies the selected meshes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp` |
| `Mesh_VertexCurvature` | Curvature Plot | Calculates the curvature of the vertices of a mesh | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Mesh/Gui/Command.cpp`<br>`src/Mod/Mesh/Gui/Workbench.cpp`<br>+1 more |

### 15 MeshPart 网格转实体（7）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `MeshPart_CreateFlatFace` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Mesh/Gui/Workbench.cpp`<br>`src/Mod/MeshPart/Gui/MeshFlatteningCommand.py` |
| `MeshPart_CreateFlatMesh` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Mesh/Gui/Workbench.cpp`<br>`src/Mod/MeshPart/Gui/MeshFlatteningCommand.py` |
| `MeshPart_CrossSections` | Cross-Sections | Applies cross-sections to the mesh | 已定义 | C++ Command/GroupCommand | `src/Mod/MeshPart/Gui/Command.cpp` |
| `MeshPart_CurveOnMesh` | Curve on Mesh | Creates an approximated curve on top of a mesh object | 已定义 | C++ Command/GroupCommand | `src/Mod/MeshPart/Gui/Command.cpp` |
| `MeshPart_Mesher` | Mesh From Shape | Tessellate shape | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/MeshPart/Gui/Command.cpp`<br>`src/Mod/MeshPart/Gui/Workbench.cpp` |
| `MeshPart_SectionByPlane` | Section | Creates a section from a mesh and plane | 已定义 | C++ Command/GroupCommand | `src/Mod/MeshPart/Gui/Command.cpp` |
| `MeshPart_TrimByPlane` | Trim Mesh | Trims a mesh with a plane | 已定义 | C++ Command/GroupCommand | `src/Mod/MeshPart/Gui/Command.cpp` |

### 20 TechDraw 工程图（130）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `TechDraw_2LineCenterLine` | Centerline Between 2 Lines | Adds a centerline between 2 selected lines | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_2PointCenterLine` | Centerline Between 2 Points | Adds a centerline between 2 selected points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_2PointCosmeticLine` | Cosmetic Line Through 2 Points | Adds a cosmetic line that passes through 2 selected points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_3PtAngleDimension` | Angle Dimension From 3 Points | Inserts an angle dimension between 3 selected points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ActiveView` | Active View |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_AlignVertexesHorizontally` | Align Vertices/Edge Horizontally | Aligns the selected vertices or edges horizontally to the view rotation | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAlign.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_AlignVertexesVertically` | Align Vertices/Edge Vertically | Aligns the selected vertices or edges vertically to the view rotation | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAlign.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_AngleDimension` | Angle Dimension | Inserts an angle dimension between two edges | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Annotation` | Text Annotation | Inserts an editable text block annotation to the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ArchView` | BIM View | Inserts a view of a BIM section plane | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_AreaDimension` | Area Annotation | Inserts an annotation showing the area of a selected face | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_AxoLengthDimension` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandAxoLengthDimension.py` |
| `TechDraw_Balloon` | Balloon Annotation | Inserts a new balloon annotation in the selected view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_BrokenView` | Broken View | Inserts a new broken view for the selected objects or base view and break definition objects | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_CenterLineGroup` | Centerline | Inserts a centerline to a face, or between 2 lines or edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ClipGroup` | Clip Group | Inserts a new clip group for the selected view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ClipGroupAdd` | Add View To Clip Group | Adds the selected view to a clip group | 已定义 | C++ Command/GroupCommand | `src/Mod/TechDraw/Gui/Command.cpp` |
| `TechDraw_ClipGroupRemove` | Remove From Clip Group | Removes a view based on the selected clip group | 已定义 | C++ Command/GroupCommand | `src/Mod/TechDraw/Gui/Command.cpp` |
| `TechDraw_CommandAddOffsetVertex` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandVertexCreations.py` |
| `TechDraw_CommandVertexCreationGroup` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandVertexCreations.py` |
| `TechDraw_CompDimensionTools` | Dimension | Dimension tools | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ComplexSection` | Complex Section View | Inserts a complex section view based on the selected view in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_CosmeticCircle` | Cosmetic 1 Point Circle | Adds a cosmetic circle based on a selected centerpoint | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_CosmeticEraser` | Remove Cosmetic Object | Removes the selected cosmetic object from the page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_CosmeticVertex` | Cosmetic Vertex | Adds a cosmetic vertex | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_CosmeticVertexGroup` | Cosmetic Vertex | Inserts a cosmetic vertex | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_DecorateLine` | Edit Line Appearance | Opens the 'Line decoration' dialog to edit the selected lines | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_DetailView` | Detail View | Inserts a new detail view based on the selected view in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_DiameterDimension` | Diameter Dimension | Inserts a diameter dimension of a circular edge or arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Dimension` | Dimension |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_DimensionRepair` | Repair Dimension References | Repairs broken or incorrect dimension references | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_DraftView` | Draft View | Inserts a view of a Draft object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExportPageDXF` | Export Page as DXF | Exports the current page as a DXF | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExportPageSVG` | Export Page as SVG | Exports the current page as an SVG | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionArcLengthAnnotation` | Arc Length Annotation | Inserts an annotation with the calculated arc length of the selected edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionAreaAnnotation` | Area Annotation | Calculates the area of multiple selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCascadeDimensionGroup` | Cascade Horizontal Dimensions | Evenly spaces the selected horizontal dimensions:<br>\
- Specify the cascade spacing (optional)<b… | 已定义 | C++ Command/GroupCommand | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp` |
| `TechDraw_ExtensionCascadeHorizDimension` | Cascade Horizontal Dimensions | Evenly spaces the selected horizontal dimensions:<br>\
- Specify the cascade spacing (optional)<b… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCascadeObliqueDimension` | Cascade Oblique Dimensions | Evenly spaces the selected oblique dimensions:<br>\
- Specify the cascade spacing (optional)<br>\… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCascadeVertDimension` | Cascade Vertical Dimensions | Evenly spaces the selected vertical dimensions:<br>\
- Specify the cascade spacing (optional)<br>… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionChamferDimensionGroup` | Horizontal Chamfer Dimension | Inserts a horizontal size and angle dimension for a chamfer from 2 selected vertices | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionChangeLineAttributes` | Change Line Attributes | Changes the selected cosmetic lines and centerlines to the specified attributes | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCircleCenterLines` | Circle Centerlines | Adds centerlines to the selected circles and arcs | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCircleCenterLinesGroup` | Circle Centerlines | Adds centerlines to selected circles and arcs | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCreateChainDimensionGroup` | Horizontal Chain Dimension |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCreateCoordDimensionGroup` | Horizontal Coordinate Dimension | Adds evenly spaced horizontal dimensions between 3 or more vertices aligned to a shared baseline | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionCreateHorizChainDimension` | Horizontal Chain Dimension |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateHorizChamferDimension` | Horizontal Chamfer Dimension | Inserts a horizontal size and angle dimension for a chamfer from 2 selected vertices | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateHorizCoordDimension` | Horizontal Coordinate Dimension | Adds evenly spaced horizontal dimensions between 3 or more vertices aligned to a shared baseline | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateLengthArc` | Arc Length Dimension | Inserts an arc length dimension to the selected arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateObliqueChainDimension` | Oblique Chain Dimension |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateObliqueCoordDimension` | Oblique Coordinate Dimension | Adds evenly spaced oblique dimensions between 3 or more vertices aligned to a shared baseline | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateVertChainDimension` | Vertical Chain Dimension |  | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateVertChamferDimension` | Vertical Chamfer Dimension | Inserts a vertical size and angle dimension for a chamfer from 2 selected vertices | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCreateVertCoordDimension` | Vertical Coordinate Dimension | Adds evenly spaced vertical dimensions between 3 or more vertices aligned to a shared baseline | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>+1 more |
| `TechDraw_ExtensionCustomizeFormat` | Customize Format Label | Customizes the format label of a selected dimension or balloon | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionDecreaseDecimal` | Decrease Decimal Places | Decreases the number of decimal places of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionDrawCirclesGroup` | Cosmetic 1 Point Circle | Adds a cosmetic circle based on two vertices, where the first selection is the centerpoint and th… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionDrawCosmArc` | Cosmetic Arc |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionDrawCosmCircle` | Cosmetic 2 Point Circle | Adds a cosmetic circle based on two selected vertices, where the first is the center point and th… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionDrawCosmCircle3Points` | Cosmetic 3 Point Circle | Adds a cosmetic circle that passes through 3 selected perimeter points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionExtendLine` | Extend Line | Extends a selected cosmetic line or centerline at both ends by the specified delta distance | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionExtendShortenLineGroup` | Extend Line | Extends a selected cosmetic line or centerline at both ends by the specified delta distance | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionHoleCircle` | Bolt Circle Centerlines | Adds centerlines to a circular pattern of three or more selected circles | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionIncreaseDecimal` | Increase Decimal Places | Increases the number of decimal places of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionIncreaseDecreaseGroup` | Increase Decimal Places | Increases the number of decimal places of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionInsertDiameter` | Insert '⌀' Prefix | Inserts a '⌀' symbol at the beginning of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionInsertPrefixGroup` | Insert '⌀' Prefix | Inserts a '⌀' symbol at the beginning of the dimension text | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionInsertRepetition` | Insert 'n×' Prefix | Inserts a repeated feature count at the beginning of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionInsertSquare` | Insert '□' Prefix | Inserts a '□' symbol at the beginning of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionLinePPGroup` | Cosmetic Parallel Line | Adds a cosmetic line parallel to the selected line through the selected vertex | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionLineParallel` | Cosmetic Parallel Line | Adds a cosmetic circle to 3 selected vertices | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionLinePerpendicular` | Cosmetic Perpendicular Line | Adds a cosmetic line perpendicular to the selected line through the selected vertex | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionLockUnlockView` | Toggle View Lock | Locks or unlocks the position of the selected views | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionPosChainDimensionGroup` | Align Horizontal Chain Dimensions | Aligns the horizontal dimensions to create a chain dimension:<br>\
- Select two or more horizonta… | 已定义 | C++ Command/GroupCommand | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp` |
| `TechDraw_ExtensionPosHorizChainDimension` | Align Horizontal Chain Dimensions | Aligns the horizontal dimensions to create a chain dimension:<br>\
- Select two or more horizonta… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionPosObliqueChainDimension` | Align Oblique Chain Dimensions | Aligns the oblique dimensions to create a chain dimension:<br>\
- Select two or more parallel obl… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionPosVertChainDimension` | Align Vertical Chain Dimensions | Aligns the vertical dimensions to create a chain dimension:<br>\
- Select two or more vertical di… | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionPositionSectionView` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandPositionSectionView.py` |
| `TechDraw_ExtensionRemovePrefixChar` | Remove Prefix | Removes the prefix symbols at the beginning of the dimension | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionSelectLineAttributes` | Select Line Attributes, Cascade Spacing and D… |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionShortenLine` | Shorten Line | Shortens a selected cosmetic line or centerline at both ends by the specified delta distance | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionThreadBoltBottom` | Cosmetic Thread Bolt Bottom View | Adds a cosmetic thread to the top or bottom view of the selected bolts/screws/rods | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionThreadBoltSide` | Cosmetic Thread Bolt Side View |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionThreadHoleBottom` | Cosmetic Thread Hole Bottom View | Adds a cosmetic thread to the top or bottom view of selected holes or circles | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionThreadHoleSide` | Cosmetic Thread Hole Side View | Adds a cosmetic thread to the side view of a hole or circle | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionThreadsGroup` | Cosmetic Thread Hole Side View | Adds a cosmetic thread to the side view of a selected hole between two selected parallel lines | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtensionVertexAtIntersection` | Cosmetic Intersection Vertices | Adds cosmetic vertices at the intersections of selected edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandExtensionPack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ExtentGroup` | Extent Dimension | Inserts a dimension showing the extent (overall length) of an object or feature | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_FaceCenterLine` | Centerline on Face | Adds a centerline to selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_FillTemplateFields` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandFillTemplateFields.py` |
| `TechDraw_GeometricHatch` | Geometric Hatch | Applies a geometric hatch pattern to the selected faces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandDecorate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Hatch` | Image Hatch | Applies a hatch pattern to the selected faces using an image file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandDecorate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_HoleShaftFit` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandHoleShaftFit.py` |
| `TechDraw_HorizontalDimension` | Horizontal Length Dimension | Inserts a horizontal length dimension of an edge or distance between two points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_HorizontalExtentDimension` | Horizontal Extent Dimension | Inserts a dimension showing the horizontal extent (overall length) of an object or feature | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Image` | Bitmap Image | Inserts a bitmap from a file into the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandDecorate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_LandmarkDimension` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_LeaderLine` | Leader Line | Adds a leader line | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_LengthDimension` | Length Dimension | Inserts a length dimension of an edge or distance between two points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_LinkDimension` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Midpoints` | Midpoint Vertices | Adds cosmetic vertices at the midpoint of the selected edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_MoveView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/TechDraw/TechDrawTools/CommandMoveView.py` |
| `TechDraw_PageDefault` | New Page | Creates a new page with the default template | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_PageTemplate` | New Page From Template | Creates a new page from a custom template | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_PrintAll` | Print All Pages | Prints all pages with the print dialog | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ProjectShape` | Project Shape | Creates a projected geometry of the selected object in the 3D view from the current camera angle | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ProjectionGroup` | Projection Group | Inserts multiple new linked views of the selected objects in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Quadrants` | Quadrant Vertices | Adds cosmetic vertices at the quadrant points of the selected circles | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_RadiusDimension` | Radius Dimension | Inserts a radius dimension of a circular edge or arc | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_RedrawPage` | Redraw Page | Redraws the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_RichTextAnnotation` | Rich Text Annotation | Inserts a rich text annotation in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_SectionGroup` | Section View (Simple or Complex) | Inserts a simple or complex section view in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_SectionView` | Section View | Inserts a new section view based on the selected view in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ShareView` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/TechDraw/Gui/Workbench.cpp`<br>`src/Mod/TechDraw/TechDrawTools/CommandShareView.py` |
| `TechDraw_ShowAll` | Toggle Edge Visibility | Toggles the visibility of the selected edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_SpreadsheetView` | Spreadsheet View | Inserts a view of a spreadsheet in the current page | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_StackBottom` | Stack Bottom | Moves the selected view to the bottom of the stack | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandStack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_StackDown` | Stack Down | Moves the selected view down 1 level in the view stack | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandStack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_StackGroup` | View Stacking Order | Adjusts the stacking order of the selected views | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandStack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_StackTop` | Stack Top | Moves the selected view to the top of the stack | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandStack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_StackUp` | Stack Up | Moves the selected view up 1 level in the view stack | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandStack.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_SurfaceFinishSymbols` | Surface Finish Symbol | Adds a surface finish symbol in the selected view | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_Symbol` | Insert SVG | Inserts a symbol from an SVG file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ToggleFrame` | Toggle View Frames | Toggles visibility of view frames and vertices | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandDecorate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_ToggleGrid` | Toggle Grid | Toggles the grid on the active page | 已定义 | C++ Command/GroupCommand | `src/Mod/TechDraw/Gui/CommandDecorate.cpp` |
| `TechDraw_VerticalDimension` | Vertical Length Dimension | Inserts a vertical length dimension of an edge or distance between two points | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_VerticalExtentDimension` | Vertical Extent Dimension | Inserts a dimension showing the vertical extent (overall length) of an object or feature | 已定义 / 被引用 | C++ Command/GroupCommand, GroupCommand member, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandCreateDims.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_View` | New View |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/Command.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |
| `TechDraw_WeldSymbol` | Weld Symbol | Adds welding information to the selected leader line | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/TechDraw/Gui/CommandAnnotate.cpp`<br>`src/Mod/TechDraw/Gui/Workbench.cpp` |

### 21 Draft 二维绘图（84）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Draft_AddConstruction` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_groups.py` |
| `Draft_AddNamedGroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_groups.py` |
| `Draft_AddToGroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_groups.py` |
| `Draft_AddToLayer` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_layers.py` |
| `Draft_AnnotationStyleEditor` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_annotationstyleeditor.py` |
| `Draft_ApplyStyle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_styles.py` |
| `Draft_Arc` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_arcs.py` |
| `Draft_ArcTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_arcs.py` |
| `Draft_Arc_3Points` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_arcs.py` |
| `Draft_ArrayTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_arrays.py` |
| `Draft_AutoGroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_groups.py` |
| `Draft_BSpline` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_splines.py` |
| `Draft_BezCurve` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_beziers.py` |
| `Draft_BezierTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_beziers.py` |
| `Draft_Circle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_circles.py` |
| `Draft_CircularArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_circulararray.py` |
| `Draft_Clone` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_clone.py` |
| `Draft_CubicBezCurve` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_beziers.py` |
| `Draft_Dimension` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_dimensions.py` |
| `Draft_Downgrade` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_downgrade.py` |
| `Draft_Draft2Sketch` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_draft2sketch.py` |
| `Draft_Edit` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_edit.py` |
| `Draft_Ellipse` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_ellipses.py` |
| `Draft_Facebinder` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_facebinders.py` |
| `Draft_Fillet` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_fillets.py` |
| `Draft_FlipDimension` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_dimension_ops.py` |
| `Draft_Hatch` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_hatch.py` |
| `Draft_Heal` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_heal.py` |
| `Draft_Hyperlink` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_hyperlink.py` |
| `Draft_Join` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_join.py` |
| `Draft_Label` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_labels.py` |
| `Draft_Layer` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_layers.py` |
| `Draft_LayerManager` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_layers.py` |
| `Draft_Line` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_lines.py` |
| `Draft_Mirror` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_mirror.py` |
| `Draft_Move` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_move.py` |
| `Draft_Offset` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_offset.py` |
| `Draft_OrthoArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_orthoarray.py` |
| `Draft_PathArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_patharray.py` |
| `Draft_PathLinkArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_patharray.py` |
| `Draft_PathTwistedArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_pathtwistedarray.py` |
| `Draft_PathTwistedLinkArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_pathtwistedarray.py` |
| `Draft_Point` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_points.py` |
| `Draft_PointArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_pointarray.py` |
| `Draft_PointLinkArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_pointarray.py` |
| `Draft_PolarArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_polararray.py` |
| `Draft_Polygon` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_polygons.py` |
| `Draft_Rectangle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_rectangles.py` |
| `Draft_Rotate` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_rotate.py` |
| `Draft_Scale` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_scale.py` |
| `Draft_SelectGroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_groups.py` |
| `Draft_SelectPlane` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_selectplane.py` |
| `Draft_SetStyle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_setstyle.py` |
| `Draft_Shape2DView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_shape2dview.py` |
| `Draft_ShapeString` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_shapestrings.py` |
| `Draft_ShowSnapBar` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Slope` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_lineslope.py` |
| `Draft_Snap_Angle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Center` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Dimensions` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Endpoint` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Extension` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Grid` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Intersection` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Lock` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Midpoint` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Near` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Ortho` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Parallel` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Perpendicular` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_Special` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Snap_WorkingPlane` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_snaps.py` |
| `Draft_Split` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_split.py` |
| `Draft_Stretch` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_stretch.py` |
| `Draft_SubelementHighlight` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_subelements.py` |
| `Draft_Text` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_texts.py` |
| `Draft_ToggleConstructionMode` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_togglemodes.py` |
| `Draft_ToggleDisplayMode` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_togglemodes.py` |
| `Draft_ToggleGrid` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_grid.py` |
| `Draft_Trimex` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_trimex.py` |
| `Draft_Upgrade` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_upgrade.py` |
| `Draft_Wire` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_lines.py` |
| `Draft_WireToBSpline` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_wire2spline.py` |
| `Draft_WorkingPlaneProxy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Draft/draftguitools/gui_planeproxy.py` |

### 22 Arch 建筑（54）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Arch_Add` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Axis` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimAxis.py` |
| `Arch_AxisSystem` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimAxis.py` |
| `Arch_AxisTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimAxis.py` |
| `Arch_Building` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/ArchBuilding.py`<br>`src/Mod/BIM/bimcommands/BimBuildingPart.py` |
| `Arch_Check` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_CloneComponent` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_CloseHoles` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Component` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_CurtainWall` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCurtainwall.py` |
| `Arch_CutPlane` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCutPlane.py` |
| `Arch_Equipment` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimEquipment.py` |
| `Arch_Fence` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimFence.py` |
| `Arch_Floor` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/ArchFloor.py` |
| `Arch_Frame` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimFrame.py` |
| `Arch_Grid` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimAxis.py` |
| `Arch_IfcSpreadsheet` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Level` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimBuildingPart.py` |
| `Arch_Material` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimMaterial.py` |
| `Arch_MaterialTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimMaterial.py` |
| `Arch_MergeWalls` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_MeshToShape` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_MultiMaterial` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimMaterial.py` |
| `Arch_Nest` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPanel.py` |
| `Arch_Panel` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPanel.py` |
| `Arch_PanelTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPanel.py` |
| `Arch_Panel_Cut` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPanel.py` |
| `Arch_Panel_Sheet` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPanel.py` |
| `Arch_Pipe` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPipe.py` |
| `Arch_PipeConnector` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPipe.py` |
| `Arch_PipeTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPipe.py` |
| `Arch_Profile` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimProfile.py` |
| `Arch_Rebar` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimRebar.py` |
| `Arch_RebarTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `Arch_Reference` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimReference.py` |
| `Arch_Remove` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_RemoveShape` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Roof` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimRoof.py` |
| `Arch_Schedule` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSchedule.py` |
| `Arch_SectionPlane` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSectionPlane.py` |
| `Arch_SelectNonSolidMeshes` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Site` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSite.py` |
| `Arch_Space` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSpace.py` |
| `Arch_SplitMesh` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Stairs` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimStairs.py` |
| `Arch_StructuralSystem` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/ArchStructure.py` |
| `Arch_StructureTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/ArchStructure.py` |
| `Arch_StructuresFromSelection` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/ArchStructure.py` |
| `Arch_Survey` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_ToggleIfcBrepFlag` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_ToggleSubs` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimArchUtils.py` |
| `Arch_Truss` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimTruss.py` |
| `Arch_Wall` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWall.py` |
| `Arch_Window` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWindow.py` |

### 23 BIM（85）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `BIM_ArcTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_ArrayTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_AxisTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Background` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimBackground.py` |
| `BIM_Beam` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimBeam.py` |
| `BIM_BooleanTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Box` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimBox.py` |
| `BIM_Builder` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimBuilder.py` |
| `BIM_Classification` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimClassification.py` |
| `BIM_Clone` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimClone.py` |
| `BIM_CloneTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Column` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimColumn.py` |
| `BIM_Common` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCommon.py` |
| `BIM_Compound` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCompound.py` |
| `BIM_Convert` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimConvert.py` |
| `BIM_Copy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCopy.py` |
| `BIM_Covering` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCovering.py` |
| `BIM_Create2DViews` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Cut` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimCut.py` |
| `BIM_Diff` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDiff.py` |
| `BIM_DimensionAligned` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDimensions.py` |
| `BIM_DimensionHorizontal` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDimensions.py` |
| `BIM_DimensionVertical` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDimensions.py` |
| `BIM_Door` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDoor.py` |
| `BIM_DrawingView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimDrawingView.py` |
| `BIM_EmptyTrash` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimEmptyTrash.py` |
| `BIM_Examples` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimExamples.py` |
| `BIM_Extrude` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimExtrude.py` |
| `BIM_Fuse` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimFuse.py` |
| `BIM_GenericTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Glue` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimGlue.py` |
| `BIM_Help` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimHelp.py` |
| `BIM_IfcElements` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimIfcElements.py` |
| `BIM_IfcExplorer` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimIfcExplorer.py` |
| `BIM_IfcManageTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_IfcProperties` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimIfcProperties.py` |
| `BIM_IfcQuantities` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimIfcQuantities.py` |
| `BIM_ImagePlane` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimImagePlane.py` |
| `BIM_Layers` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimLayers.py` |
| `BIM_Leader` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimLeader.py` |
| `BIM_Library` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimLibrary.py` |
| `BIM_LinkMake` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimLink.py` |
| `BIM_Material` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimMaterial.py` |
| `BIM_MoveView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimMoveView.py` |
| `BIM_Nudge_Down` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Extend` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Left` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Right` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_RotateLeft` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_RotateRight` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Shrink` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Switch` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Nudge_Up` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimNudge.py` |
| `BIM_Offset2D` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimOffset.py` |
| `BIM_OffsetTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_Preflight` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimPreflight.py` |
| `BIM_Project` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimProject.py` |
| `BIM_ProjectManager` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimProjectManager.py` |
| `BIM_Reextrude` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimReextrude.py` |
| `BIM_Reorder` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimReorder.py` |
| `BIM_Report` | BIM Report | Create a new BIM Report to query model data with SQL | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimReport.py` |
| `BIM_ReportTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_ResetCloneColors` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimResetCloneColors.py` |
| `BIM_Rewire` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimRewire.py` |
| `BIM_SetWPFront` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWPCommands.py` |
| `BIM_SetWPSide` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWPCommands.py` |
| `BIM_SetWPTop` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWPCommands.py` |
| `BIM_Setup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSetup.py` |
| `BIM_Shape2DCut` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimShape2DView.py` |
| `BIM_Shape2DView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimShape2DView.py` |
| `BIM_SimpleCopy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSimpleCopy.py` |
| `BIM_Sketch` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSketch.py` |
| `BIM_Slab` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimSlab.py` |
| `BIM_SplineTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/InitGui.py` |
| `BIM_TDPage` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimTDPage.py` |
| `BIM_TDView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimTDView.py` |
| `BIM_Text` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimText.py` |
| `BIM_Trash` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimTrash.py` |
| `BIM_Tutorial` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimTutorial.py` |
| `BIM_Unclone` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimUnclone.py` |
| `BIM_Ungroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimUngroup.py` |
| `BIM_Views` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimViews.py` |
| `BIM_WPView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWPCommands.py` |
| `BIM_Welcome` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWelcome.py` |
| `BIM_Windows` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/bimcommands/BimWindows.py` |

### 24 Assembly 装配（31）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Assembly_ActivateAssembly` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateAssembly.py` |
| `Assembly_CreateAssembly` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Assembly/CommandCreateAssembly.py`<br>`src/Mod/Start/Gui/StartView.cpp` |
| `Assembly_CreateBom` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateBom.py` |
| `Assembly_CreateJointAngle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointBall` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointBelt` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointCylindrical` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointDistance` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointFixed` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointGearBelt` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointGears` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointParallel` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointPerpendicular` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointRackPinion` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointRevolute` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointScrew` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateJointSlider` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |
| `Assembly_CreateSimulation` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateSimulation.py` |
| `Assembly_CreateView` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateView.py` |
| `Assembly_ExportASMT` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandExportASMT.py` |
| `Assembly_Insert` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandInsertLink.py` |
| `Assembly_InsertLink` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandInsertLink.py` |
| `Assembly_InsertNewPart` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandInsertNewPart.py` |
| `Assembly_LinkSelectLinked` | Go to Linked Assembly | Selects the linked assembly and switches to its original document | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SelectComponentsWithDoFs` | Select Components With DoFs | Selects unconstrained components in the active assembly | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SelectConflictingConstraints` | Select Conflicting Constraints | Selects conflicting joints in the active assembly | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SelectJointsOfComponent` | Select Component Joints | Selects all joints referencing the selected component | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SelectMalformedConstraints` | Select Malformed Constraints | Selects malformed joints in the active assembly | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SelectRedundantConstraints` | Select Redundant Constraints | Selects redundant joints in the active assembly | 已定义 | C++ Command/GroupCommand | `src/Mod/Assembly/Gui/Commands.cpp` |
| `Assembly_SolveAssembly` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandSolveAssembly.py` |
| `Assembly_ToggleGrounded` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Assembly/CommandCreateJoint.py` |

### 30 FEM 有限元（102）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `FEM_Analysis` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ClippingPlaneAdd` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ClippingPlaneRemoveAll` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_CompEmConstraints` | Electromagnetic Boundary Conditions | Electromagnetic boundary conditions | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_CompEmEquations` | Electromagnetic Equations | Electromagnetic equations for the Elmer solver | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_CompMechEquations` | Mechanical Equations | Mechanical equations for the Elmer solver | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_CompSolvers` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstantVacuumPermittivity` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintBearing` | Bearing Constraint | Creates a bearing constraint | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintBodyHeatSource` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintCentrif` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintContact` | Contact Constraint | Creates a contact constraint between faces | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintCurrentDensity` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp`<br>+1 more |
| `FEM_ConstraintDisplacement` | Displacement Boundary Condition | Creates a displacement boundary condition for a geometric entity | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintElectricChargeDensity` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp`<br>+1 more |
| `FEM_ConstraintElectromagnetic` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp`<br>+1 more |
| `FEM_ConstraintFixed` | Fixed Boundary Condition | Creates a fixed boundary condition for a geometric entity | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintFlowVelocity` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintFluidBoundary` | Fluid Boundary Condition |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintForce` | Force Load | Creates a force load applied to a geometric entity | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintGear` | Gear Constraint | Creates a gear constraint | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintHeatflux` | Heat Flux Load | Creates a heat flux load acting on a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintInitialFlowVelocity` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintInitialPressure` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintInitialTemperature` | Initial Temperature | Creates an initial temperature acting on a body | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintMagnetization` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp`<br>+1 more |
| `FEM_ConstraintPlaneRotation` | Plane Multi-Point Constraint | Creates a plane multi-point constraint for a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintPressure` | Pressure Load | Creates a pressure load acting on a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintPulley` | Pulley Constraint | Creates a pulley constraint | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintRigidBody` | Rigid Body Constraint | Creates a rigid body constraint for a geometric entity | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintSectionPrint` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintSelfWeight` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintSpring` | Spring Boundary Condition | Creates a spring boundary condition on a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintTemperature` | Temperature Boundary Condition | Creates a temperature/concentrated heat flux load acting on a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_ConstraintTie` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ConstraintTransform` | Local Coordinate System | Creates a local coordinate system on a face | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_CreateElementsSet` | Erase Elements | Creates a FEM mesh elements set | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_CreateNodesSet` | Nodes Set | Creates a FEM mesh nodes set | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_DefineElementsSet` | Element Set From Polygon | Creates a collection of elements selected by a polygon | 已定义 | C++ Command/GroupCommand | `src/Mod/Fem/Gui/Command.cpp` |
| `FEM_DefineNodesSet` | Node Set by Polygon | Creates a node set by polygon selection | 已定义 | C++ Command/GroupCommand | `src/Mod/Fem/Gui/Command.cpp` |
| `FEM_ElementFluid1D` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ElementGeometry1D` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ElementGeometry2D` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ElementRotation1D` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationDeformation` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationElasticity` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationElectricforce` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationElectrostatic` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationFlow` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationFlux` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationHeat` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationMagnetodynamic` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationMagnetodynamic2D` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_EquationStaticCurrent` |  |  | 已定义 / 被引用 | Python Gui.addCommand, runCommandByName | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_Examples` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_FEMMesh2Mesh` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_FemAddPart` | Add Part to Analysis | Adds a part to the analysis | 已定义 | C++ Command/GroupCommand | `src/Mod/Fem/Gui/Command.cpp` |
| `FEM_MaterialEditor` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MaterialFluid` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MaterialMechanicalNonlinear` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MaterialReinforced` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MaterialSolid` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshAdvanced` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshBoundaryLayer` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshClear` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshClearGroups` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshDisplayInfo` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshDistance` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshGMSHRefinement` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshGmshFromShape` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshGroup` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshManipulate` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshNetgenFromShape` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshRegion` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshShape` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshTransfiniteCurve` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshTransfiniteSurface` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_MeshTransfiniteVolume` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_PostApplyChanges` | Apply Changes to Pipeline | Applies changes to parameters directly and not on recompute only | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostBranchFilter` | Pipeline Branch | Branches the pipeline into a new path | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostCreateFunctions` | Filter Functions | Functions for use in postprocessing filter | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterCalculator` | Calculator Filter | Creates a new field from current data | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterClipRegion` | Region Clip Filter |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterClipScalar` | Scalar Clip Filter | Defines a clip filter which clips a field with a scalar value | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterContours` | Contours Filter | Defines a contours filter that displays iso contours | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterCutFunction` | Function Cut Filter | Cuts the data along an implicit function | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterDataAlongLine` | Line Clip Filter | Defines a clip filter which clips a field along a line | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterDataAtPoint` | Data at Point Clip Filter | Defines a clip filter which clips a field data at point | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterGlyph` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_PostFilterLinearizedStresses` | Stress Linearization Plot | Defines a stress linearization plot | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostFilterWarp` | Warp Filter | Warps the geometry along a vector field by a certain factor | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostPipelineFromResult` | Post Pipeline From Result | Creates a post processing pipeline from a result object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Fem/Gui/Command.cpp`<br>`src/Mod/Fem/Gui/Workbench.cpp` |
| `FEM_PostVisualization` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femguiutils/post_visualization.py` |
| `FEM_ResultShow` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_ResultsPurge` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverCalculiX` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverCalculiXCcxTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverControl` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverElmer` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverMystran` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverRun` |  |  | 已定义 / 被引用 | Menu/Toolbar item, Python Gui.addCommand | `src/Mod/Fem/Gui/Workbench.cpp`<br>`src/Mod/Fem/femcommands/commands.py` |
| `FEM_SolverZ88` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/Fem/femcommands/commands.py` |

### 31 CAM/Path 加工（49）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `CAM_3dTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_Area` | Area | Creates a feature area from the selected objects | 已定义 | C++ Command/GroupCommand | `src/Mod/CAM/Gui/Command.cpp` |
| `CAM_Area_Workplane` | Area Workplane | Selects a workplane for a feature area | 已定义 | C++ Command/GroupCommand | `src/Mod/CAM/Gui/Command.cpp` |
| `CAM_Array` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/Array.py` |
| `CAM_Camotics` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/Camotics.py` |
| `CAM_Comment` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/Comment.py` |
| `CAM_Compound` | Compound | Creates a compound from the selected toolpaths | 已定义 | C++ Command/GroupCommand | `src/Mod/CAM/Gui/Command.cpp` |
| `CAM_Copy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/Copy.py` |
| `CAM_DressupArray` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/Array.py` |
| `CAM_DressupAxisMap` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/AxisMap.py` |
| `CAM_DressupDogbone` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/DogboneII.py` |
| `CAM_DressupDragKnife` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/Dragknife.py` |
| `CAM_DressupLeadInOut` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/LeadInOut.py` |
| `CAM_DressupMirror` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/Mirror.py` |
| `CAM_DressupPathBoundary` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/Boundary.py` |
| `CAM_DressupRampEntry` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/RampEntry.py` |
| `CAM_DressupTag` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/Tags.py` |
| `CAM_DressupTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_DressupZCorrect` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Dressup/Gui/ZCorrect.py` |
| `CAM_DrillingTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_EngraveTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_ExportTemplate` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/JobCmd.py` |
| `CAM_Fixture` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/Fixture.py` |
| `CAM_Inspect` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/Inspect.py` |
| `CAM_Job` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/JobCmd.py` |
| `CAM_OpActiveToggle` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/PathCommands.py` |
| `CAM_OperationCopy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/PathCommands.py` |
| `CAM_PathShapeTC` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/PathShapeTC.py` |
| `CAM_Post` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Post/Command.py` |
| `CAM_PostSelected` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Post/Command.py` |
| `CAM_PostTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_PropertyBag` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Base/Gui/PropertyBag.py` |
| `CAM_QuickValidate` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/SanityCmd.py` |
| `CAM_Sanity` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/SanityCmd.py` |
| `CAM_SelectLoop` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/PathCommands.py` |
| `CAM_SetStartPoint` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/Base.py` |
| `CAM_Shape` | From Shape | Creates a toolpath from a selected shape | 已定义 | C++ Command/GroupCommand | `src/Mod/CAM/Gui/Command.cpp` |
| `CAM_SimTools` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/InitGui.py` |
| `CAM_SimpleCopy` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/SimpleCopy.py` |
| `CAM_Simulator` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/Simulator.py` |
| `CAM_SimulatorGL` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Main/Gui/SimulatorGL.py` |
| `CAM_Stop` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Op/Gui/Stop.py` |
| `CAM_ToolBitCreate` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/toolbit/ui/cmd.py` |
| `CAM_ToolBitDock` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/library/ui/cmd.py` |
| `CAM_ToolBitLibraryOpen` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/library/ui/cmd.py` |
| `CAM_ToolBitLoad` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/toolbit/ui/cmd.py` |
| `CAM_ToolBitSave` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/toolbit/ui/cmd.py` |
| `CAM_ToolBitSaveAs` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/toolbit/ui/cmd.py` |
| `CAM_ToolController` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/CAM/Path/Tool/Gui/Controller.py` |

### 32 Robot 机器人（15）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Robot_AddToolShape` | Tool | Adds a tool shape to the robot | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandInsertRobot.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_Create` | Place Robot | Places a robot in the scene | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/Command.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_CreateTrajectory` | Trajectory | Creates a new empty trajectory | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_Edge2Trac` | Edge to Trajectory | Generates a trajectory from the selected edges | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_ExportKukaCompact` | Kuka Compact Subroutine | Exports the trajectory as a compact KRL subroutine | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandExport.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_ExportKukaFull` | Kuka Full Subroutine | Exports the trajectory as a full KRL subroutine | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandExport.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_InsertWaypoint` | Insert in Trajectory | Inserts the robot tool location into the trajectory | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_InsertWaypointPreselect` | Insert in Trajectory | Inserts the preselection position into the trajectory (W) | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_RestoreHomePos` | Move to Home | Moves to the home position | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/Command.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_SetDefaultOrientation` | Set Default Orientation |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_SetDefaultValues` | Set Default Values |  | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_SetHomePos` | Set Home Position | Sets the home position | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/Command.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_Simulate` | Simulate Trajectory | Simulates robot movement along a selected trajectory | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/Command.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_TrajectoryCompound` | Trajectory Compound | Groups and connects multiple trajectories into one | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |
| `Robot_TrajectoryDressUp` | Dress-Up Trajectory | Creates a dress-up object that overrides aspects of a trajectory | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Robot/Gui/CommandTrajectory.cpp`<br>`src/Mod/Robot/Gui/Workbench.cpp` |

### 40 Spreadsheet 表格（16）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Spreadsheet_AlignBottom` | Align &Bottom | Aligns cell contents to the bottom | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_AlignCenter` | Align Horizontal &Center | Aligns cell contents to the horizontal center | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_AlignLeft` | Align &Left | Aligns cell contents to the left | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_AlignRight` | Align &Right | Aligns cell contents to the right | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_AlignTop` | Align &Top | Aligns cell contents to the top | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_AlignVCenter` | Align &Vertical Center | Aligns cell contents to the vertical center | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_CreateSheet` | &New Spreadsheet | Creates a new spreadsheet | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_Export` | &Export Spreadsheet | Exports the spreadsheet to a CSV file | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_Import` | &Import Spreadsheet | Imports a CSV file into a new spreadsheet | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_MergeCells` | &Merge Cells | Merges the selected cells | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/SheetTableView.cpp`<br>+1 more |
| `Spreadsheet_Open` |  |  | 被引用 | Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_SetAlias` | Set Alias | Sets an alias for the selected cell | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_SplitCell` | Sp&lit Cell | Splits a previously merged cell | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/SheetTableView.cpp`<br>+1 more |
| `Spreadsheet_StyleBold` | &Bold Text | Sets the text in the selected cells bold | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_StyleItalic` | &Italic Text | Sets the text in the selected cells italic | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |
| `Spreadsheet_StyleUnderline` | &Underline Text | Underlines the text in the selected cells | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Spreadsheet/Gui/Command.cpp`<br>`src/Mod/Spreadsheet/Gui/Workbench.cpp` |

### 41 OpenSCAD（15）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `OpenSCAD_AddOpenSCADElement` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ColorCodeShape` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_Edgestofaces` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ExpandPlacements` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ExplodeGroup` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_Hull` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_IncreaseToleranceFeature` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_MeshBoolean` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_Minkowski` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_MirrorMeshFeature` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_RefineShapeFeature` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_RemoveSubtree` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ReplaceObject` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ResizeMeshFeature` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |
| `OpenSCAD_ScaleMeshFeature` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/OpenSCAD/OpenSCADCommands.py` |

### 42 Materials 材料（4）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Material_Edit` | Edit | Edits material properties | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item, runCommandByName | `src/Mod/Material/Gui/Command.cpp`<br>`src/Mod/Material/Gui/MaterialTreeWidget.cpp`<br>+1 more |
| `Materials_InspectAppearance` | Inspect Appearance | Inspects the appearance properties of the selected object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Material/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp`<br>+1 more |
| `Materials_InspectMaterial` | Inspect Material | Inspects the material properties of the selected object | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Material/Gui/Command.cpp`<br>`src/Mod/Part/Gui/Workbench.cpp`<br>+1 more |
| `Materials_MigrateToExternal` | Migrate | Migrates the materials to the external materials manager | 已定义 | C++ Command/GroupCommand | `src/Mod/Material/Gui/Command.cpp` |

### 43 Points 点云（6）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Points_Convert` | Convert to Points | Converts to points | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |
| `Points_Export` | Export Points… | Exports a point cloud | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |
| `Points_Import` | Import Points… | Imports a point cloud | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |
| `Points_Merge` | Merge Point Clouds | Merges several point clouds into one | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |
| `Points_PolyCut` | Cut Point Cloud | Cuts a point cloud with a selected polygon | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |
| `Points_Structure` | Structured Point Cloud | Converts points to a structured point cloud | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Points/Gui/Command.cpp`<br>`src/Mod/Points/Gui/Workbench.cpp` |

### 44 Reverse Engineering 逆向（12）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Reen_ApproxCurve` | Approximate B-Spline Curve… | Approximates a B-spline curve | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ApproxCylinder` | Cylinder | Approximates a cylinder | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ApproxPlane` | Plane | Approximates a plane | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ApproxPolynomial` | Polynomial Surface | Approximates a polynomial surface | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ApproxSphere` | Sphere | Approximates a sphere | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ApproxSurface` | Approximate B-Spline Surface… | Approximates a B-spline surface | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_MeshBoundary` | Wire From Mesh Boundary… | Creates a wire from mesh boundaries | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_PoissonReconstruction` | Poisson… | Performs Poisson surface reconstruction | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_Segmentation` | Mesh Segmentation… | Creates separate mesh segments based on surface types | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_SegmentationFromComponents` | From Components | Creates mesh segments from components | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_SegmentationManual` | Manual Segmentation… | Creates mesh segments manually | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |
| `Reen_ViewTriangulation` | Structured Point Clouds | Triangulates structured point clouds | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/ReverseEngineering/Gui/Command.cpp`<br>`src/Mod/ReverseEngineering/Gui/Workbench.cpp` |

### 45 Inspection 检测（2）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Inspection_InspectElement` | Inspection… | Inspects distance information | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Inspection/Gui/Command.cpp`<br>`src/Mod/Inspection/Gui/Workbench.cpp` |
| `Inspection_VisualInspection` | Visual Inspection… | Inspects the objects visually | 已定义 / 被引用 | C++ Command/GroupCommand, Menu/Toolbar item | `src/Mod/Inspection/Gui/Command.cpp`<br>`src/Mod/Inspection/Gui/Workbench.cpp` |

### 46 IFC（7）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `IFC_ConvertDocument` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_Diff` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_Expand` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_MakeProject` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_Save` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_SaveAs` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_commands.py` |
| `IFC_UpdateIOS` |  |  | 已定义 | Python Gui.addCommand | `src/Mod/BIM/nativeifc/ifc_openshell.py` |

### 47 Import/导入导出（2）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Import_Iges` |  |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Import/Gui/Command.cpp` |
| `Import_ReadBREP` |  |  | 已定义 | C++ Command/GroupCommand | `src/Mod/Import/Gui/Command.cpp` |

### 90 Start/模板/其它（1）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Start_Start` | &Start Page | Displays the start page | 已定义 / 被引用 | C++ Command/GroupCommand, runCommandByName | `src/Mod/Start/Gui/AppStartGui.cpp`<br>`src/Mod/Start/Gui/Manipulator.cpp` |

### 91 Test/开发测试（11）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `Test_InsertFeature` | Test base | Test the basic functions of FreeCAD | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_Test` | Test Document | Runs all tests at once (can take very long!) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestAll` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestAllText` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestBase` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestBaseText` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestCreateMenu` | Test base | Test the basic functions of FreeCAD | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestDeleteMenu` | Test base | Test the basic functions of FreeCAD | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestDoc` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestDocText` | Test Document | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |
| `Test_TestWork` | Test base | Test the document (creation, save, load and destruction) | 已定义 | Python Gui.addCommand | `src/Mod/Test/TestGui.py` |

### 92 Template/示例（6）

| 命令 | 菜单/显示名 | Tooltip/说明 | 状态 | 来源类型 | 文件 |
|---|---|---|---|---|---|
| `TemplatePyGrp_1` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |
| `TemplatePyGrp_2` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |
| `TemplatePyGrp_3` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |
| `TemplatePyMod_Cmd4` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |
| `TemplatePyMod_Cmd5` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |
| `TemplatePyMod_Cmd6` | Create spheres... | Click on the screen to create a sphere | 已定义 | Python Gui.addCommand | `src/Mod/TemplatePyMod/Commands.py` |


## 后续建议

1. 启动 FreeCAD 后用运行时 `CommandManager` 导出真实可用命令，和本文档做 diff。
2. 对 `Std_*`、`PartDesign_*`、`Sketcher_*`、`Part_*`、`TechDraw_*`、`CAM_*` 做 McStudio 优先级标注：`首页常驻 / Tab 常驻 / 下拉组 / 高级 / 不迁移`。
3. 为所有进入 Ribbon 的长命令补充短 `label`，避免按钮文字遮挡。
4. 再做 `ShowClassicMenuBar=false`，否则隐藏经典菜单会丢失未迁移入口。