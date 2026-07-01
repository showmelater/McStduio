// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"

#include <initializer_list>

#include <App/Application.h>
#include <Base/Console.h>
#include <Gui/Application.h>
#include <Gui/Command.h>

namespace
{
void ensureDocument()
{
    if (!App::GetApplication().getActiveDocument()) {
        Gui::Command::doCommand(Gui::Command::Doc, "App.newDocument('McStudioDocument')");
    }
}

void runFreeCADCommand(const char* commandName, bool createDocument = true)
{
    if (createDocument) {
        ensureDocument();
    }

    if (Gui::Application::Instance) {
        Gui::Application::Instance->commandManager().runCommandByName(commandName);
    }
}

void printPlaceholder(const char* commandName)
{
    Base::Console().message("McStudio: %s is a placeholder in the first C++ workbench milestone.\n",
                            commandName);
}

class CmdMcStudioCommandGroup: public Gui::GroupCommand
{
public:
    CmdMcStudioCommandGroup(const char* commandName,
                            const char* menuText,
                            const char* toolTipText,
                            const char* pixmap,
                            int type,
                            std::initializer_list<const char*> commands)
        : GroupCommand(commandName)
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = menuText;
        sToolTipText = toolTipText;
        sStatusTip = sToolTipText;
        sWhatsThis = commandName;
        sPixmap = pixmap;
        eType = type;

        setCheckable(false);
        setRememberLast(true);

        for (const char* command : commands) {
            addCommand(command);
        }
    }

    const char* className() const override
    {
        return "CmdMcStudioCommandGroup";
    }

    bool isActive() override
    {
        return true;
    }
};
}  // namespace

DEF_STD_CMD_A(CmdMcStudioCreateReferencePlane)

CmdMcStudioCreateReferencePlane::CmdMcStudioCreateReferencePlane()
    : Command("McStudio_CreateReferencePlane")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Create McStudio Reference Plane");
    sToolTipText = QT_TR_NOOP("Create a reference plane using the Part Design datum plane command");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_CreateReferencePlane";
    sPixmap = "PartDesign_Plane";
}

void CmdMcStudioCreateReferencePlane::activated(int)
{
    runFreeCADCommand("PartDesign_Plane");
}

bool CmdMcStudioCreateReferencePlane::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioCreateWorkpiece)

CmdMcStudioCreateWorkpiece::CmdMcStudioCreateWorkpiece()
    : Command("McStudio_CreateWorkpiece")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Create Workpiece");
    sToolTipText = QT_TR_NOOP("Create a McStudio workpiece placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_CreateWorkpiece";
    sPixmap = "PartDesign_Body";
}

void CmdMcStudioCreateWorkpiece::activated(int)
{
    ensureDocument();
    printPlaceholder("McStudio_CreateWorkpiece");
}

bool CmdMcStudioCreateWorkpiece::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioCreateStock)

CmdMcStudioCreateStock::CmdMcStudioCreateStock()
    : Command("McStudio_CreateStock")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Create Stock");
    sToolTipText = QT_TR_NOOP("Create a McStudio stock placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_CreateStock";
    sPixmap = "PartDesign_Body";
}

void CmdMcStudioCreateStock::activated(int)
{
    ensureDocument();
    printPlaceholder("McStudio_CreateStock");
}

bool CmdMcStudioCreateStock::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioCreateFixture)

CmdMcStudioCreateFixture::CmdMcStudioCreateFixture()
    : Command("McStudio_CreateFixture")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Create Fixture");
    sToolTipText = QT_TR_NOOP("Create a McStudio fixture placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_CreateFixture";
    sPixmap = "PartDesign_Body";
}

void CmdMcStudioCreateFixture::activated(int)
{
    ensureDocument();
    printPlaceholder("McStudio_CreateFixture");
}

bool CmdMcStudioCreateFixture::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioSetFixtureLocation)

CmdMcStudioSetFixtureLocation::CmdMcStudioSetFixtureLocation()
    : Command("McStudio_SetFixtureLocation")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Set Fixture Location");
    sToolTipText = QT_TR_NOOP("Set fixture location placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_SetFixtureLocation";
    sPixmap = "PartDesign_CoordinateSystem";
}

