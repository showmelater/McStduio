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
                    // Body creation, sketch entry, and datum/reference geometry.
                    QT_TR_NOOP("Body & Reference"),
                    {
                        largeCommand("PartDesign_Body", QT_TR_NOOP("Body")),
                        mediumDropDownCommand("PartDesign_CompSketches", QT_TR_NOOP("Sketch")),
                        mediumDropDownCommand("PartDesign_CompDatums", QT_TR_NOOP("Datum Geometry")),
                    },
                },
                {
                    // Additive features (material-adding). Mirrors FreeCAD's
                    // "Create an additive feature" menu group.
                    QT_TR_NOOP("Additive"),
                    {
                        largeCommand("PartDesign_Pad", QT_TR_NOOP("Pad")),
                        largeCommand("PartDesign_Revolution", QT_TR_NOOP("Revolution")),
                        mediumCommand("PartDesign_AdditiveLoft", QT_TR_NOOP("Additive Loft")),
                        mediumCommand("PartDesign_AdditivePipe", QT_TR_NOOP("Additive Pipe")),
                        mediumCommand("PartDesign_AdditiveHelix", QT_TR_NOOP("Additive Helix")),
                    },
                },
                {
                    // FreeCAD built-in dropdown aggregating 8 additive primitives.
                    QT_TR_NOOP("Additive Primitives"),
                    {
                        dropDownCommand("PartDesign_CompPrimitiveAdditive", QT_TR_NOOP("Primitive")),
                    },
                },
                {
                    // Subtractive features (material-removing).
                    QT_TR_NOOP("Subtractive"),
                    {
                        largeCommand("PartDesign_Pocket", QT_TR_NOOP("Pocket")),
                        largeCommand("PartDesign_Hole", QT_TR_NOOP("Hole")),
                        mediumCommand("PartDesign_Groove", QT_TR_NOOP("Groove")),
                        mediumCommand("PartDesign_SubtractiveLoft", QT_TR_NOOP("Subtractive Loft")),
                        mediumCommand("PartDesign_SubtractivePipe", QT_TR_NOOP("Subtractive Pipe")),
                        mediumCommand("PartDesign_SubtractiveHelix", QT_TR_NOOP("Subtractive Helix")),
                    },
                },
                {
                    // FreeCAD built-in dropdown aggregating 8 subtractive primitives.
                    QT_TR_NOOP("Subtractive Primitives"),
                    {
                        dropDownCommand("PartDesign_CompPrimitiveSubtractive", QT_TR_NOOP("Primitive")),
                    },
                },
                {
                    // Dress-up features as flat, independent SolidWorks-style entries
                    // (replacing the previous McStudio_CompDressUpFeatures dropdown).
                    QT_TR_NOOP("Dress-Up"),
                    {
                        largeCommand("PartDesign_Fillet", QT_TR_NOOP("Fillet")),
                        largeCommand("PartDesign_Chamfer", QT_TR_NOOP("Chamfer")),
                        mediumCommand("PartDesign_Draft", QT_TR_NOOP("Draft")),
                        mediumCommand("PartDesign_Thickness", QT_TR_NOOP("Thickness")),
                    },
                },
                {
                    // Transformation/pattern features as flat entries
                    // (replacing the previous McStudio_CompTransformFeatures dropdown).
                    QT_TR_NOOP("Pattern"),
                    {
                        largeCommand("PartDesign_Mirrored", QT_TR_NOOP("Mirror")),
                        mediumCommand("PartDesign_LinearPattern", QT_TR_NOOP("Linear Pattern")),
                        mediumCommand("PartDesign_PolarPattern", QT_TR_NOOP("Polar Pattern")),
                        mediumCommand("PartDesign_MultiTransform", QT_TR_NOOP("Multi-Transform")),
                        mediumCommand("PartDesign_Scaled", QT_TR_NOOP("Scaled")),
                    },
                },
                {
                    // Boolean and structural helpers (binders, clone, migrate).
                    QT_TR_NOOP("Boolean & Structure"),
                    {
                        largeCommand("PartDesign_Boolean", QT_TR_NOOP("Boolean")),
                        mediumCommand("PartDesign_ShapeBinder", QT_TR_NOOP("Shape Binder")),
                        mediumCommand("PartDesign_SubShapeBinder", QT_TR_NOOP("Sub-Object Binder")),
                        smallCommand("PartDesign_Clone", QT_TR_NOOP("Clone")),
                        smallCommand("PartDesign_Migrate", QT_TR_NOOP("Migrate")),
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
                        mediumCommand("Sketcher_ValidateSketch", QT_TR_NOOP("Validate")),
                    },
                },
                {
                    QT_TR_NOOP("Draw"),
                    {
                        dropDownCommand("Sketcher_CompLine", QT_TR_NOOP("Line")),
                        dropDownCommand("Sketcher_CompCreateRectangles", QT_TR_NOOP("Rectangle")),
                        mediumDropDownCommand("Sketcher_CompCreateArc", QT_TR_NOOP("Arc")),
                        mediumDropDownCommand("Sketcher_CompCreateConic", QT_TR_NOOP("Conic")),
                    },
                },
                {
                    QT_TR_NOOP("Modify"),
                    {
                        mediumDropDownCommand("Sketcher_CompCreateFillets", QT_TR_NOOP("Fillet")),
                        mediumDropDownCommand("Sketcher_CompCurveEdition", QT_TR_NOOP("Curve Edit")),
                    },
                },
                {
                    QT_TR_NOOP("Dimensions"),
                    {
                        dropDownCommand("Sketcher_CompDimensionTools", QT_TR_NOOP("Dimension")),
                        mediumDropDownCommand("Sketcher_CompHorVer", QT_TR_NOOP("Horizontal / Vertical")),
                    },
                },
                {
                    QT_TR_NOOP("Relations"),
                    {
                        smallCommand("Sketcher_ConstrainCoincidentUnified", QT_TR_NOOP("Coincident")),
                        smallCommand("Sketcher_ConstrainParallel", QT_TR_NOOP("Parallel")),
                        smallCommand("Sketcher_ConstrainTangent", QT_TR_NOOP("Tangent")),
                        smallCommand("Sketcher_ConstrainEqual", QT_TR_NOOP("Equal")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Annotate"),
            {
                {
                    QT_TR_NOOP("Dimensions"),
                    {
                        dropDownCommand("Sketcher_CompDimensionTools", QT_TR_NOOP("Dimension")),
                        mediumDropDownCommand("Sketcher_CompHorVer", QT_TR_NOOP("Horizontal / Vertical")),
                    },
                },
                {
                    QT_TR_NOOP("Display Notes"),
                    {
                        smallCommand("Std_ViewScreenShot", QT_TR_NOOP("Screenshot")),
                        smallCommand("Std_TextDocument", QT_TR_NOOP("Text")),
                    },
                },
            },
        },
        {
            QT_TR_NOOP("Evaluate"),
            {
                {
                    QT_TR_NOOP("Measure / Inspect"),
                    {
                        dropDownCommand("McStudio_CompEvaluateInspect", QT_TR_NOOP("Measure")),
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
                    QT_TR_NOOP("Create Surface"),
                    {
                        dropDownCommand("McStudio_CompSurfaceCreate", QT_TR_NOOP("Create Surface")),
                    },
                },
                {
                    QT_TR_NOOP("Edit Surface"),
                    {
                        dropDownCommand("McStudio_CompSurfaceEdit", QT_TR_NOOP("Edit Surface")),
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
                            QT_TR_NOOP("Start"),
                            {
                                largeCommand("Sketcher_LeaveSketch", QT_TR_NOOP("Exit Sketch")),
                                mediumCommand("Sketcher_ValidateSketch", QT_TR_NOOP("Validate")),
                            },
                        },
                        {
                            QT_TR_NOOP("Draw"),
                            {
                                dropDownCommand("Sketcher_CompLine", QT_TR_NOOP("Line")),
                                dropDownCommand("Sketcher_CompCreateRectangles", QT_TR_NOOP("Rectangle")),
                                mediumDropDownCommand("Sketcher_CompCreateArc", QT_TR_NOOP("Arc")),
                                mediumDropDownCommand("Sketcher_CompCreateConic", QT_TR_NOOP("Conic")),
                            },
                        },
                        {
                            QT_TR_NOOP("Modify"),
                            {
                                mediumDropDownCommand("Sketcher_CompCreateFillets", QT_TR_NOOP("Fillet")),
                                mediumDropDownCommand("Sketcher_CompCurveEdition", QT_TR_NOOP("Curve Edit")),
                            },
                        },
                        {
                            QT_TR_NOOP("Relations"),
                            {
                                mediumCommand("Sketcher_ConstrainCoincidentUnified", QT_TR_NOOP("Coincident")),
                                mediumCommand("Sketcher_ConstrainParallel", QT_TR_NOOP("Parallel")),
                                mediumCommand("Sketcher_ConstrainTangent", QT_TR_NOOP("Tangent")),
                                mediumCommand("Sketcher_ConstrainEqual", QT_TR_NOOP("Equal")),
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
