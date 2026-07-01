// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"
#include "RibbonModel.h"

#include <QCoreApplication>

namespace McStudioGui
{

RibbonCommandDefinition largeCommand(const char* command)
{
    return {command, RibbonCommandSize::Large, RibbonCommandStyle::TextUnderIcon, nullptr};
}

RibbonCommandDefinition largeCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Large, RibbonCommandStyle::TextUnderIcon, label};
}


RibbonCommandDefinition mediumCommand(const char* command)
{
    return {command, RibbonCommandSize::Medium, RibbonCommandStyle::TextBesideIcon, nullptr};
}

RibbonCommandDefinition mediumCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Medium, RibbonCommandStyle::TextBesideIcon, label};
}

RibbonCommandDefinition smallCommand(const char* command)
{
    return {command, RibbonCommandSize::Small, RibbonCommandStyle::TextBesideIcon, nullptr};
}

RibbonCommandDefinition smallCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Small, RibbonCommandStyle::TextBesideIcon, label};
}

RibbonCommandDefinition iconCommand(const char* command)
{
    return {command, RibbonCommandSize::Small, RibbonCommandStyle::IconOnly, nullptr};
}

RibbonCommandDefinition dropDownCommand(const char* command)
{
    return {command, RibbonCommandSize::Large, RibbonCommandStyle::TextUnderIcon, nullptr};
}

RibbonCommandDefinition dropDownCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Large, RibbonCommandStyle::TextUnderIcon, label};
}


RibbonCommandDefinition mediumDropDownCommand(const char* command)
{
    return {command, RibbonCommandSize::Medium, RibbonCommandStyle::TextBesideIcon, nullptr};
}

RibbonCommandDefinition mediumDropDownCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Medium, RibbonCommandStyle::TextBesideIcon, label};
}

RibbonCommandDefinition smallDropDownCommand(const char* command)
{
    return {command, RibbonCommandSize::Small, RibbonCommandStyle::TextBesideIcon, nullptr};
}

RibbonCommandDefinition smallDropDownCommand(const char* command, const char* label)
{
    return {command, RibbonCommandSize::Small, RibbonCommandStyle::TextBesideIcon, label};
}

