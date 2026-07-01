# McStudio Ribbon Architecture Stabilization Plan

Date: 2026-06-27
Status: planned
Owner: McStudio C++ GUI workbench

## Plan Basis

McStudio already has a working C++ Ribbon prototype:

- `McStudioGui::Workbench` loads and activates.
- `RibbonWidget` creates a top-level `QToolBar` host named `McStudioRibbonToolBar`.
- The host contains a `QTabWidget` named `McStudioRibbonTabs`.
- `RibbonModel` provides SolidWorks-style tab/group/command organization.
- Ribbon commands are created through `Gui::Application::Instance->commandManager().addTo()`.
- Ribbon scale is read from `User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon/Scale`.

The current prototype works, but the architecture is still too thin:

- `RibbonModel` only stores raw command strings.
- `RibbonWidget` treats every command the same.
- There is no first-class button size, visibility, command validation, separator, or future dropdown representation.
- Unknown commands currently only emit a warning inside button creation.
- UI sizing and layout are not yet organized as a stable policy object.

## BaselineUsageDraft

- Required baseline refs:
  - Current McStudio C++ Ribbon files.
  - FreeCAD `Workbench`, `ToolBarManager`, `CommandManager`, and module workbench patterns.
  - Existing Aegis McStudio C++ workbench research plan.
- Delivered context refs:
  - User asked to stabilize Ribbon architecture first.
  - User requested code style to follow FreeCAD style.
- Acknowledged before plan refs:
  - Current C++ Ribbon displays and has SolidWorks-style tabs.
  - Current Ribbon scale parameter exists.
- Cited in plan refs:
  - `src/Mod/McStudio/Gui/RibbonModel.h`
  - `src/Mod/McStudio/Gui/RibbonModel.cpp`
  - `src/Mod/McStudio/Gui/RibbonWidget.cpp`
  - `src/Mod/McStudio/Gui/Workbench.cpp`
  - `src/Gui/ToolBarManager.cpp`
  - `src/Gui/Command.cpp`
- Missing refs:
  - No formal McStudio product UX spec yet.
- Decision: continue

## Requirement Ready Check

- Requirement source refs:
  - User: “把 Ribbon 架构稳定，先用 aegies 进行规划一下，代码要仿照 freecad 的风格写。”
- Goals and scope refs:
  - Stabilize Ribbon architecture before adding deeper business functionality.
  - Follow FreeCAD C++ style.
- User / scenario refs:
  - McStudio should feel like a SolidWorks-style CommandManager while remaining inside FreeCAD.
- Requirement item refs:
  - Stable model/widget split.
  - FreeCAD-style implementation.
  - Preserve current working Ribbon behavior.
- Acceptance / verification criteria refs:
  - Builds with `ninja McStudioGui McStudio`.
  - No regression in workbench activation.
  - Ribbon still displays tabs and commands.
  - Unknown commands are handled deliberately.
- Open blocker questions:
  - None for architecture stabilization slice.
- Decision: ready

## Compatibility Boundary

Preserve:

- `InitGui.py` workbench registration behavior.
- `McStudioGui::Workbench::activated()` / `deactivated()` lifecycle.
- `CommandManager::addTo()` as the only command-button creation path.
- Current Ribbon object names:
  - `McStudioRibbonToolBar`
  - `McStudioRibbonTabs`
- Current `Scale` parameter path:
  - `User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon/Scale`

Do not change in this plan:

- FreeCAD core `ToolBarManager`.
- FreeCAD core `MainWindow`.
- FreeCAD core `CommandManager`.
- McStudio placeholder command behavior.
- McStudio App-layer document objects.

## FreeCAD Style Rules for This Plan

Follow these existing FreeCAD patterns:

1. Use module namespace boundaries:
   - Public declarations under `namespace McStudioGui`.
   - Local helpers inside anonymous namespace in `.cpp` files.
2. Keep lifecycle code in `Workbench.cpp`, not in the model.
3. Keep UI rendering in `RibbonWidget.cpp`, not in `Workbench.cpp`.
4. Keep declarative tab/group/command data in `RibbonModel.cpp`.
5. Use `ParameterGrp::handle` and `App::GetApplication().GetParameterGroupByPath()` for preferences.
6. Use `Gui::Application::Instance->commandManager().addTo()` for command buttons.
7. Prefer small structs/enums over dynamic ad-hoc maps.
8. Prefer explicit object names for Qt widgets.
9. Keep CMake source lists explicit.
10. Do not introduce a generic UI framework beyond what McStudio needs now.

## Architecture Integrity Lens

- Invariant:
  - Ribbon is McStudio-owned UI, but command execution and action state remain FreeCAD-owned.
- Canonical owner / contract:
  - `RibbonModel`: command layout contract.
  - `RibbonWidget`: Qt rendering and preference application contract.
  - `Workbench`: activation contract.
  - `CommandManager`: FreeCAD command execution contract.
- Responsibility overlap:
  - Avoid putting rendering rules into `RibbonModel` beyond declarative metadata.
  - Avoid putting command layout arrays into `RibbonWidget`.
- Higher-level simplification:
  - Extend current `RibbonModel` structs rather than introducing a JSON/theme parser.
- Retirement / falsifier:
  - If `RibbonWidget.cpp` grows into unrelated preference, command, and styling owners, split `RibbonStyle` or `RibbonPreferences` later.
- Verdict:
  - Proceed with focused stabilization in existing McStudio GUI module.

## Plan-Time Complexity Check

- Target files:
  - `src/Mod/McStudio/Gui/RibbonModel.h`
  - `src/Mod/McStudio/Gui/RibbonModel.cpp`
  - `src/Mod/McStudio/Gui/RibbonWidget.cpp`
  - Possibly `src/Mod/McStudio/Gui/RibbonWidget.h`
- Existing size / shape signals:
  - `RibbonModel` is small and data-only.
  - `RibbonWidget` already owns metrics, widget creation, command creation, and show/hide.
- Owner fit:
  - Stable enough for one more architecture slice.
- Add-in-place risk:
  - Medium if large/small button and validation are added without helper extraction.
- Better file boundary:
  - Keep this slice in `RibbonModel` / `RibbonWidget` only.
  - Defer `RibbonStyle` / `RibbonPreferences` files until pressure is visible.