void CmdMcStudioSetFixtureLocation::activated(int)
{
    ensureDocument();
    printPlaceholder("McStudio_SetFixtureLocation");
}

bool CmdMcStudioSetFixtureLocation::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioSetWorkCoordinateSystem)

CmdMcStudioSetWorkCoordinateSystem::CmdMcStudioSetWorkCoordinateSystem()
    : Command("McStudio_SetWorkCoordinateSystem")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Set Work Coordinate System");
    sToolTipText = QT_TR_NOOP("Create a coordinate system using the Part Design datum coordinate system command");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_SetWorkCoordinateSystem";
    sPixmap = "PartDesign_CoordinateSystem";
}

void CmdMcStudioSetWorkCoordinateSystem::activated(int)
{
    runFreeCADCommand("PartDesign_CoordinateSystem");
}

bool CmdMcStudioSetWorkCoordinateSystem::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioCreateSetupDatum)

CmdMcStudioCreateSetupDatum::CmdMcStudioCreateSetupDatum()
    : Command("McStudio_CreateSetupDatum")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Create Setup Datum");
    sToolTipText = QT_TR_NOOP("Create a setup datum using the Part Design datum plane command");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_CreateSetupDatum";
    sPixmap = "PartDesign_Plane";
}

void CmdMcStudioCreateSetupDatum::activated(int)
{
    runFreeCADCommand("PartDesign_Plane");
}

bool CmdMcStudioCreateSetupDatum::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioGenerateProcessReport)

CmdMcStudioGenerateProcessReport::CmdMcStudioGenerateProcessReport()
    : Command("McStudio_GenerateProcessReport")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Generate Process Report");
    sToolTipText = QT_TR_NOOP("Generate a McStudio process report placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_GenerateProcessReport";
    sPixmap = "Std_Export";
}

void CmdMcStudioGenerateProcessReport::activated(int)
{
    printPlaceholder("McStudio_GenerateProcessReport");
}

bool CmdMcStudioGenerateProcessReport::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioExportSetupSheet)

CmdMcStudioExportSetupSheet::CmdMcStudioExportSetupSheet()
    : Command("McStudio_ExportSetupSheet")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Export Setup Sheet");
    sToolTipText = QT_TR_NOOP("Export a McStudio setup sheet placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_ExportSetupSheet";
    sPixmap = "Std_Export";
}

void CmdMcStudioExportSetupSheet::activated(int)
{
    printPlaceholder("McStudio_ExportSetupSheet");
}

bool CmdMcStudioExportSetupSheet::isActive()
{
    return true;
}

DEF_STD_CMD_A(CmdMcStudioGenerateBOM)

CmdMcStudioGenerateBOM::CmdMcStudioGenerateBOM()
    : Command("McStudio_GenerateBOM")
{
    sAppModule = "McStudio";
    sGroup = QT_TR_NOOP("McStudio");
    sMenuText = QT_TR_NOOP("Generate BOM");
    sToolTipText = QT_TR_NOOP("Generate a McStudio BOM placeholder");
    sStatusTip = sToolTipText;
    sWhatsThis = "McStudio_GenerateBOM";
    sPixmap = "Std_Export";
}

void CmdMcStudioGenerateBOM::activated(int)
{
    printPlaceholder("McStudio_GenerateBOM");
}

bool CmdMcStudioGenerateBOM::isActive()
{
    return true;
}


class CmdMcStudioCompDressUpFeatures: public Gui::GroupCommand
{
public:
    CmdMcStudioCompDressUpFeatures()
        : GroupCommand("McStudio_CompDressUpFeatures")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Dress-Up Features");
        sToolTipText = QT_TR_NOOP("Create fillet, chamfer, draft, or thickness features");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompDressUpFeatures";
        sPixmap = "PartDesign_Fillet";
        eType = ForEdit;

        setCheckable(false);
        setRememberLast(true);

        addCommand("PartDesign_Fillet");
        addCommand("PartDesign_Chamfer");
        addCommand("PartDesign_Draft");
        addCommand("PartDesign_Thickness");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompDressUpFeatures";
    }

    bool isActive() override
    {
        return true;
    }
};


