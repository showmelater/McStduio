# McStudio RibbonHost 长期架构调研与迁移计划

日期：2026-06-30
状态：Research / Plan
范围：`src/Mod/McStudio/Gui/RibbonWidget.*`、`Workbench.cpp`、`RibbonModel.*`、McStudio Gui CMake

## 目标

将当前“完整 Ribbon 塞进 `QToolBar`”的实现，迁移为长期可维护的 `RibbonHost` 架构：

```text
Gui::MainWindow central widget
  -> McStudioRibbonCentralHost QWidget
      -> McStudioRibbonHost QFrame / QWidget
      -> existing QMdiArea
```

核心目标：

1. Ribbon 不再参与 FreeCAD 顶部工具栏布局计算。
2. Ribbon 高度由 McStudio 自己控制，不再被 `QToolBar` 拉伸。
3. 保留 FreeCAD 现有 `QMdiArea` 实例，不破坏 `MainWindow::getMdiArea()`。
4. McStudio 工作台激活时显示 Ribbon，离开时隐藏或恢复原 central widget。
5. 不修改 FreeCAD 核心 `Gui::MainWindow` 源码，优先保持改动局限在 `src/Mod/McStudio/Gui/`。

## 当前实现问题

当前代码路径：

```cpp
// RibbonWidget.cpp
ribbon = new QToolBar(mainWindow);
ribbon->addWidget(createRibbonWidget(ribbon));
mainWindow->addToolBar(Qt::TopToolBarArea, ribbon);
```

问题：

1. `QToolBar` 是 FreeCAD 顶部工具栏系统的一部分，会天然参与顶部工具栏行宽、高度、拖拽、持久化菜单。
2. 完整 `QTabWidget + QScrollArea + Ribbon groups` 被当成一个 toolbar widget 使用，导致布局语义错位。
3. `QScrollArea::setWidgetResizable(true)` 与 `layout->addStretch(1)` 会放大右侧空白。
4. McStudio Ribbon 需要的是“工作台命令面板”，不是普通 toolbar。

## FreeCAD 主窗口调研

`Gui::MainWindow` 继承 `QMainWindow`。

关键事实：

```cpp
// src/Gui/MainWindow.cpp
 d->mdiArea = new QMdiArea();
 setCentralWidget(d->mdiArea);
```

`MainWindow` 公开：

```cpp
QMdiArea* getMdiArea() const;
```

大量 C++ 代码通过 `getMainWindow()->getMdiArea()` 访问 MDI 区，而不是依赖 `centralWidget()`。

风险点：

- 少量 Python 代码，例如 `src/Mod/Plot/Plot.py`，会在 `MainWindow.children()` 中找 `QMdiArea`。
- 因此迁移时不能销毁或重新创建 `QMdiArea`，也不要把它变成非 MainWindow 间接不可见对象。
- 使用 `QMainWindow::takeCentralWidget()` 移出原 `QMdiArea`，再放进新的 central host，理论上仍保留原对象与 C++ 指针。

## 方案比较

### 方案 A：继续使用 QToolBar，限制高度

优点：改动小。

缺点：仍然挂在 FreeCAD 工具栏系统里，长期会继续与 toolbar 持久化、拖拽、锁定、换行逻辑冲突。

结论：只适合短期止血，不是长期方案。

### 方案 B：Top QDockWidget

优点：不用替换 central widget。

缺点：

1. Dock 语义是可停靠窗口，不是 Ribbon。
2. 会参与 DockWindowManager / OverlayManager。
3. 顶部 dock 会影响 overlay 几何和用户停靠状态。
4. 用户可能拖动、关闭、浮动 Ribbon，违背工作台命令面板语义。

结论：不推荐。

### 方案 C：包装 central widget，插入 RibbonHost

结构：

```text
QMainWindow
  centralWidget = McStudioRibbonCentralHost
    QVBoxLayout
      McStudioRibbonHost
      existing QMdiArea
```

优点：

1. Ribbon 脱离 toolbar/dock 系统。
2. 视觉位置正确：菜单/工具栏之下，MDI 区之上。
3. 不改 FreeCAD 核心 `MainWindow`。
4. 可由 McStudio Workbench 激活/退出控制。
5. 保留原 `QMdiArea` 实例，`MainWindow::getMdiArea()` 继续有效。

主要风险：

1. `centralWidget()` 不再是 `QMdiArea`，极少数外部代码可能受影响。
2. `QMdiArea` 的 `pos()` 会因 RibbonHost 增加一个 Y 偏移，Overlay 代码依赖 `getMdiArea()->pos()`，理论上这是正确的，但需要手工验证 overlay/dock 自动隐藏。
3. 多次进入/退出 McStudio 需要避免重复包装。
4. 离开 McStudio 后是否恢复原 central widget，需要明确策略。

结论：推荐作为长期方案。

## 推荐架构

新增两个概念：