const std::vector<RibbonTabDefinition>& ribbonTabs()
{
    static const std::vector<RibbonTabDefinition> tabs = {
{
            QT_TR_NOOP("Features"),
            {
                {
                    // Body, sketch entry, datum/reference geometry. One large anchor
                    // (Body) + dropdowns stacked as medium rows for a compact panel.
                    QT_TR_NOOP("Body"),
                    {
                        largeCommand("PartDesign_Body", QT_TR_NOOP("Body")),
                        mediumDropDownCommand("PartDesign_CompSketches", QT_TR_NOOP("Sketch")),
                        mediumDropDownCommand("PartDesign_CompDatums", QT_TR_NOOP("Datum Geometry")),
                    },
                },
                {
                    // Additive features + primitive dropdown packed into one panel:
                    // Pad large, the rest medium rows, primitives as a dropdown.
                    QT_TR_NOOP("Additive"),
                    {
                        largeCommand("PartDesign_Pad", QT_TR_NOOP("Pad")),
                        mediumCommand("PartDesign_Revolution", QT_TR_NOOP("Revolution")),
                        mediumCommand("PartDesign_AdditiveLoft", QT_TR_NOOP("Loft")),
                        mediumCommand("PartDesign_AdditivePipe", QT_TR_NOOP("Pipe")),
                        mediumCommand("PartDesign_AdditiveHelix", QT_TR_NOOP("Helix")),
                        mediumDropDownCommand("PartDesign_CompPrimitiveAdditive", QT_TR_NOOP("Primitive")),
                    },
                },
                {
                    // Subtractive features + primitive dropdown, same compact packing.
                    QT_TR_NOOP("Subtractive"),
                    {
                        largeCommand("PartDesign_Pocket", QT_TR_NOOP("Pocket")),
                        mediumCommand("PartDesign_Hole", QT_TR_NOOP("Hole")),
                        mediumCommand("PartDesign_Groove", QT_TR_NOOP("Groove")),
                        mediumCommand("PartDesign_SubtractiveLoft", QT_TR_NOOP("Cut Loft")),
                        mediumCommand("PartDesign_SubtractivePipe", QT_TR_NOOP("Cut Pipe")),
                        mediumCommand("PartDesign_SubtractiveHelix", QT_TR_NOOP("Cut Helix")),
                        mediumDropDownCommand("PartDesign_CompPrimitiveSubtractive", QT_TR_NOOP("Primitive")),
                    },
                },
                {
                    // Dress-up and pattern collapse into their McStudio dropdown
                    // groups (click to pick the variant), keeping the panel compact.
                    // Boolean + binders stay as medium/small rows alongside.
                    QT_TR_NOOP("Modify"),
                    {
                        dropDownCommand("McStudio_CompDressUpFeatures", QT_TR_NOOP("Dress-Up")),
                        dropDownCommand("McStudio_CompTransformFeatures", QT_TR_NOOP("Pattern")),
                        mediumCommand("PartDesign_Boolean", QT_TR_NOOP("Boolean")),
                        smallCommand("PartDesign_ShapeBinder", QT_TR_NOOP("Shape Binder")),
                        smallCommand("PartDesign_SubShapeBinder", QT_TR_NOOP("Sub-Object Binder")),
                        smallCommand("PartDesign_Clone", QT_TR_NOOP("Clone")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Assembly"),
            {
                {
                    // Assembly lifecycle: create, ground, solve.
                    QT_TR_NOOP("Assembly"),
                    {
                        largeCommand("Assembly_CreateAssembly", QT_TR_NOOP("Create Assembly")),
                        mediumCommand("Assembly_ToggleGrounded", QT_TR_NOOP("Ground / Unground")),
                        mediumCommand("Assembly_SolveAssembly", QT_TR_NOOP("Solve")),
                    },
                },
                {
                    QT_TR_NOOP("Insert"),
                    {
                        dropDownCommand("McStudio_CompAssemblyInsert", QT_TR_NOOP("Insert Component")),
                    },
                },
                {
                    QT_TR_NOOP("Joints"),
                    {
                        dropDownCommand("McStudio_CompAssemblyJoints", QT_TR_NOOP("Joints")),
                        mediumDropDownCommand("McStudio_CompAssemblySelect", QT_TR_NOOP("Diagnose")),
                    },
                },
                {
                    // Assembly outputs: exploded view, BOM, drawing view.
                    QT_TR_NOOP("Output"),
                    {
                        mediumCommand("Assembly_ExplodedView", QT_TR_NOOP("Exploded View")),
                        mediumCommand("Assembly_CreateBom", QT_TR_NOOP("Bill of Materials")),
                        mediumCommand("Assembly_CreateView", QT_TR_NOOP("Create View")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Sketch"),
            {
                {
                    QT_TR_NOOP("Start"),
                    {
                        largeCommand("Sketcher_LeaveSketch", QT_TR_NOOP("Exit Sketch")),
                        dropDownCommand("PartDesign_CompSketches", QT_TR_NOOP("Sketch")),
                        mediumCommand("Sketcher_EditSketch", QT_TR_NOOP("Edit Sketch")),
                        mediumCommand("Sketcher_ViewSketch", QT_TR_NOOP("View Sketch")),
                        mediumCommand("Sketcher_ValidateSketch", QT_TR_NOOP("Validate")),
                    },
                },
                {
                    // Drawing geometry. Every family collapses to its FreeCAD Comp
                    // dropdown (click to pick the variant), so a rich toolset packs
                    // into one compact panel of medium rows + a couple of small ones.
                    QT_TR_NOOP("Draw"),
                    {
                        dropDownCommand("Sketcher_CompLine", QT_TR_NOOP("Line")),
                        mediumDropDownCommand("Sketcher_CompCreateRectangles", QT_TR_NOOP("Rectangle")),
                        mediumCommand("Sketcher_CreateCircle", QT_TR_NOOP("Circle")),
                        mediumDropDownCommand("Sketcher_CompCreateArc", QT_TR_NOOP("Arc")),
                        mediumDropDownCommand("Sketcher_CompCreateConic", QT_TR_NOOP("Conic")),
                        mediumDropDownCommand("Sketcher_CompCreateRegularPolygon", QT_TR_NOOP("Polygon")),
                        mediumDropDownCommand("Sketcher_CompSlot", QT_TR_NOOP("Slot")),
                        mediumDropDownCommand("Sketcher_CompCreateBSpline", QT_TR_NOOP("B-spline")),
                        smallCommand("Sketcher_CreatePoint", QT_TR_NOOP("Point")),
                        smallCommand("Sketcher_CreateText", QT_TR_NOOP("Text")),
                    },
                },
                {
                    // Constraints: use FreeCAD's own dimension/constraint dropdown
                    // groups instead of flattening every constraint into a button row.
                    QT_TR_NOOP("Constraints"),
                    {
                        dropDownCommand("Sketcher_CompDimensionTools", QT_TR_NOOP("Dimension")),
                        dropDownCommand("Sketcher_CompConstrainTools", QT_TR_NOOP("Relations")),
                        mediumCommand("Sketcher_ConstrainCoincidentUnified", QT_TR_NOOP("Coincident")),
                        mediumDropDownCommand("Sketcher_CompConstrainRadDia", QT_TR_NOOP("Radius / Diameter")),
                    },
                },
                {
                    // Edit + transform tools. Fillet/chamfer, external geometry,
                    // curve edit and clone/move stay as dropdown groups; trim/extend
                    // and the rest are medium/small rows.
                    QT_TR_NOOP("Tools"),
                    {
                        mediumDropDownCommand("Sketcher_CompCreateFillets", QT_TR_NOOP("Fillet / Chamfer")),
                        mediumCommand("Sketcher_Trimming", QT_TR_NOOP("Trim")),
                        mediumCommand("Sketcher_Extend", QT_TR_NOOP("Extend")),
                        smallCommand("Sketcher_Split", QT_TR_NOOP("Split")),
                        smallCommand("Sketcher_Offset", QT_TR_NOOP("Offset")),
                        mediumDropDownCommand("Sketcher_CompExternal", QT_TR_NOOP("External Geometry")),
                        mediumDropDownCommand("Sketcher_CompCurveEdition", QT_TR_NOOP("Curve Edit")),
                        mediumDropDownCommand("Sketcher_CompCopy", QT_TR_NOOP("Clone / Move")),
                        smallCommand("Sketcher_RectangularArray", QT_TR_NOOP("Array")),
                        smallCommand("Sketcher_Symmetry", QT_TR_NOOP("Symmetry")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Annotate"),
            {
                {
                    // Sketch dimensions + Draft 3D dimensions/annotations.
                    QT_TR_NOOP("Dimensions"),
                    {
                        dropDownCommand("Sketcher_CompDimensionTools", QT_TR_NOOP("Dimension")),
                        mediumDropDownCommand("Sketcher_CompHorVer", QT_TR_NOOP("Horizontal / Vertical")),
                    },
                },
                {
                    // Draft-based notes: dimension/hatch collapse into the dropdown;
                    // text + label stay tiled as high-frequency buttons (no dup).
                    QT_TR_NOOP("Notes"),
                    {
                        dropDownCommand("McStudio_CompAnnotate", QT_TR_NOOP("Dimension / Hatch")),
                        mediumCommand("Draft_Text", QT_TR_NOOP("Text")),
                        mediumCommand("Draft_Label", QT_TR_NOOP("Label")),
                    },
                },
                {
                    QT_TR_NOOP("Display Notes"),
                    {
                        smallCommand("Std_ViewScreenShot", QT_TR_NOOP("Screenshot")),
                        smallCommand("Std_TextDocument", QT_TR_NOOP("Text Document")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Evaluate"),
            {
                {
                    // High-frequency measurement anchored large + mass props,
                    // with the full inspect toolset in the dropdown.
                    QT_TR_NOOP("Measure"),
                    {
                        largeCommand("Std_Measure", QT_TR_NOOP("Measure")),
                        mediumCommand("Std_MassProperties", QT_TR_NOOP("Mass Properties")),
                        mediumDropDownCommand("McStudio_CompEvaluateInspect", QT_TR_NOOP("Inspect")),
                    },
                },
                {
                    // Geometry inspection rows.
                    QT_TR_NOOP("Inspect"),
                    {
                        mediumCommand("Part_CheckGeometry", QT_TR_NOOP("Check Geometry")),
                        mediumCommand("Part_ShapeInfo", QT_TR_NOOP("Shape Info")),
                        smallCommand("Std_DependencyGraph", QT_TR_NOOP("Dependency Graph")),
                    },
                },
                {
                    QT_TR_NOOP("Section / Cut"),
                    {
                        dropDownCommand("McStudio_CompSectionTools", QT_TR_NOOP("Section")),
                    },
                },
                {
                    QT_TR_NOOP("Display Check"),
                    {
                        smallCommand("Std_ViewHome", QT_TR_NOOP("Home View")),
                        smallCommand("Std_DrawStyle", QT_TR_NOOP("Draw Style")),
                        iconCommand("Std_ToggleTransparency"),
                    },
                },
            },
        },
                {
            QT_TR_NOOP("Surface"),
            {
                {
                    // Surface creation: full comp dropdown + high-frequency
                    // extrude/revolve tiled as medium rows (SolidWorks density).
                    QT_TR_NOOP("Create"),
                    {
                        dropDownCommand("McStudio_CompSurfaceCreate", QT_TR_NOOP("Create Surface")),
                        mediumCommand("Part_Extrude", QT_TR_NOOP("Extrude")),
                        mediumCommand("Part_Revolve", QT_TR_NOOP("Revolve")),
                        mediumCommand("Part_RuledSurface", QT_TR_NOOP("Ruled Surface")),
                    },
                },
                {
                    // Surface editing: comp dropdown + common offset/thicken rows.
                    QT_TR_NOOP("Edit"),
                    {
                        dropDownCommand("McStudio_CompSurfaceEdit", QT_TR_NOOP("Edit Surface")),
                        mediumCommand("Part_Offset", QT_TR_NOOP("Offset")),
                        mediumCommand("Part_Thickness", QT_TR_NOOP("Thickness")),
                    },
                },
            },
        },
{
            QT_TR_NOOP("View"),
            {
                {
                    QT_TR_NOOP("Navigation"),
                    {
                        largeCommand("Std_ViewFitAll", QT_TR_NOOP("Fit All")),
                        largeCommand("Std_ViewFitSelection", QT_TR_NOOP("Fit Selection")),
                        dropDownCommand("McStudio_CompViewNavigate", QT_TR_NOOP("Navigate")),
                    },
                },
                {
                    QT_TR_NOOP("Orientation"),
                    {
                        dropDownCommand("McStudio_CompStandardViews", QT_TR_NOOP("Standard Views")),
                        iconCommand("Std_ViewHome"),
                    },
                },
                {
                    QT_TR_NOOP("Display"),
                    {
                        dropDownCommand("McStudio_CompViewDisplay", QT_TR_NOOP("Display")),
                        smallCommand("Std_DrawStyle", QT_TR_NOOP("Draw Style")),
                        iconCommand("Std_ToggleTransparency"),
                    },
                },
                {
                    QT_TR_NOOP("Visibility"),
                    {
                        dropDownCommand("McStudio_CompViewVisibility", QT_TR_NOOP("Visibility")),
                    },
                },
                {
                    QT_TR_NOOP("Panels / Windows"),
                    {
                        dropDownCommand("McStudio_CompPanelWindowTools", QT_TR_NOOP("Panels / Windows")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Output"),
            {
                {
                    QT_TR_NOOP("Project"),
                    {
                        mediumCommand("Std_Import", QT_TR_NOOP("Import")),
                        mediumCommand("Std_Export", QT_TR_NOOP("Export")),
                    },
                },
                {
                    QT_TR_NOOP("Model Processing"),
                    {
                        smallCommand("Part_Import", QT_TR_NOOP("Import Shape")),
                        smallCommand("Part_Export", QT_TR_NOOP("Export Shape")),
                        smallCommand("Part_SimpleCopy", QT_TR_NOOP("Simple Copy")),
                        smallCommand("Part_RefineShape", QT_TR_NOOP("Refine Shape")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("MBD"),
            {
                {
                    QT_TR_NOOP("Model Definition"),
                    {
                        smallCommand("Std_ViewScreenShot", QT_TR_NOOP("Capture")),
                        smallCommand("Std_ViewHome", QT_TR_NOOP("Home View")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Add-ins"),
            {
                {
                    QT_TR_NOOP("Extensions"),
                    {
                        largeCommand("Std_AddonMgr", QT_TR_NOOP("Add-on Manager")),
                        smallCommand("Std_DlgCustomize", QT_TR_NOOP("Customize")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Advanced"),
            {
                {
                    QT_TR_NOOP("Settings"),
                    {
                        largeCommand("Std_DlgPreferences", QT_TR_NOOP("Preferences")),
                        smallCommand("Std_DlgCustomize", QT_TR_NOOP("Customize")),
                    },
                },
                {
                    QT_TR_NOOP("Structure"),
                    {
                        smallCommand("Std_Part", QT_TR_NOOP("Part")),
                        smallCommand("Std_Group", QT_TR_NOOP("Group")),
                        smallCommand("Std_LinkActions", QT_TR_NOOP("Link")),
                        smallCommand("Std_VarSet", QT_TR_NOOP("Variable Set")),
                    },
                },
                {
                    QT_TR_NOOP("Extensions / Diagnostics"),
                    {
                        dropDownCommand("McStudio_CompAdvancedTools", QT_TR_NOOP("Advanced Tools")),
                    },
                },
                {
                    QT_TR_NOOP("Automation"),
                    {
                        dropDownCommand("McStudio_CompMacroTools", QT_TR_NOOP("Macros / Automation")),
                    },
                },
                {
                    QT_TR_NOOP("Help / Development"),
                    {
                        dropDownCommand("McStudio_CompHelp", QT_TR_NOOP("Help")),
                    },
                },
            },
        },
    };

    return tabs;
}

const std::vector<RibbonContextDefinition>& ribbonContexts()
{
    static const std::vector<RibbonContextDefinition> contexts = {
        {
            "SketchContext",
            QT_TR_NOOP("Sketch Tools"),
            "#d99a2b",
            {
                {
                    QT_TR_NOOP("Sketch Tools"),
                    {
                        {
                            // Compact context toolbar mirroring the main Sketch tab:
                            // exit/validate + dropdown-driven draw/constraint/edit groups.
                            QT_TR_NOOP("Start"),
                            {
                                largeCommand("Sketcher_LeaveSketch", QT_TR_NOOP("Exit Sketch")),
                                mediumCommand("Sketcher_ViewSketch", QT_TR_NOOP("View Sketch")),
                                mediumCommand("Sketcher_ValidateSketch", QT_TR_NOOP("Validate")),
                            },
                        },
                        {
                            QT_TR_NOOP("Draw"),
                            {
                                dropDownCommand("Sketcher_CompLine", QT_TR_NOOP("Line")),
                                mediumDropDownCommand("Sketcher_CompCreateRectangles", QT_TR_NOOP("Rectangle")),
                                mediumCommand("Sketcher_CreateCircle", QT_TR_NOOP("Circle")),
                                mediumDropDownCommand("Sketcher_CompCreateArc", QT_TR_NOOP("Arc")),
                                mediumDropDownCommand("Sketcher_CompCreateConic", QT_TR_NOOP("Conic")),
                                mediumDropDownCommand("Sketcher_CompCreateRegularPolygon", QT_TR_NOOP("Polygon")),
                                mediumDropDownCommand("Sketcher_CompSlot", QT_TR_NOOP("Slot")),
                                mediumDropDownCommand("Sketcher_CompCreateBSpline", QT_TR_NOOP("B-spline")),
                                smallCommand("Sketcher_CreatePoint", QT_TR_NOOP("Point")),
                                smallCommand("Sketcher_CreateText", QT_TR_NOOP("Text")),
                            },
                        },
                        {
                            QT_TR_NOOP("Constraints"),
                            {
                                dropDownCommand("Sketcher_CompDimensionTools", QT_TR_NOOP("Dimension")),
                                dropDownCommand("Sketcher_CompConstrainTools", QT_TR_NOOP("Relations")),
                                mediumCommand("Sketcher_ConstrainCoincidentUnified", QT_TR_NOOP("Coincident")),
                                mediumDropDownCommand("Sketcher_CompConstrainRadDia", QT_TR_NOOP("Radius / Diameter")),
                            },
                        },
                        {
                            QT_TR_NOOP("Tools"),
                            {
                                mediumDropDownCommand("Sketcher_CompCreateFillets", QT_TR_NOOP("Fillet / Chamfer")),
                                mediumCommand("Sketcher_Trimming", QT_TR_NOOP("Trim")),
                                mediumCommand("Sketcher_Extend", QT_TR_NOOP("Extend")),
                                smallCommand("Sketcher_Split", QT_TR_NOOP("Split")),
                                mediumDropDownCommand("Sketcher_CompExternal", QT_TR_NOOP("External Geometry")),
                                mediumDropDownCommand("Sketcher_CompCurveEdition", QT_TR_NOOP("Curve Edit")),
                                mediumDropDownCommand("Sketcher_CompCopy", QT_TR_NOOP("Clone / Move")),
                            },
                        },
                    },
                },
            },
        },
        {
            "SelectionContext",
            QT_TR_NOOP("Selection Tools"),
            "#4c8ed9",
            {
                {
                    QT_TR_NOOP("Selection Tools"),
                    {
                        {
                            QT_TR_NOOP("Edit Selection"),
                            {
                                largeCommand("Std_Delete", QT_TR_NOOP("Delete")),
                                mediumCommand("Std_ToggleVisibility", QT_TR_NOOP("Show / Hide")),
                                mediumCommand("Std_SetAppearance", QT_TR_NOOP("Appearance")),
                            },
                        },
                        {
                            QT_TR_NOOP("View Selection"),
                            {
                                largeCommand("Std_ViewFitSelection", QT_TR_NOOP("Fit Selection")),
                                mediumCommand("Std_ToggleTransparency", QT_TR_NOOP("Transparency")),
                                mediumDropDownCommand("McStudio_CompViewDisplay", QT_TR_NOOP("Display")),
                            },
                        },
                    },
                },
            },
        },
        {
            "ManufacturingContext",
            QT_TR_NOOP("Manufacturing Context"),
            "#57a86b",
            {
                {
                    QT_TR_NOOP("Manufacturing Tools"),
                    {
                        {
                            QT_TR_NOOP("Setup"),
                            {
                                largeCommand("McStudio_CreateWorkpiece", QT_TR_NOOP("Workpiece")),
                                largeCommand("McStudio_CreateStock", QT_TR_NOOP("Stock")),
                                mediumCommand("McStudio_SetWorkCoordinateSystem", QT_TR_NOOP("Work Coordinate")),
                            },
                        },
                        {
                            QT_TR_NOOP("Output"),
                            {
                                largeCommand("McStudio_GenerateProcessReport", QT_TR_NOOP("Process Report")),
                                mediumCommand("McStudio_ExportSetupSheet", QT_TR_NOOP("Setup Sheet")),
                            },
                        },
                    },
                },
            },
        },
    };

    return contexts;
}

}  // namespace McStudioGui