class CmdMcStudioCompTransformFeatures: public Gui::GroupCommand
{
public:
    CmdMcStudioCompTransformFeatures()
        : GroupCommand("McStudio_CompTransformFeatures")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Pattern and Mirror");
        sToolTipText = QT_TR_NOOP("Create mirror, linear pattern, polar pattern, or multi-transform features");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompTransformFeatures";
        sPixmap = "PartDesign_Mirrored";
        eType = ForEdit;

        setCheckable(false);
        setRememberLast(true);

        addCommand("PartDesign_Mirrored");
        addCommand("PartDesign_LinearPattern");
        addCommand("PartDesign_PolarPattern");
        addCommand("PartDesign_MultiTransform");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompTransformFeatures";
    }

    bool isActive() override
    {
        return true;
    }
};

class CmdMcStudioCompSurfaceCreate: public Gui::GroupCommand
{
public:
    CmdMcStudioCompSurfaceCreate()
        : GroupCommand("McStudio_CompSurfaceCreate")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Create Surface");
        sToolTipText = QT_TR_NOOP("Create filling, fill surface, sections, loft, sweep, or ruled surface features");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompSurfaceCreate";
        sPixmap = "Surface_Filling";
        eType = ForEdit;

        setCheckable(false);
        setRememberLast(true);

        addCommand("Surface_Filling");
        addCommand("Surface_GeomFillSurface");
        addCommand("Surface_Sections");
        addCommand("Part_Loft");
        addCommand("Part_Sweep");
        addCommand("Part_RuledSurface");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompSurfaceCreate";
    }

    bool isActive() override
    {
        return true;
    }
};

class CmdMcStudioCompSurfaceEdit: public Gui::GroupCommand
{
public:
    CmdMcStudioCompSurfaceEdit()
        : GroupCommand("McStudio_CompSurfaceEdit")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Edit Surface");
        sToolTipText = QT_TR_NOOP("Cut, extend, blend, offset, thicken, or project surfaces");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompSurfaceEdit";
        sPixmap = "Surface_Cut";
        eType = ForEdit;

        setCheckable(false);
        setRememberLast(true);

        addCommand("Surface_Cut");
        addCommand("Surface_ExtendFace");
        addCommand("Surface_BlendCurve");
        addCommand("Part_Offset");
        addCommand("Part_Thickness");
        addCommand("Part_ProjectionOnSurface");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompSurfaceEdit";
    }

    bool isActive() override
    {
        return true;
    }
};

class CmdMcStudioCompEvaluateInspect: public Gui::GroupCommand
{
public:
    CmdMcStudioCompEvaluateInspect()
        : GroupCommand("McStudio_CompEvaluateInspect")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Measure and Inspect");
        sToolTipText = QT_TR_NOOP("Measure model properties and inspect geometry");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompEvaluateInspect";
        sPixmap = "Std_Measure";
        eType = AlterDoc;

        setCheckable(false);
        setRememberLast(true);

        addCommand("Std_Measure");
        addCommand("Std_MassProperties");
        addCommand("Part_ShapeInfo");
        addCommand("Part_CheckGeometry");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompEvaluateInspect";
    }

    bool isActive() override
    {
        return true;
    }
};

class CmdMcStudioCompSectionTools: public Gui::GroupCommand
{
public:
    CmdMcStudioCompSectionTools()
        : GroupCommand("McStudio_CompSectionTools")
    {
        sAppModule = "McStudio";
        sGroup = QT_TR_NOOP("McStudio");
        sMenuText = QT_TR_NOOP("Section Tools");
        sToolTipText = QT_TR_NOOP("Create sections, cross sections, section cuts, or clipping planes");
        sStatusTip = sToolTipText;
        sWhatsThis = "McStudio_CompSectionTools";
        sPixmap = "Part_Section";
        eType = AlterDoc;

        setCheckable(false);
        setRememberLast(true);

        addCommand("Part_Section");
        addCommand("Part_CrossSections");
        addCommand("Part_SectionCut");
        addCommand("Std_ToggleClipPlane");
    }

    const char* className() const override
    {
        return "CmdMcStudioCompSectionTools";
    }