- Recommendation: edit-in-place with small helper structs; do not create extra files yet.

## Target Design

### Ribbon model

Change the model from raw commands:

```cpp
struct RibbonGroupDefinition
{
    const char* title;
    std::vector<const char*> commands;
};
```

to explicit command metadata:

```cpp
enum class RibbonCommandSize
{
    Large,
    Small,
};

enum class RibbonCommandStyle
{
    TextUnderIcon,
    TextBesideIcon,
    IconOnly,
};

struct RibbonCommandDefinition
{
    const char* command;
    RibbonCommandSize size;
    RibbonCommandStyle style;
};

struct RibbonGroupDefinition
{
    const char* title;
    std::vector<RibbonCommandDefinition> commands;
};
```

Add helper constructors to keep `RibbonModel.cpp` readable and FreeCAD-like:

```cpp
RibbonCommandDefinition largeCommand(const char* command);
RibbonCommandDefinition smallCommand(const char* command);
RibbonCommandDefinition iconCommand(const char* command);
```

### Ribbon widget

Rendering rules:

- Large commands:
  - icon size: `32 * Scale`
  - button style: `Qt::ToolButtonTextUnderIcon`
- Small commands:
  - icon size: `20 * Scale`
  - button style: `Qt::ToolButtonTextBesideIcon`
- Icon-only commands:
  - icon size: `20 * Scale`
  - button style: `Qt::ToolButtonIconOnly`

If a command does not exist in `CommandManager`, do not add a placeholder button. Log once:

```text
McStudio Ribbon: skipping unknown command '<name>'.
```

### Acceptance behavior

After this plan is implemented:

- Ribbon still appears when switching to McStudio.
- Tabs remain at bottom.
- Scale parameter still applies.
- SolidWorks-style tab organization remains.
- Main feature commands can be visually larger than secondary tools.
- Unknown commands are skipped instead of producing empty/failed actions.
- No FreeCAD core files are modified.

## Task Breakdown

### Task 1 — Extend Ribbon model types

Files:

- `src/Mod/McStudio/Gui/RibbonModel.h`

Steps:

1. Add `RibbonCommandSize` enum.
2. Add `RibbonCommandStyle` enum.
3. Add `RibbonCommandDefinition` struct.
4. Change `RibbonGroupDefinition::commands` from `std::vector<const char*>` to `std::vector<RibbonCommandDefinition>`.
5. Declare helper functions:
   - `largeCommand(const char*)`
   - `smallCommand(const char*)`
   - `iconCommand(const char*)`

Verification:

```bash
cd /home/aa/Desktop/McStduio
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py
```

Expected:

```text
No Python syntax errors.
```

### Task 2 — Convert RibbonModel.cpp to explicit button metadata

Files:

- `src/Mod/McStudio/Gui/RibbonModel.cpp`

Steps:

1. Define the helper functions declared in the header.
2. Convert all command strings to `largeCommand(...)` or `smallCommand(...)`.
3. Use large commands for primary SolidWorks-like entry points:
   - `PartDesign_Pad`
   - `PartDesign_Pocket`
   - `PartDesign_Hole`
   - `Sketcher_LeaveSketch`
   - `PartDesign_NewSketch`
   - `McStudio_CreateWorkpiece`
   - `McStudio_CreateStock`
   - `McStudio_SetWorkCoordinateSystem`
4. Use small commands for secondary tools and view/evaluation commands.
5. Do not add new commands in this task.

Verification:

```bash
cd /home/aa/Desktop/McStduio/build/debug
ninja McStudioGui
```

Expected:

```text
McStudioGui builds.
```

### Task 3 — Render command sizes in RibbonWidget

Files:

- `src/Mod/McStudio/Gui/RibbonWidget.cpp`

Steps:

1. Update `addCommand()` to accept `RibbonCommandDefinition` and metrics.
2. Create a helper `configureActionWidget()` if needed, but prefer minimal code.
3. Use `QToolBar::setToolButtonStyle()` per group or per widget where feasible.
4. If per-button style is not feasible through `CommandManager::addTo()`, choose the simpler FreeCAD-compatible rule:
   - one internal toolbar for large commands
   - one internal toolbar for small commands
   - both inside the same `RibbonGroup` frame.
5. Keep command creation through `CommandManager::addTo()`.
6. Skip unknown commands with a warning.

Verification:

```bash
cd /home/aa/Desktop/McStduio/build/debug
ninja McStudioGui McStudio
```

Expected:

```text
Build completes and McStudioGui.so is updated.
```

### Task 4 — Preserve scale behavior

Files:

- `src/Mod/McStudio/Gui/RibbonWidget.cpp`

Steps:

1. Keep `ribbonScale()` and `ribbonMetrics()`.
2. Add metrics for large and small icon sizes:
   - `largeIconSize = 32 * Scale`
   - `smallIconSize = 20 * Scale`
3. Keep group/page margins scaled.
4. Keep `McStudioRibbonScale` property rebuild behavior.

Verification:

In FreeCAD Python Console:

```python
import FreeCAD
p = FreeCAD.ParamGet("User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon")
p.SetFloat("Scale", 1.25)
```

Then switch away from McStudio and back.

Expected:

```text
Ribbon is rebuilt with larger icons, fonts, margins, and spacing.
```

### Task 5 — Runtime diagnostic check

Files:

- No source changes unless a bug is found.

Steps:

1. Start FreeCAD with a fresh log:

```bash
cd /home/aa/Desktop/McStduio
rm -f /tmp/mcstudio.log
./build/debug/bin/FreeCAD --log-file /tmp/mcstudio.log
```

2. Switch to McStudio.
3. Check logs:

```bash
grep -E "McStudio Ribbon|unknown command|skipping unknown command|McStudioGui" /tmp/mcstudio.log
```

Expected:

```text
No unknown commands for the currently configured Ribbon command list.
McStudioGui activates normally.
```

### Task 6 — Documentation checkpoint

Files:

- `docs/aegis/plans/2026-06-27-mcstudio-cpp-ribbon-workbench-research.md`
- This plan file.

Steps:

1. Append an implementation checkpoint after code changes.
2. Record the final command list and verification output.
3. Run Aegis workspace check:

```bash
cd /home/aa/Desktop/McStduio
python aegis-method-pack/scripts/aegis-workspace.py check --root .
```

Expected:

```text
Aegis workspace check passed
```

## Risks

1. Per-button style may be limited by FreeCAD `CommandManager::addTo()` returning a generic `QAction` inside `QToolBar`.
   - Mitigation: use separate internal toolbars for large and small commands inside a group.
2. Some commands may only be registered after their GUI module imports.
   - Mitigation: keep required imports in `AppMcStudioGui.cpp` and skip unknown commands.
3. RibbonWidget may grow too large.
   - Mitigation: after this stabilization, split `RibbonStyle` or `RibbonPreferences` only if file pressure increases.
4. Large icons may make the toolbar too tall on smaller screens.
   - Mitigation: scale parameter and future compact mode.

## Non-goals

- No custom preference page in this slice.
- No FreeCAD core toolbar manager changes.
- No actual McStudio document object implementation.
- No CAM/Path operation implementation.
- No full SolidWorks visual parity yet.

## Verification Commands

Minimum verification before claiming implementation complete:

```bash
cd /home/aa/Desktop/McStduio
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py
cd build/debug && ninja McStudioGui McStudio
cd ../.. && python aegis-method-pack/scripts/aegis-workspace.py check --root .
```

Manual verification:

```bash
cd /home/aa/Desktop/McStduio
rm -f /tmp/mcstudio.log
./build/debug/bin/FreeCAD --log-file /tmp/mcstudio.log
```

Then switch to McStudio and confirm:

- Ribbon visible.
- Tabs at bottom.
- Large/small button layout visible.
- Scale parameter works after switching away/back.
- No unknown command warnings for configured commands.

## Retirement / Follow-up

After this plan lands, the next likely workstream is one of:

1. Add McStudio Ribbon preference page for `Scale`, icon mode, and compact mode.
2. Implement first real McStudio command, likely `McStudio_CreateWorkpiece`.
3. Add context-sensitive Ribbon behavior for Sketch edit mode.

The temporary risk to retire is “RibbonWidget is a prototype blob.” This plan reduces that by formalizing model metadata and rendering policy.

## Implementation Checkpoint 2026-06-27 16:12

Implemented tasks:

```text
Task 1 — Extend Ribbon model types: done
Task 2 — Convert RibbonModel.cpp to explicit button metadata: done
Task 3 — Render command sizes in RibbonWidget: done
Task 4 — Preserve scale behavior: done
```

Changed files:

```text
src/Mod/McStudio/Gui/RibbonModel.h
src/Mod/McStudio/Gui/RibbonModel.cpp
src/Mod/McStudio/Gui/RibbonWidget.cpp
```

Model changes:

```text
RibbonCommandSize: Large / Small
RibbonCommandStyle: TextUnderIcon / TextBesideIcon / IconOnly
RibbonCommandDefinition: command + size + style
largeCommand() / smallCommand() / iconCommand() helpers
```

Widget rendering changes:

```text
Large commands render in a dedicated large command toolbar.
Small text commands render in a secondary small command toolbar.
Icon-only commands render in a secondary icon-only toolbar.
Unknown commands are skipped with a clear warning instead of creating failed buttons.
Ribbon scale now drives large icon size, small icon size, fonts, margins, and spacing.
```

FreeCAD-style boundaries held:

```text
Command buttons still use Gui::Application::Instance->commandManager().addTo().
Preferences still use App::GetApplication().GetParameterGroupByPath().
Workbench lifecycle remains in Workbench.cpp.
Ribbon model data remains in RibbonModel.cpp.
Ribbon rendering remains in RibbonWidget.cpp.
No FreeCAD core ToolBarManager/MainWindow/CommandManager files were changed.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 16:12, size about 2.3M
python aegis-method-pack/scripts/aegis-workspace.py check --root . -> Aegis workspace check passed
```

Residual risk:

```text
Manual GUI verification is still required to tune visual balance between large and small buttons.
Runtime unknown-command warnings are now safer because commands are skipped, but the log still needs checking in a GUI run.
```

## Dropdown Group Research 2026-06-27

User requirement:

```text
同类的操作要下拉框进行选择，比如圆角 / 倒角。
先完成基础建设，先调研哪些地方需要改。
```

FreeCAD source findings:

```text
FreeCAD already has native dropdown command infrastructure through Gui::GroupCommand and Gui::ActionGroup.
GroupCommand defaults: checkable = true, exclusive = false, rememberLast = true, dropDownMenu = true.
GroupCommand::createAction() creates an ActionGroup.
ActionGroup::addTo(QToolBar) adds one toolbar action, finds the generated QToolButton, sets QToolButton::MenuButtonPopup, creates a QMenu, and attaches child command actions.
CommandManager::addTo() is still the correct entry point.
```

Relevant source anchors:

```text
src/Gui/Command.h:746-786       Gui::GroupCommand definition and default behavior
src/Gui/Command.cpp:1106-1268   GroupCommand constructor, addCommand(), createAction(), setup()
src/Gui/Action.cpp:454-493      ActionGroup::addTo(); QToolBar branch creates MenuButtonPopup
src/Mod/PartDesign/Gui/Command.cpp:2640-2660 PartDesign_CompDatums example
src/Mod/PartDesign/Gui/Command.cpp:2675-2695 PartDesign_CompSketches example
src/Mod/PartDesign/Gui/Workbench.cpp:580-629 official toolbar use of group commands
```

Design decision:

```text
Do not implement dropdowns as raw QComboBox widgets.
Use FreeCAD-native Gui::GroupCommand for same-kind command groups.
Expose those GroupCommand names through RibbonModel like normal commands.
RibbonWidget should keep using CommandManager::addTo(); ActionGroup will create the dropdown menu automatically.
```

Existing FreeCAD groups that can be reused:

```text
PartDesign_CompDatums: PartDesign_Plane / PartDesign_Line / PartDesign_Point / PartDesign_CoordinateSystem
PartDesign_CompSketches: PartDesign_NewSketch / Sketcher_MapSketch / Sketcher_EditSketch
PartDesign_CompPrimitiveAdditive: additive primitive group
PartDesign_CompPrimitiveSubtractive: subtractive primitive group
Std_ViewGroup: standard view group
```

Groups McStudio probably needs to add:

```text
McStudio_CompDressUpFeatures:
  PartDesign_Fillet
  PartDesign_Chamfer
  PartDesign_Draft
  PartDesign_Thickness

McStudio_CompTransformFeatures:
  PartDesign_Mirrored
  PartDesign_LinearPattern
  PartDesign_PolarPattern
  PartDesign_MultiTransform

McStudio_CompSketchSession:
  Sketcher_LeaveSketch
  PartDesign_NewSketch
  Sketcher_EditSketch
  Sketcher_MapSketch
  Sketcher_ValidateSketch

McStudio_CompSurfaceCreate:
  Surface_Filling
  Surface_GeomFillSurface
  Surface_Sections
  Part_Loft
  Part_Sweep
  Part_RuledSurface

McStudio_CompSurfaceEdit:
  Surface_Cut
  Surface_ExtendFace
  Surface_BlendCurve
  Part_Offset
  Part_Thickness
  Part_ProjectionOnSurface

McStudio_CompSetupObjects:
  McStudio_CreateWorkpiece
  McStudio_CreateStock
  McStudio_CreateFixture

McStudio_CompSetupCoordinate:
  McStudio_SetFixtureLocation
  McStudio_SetWorkCoordinateSystem
  McStudio_CreateSetupDatum
```

Likely files to modify for implementation:

```text
src/Mod/McStudio/Gui/Command.cpp
  Add McStudio GroupCommand classes and register them in CreateMcStudioCommands().

src/Mod/McStudio/Gui/RibbonModel.h
  Optional: add dropDownCommand() helper for readability. Not required for mechanics because GroupCommand is still a normal command.

src/Mod/McStudio/Gui/RibbonModel.cpp
  Replace same-kind individual commands with McStudio_Comp* group command names or existing PartDesign_Comp* group names.

src/Mod/McStudio/Gui/RibbonWidget.cpp
  Probably no structural change required. It already uses CommandManager::addTo() on QToolBar, which is exactly what ActionGroup::addTo() expects.

src/Mod/McStudio/Gui/AppMcStudioGui.cpp
  No new change currently required if existing imports remain: PartGui / PartDesignGui / SketcherGui / SurfaceGui / MeasureGui.
```

Implementation risk:

```text
GroupCommand::addCommand("ChildCommand") only finds commands already registered in CommandManager.
Therefore AppMcStudioGui.cpp must import dependency GUI modules before CreateMcStudioCommands(). Current code already does this.
```

Next implementation slice:

```text
1. Add McStudio GroupCommand classes in Command.cpp.
2. Register them after existing McStudio commands.
3. Add a dropDownCommand() helper to RibbonModel for readability.
4. Replace individual same-kind commands in RibbonModel.cpp with group commands.
5. Build and run log check for unknown/skipping command warnings.
```

## Dropdown Group Implementation Checkpoint 2026-06-27 16:27

Implemented minimal dropdown infrastructure slice:

```text
Added McStudio_CompDressUpFeatures as a FreeCAD-native Gui::GroupCommand.
Replaced the flat dress-up feature buttons in RibbonModel with one dropdown command.
Kept RibbonWidget unchanged structurally because CommandManager::addTo() already triggers ActionGroup dropdown behavior.
```

Changed files:

```text
src/Mod/McStudio/Gui/Command.cpp
src/Mod/McStudio/Gui/RibbonModel.h
src/Mod/McStudio/Gui/RibbonModel.cpp
```

New command:

```text
McStudio_CompDressUpFeatures
```

Children:

```text
PartDesign_Fillet
PartDesign_Chamfer
PartDesign_Draft
PartDesign_Thickness
```

Ribbon model change:

```text
Added dropDownCommand(const char*) helper.
Changed “修改特征” group from four flat commands to:
  dropDownCommand("McStudio_CompDressUpFeatures")
```

FreeCAD-style boundary held:

```text
The dropdown is implemented through Gui::GroupCommand.
The Ribbon still uses CommandManager::addTo().
No QComboBox or custom dropdown widget was introduced.
No FreeCAD core files were changed.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 16:27, size about 2.3M
```

Manual verification still required:

```text
Switch to McStudio and confirm “修改特征” renders as a dropdown/menu-button.
Check /tmp/mcstudio.log for McStudio Ribbon warnings.
```

## Ribbon Layout Size Adjustment Checkpoint 2026-06-27 16:36

User feedback:

```text
Dropdown mechanism works, but the overall icons are too small.
```

Layout analysis:

```text
The previous default large icon size was 32px and small icon size was 20px.
Those values made the ribbon look closer to a regular FreeCAD toolbar than a SolidWorks-style CommandManager.
Group margins were also small: horizontal 4px, vertical 2px, page margin 4px, spacing 4px.
```

Implemented adjustment:

```text
Base large icon size: 32 -> 44
Base small icon size: 20 -> 28
Base group horizontal margin: 4 -> 6
Base group vertical margin: 2 -> 4
Base page margin: 4 -> 6
Base spacing: 4 -> 6
Command row alignment: Qt::AlignTop
Secondary column alignment: Qt::AlignTop
```

Scale behavior retained:

```text
All new base values are still multiplied by User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon/Scale.
Scale remains clamped between 0.70 and 1.80.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 16:36, size about 2.3M
```

Residual risk:

```text
Manual GUI verification is still required to confirm whether 44/28 is visually balanced on the user's display.
If still small, raise Scale to 1.15 or increase base large icon size to 48.
If too tall, reduce base large icon size to 40 or use Scale 0.90.
```

## Ribbon Size Rebalance / Width Research Checkpoint 2026-06-27 16:43

User feedback:

```text
Large icons should go back to the previous size.
Small icons should be only slightly larger.
The whole workbench/ribbon appears too long horizontally, making the page long.
```

Implemented size rebalance:

```text
Base large icon size: 44 -> 32
Base small icon size: 28 -> 24
Base group horizontal margin: 6 -> 4
Base group vertical margin: 4 -> 3
Base page margin: 6 -> 4
Base spacing: 6 -> 4
```

Scale behavior retained:

```text
All base values remain multiplied by User parameter:BaseApp/Preferences/Mod/McStudio/Ribbon/Scale.
```

Width research finding:

```text
Current Ribbon page layout uses one QHBoxLayout in createRibbonPage().
Every Ribbon group is appended left-to-right through layout->addWidget(createRibbonGroup(...)).
The layout ends with layout->addStretch(1).
There is no wrapping, no horizontal scroll area, and no compact/overflow policy.
Therefore tab pages with many groups or many visible commands will naturally become horizontally long.
```

