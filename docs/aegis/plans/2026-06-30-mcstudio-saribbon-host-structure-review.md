# McStudio SARibbon Host Structure Review

日期：2026-06-30
状态：Aegis Architecture Review

## 被审查结构

用户提出的当前结构：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    McStudioSARibbonBackend
      SARibbonBar
        SARibbonCategory
          SARibbonPanel
            SARibbonToolButton
```

## 结论

方向正确，但这棵树的表达方式需要修正。

`McStudioSARibbonBackend` 不应该出现在 QWidget 视觉树里。它不是 UI 节点，而是 adapter/controller。

更准确的长期结构应该分成两棵树：

### 1. QWidget 视觉树

```text
Gui::MainWindow
  QDockWidget McStudioRibbonDockHost
    QFrame McStudioRibbonHost
      SARibbonBar
        SARibbonCategory
          SARibbonPanel
            SARibbonToolButton
```

### 2. 控制/适配对象树

```text
RibbonWidget facade
  McStudioRibbonController / Host controller
    McStudioSARibbonBackend
      RibbonModel -> SARibbon mapping
      FreeCAD CommandManager -> QAction adapter
      QAction proxy / menu adapter
```

也就是说：

```text
Backend owns logic, not pixels.
SARibbonBar owns pixels, not FreeCAD adaptation policy.
```

## 当前代码状态

当前代码中：

```text
QDockWidget McStudioRibbonDockHost
  QFrame McStudioRibbonHost
    QWidget McStudioSARibbonHost
      SARibbonBar
```

其中 `McStudioSARibbonBackend` 是 C++ 对象，不是 QWidget；这一点是正确的。

但当前存在一个额外视觉容器：

```text
McStudioSARibbonHost
```

短期可以接受，因为它给 backend 提供独立根 widget 和 fallback 空间。

长期建议：如果 SARibbon 成为唯一后端，可以删除这个中间 `McStudioSARibbonHost`，让结构更简单：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    SARibbonBar
```

## 为什么需要 McStudioRibbonDockHost

需要保留。

原因：FreeCAD 的主窗口仍然是 `Gui::MainWindow`，McStudio 不替换 MainWindow。顶部 Ribbon 需要挂到 FreeCAD Dock 系统：

```cpp
mainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
```

`McStudioRibbonDockHost` 的职责：

- 管理 McStudio Ribbon 在 FreeCAD MainWindow 中的位置。
- 切换到 McStudio 时 show。
- 离开 McStudio 时 hide。
- 锁定 TopDockWidgetArea。
- 隐藏 dock title bar。

所以 DockHost 是 FreeCAD 集成边界，应该保留。

## 为什么需要 McStudioRibbonHost

长期建议保留，但职责要明确。

`McStudioRibbonHost` 是 McStudio 自己的 UI 边界层，用来隔离：

- FreeCAD Dock 系统
- SARibbon 第三方控件
- McStudio 后端切换/样式/生命周期

职责：

- 统一 objectName / accessibility / stylesheet。
- 统一高度策略。
- 持有 backend/controller。
- 作为后续 Custom/SARibbon backend 切换的容器。

如果后续完全删除 Custom backend，也仍建议保留 `McStudioRibbonHost`，因为它是 McStudio 和第三方 SARibbon 的隔离层。

## McStudioSARibbonBackend 的正确定位

`McStudioSARibbonBackend` 不应该被理解为 UI 子控件。

它应该是：

```text
Backend / Adapter / Presenter
```

职责：

1. 读取 `RibbonModel`。
2. 创建/更新 SARibbonBar 内容。
3. 从 FreeCAD `CommandManager` 获取命令 QAction。
4. 创建 Ribbon 短名代理 QAction。
5. 创建 ActionGroup / QAction menu 的 SARibbon 菜单映射。
6. 管理 rebuild / refresh / theme / command-state sync。

长期最好让它继承 QObject：

```cpp
class McStudioSARibbonBackend : public QObject, public McStudioRibbonBackend
```

这样生命周期可以直接用 Qt parent 管理，而不是当前：

```cpp
unique_ptr.release();
QObject::connect(host, &QObject::destroyed, ... delete backendOwner);
```

当前方式可工作，但长期不优雅。

## 当前实现中需要调整的点

### 1. 不要把 Backend 写进 UI 层级图

文档和设计图中应该避免：

