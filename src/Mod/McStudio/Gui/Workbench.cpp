// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"
#include "Workbench.h"

#include <Base/Console.h>
#include <Gui/DockWindowManager.h>
#include <Gui/MenuManager.h>
#include <Gui/ToolBarManager.h>

#include "RibbonWidget.h"

using namespace McStudioGui;

TYPESYSTEM_SOURCE(McStudioGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench() = default;
Workbench::~Workbench() = default;

void Workbench::activated()
{
    Base::Console().message("McStudioDiag[Workbench::activated]: begin.\n");
    Gui::Workbench::activated();
    Base::Console().message("McStudioDiag[Workbench::activated]: after base activated; before RibbonWidget::show.\n");
    RibbonWidget::show();
    Base::Console().message("McStudioDiag[Workbench::activated]: end.\n");
    Base::Console().message("McStudioGui::Workbench activated.\n");
}

void Workbench::deactivated()
{
    Base::Console().message("McStudioDiag[Workbench::deactivated]: begin.\n");
    Base::Console().message("McStudioGui::Workbench deactivated.\n");
    RibbonWidget::hide();
    Base::Console().message("McStudioDiag[Workbench::deactivated]: after RibbonWidget::hide; before base deactivated.\n");
    Gui::Workbench::deactivated();
    Base::Console().message("McStudioDiag[Workbench::deactivated]: end.\n");
}

Gui::MenuItem* Workbench::setupMenuBar() const
{
    Base::Console().message("McStudioDiag[Workbench::setupMenuBar]: begin.\n");
    Base::Console().message("McStudioGui::Workbench setupMenuBar.\n");

    Gui::MenuItem* root = StdWorkbench::setupMenuBar();
    Gui::MenuItem* item = root->findItem("&Windows");

    auto* mcstudio = new Gui::MenuItem;
    if (!root->insertItem(item, mcstudio)) {
        Base::Console().warning("McStudioGui: &Windows anchor not found; appending McStudio menu.\n");
        root->appendItem(mcstudio);
    }
    mcstudio->setCommand("McStudio");

    *mcstudio << "Std_New"
              << "Std_Open"
              << "Std_Save"
              << "Separator"
              << "PartDesign_Body"
              << "PartDesign_Pad"
              << "PartDesign_Pocket"
              << "PartDesign_Hole"
              << "Separator"
              << "Sketcher_LeaveSketch"
              << "PartDesign_NewSketch"
              << "Sketcher_EditSketch"
              << "Sketcher_MapSketch"
              << "Sketcher_ValidateSketch"
              << "Separator"
              << "McStudio_CreateReferencePlane"
              << "McStudio_CreateWorkpiece"
              << "McStudio_CreateStock"
              << "McStudio_CreateFixture"
              << "McStudio_SetWorkCoordinateSystem"
              << "McStudio_CreateSetupDatum"
              << "Separator"
              << "McStudio_GenerateProcessReport"
              << "McStudio_ExportSetupSheet"
              << "McStudio_GenerateBOM";

    Base::Console().message("McStudioDiag[Workbench::setupMenuBar]: end.\n");
    return root;
}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
    Base::Console().message("McStudioDiag[Workbench::setupToolBars]: begin.\n");
    Base::Console().message("McStudioGui::Workbench setupToolBars: Ribbon-only mode.\n");

    // Ribbon-only mode: do not create FreeCAD's standard QToolBar set for
    // McStudio. Standard commands are exposed through RibbonWidget instead.
    // Returning an empty root lets ToolBarManager hide toolbars that belong to
    // other workbenches; returning nullptr would leave previous toolbars alone.
    Base::Console().message("McStudioDiag[Workbench::setupToolBars]: returning empty ToolBarItem root; FreeCAD may still append custom/global toolbars afterwards.\n");
    return new Gui::ToolBarItem();
}

Gui::DockWindowItems* Workbench::setupDockWindows() const
{
    Base::Console().message("McStudioDiag[Workbench::setupDockWindows]: begin.\n");
    Base::Console().message("McStudioGui::Workbench setupDockWindows: CAD shell layout.\n");

    auto* root = new Gui::DockWindowItems();
    root->addDockWidget("Std_ComboView", Qt::LeftDockWidgetArea, Gui::DockWindowOption::VisibleTabbed);
    root->addDockWidget("Std_TaskView", Qt::RightDockWidgetArea, Gui::DockWindowOption::VisibleTabbed);
    root->addDockWidget("Std_ReportView", Qt::BottomDockWidgetArea, Gui::DockWindowOption::HiddenTabbed);
    root->addDockWidget("Std_PythonView", Qt::BottomDockWidgetArea, Gui::DockWindowOption::HiddenTabbed);
    Base::Console().message("McStudioDiag[Workbench::setupDockWindows]: end.\n");
    return root;
}