Relevant current implementation:

```text
src/Mod/McStudio/Gui/RibbonWidget.cpp
  createRibbonPage(): QHBoxLayout page layout
  createRibbonGroup(): large toolbar + secondary small/icon toolbar inside one group frame
```

Likely solutions, from lowest risk to highest:

```text
1. Continue replacing same-kind command clusters with GroupCommand dropdowns.
   This is closest to FreeCAD style and reduces horizontal width.

2. Add a horizontal QScrollArea around each Ribbon page.
   Keeps single-row ribbon, avoids page expansion, but introduces scroll behavior.

3. Add a CompactMode preference that hides text for small commands or converts more commands to dropdown groups.
   Good for smaller screens, but needs preference UI later.

4. Implement a custom flow/wrap layout for Ribbon groups.
   Reduces width by wrapping groups to a second row, but increases ribbon height and adds custom layout complexity.
```

Recommendation:

```text
Do not implement custom wrapping yet.
First reduce width by grouping same-kind commands into dropdown GroupCommands:
  Transform features
  Reference geometry
  Sketch session
  Surface create/edit
  Setup objects/coordinates
Then re-check visual width.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 16:43, size about 2.3M
```

## Same-Kind Dropdown Merge Research 2026-06-27

User request:

```text
继续合并同类命令为下拉组。先看看 FreeCAD 中和 SolidWorks 中哪些是同类命令适合合并。
```

Research basis from FreeCAD source:

```text
PartDesign Workbench groups commands into:
  Additive Features
  Subtractive Features
  Dress-Up Features
  Transformation Features

PartDesign Toolbars already use existing GroupCommands:
  PartDesign_CompSketches
  PartDesign_CompPrimitiveAdditive
  PartDesign_CompPrimitiveSubtractive

Sketcher Workbench heavily uses Sketcher_Comp* GroupCommands:
  Sketcher_CompLine
  Sketcher_CompCreateArc
  Sketcher_CompCreateConic
  Sketcher_CompCreateRectangles
  Sketcher_CompCreateRegularPolygon
  Sketcher_CompSlot
  Sketcher_CompCreateBSpline
  Sketcher_CompCreateFillets
  Sketcher_CompCurveEdition
  Sketcher_CompDimensionTools
  Sketcher_CompHorVer
  Sketcher_CompToggleConstraints

Surface Workbench groups all surface commands into one surface toolbar, but McStudio should split them into create/edit groups for SolidWorks-style organization.
```

SolidWorks-style grouping principle for McStudio:

```text
Keep core direct operations visible as large buttons:
  Body
  Pad / Extrude Boss
  Pocket / Extrude Cut
  Hole
  New Sketch / Exit Sketch
  Workpiece / Stock / WCS

Merge same-kind variants into dropdowns:
  Dress-up features: fillet/chamfer/draft/thickness
  Pattern/transform features: mirror/linear pattern/polar pattern/multi-transform
  Additive feature variants: pad/revolution/loft/pipe/primitive
  Subtractive feature variants: pocket/hole/groove/subtractive loft/subtractive pipe/subtractive primitive
  Reference geometry: plane/line/point/coordinate system
  Sketch creation variants: line/arc/conic/rectangle/polygon/slot/fillet/trim
  Constraint variants: dimension/hor-ver/radius-diameter/toggle constraints
  Surface create/edit variants
  Setup object/coordinate variants
```

High-confidence merge candidates:

```text
A. Reuse existing FreeCAD GroupCommands directly:
  PartDesign_CompDatums
    PartDesign_Plane
    PartDesign_Line
    PartDesign_Point
    PartDesign_CoordinateSystem

  PartDesign_CompSketches
    PartDesign_NewSketch
    Sketcher_MapSketch
    Sketcher_EditSketch

  PartDesign_CompPrimitiveAdditive
  PartDesign_CompPrimitiveSubtractive

  Sketcher_CompLine
  Sketcher_CompCreateArc
  Sketcher_CompCreateConic
  Sketcher_CompCreateRectangles
  Sketcher_CompCreateRegularPolygon
  Sketcher_CompSlot
  Sketcher_CompCreateBSpline
  Sketcher_CompCreateFillets
  Sketcher_CompCurveEdition
  Sketcher_CompDimensionTools
  Sketcher_CompHorVer
  Sketcher_CompToggleConstraints
```

New McStudio GroupCommands to add:

```text
McStudio_CompTransformFeatures
  PartDesign_Mirrored
  PartDesign_LinearPattern
  PartDesign_PolarPattern
  PartDesign_MultiTransform

McStudio_CompAdditiveFeatures
  PartDesign_Pad
  PartDesign_Revolution
  PartDesign_AdditiveLoft
  PartDesign_AdditivePipe
  PartDesign_CompPrimitiveAdditive

McStudio_CompSubtractiveFeatures
  PartDesign_Pocket
  PartDesign_Hole
  PartDesign_Groove
  PartDesign_SubtractiveLoft
  PartDesign_SubtractivePipe
  PartDesign_CompPrimitiveSubtractive

McStudio_CompSketchSession
  Sketcher_LeaveSketch
  PartDesign_NewSketch
  Sketcher_EditSketch
  Sketcher_MapSketch
  Sketcher_ValidateSketch

McStudio_CompSurfaceCreate
  Surface_Filling
  Surface_GeomFillSurface
  Surface_Sections
  Part_Loft
  Part_Sweep
  Part_RuledSurface

McStudio_CompSurfaceEdit
  Surface_Cut
  Surface_ExtendFace
  Surface_BlendCurve
  Part_Offset
  Part_Thickness
  Part_ProjectionOnSurface

McStudio_CompEvaluateInspect
  Std_Measure
  Std_MassProperties
  Part_ShapeInfo
  Part_CheckGeometry

McStudio_CompSectionTools
  Part_Section
  Part_CrossSections
  Part_SectionCut
  Std_ToggleClipPlane

McStudio_CompSetupObjects
  McStudio_CreateWorkpiece
  McStudio_CreateStock
  McStudio_CreateFixture

McStudio_CompSetupCoordinate
  McStudio_SetFixtureLocation
  McStudio_SetWorkCoordinateSystem
  McStudio_CreateSetupDatum

McStudio_CompModelOutput
  Part_Import
  Part_Export
  Part_SimpleCopy
  Part_RefineShape
```

