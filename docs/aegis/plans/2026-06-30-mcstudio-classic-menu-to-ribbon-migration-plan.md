# McStudio 经典菜单到 Ribbon / 应用菜单迁移规划

日期：2026-06-30
状态：Planning

## 背景

FreeCAD `StdWorkbench::setupMenuBar()` 默认提供：

- File
- Edit
- View
- Tools
- Macro
- Windows
- Help

McStudio 当前直接继承 `StdWorkbench::setupMenuBar()`，所以顶部仍显示传统菜单栏。产品化目标是仿 SOLIDWORKS / NX：主要命令入口迁移到 Ribbon，传统菜单栏默认隐藏或简化。

## 迁移原则

不要把所有菜单命令平铺到 Ribbon。按优先级分为四层：

1. 高频主流程：直接放 Ribbon 大按钮。
2. 中频辅助：放 Ribbon 小按钮或组合下拉。
3. 低频系统命令：放 McStudio 应用菜单 / 设置入口。
4. 开发者/高级命令：放高级模式或命令搜索。

## 推荐顶层布局

默认 McStudio 产品模式：

```text
标题栏
McStudio Ribbon
  [应用菜单] 首页 特征 草图 曲面 评估 装夹/工艺 输出 视图 高级
左侧模型/任务 | 右侧图形区
```

经典 FreeCAD 菜单栏默认隐藏，但通过偏好项恢复：

```text
User parameter:BaseApp/Preferences/Mod/McStudio/UI
  ShowClassicMenuBar = false
```

## 命令分层

### 应用菜单

用于承载传统 File + 系统级入口：

- 新建 `Std_New`
- 打开 `Std_Open`
- 最近文件 `Std_RecentFiles`
- 保存 `Std_Save`
- 另存为 `Std_SaveAs`
- 保存副本 `Std_SaveCopy`
- 全部保存 `Std_SaveAll`
- 关闭当前 `Std_CloseActiveWindow`
- 关闭全部 `Std_CloseAllWindows`
- 导入 `Std_Import`
- 导出 `Std_Export`
- 合并项目 `Std_MergeProjects`
- 项目信息 `Std_ProjectInfo`
- 打印 / PDF `Std_Print`, `Std_PrintPreview`, `Std_PrintPdf`
- 偏好设置 `Std_DlgPreferences`
- 退出 `Std_Quit`

### 首页 tab

只放启动和高频通用命令：

- 项目：新建、打开、保存
- 快捷编辑：撤销、重做、重新计算、删除
- 快捷视图：适应全部、适应所选、标准视图、显示样式
- 结构：新零件、新建组、链接操作、变量集
- 帮助：这是什么、命令搜索

### 视图 tab

承接传统 View 菜单，而不是全部塞首页：

- 视图导航：Home、Front、Top、Right、Isometric、Fit All、Fit Selection
- 相机：正交、透视
- 缩放：放大、缩小、框选缩放
- 显示样式：DrawStyle、透明、随机颜色、纹理映射
- 可见性：隐藏/显示选择、显示/隐藏对象、选择可见对象
- 面板：工具栏菜单、Dock 菜单、底部面板、状态栏
- 全屏 / 停靠全屏

### 编辑 tab 或首页编辑组

只暴露常用编辑，复杂编辑进下拉：

- 撤销 / 重做
- 剪切 / 复制 / 粘贴
- 复制所选
- 删除
- 框选 / 元素框选 / 全选
- 放置 / 变换 / 对齐
- 属性
- 用户编辑模式

### 工具 / 评估 tab

承接传统 Tools 中与建模评估相关的命令：

- 测量 `Std_Measure`
- 质量属性 `Std_MassProperties`
- 注释标签 `Std_AnnotationLabel`
- 单位计算器 `Std_UnitsCalculator`
- 明确选择 `Std_ClarifySelection`
- 截图 `Std_ViewScreenShot`
- 加载图片 `Std_ViewLoadImage`
- 文本文档 `Std_TextDocument`