```text
McStudioSARibbonBackend
  SARibbonBar
```

更准确是：

```text
McStudioSARibbonBackend --controls/builds--> SARibbonBar
```

### 2. Backend 应改成 QObject 生命周期

长期建议：

```cpp
class McStudioRibbonBackend : public QObject
{
    Q_OBJECT
public:
    explicit McStudioRibbonBackend(QObject* parent = nullptr);
    virtual QWidget* widget() = 0;
    virtual void rebuild() = 0;
};
```

或者：

```cpp
class McStudioSARibbonBackend final : public QObject
```

并让 `McStudioRibbonHost` 持有：

```cpp
std::unique_ptr<McStudioRibbonBackend> _backend;
```

不建议长期用 lambda delete 裸指针。

### 3. `McStudioSARibbonHost` 可以短期保留，长期视情况删除

短期结构：

```text
McStudioRibbonHost
  McStudioSARibbonHost
    SARibbonBar
```

好处：

- backend 有自己的根 widget。
- fallback 简单。
- SARibbon 样式可以隔离。

长期如果 SARibbon 成为唯一后端，建议简化成：

```text
McStudioRibbonHost
  SARibbonBar
```

### 4. 不要让 RibbonWidget 继续变胖

当前 `RibbonWidget.cpp` 同时负责：

- dock host 创建
- custom ribbon 创建
- SARibbon backend 创建
- 样式和 metrics

长期建议拆分：

```text
RibbonWidget.cpp
  show/hide facade only

McStudioRibbonDockController.cpp
  dock host 管理

McStudioRibbonHost.cpp
  host widget + backend lifecycle

McStudioSARibbonBackend.cpp
  SARibbon mapping
```

## 推荐长期最终结构

### QWidget Tree

```text
Gui::MainWindow
  QDockWidget McStudioRibbonDockHost
    McStudioRibbonHost : QFrame
      SARibbonBar
        SARibbonCategory  // Home, Features, View...
          SARibbonPanel   // File, Edit, Main Features...
            SARibbonToolButton
```

### Object / Logic Tree

```text
RibbonWidget facade
  McStudioRibbonDockController
    McStudioRibbonHost
      McStudioSARibbonBackend
        McStudioRibbonActionAdapter
        McStudioRibbonModelAdapter
```

### Dependency Direction

```text
RibbonWidget
  -> DockController
    -> Host
      -> Backend interface
        -> SARibbonBackend
          -> SARibbon third-party
          -> FreeCAD CommandManager
```

不要反向依赖：

```text
SARibbon -> McStudio
FreeCAD MainWindow -> SARibbonBackend
```

## 结构合理性评分

| 层级 | 是否合理 | 说明 |
|---|---|---|
| McStudioRibbonDockHost | 合理，必须保留 | FreeCAD MainWindow 集成边界 |
| McStudioRibbonHost | 合理，建议保留 | McStudio UI 边界、样式、生命周期隔离 |
| McStudioSARibbonBackend | 合理但不是 UI 节点 | 应作为 adapter/controller，不在 QWidget tree 里 |
| SARibbonBar | 合理 | 第三方 Ribbon 根控件 |
| SARibbonCategory | 合理 | Ribbon Tab/Page |
| SARibbonPanel | 合理 | Ribbon Group |
| SARibbonToolButton | 合理 | Ribbon Button |
| McStudioSARibbonHost | 短期合理，长期可删除 | 额外 root widget，便于 fallback，但不是必需 |

## 决策

长期接受以下架构：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    SARibbonBar
      SARibbonCategory
        SARibbonPanel
          SARibbonToolButton
```

同时接受以下逻辑层：

```text
McStudioSARibbonBackend controls/builds SARibbonBar
```

不接受把 `McStudioSARibbonBackend` 当成 QWidget 视觉树节点。

## 下一步建议

1. 保持当前代码可运行，先完成 GUI 验证。
2. 下一轮重构 `McStudioRibbonBackend` 为 QObject 生命周期。
3. 拆出 `McStudioRibbonDockController`，让 `RibbonWidget.cpp` 只做 show/hide facade。
4. 如果 SARibbon 稳定，删除旧 custom ribbon 创建代码和多余 fallback。
5. 如果删除 custom backend，则考虑移除 `McStudioSARibbonHost`，让 SARibbonBar 直接作为 `McStudioRibbonHost` 的 child。