Recommended implementation order:

```text
1. Reuse PartDesign_CompDatums and PartDesign_CompSketches in RibbonModel.
2. Add McStudio_CompTransformFeatures.
3. Add McStudio_CompSketchSession, but keep Sketcher_LeaveSketch as a separate first button if sketch workflow requires it.
4. Replace sketch drawing commands with existing Sketcher_Comp* groups.
5. Add McStudio_CompSurfaceCreate and McStudio_CompSurfaceEdit.
6. Add McStudio_CompSetupObjects and McStudio_CompSetupCoordinate.
7. Consider Additive/Subtractive dropdowns only after checking whether Pad/Pocket/Hole should remain direct large buttons.
```

Important UX decision:

```text
Do not over-collapse the primary modeling actions.
SolidWorks-style ribbon keeps major operations discoverable while grouping variants.
For McStudio, keep Pad/Pocket/Hole/New Sketch/Exit Sketch/WCS/Workpiece visible unless horizontal width remains too large.
```

## Same-Kind Dropdown Merge Implementation Checkpoint 2026-06-27 16:55

Implemented first high-confidence same-kind dropdown merge batch.

Changed files:

```text
src/Mod/McStudio/Gui/Command.cpp
src/Mod/McStudio/Gui/RibbonModel.h
src/Mod/McStudio/Gui/RibbonModel.cpp
```

New McStudio GroupCommands:

```text
McStudio_CompTransformFeatures
  PartDesign_Mirrored
  PartDesign_LinearPattern
  PartDesign_PolarPattern
  PartDesign_MultiTransform

McStudio_CompSurfaceCreate
  Surface_Filling
  Surface_GeomFillSurface
  Surface_Sections
  Part_Loft
  Part_Sweep
  Part_RuledSurface

McStudio_CompSurfaceEdit
  Surface_Cut
  Surface_ExtendFace
  Surface_BlendCurve
  Part_Offset
  Part_Thickness
  Part_ProjectionOnSurface

McStudio_CompEvaluateInspect
  Std_Measure
  Std_MassProperties
  Part_ShapeInfo
  Part_CheckGeometry

McStudio_CompSectionTools
  Part_Section
  Part_CrossSections
  Part_SectionCut
  Std_ToggleClipPlane
```

Reused existing FreeCAD GroupCommands in RibbonModel:

```text
PartDesign_CompDatums
PartDesign_CompSketches
Sketcher_CompLine
Sketcher_CompCreateArc
Sketcher_CompCreateConic
Sketcher_CompCreateRectangles
Sketcher_CompCreateFillets
Sketcher_CompCurveEdition
Sketcher_CompDimensionTools
Sketcher_CompHorVer
```

Model helper added:

```text
smallDropDownCommand(const char*)
```

UX decisions preserved:

```text
PartDesign_Pad remains direct large button.
PartDesign_Pocket remains direct large button.
PartDesign_Hole remains direct large button.
Sketcher_LeaveSketch remains first direct large button in the Sketch tab.
McStudio_CreateWorkpiece, McStudio_CreateStock, and McStudio_SetWorkCoordinateSystem remain direct large buttons.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 16:55, size about 2.3M
```

Manual verification required:

```text
Start FreeCAD, switch to McStudio, and check that the new groups show as dropdown buttons.
Check /tmp/mcstudio.log for McStudio Ribbon skipping unknown command warnings.
```

## Standard Toolbar to Ribbon Migration Research 2026-06-27

User request:

```text
希望把标准工具栏都变成 Ribbon。应该隐藏还是怎么做？
```

Source finding:

```text
McStudio currently inherits Gui::StdWorkbench.
McStudio Workbench::setupToolBars() returns StdWorkbench::setupToolBars().
Therefore FreeCAD standard toolbars are created by ToolBarManager in parallel with McStudio RibbonWidget.
```

Current source:

```text
src/Mod/McStudio/Gui/Workbench.h
  class Workbench: public Gui::StdWorkbench

src/Mod/McStudio/Gui/Workbench.cpp
  Workbench::setupToolBars() returns StdWorkbench::setupToolBars()
```

FreeCAD standard toolbar source:

```text
src/Gui/Workbench.cpp StdWorkbench::setupToolBars()
  File: Std_New / Std_Open / Std_Save
  Edit: Std_Undo / Std_Redo / Std_Refresh
  Workbench: Std_Workbench
  View: Std_ViewFitAll / Std_ViewFitSelection / Std_ViewGroup / Std_AlignToSelection / Std_DrawStyle / Std_Measure / Std_MassProperties
  Structure: Std_Part / Std_Group / Std_LinkActions / Std_VarSet
  Help: Std_WhatsThis
```

ToolBarManager behavior:

```text
Workbench::activate() always calls setupToolBars() then ToolBarManager::setup(tb).
If setupToolBars() returns an empty ToolBarItem root, ToolBarManager sees no toolbar items for the active workbench and hides existing standard toolbars.
If setupToolBars() returns nullptr, ToolBarManager::setup() returns early and does not hide existing toolbars.
Therefore McStudio should return a new empty ToolBarItem, not nullptr.
```

Design decision:

```text
Do not hide standard toolbars manually after creation.
Do not modify FreeCAD ToolBarManager.
Do not subclass BlankWorkbench yet because McStudio still wants StdWorkbench menu/dock behavior.
Instead:
  1. Move standard commands into McStudio RibbonModel.
  2. Change McStudio Workbench::setupToolBars() to return an empty Gui::ToolBarItem root.
```

Commands to migrate into Ribbon before hiding standard toolbars:

```text
File / Project:
  Std_New
  Std_Open
  Std_Save
  Std_Import
  Std_Export

Workbench switcher:
  Std_Workbench

Edit:
  Std_Undo
  Std_Redo
  Std_Refresh

View:
  Std_ViewFitAll
  Std_ViewFitSelection
  Std_ViewGroup
  Std_AlignToSelection
  Std_DrawStyle

Measure / Evaluate:
  Std_Measure
  Std_MassProperties
  Part_ShapeInfo
  Part_CheckGeometry

Structure:
  Std_Part
  Std_Group
  Std_LinkActions
  Std_VarSet

Help / discovery:
  Std_WhatsThis
```