```cpp
class RibbonHost;          // 只负责 Ribbon UI 本体
class RibbonCentralHost;   // 包装原 QMdiArea + RibbonHost
```

但可以先不暴露两个类，先在 `RibbonWidget.cpp` 内部匿名 namespace 实现，降低头文件扩散。

### 对外接口保持不变

```cpp
class RibbonWidget
{
public:
    static void show();
    static void hide();
};
```

Workbench 不需要改调用方式：

```cpp
void Workbench::activated()
{
    Gui::Workbench::activated();
    RibbonWidget::show();
}

void Workbench::deactivated()
{
    RibbonWidget::hide();
    Gui::Workbench::deactivated();
}
```

## 建议实现步骤

### Phase 1：引入 central wrapper，但不重构 Ribbon 细节

1. 删除外层 `QToolBar#McStudioRibbonToolBar` host。
2. 新增 `McStudioRibbonCentralHost` objectName。
3. `show()` 时：
   - 获取 `Gui::MainWindow*`。
   - 如果当前 central widget 已经是 `McStudioRibbonCentralHost`：直接显示 Ribbon。
   - 否则调用 `mainWindow->takeCentralWidget()` 拿到原 central widget。
   - 创建 wrapper。
   - 把 Ribbon widget 放在第一行。
   - 把原 central widget 放在第二行，stretch=1。
   - `mainWindow->setCentralWidget(wrapper)`。
4. `hide()` 时：
   - 初期只隐藏 RibbonHost，不恢复 central widget，避免反复 reparent MDI。
   - 如果需要彻底恢复，可在后续 Phase 增加 unwrap。

### Phase 2：清理 QSS 与对象名

1. `QToolBar#McStudioRibbonToolBar` 改为 `QFrame#McStudioRibbonHost`。
2. command bar 内部仍可暂时使用 `QToolBar[ribbonRole=...]`，因为它只作为按钮容器，不再是 MainWindow 顶部 toolbar。
3. 外层 RibbonHost 固定高度或 maximumHeight。

### Phase 3：行为验证

验证点：

1. 启动 FreeCAD，切换 McStudio，Ribbon 位于菜单/普通工具栏下、MDI 上。
2. 离开 McStudio，Ribbon 隐藏，MDI 不丢失。
3. 反复切换 McStudio 不重复包装。
4. 新建/打开/切换 3D 视图正常。
5. Dock/overlay/任务面板/报告视图正常。
6. Plot 模块或其他查找 MDI 的代码不崩。

### Phase 4：决定是否 unwrap

两种策略：

A. Hide-only：离开 McStudio 时只隐藏 RibbonHost，central wrapper 保留。

优点：稳定，不反复 reparent `QMdiArea`。
缺点：其他工作台下 centralWidget 仍是 wrapper，但 Ribbon 高度为 0/hidden。

B. Restore：离开 McStudio 时把原 central widget 恢复给 MainWindow。

优点：其他工作台恢复原始结构。
缺点：每次工作台切换都 reparent MDI，风险更高。

建议先采用 A，稳定后再考虑 B。

## 伪代码

```cpp
constexpr const char* CentralHostObjectName = "McStudioRibbonCentralHost";
constexpr const char* RibbonHostObjectName = "McStudioRibbonHost";
constexpr const char* OriginalCentralWidgetProperty = "McStudioOriginalCentralWidget";

QWidget* createRibbonHost(QWidget* parent)
{
    auto* host = new QFrame(parent);
    host->setObjectName(QStringLiteral(RibbonHostObjectName));
    host->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* layout = new QVBoxLayout(host);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(createRibbonWidget(host));
    return host;
}

QWidget* ensureCentralHost(Gui::MainWindow* mainWindow)
{
    if (auto* existing = mainWindow->findChild<QWidget*>(QStringLiteral(CentralHostObjectName))) {
        return existing;
    }

    QWidget* original = mainWindow->takeCentralWidget();
    if (!original) {
        original = mainWindow->getMdiArea();
    }

    auto* centralHost = new QWidget(mainWindow);
    centralHost->setObjectName(QStringLiteral(CentralHostObjectName));

    auto* layout = new QVBoxLayout(centralHost);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(createRibbonHost(centralHost), 0);
    layout->addWidget(original, 1);

    mainWindow->setCentralWidget(centralHost);
    return centralHost;
}
```

## 构建修改点

如果拆新文件：

```text
RibbonHost.cpp
RibbonHost.h
```

需要加入：

```cmake
set(McStudioGui_SRCS
    ...
    RibbonHost.cpp
    RibbonHost.h
)
```

但第一阶段可以先只改 `RibbonWidget.cpp`，减少 CMake 变更。

## 推荐下一步

先做 Phase 1：在 `RibbonWidget.cpp` 中完成 central wrapper，实现 `QToolBar` 到 `RibbonCentralHost` 的迁移，并保留 `RibbonWidget::show/hide` 接口不变。

