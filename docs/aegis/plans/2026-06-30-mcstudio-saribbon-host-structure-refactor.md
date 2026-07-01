# McStudio SARibbon Host Structure Refactor

日期：2026-06-30
状态：Implementation

## 目标

根据 `2026-06-30-mcstudio-saribbon-host-structure-review.md`，将当前结构从：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    McStudioSARibbonHost
      SARibbonBar
```

收敛为长期 UI 结构：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    SARibbonBar
```

同时将 `McStudioSARibbonBackend` 明确为 QObject adapter/controller，而不是 UI 节点。

## 本轮修改范围

1. `McStudioRibbonBackend` 改为 QObject 生命周期。
2. `createSARibbonBackend()` 返回 parent-owned backend 指针。
3. `McStudioSARibbonBackend` 直接创建 `SARibbonBar(parent)`。
4. 删除中间 `McStudioSARibbonHost` QWidget。
5. `RibbonWidget.cpp` 不再用 `unique_ptr.release() + destroyed lambda` 管理 backend。
6. 保留旧 custom ribbon fallback。

## 不在本轮处理

- 不拆 `McStudioRibbonDockController`。
- 不删除旧 custom ribbon 代码。
- 不修改经典菜单栏。
- 不做视觉细节调参。

## 2026-06-30 执行记录

已完成结构收敛：

### Backend 生命周期

`McStudioRibbonBackend` 从普通 C++ 抽象类改为 QObject 派生类：

```cpp
class McStudioRibbonBackend: public QObject
```

由 `McStudioRibbonHost` 作为 QObject parent 管理生命周期。

删除了旧方式：

```cpp
unique_ptr.release()
QObject::connect(host, &QObject::destroyed, ... delete backendOwner)
```

### UI 树收敛

`McStudioSARibbonBackend` 不再创建中间 widget：

```text
McStudioSARibbonHost
```

现在直接创建：

```cpp
_ribbon = new SARibbonBar(parent);
```

长期 UI 树变为：

```text
McStudioRibbonDockHost
  McStudioRibbonHost
    SARibbonBar
      SARibbonCategory
        SARibbonPanel
          SARibbonToolButton
```

### 逻辑层

`McStudioSARibbonBackend` 仍负责：

- RibbonModel -> SARibbon 映射
- FreeCAD CommandManager -> QAction 获取
- Ribbon proxy QAction 创建
- ActionGroup/QMenu 下拉适配

但它不再被表达为 QWidget 层级节点。

### 构建验证

执行：

```bash
ninja -C build/debug McStudioGui
```

结果：构建成功，`Mod/McStudio/McStudioGui.so` 链接成功。

既有 version generation 仍输出：

```text
致命错误：Not a valid object name upstream/main
```

该问题与本轮 SARibbon host 结构重构无关。