Implementation slice proposal:

```text
1. Add Std_Workbench to the Ribbon, likely in Output/Project or a new Home tab.
2. Add Edit and View groups to RibbonModel.
3. Confirm Measure is already reachable through McStudio_CompEvaluateInspect.
4. Change Workbench::setupToolBars() from StdWorkbench::setupToolBars() to new Gui::ToolBarItem().
5. Build and test fresh user config.
```

## Standard Toolbar to Ribbon Migration Implementation Checkpoint 2026-06-27 17:14

Implemented the first Ribbon-only standard toolbar migration slice.

Changed files:

```text
src/Mod/McStudio/Gui/RibbonModel.cpp
src/Mod/McStudio/Gui/Workbench.cpp
```

RibbonModel changes:

```text
Added a new first tab: 首页

首页 / 工作台:
  Std_Workbench

首页 / 文件:
  Std_New
  Std_Open
  Std_Save
  Std_Import
  Std_Export

首页 / 编辑:
  Std_Undo
  Std_Redo
  Std_Refresh

首页 / 视图:
  Std_ViewFitAll
  Std_ViewFitSelection
  Std_ViewGroup
  Std_AlignToSelection
  Std_DrawStyle

首页 / 结构:
  Std_Part
  Std_Group
  Std_LinkActions
  Std_VarSet

首页 / 帮助:
  Std_WhatsThis
```

Output tab cleanup:

```text
输出 / 项目 now keeps only:
  Std_Import
  Std_Export

Std_New / Std_Open / Std_Save moved to 首页 / 文件.
```

Workbench lifecycle change:

```text
McStudioGui::Workbench::setupToolBars() now returns an empty Gui::ToolBarItem root.
This prevents FreeCAD's standard File/Edit/Workbench/View/Structure/Help QToolBars from being created for McStudio.
Returning an empty root, rather than nullptr, lets ToolBarManager hide previously active standard toolbars during workbench switching.
```

FreeCAD-style boundary held:

```text
No FreeCAD core files changed.
No manual findChild/hide toolbar hack was introduced.
No ToolBarManager behavior was modified.
Standard commands still run through CommandManager::addTo() inside RibbonWidget.
McStudio still inherits Gui::StdWorkbench for menu and dock behavior.
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/Mod/McStudio/McStudioGui.so updated at 17:14, size about 2.3M
```

Manual verification required:

```text
Start with a fresh user config.
Switch to McStudio.
Confirm FreeCAD standard top QToolBars are hidden.
Confirm McStudio Ribbon still appears.
Confirm 首页 contains Workbench selector, File, Edit, View, Structure, and Help groups.
Confirm Std_Workbench appears as a selector widget or selector action inside the Ribbon host.
Check /tmp/mcstudio.log for McStudio Ribbon skipping unknown command warnings.
```

## FreeCAD UI Design Research / McStudio Ribbon Styling Direction 2026-06-27

User feedback:

```text
McStudio's rough Ribbon outline is now visible, but the UI layout is visually rough.
Analyze how FreeCAD designs UI styling and layout.
```

FreeCAD UI design findings:

```text
FreeCAD primarily uses Qt native widgets and layouts, then applies a global QSS stylesheet/theme layer.
Most workbench UI does not hard-code colors in module code.
The global stylesheet is loaded during startup from Gui/Stylesheets.
Theme tokens are replaced into QSS before qApp->setStyleSheet().
Icons use QIcon theme paths and BitmapFactory/iconFromTheme.
Toolbar icon size is controlled by BaseApp/Preferences/General/ToolbarIconSize.
```

Important source anchors:

```text
src/Gui/StartupProcess.cpp
  setStyleSheetPaths(): registers qss: search paths
  setThemePaths(): configures QIcon theme search path and theme name
  setStyleSheet(): reads BaseApp/Preferences/MainWindow/StyleSheet and applies it

src/Gui/Application.cpp
  Application::setStyleSheet(): loads defaults.qss plus selected stylesheet and calls qApp->setStyleSheet()
  Application::replaceVariablesInQss(): replaces @tokens using StyleParameters

src/Gui/Stylesheets/FreeCAD.qss
  Global QToolBar/QToolButton/QTabWidget/QTabBar/QGroupBox/QFrame styling

src/Gui/Stylesheets/defaults.qss
  Shared defaults for FreeCAD-specific widgets such as Gui--SplitButton

src/Gui/ToolBarManager.cpp
  ToolbarIconSize / StatusBarIconSize / MenuBarIconSize preferences
  setToolBarIconSize() applies icon size to QToolBar
```

Current McStudio Ribbon issue:

```text
McStudio currently builds a custom Ribbon out of generic QToolBar + QTabWidget + QFrame + QLabel widgets.
It has only limited object names:
  McStudioRibbonToolBar
  McStudioRibbonTabs
  generated command toolbar names

The Ribbon group QFrame, group label, command row, large toolbar, small toolbar, and icon-only toolbar do not yet have stable object names/classes for QSS.
Because of that, the global FreeCAD stylesheet treats most of the Ribbon as ordinary QToolBar/QTabWidget/QFrame elements.
The result works functionally but looks visually unstructured.
```

Recommended FreeCAD-style UI approach for McStudio:

```text
1. Do not hard-code many colors directly in C++.
2. Add stable object names and dynamic properties to Ribbon widgets.
3. Add a module-owned QSS file for McStudio Ribbon-specific layout polish.
4. Keep colors mostly palette/theme-neutral or use transparent backgrounds where possible.
5. Keep icon sizing and compactness controlled by preferences, not scattered constants.
6. Continue using CommandManager::addTo() so FreeCAD commands and dropdowns stay native.
```

Object names / properties to add:

```text
Outer QToolBar:
  objectName = McStudioRibbonToolBar

QTabWidget:
  objectName = McStudioRibbonTabs

Ribbon page QWidget:
  objectName = McStudioRibbonPage

Group QFrame:
  objectName = McStudioRibbonGroup
  property class = group

Group title QLabel:
  objectName = McStudioRibbonGroupLabel

Command row QWidget:
  objectName = McStudioRibbonCommandRow

Large command toolbar:
  property ribbonRole = large

Small command toolbar:
  property ribbonRole = small

Icon-only command toolbar:
  property ribbonRole = icon
```