### 高级 tab

承接低频、诊断、开发工具：

- 插件管理器 `Std_AddonMgr`
- 参数编辑器 `Std_DlgParameter`
- 自定义 `Std_DlgCustomize`
- 场景检查器 `Std_SceneInspector`
- 依赖图 `Std_DependencyGraph`
- 导出依赖图 `Std_ExportDependencyGraph`
- 项目工具 `Std_ProjectUtil`
- Demo mode `Std_DemoMode`
- 宏录制 / 执行 / 最近宏 / 直接执行 / 附加调试器
- Python 帮助 / 开发手册

### 窗口 / 面板

不建议常驻 Ribbon。放在高级或视图的“面板/窗口”下拉：

- 下一个窗口 / 上一个窗口
- 平铺 / 层叠
- 窗口列表
- 工具栏菜单
- Dock 菜单
- 重置/恢复布局（后续新增 McStudio 命令）

### 帮助入口

右上角帮助按钮下拉：

- 这是什么 `Std_WhatsThis`
- 用户中心
- 论坛
- 报告 Bug
- 安全模式重启
- 开发手册
- Python 帮助
- 官网
- 捐赠
- 关于 FreeCAD / 关于 McStudio

## 实施顺序

### Phase 1：不要隐藏菜单，先补齐 Ribbon / 应用菜单命令组

新增组合命令：

- `McStudio_CompApplicationMenu`
- `McStudio_CompEditTools`
- `McStudio_CompViewNavigate`
- `McStudio_CompViewDisplay`
- `McStudio_CompViewVisibility`
- `McStudio_CompPanelWindowTools`
- `McStudio_CompAdvancedTools`
- `McStudio_CompMacroTools`
- `McStudio_CompHelp`

### Phase 2：重排 RibbonModel

新增或调整 tabs：

```text
首页 / 特征 / 草图 / 曲面 / 评估 / 装夹工艺 / 输出 / 视图 / 高级
```

首页只保留高频命令。

### Phase 3：简化 setupMenuBar

从：

```cpp
StdWorkbench::setupMenuBar()
```

改为：

```cpp
new MenuItem
  McStudio
  Help
```

或者根据偏好项决定是否返回完整 StdWorkbench 菜单。

### Phase 4：经典菜单栏隐藏开关

`activated()`：

- 如果 `ShowClassicMenuBar=false`，隐藏 `mainWindow->menuBar()`。

`deactivated()`：

- 恢复菜单栏显示。

### Phase 5：命令搜索

新增 McStudio command finder，解决低频命令不在 Ribbon 上的问题。

## 2026-06-30 执行记录

已完成 Phase 1/2 的第一轮实现：

- 新增通用 `CmdMcStudioCommandGroup`，减少组合命令重复样板代码。
- 新增组合命令：
  - `McStudio_CompApplicationMenu`
  - `McStudio_CompEditTools`
  - `McStudio_CompViewNavigate`
  - `McStudio_CompStandardViews`
  - `McStudio_CompViewDisplay`
  - `McStudio_CompViewVisibility`
  - `McStudio_CompPanelWindowTools`
  - `McStudio_CompAdvancedTools`
  - `McStudio_CompMacroTools`
  - `McStudio_CompHelp`
- 重排 `RibbonModel.cpp`：
  - 首页新增“应用”组。
  - 首页“编辑”组增加编辑工具下拉。
  - 首页“帮助”改为帮助组合下拉。
  - 新增“视图”tab，承接导航、标准视图、显示、可见性、面板/窗口。
  - 新增“高级”tab，承接设置、扩展/诊断、自动化、帮助/开发。

暂未隐藏经典菜单栏。下一阶段在 GUI 验证通过后，再实现 `ShowClassicMenuBar` 偏好项和简化 `setupMenuBar()`。