    bool isActive() override
    {
        return true;
    }
};

void CreateMcStudioCommands()
{
    Gui::CommandManager& rcCmdMgr = Gui::Application::Instance->commandManager();
    rcCmdMgr.addCommand(new CmdMcStudioCreateReferencePlane());
    rcCmdMgr.addCommand(new CmdMcStudioCreateWorkpiece());
    rcCmdMgr.addCommand(new CmdMcStudioCreateStock());
    rcCmdMgr.addCommand(new CmdMcStudioCreateFixture());
    rcCmdMgr.addCommand(new CmdMcStudioSetFixtureLocation());
    rcCmdMgr.addCommand(new CmdMcStudioSetWorkCoordinateSystem());
    rcCmdMgr.addCommand(new CmdMcStudioCreateSetupDatum());
    rcCmdMgr.addCommand(new CmdMcStudioGenerateProcessReport());
    rcCmdMgr.addCommand(new CmdMcStudioExportSetupSheet());
    rcCmdMgr.addCommand(new CmdMcStudioGenerateBOM());
    rcCmdMgr.addCommand(new CmdMcStudioCompDressUpFeatures());
    rcCmdMgr.addCommand(new CmdMcStudioCompTransformFeatures());
    rcCmdMgr.addCommand(new CmdMcStudioCompSurfaceCreate());
    rcCmdMgr.addCommand(new CmdMcStudioCompSurfaceEdit());
    rcCmdMgr.addCommand(new CmdMcStudioCompEvaluateInspect());
    rcCmdMgr.addCommand(new CmdMcStudioCompSectionTools());
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompApplicationMenu",
                                                     QT_TR_NOOP("File"),
                                                     QT_TR_NOOP("Open file, save, import, export, print, preferences, and quit commands"),
                                                     "Std_Open",
                                                     0,
                                                     {"Std_New",
                                                      "Std_Open",
                                                      "Std_RecentFiles",
                                                      "Std_CloseActiveWindow",
                                                      "Std_CloseAllWindows",
                                                      "Std_Save",
                                                      "Std_SaveAs",
                                                      "Std_SaveCopy",
                                                      "Std_SaveAll",
                                                      "Std_Revert",
                                                      "Std_Import",
                                                      "Std_Export",
                                                      "Std_MergeProjects",
                                                      "Std_ProjectInfo",
                                                      "Std_Print",
                                                      "Std_PrintPreview",
                                                      "Std_PrintPdf",
                                                      "Std_DlgPreferences",
                                                      "Std_Quit"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompEditTools",
                                                     QT_TR_NOOP("Edit Tools"),
                                                     QT_TR_NOOP("Cut, copy, paste, select, delete, placement, transform, alignment, and properties"),
                                                     "Std_Edit",
                                                     0,
                                                     {"Std_Cut",
                                                      "Std_Copy",
                                                      "Std_Paste",
                                                      "Std_DuplicateSelection",
                                                      "Std_BoxSelection",
                                                      "Std_BoxElementSelection",
                                                      "Std_SelectAll",
                                                      "Std_Delete",
                                                      "Std_Placement",
                                                      "Std_TransformManip",
                                                      "Std_Alignment",
                                                      "Std_Edit",
                                                      "Std_Properties"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompViewNavigate",
                                                     QT_TR_NOOP("View Navigation"),
                                                     QT_TR_NOOP("Fit, home, align, rotate, and working view commands"),
                                                     "Std_ViewFitAll",
                                                     0,
                                                     {"Std_ViewFitAll",
                                                      "Std_ViewFitSelection",
                                                      "Std_AlignToSelection",
                                                      "Std_ViewHome",
                                                      "Std_ViewRotateLeft",
                                                      "Std_ViewRotateRight",
                                                      "Std_StoreWorkingView",
                                                      "Std_RecallWorkingView"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompStandardViews",
                                                     QT_TR_NOOP("Standard Views"),
                                                     QT_TR_NOOP("Switch to axonometric and orthogonal standard views"),
                                                     "Std_ViewIsometric",
                                                     0,
                                                     {"Std_ViewIsometric",
                                                      "Std_ViewDimetric",
                                                      "Std_ViewTrimetric",
                                                      "Std_ViewFront",
                                                      "Std_ViewTop",
                                                      "Std_ViewRight",
                                                      "Std_ViewRear",
                                                      "Std_ViewBottom",
                                                      "Std_ViewLeft"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompViewDisplay",
                                                     QT_TR_NOOP("Display"),
                                                     QT_TR_NOOP("Camera, draw style, fullscreen, clipping, transparency, and color commands"),
                                                     "Std_DrawStyle",
                                                     0,
                                                     {"Std_OrthographicCamera",
                                                      "Std_PerspectiveCamera",
                                                      "Std_DrawStyle",
                                                      "Std_MainFullscreen",
                                                      "Std_ViewDockUndockFullscreen",
                                                      "Std_AxisCross",
                                                      "Std_ToggleClipPlane",
                                                      "Std_TextureMapping",
                                                      "Std_RandomColor",
                                                      "Std_ToggleTransparency"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompViewVisibility",
                                                     QT_TR_NOOP("Visibility"),
                                                     QT_TR_NOOP("Show, hide, toggle, and select visible model objects"),
                                                     "Std_ToggleVisibility",
                                                     0,
                                                     {"Std_ToggleVisibility",
                                                      "Std_ShowSelection",
                                                      "Std_HideSelection",
                                                      "Std_SelectVisibleObjects",
                                                      "Std_ToggleObjects",
                                                      "Std_ShowObjects",
                                                      "Std_HideObjects",
                                                      "Std_ToggleSelectability"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompPanelWindowTools",
                                                     QT_TR_NOOP("Panels / Windows"),
                                                     QT_TR_NOOP("Show toolbars, docks, panels, status bar, and arrange document windows"),
                                                     "Std_DockViewMenu",
                                                     0,
                                                     {"Std_Workbench",
                                                      "Std_ToolBarMenu",
                                                      "Std_DockViewMenu",
                                                      "Std_ToggleBottomPanels",
                                                      "Std_ViewStatusBar",
                                                      "Std_ActivateNextWindow",
                                                      "Std_ActivatePrevWindow",
                                                      "Std_TileWindows",
                                                      "Std_CascadeWindows",
                                                      "Std_WindowsMenu",
                                                      "Std_Windows"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompAdvancedTools",
                                                     QT_TR_NOOP("Advanced Tools"),
                                                     QT_TR_NOOP("Add-ons, customization, parameters, diagnostics, dependency graphs, and project utilities"),
                                                     "Std_DlgCustomize",
                                                     0,
                                                     {"Std_AddonMgr",
                                                      "Std_DlgPreferences",
                                                      "Std_DlgCustomize",
                                                      "Std_DlgParameter",
                                                      "Std_ProjectUtil",
                                                      "Std_SceneInspector",
                                                      "Std_DependencyGraph",
                                                      "Std_ExportDependencyGraph",
                                                      "Std_DemoMode",
                                                      "Std_TextDocument"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompMacroTools",
                                                     QT_TR_NOOP("Macros / Automation"),
                                                     QT_TR_NOOP("Record, execute, and debug macros"),
                                                     "Std_DlgMacroExecute",
                                                     0,
                                                     {"Std_DlgMacroRecord",
                                                      "Std_DlgMacroExecute",
                                                      "Std_RecentMacros",
                                                      "Std_DlgMacroExecuteDirect",
                                                      "Std_MacroAttachDebugger"}));
    rcCmdMgr.addCommand(new CmdMcStudioCommandGroup("McStudio_CompHelp",
                                                     QT_TR_NOOP("Help"),
                                                     QT_TR_NOOP("Help, user resources, developer documentation, safe mode, and about commands"),
                                                     "Std_WhatsThis",
                                                     0,
                                                     {"Std_WhatsThis",
                                                      "Std_FreeCADUserHub",
                                                      "Std_FreeCADForum",
                                                      "Std_ReportBug",
                                                      "Std_RestartInSafeMode",
                                                      "Std_DevHandbook",
                                                      "Std_PythonHelp",
                                                      "Std_FreeCADWebsite",
                                                      "Std_FreeCADDonation",
                                                      "Std_About"}));
}