Styling target:

```text
Outer ribbon background: visually flat, not heavy QToolBar gradient.
Tab widget pane: thin border, small radius, integrated with FreeCAD theme.
Group frame: light panel with subtle border; less boxed than QFrame::StyledPanel.
Group label: small, centered, muted color, top/bottom spacing balanced.
Large buttons: centered text under icon, more vertical breathing room.
Small buttons: text beside icon, consistent row height, less vertical jitter.
Dropdown buttons: leave FreeCAD QToolButton menu behavior intact.
```

Implementation direction:

```text
Phase 1: Structural styling hooks only
  Add objectName/property hooks to RibbonWidget.
  Remove StyledPanel dependency or replace with NoFrame plus QSS border.

Phase 2: Minimal QSS
  Add Resources/McStudio.qrc or a copied style file.
  Add McStudioRibbon.qss.
  Load/apply only to the Ribbon subtree or register as module resource.

Phase 3: Preference-backed UI tuning
  Add parameters:
    Scale
    CompactMode
    LargeIconSize
    SmallIconSize
    ShowGroupFrames
    TabPosition

Phase 4: Layout polishing
  Consider scroll area or overflow policy only after styling and grouping are complete.
```

Risk note:

```text
Applying qApp-wide McStudio QSS is risky because it may affect all FreeCAD UI.
Prefer object-name-scoped selectors such as QToolBar#McStudioRibbonToolBar and QTabWidget#McStudioRibbonTabs.
If using setStyleSheet() on the Ribbon root only, ensure menu/dropdown QToolButtons remain usable.
```

## FreeCAD UI Design Research Document Checkpoint 2026-06-27 17:28

Created a standalone research document for FreeCAD UI design and McStudio Ribbon styling direction.

Document path:

```text
docs/freecad_ui_design_research.md
```

Document coverage:

```text
FreeCAD UI startup and stylesheet loading chain
FreeCAD QSS / Theme / StyleParameters design
FreeCAD toolbar and command lifecycle
FreeCAD icon and BitmapFactory system
FreeCAD resource/qrc organization
Current McStudio Ribbon UI structure and problems
Recommended McStudio objectName/property styling hooks
Recommended McStudio module QSS structure
Recommended phased implementation plan
```

Next recommended implementation slice:

```text
1. Add stable objectName/property hooks in RibbonWidget.cpp.
2. Add Resources/McStudio.qrc.
3. Add Resources/styles/McStudioRibbon.qss.
4. Load McStudioRibbon.qss only into the McStudio Ribbon subtree.
5. Keep colors theme-neutral and selectors scoped to McStudio object names.
```

## Unified Ribbon UI Styling Implementation Checkpoint 2026-06-27 17:32

Implemented the first FreeCAD-style unified UI styling slice for McStudio Ribbon.

Changed files:

```text
src/Mod/McStudio/Gui/RibbonWidget.cpp
src/Mod/McStudio/Gui/CMakeLists.txt
src/Mod/McStudio/Gui/Resources/McStudio.qrc
src/Mod/McStudio/Gui/Resources/styles/McStudioRibbon.qss
```

Structural hooks added in RibbonWidget.cpp:

```text
QToolBar#McStudioRibbonToolBar
QTabWidget#McStudioRibbonTabs
QWidget#McStudioRibbonPage
QFrame#McStudioRibbonGroup
QWidget#McStudioRibbonCommandRow
QWidget#McStudioRibbonSecondaryColumn
QLabel#McStudioRibbonGroupLabel
QToolBar[ribbonRole="large"]
QToolBar[ribbonRole="small"]
QToolBar[ribbonRole="icon"]
```

Behavior changes:

```text
Ribbon group frames now use QFrame::NoFrame instead of QFrame::StyledPanel.
Visual borders/backgrounds are delegated to McStudioRibbon.qss.
RibbonWidget loads :/Mod/McStudio/styles/McStudioRibbon.qss and applies it to the McStudio Ribbon root only.
No qApp-wide stylesheet mutation was introduced.
```

Resource integration:

```text
Added Resources/McStudio.qrc with prefix /Mod/McStudio.
Added Resources/styles/McStudioRibbon.qss.
CMake now uses qt_add_resources(McStudioGui_QRC_SRCS Resources/McStudio.qrc).
Generated qrc_McStudio.cpp was built and linked into McStudioGui.so.
```

QSS design boundaries:

```text
Selectors are scoped to McStudio object names/properties.
Colors use Qt palette roles instead of hard-coded color constants.
Dropdown commands still use FreeCAD's native QToolButton/MenuButtonPopup behavior.
Command creation still goes through CommandManager::addTo().
```

Verification evidence:

```text
python3 -m py_compile src/Mod/McStudio/Init.py src/Mod/McStudio/InitGui.py -> PY_OK
cd build/debug && ninja McStudioGui McStudio -> passed
build/debug/src/Mod/McStudio/Gui/qrc_McStudio.cpp generated
build/debug/Mod/McStudio/McStudioGui.so updated at 17:32, size about 2.3M
```

Manual verification required:

```text
Start FreeCAD with a fresh user config.
Switch to McStudio.
Confirm the Ribbon is styled by the lighter group frames and scoped tab styling.
Check /tmp/mcstudio.log for stylesheet load warnings and unknown/skipping command warnings.
```

## GUI-First Scope Decision 2026-06-27

Decision:

```text
The cmd mode / App-layer discussion was architectural exploration only.
Current McStudio development continues as GUI-first / Ribbon-first.
```

Current active scope:

```text
McStudioGui Workbench
McStudio RibbonWidget
McStudio RibbonModel
FreeCAD Gui::Command / Gui::GroupCommand integration
FreeCAD native CommandManager command reuse
Ribbon layout and styling polish
```

Out of current scope:

```text
McStudio App-layer document object model
FreeCADCmd/headless API
CLI batch mode
cmd-mode initialization behavior changes
```

Next recommended work:

```text
Continue improving the GUI Ribbon layout and style based on the unified FreeCAD-style UI design:
  1. Fine-tune McStudioRibbon.qss.
  2. Improve group sizing/alignment.
  3. Reduce visual clutter in the Home tab.
  4. Add optional compact mode only if needed.
```
