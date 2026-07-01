// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <Gui/Workbench.h>
#include <Mod/McStudio/McStudioGlobal.h>

namespace McStudioGui
{

class McStudioGuiExport Workbench: public Gui::StdWorkbench
{
    TYPESYSTEM_HEADER_WITH_OVERRIDE();

public:
    Workbench();
    ~Workbench() override;

    void activated() override;
    void deactivated() override;

protected:
    Gui::MenuItem* setupMenuBar() const override;
    Gui::ToolBarItem* setupToolBars() const override;
    Gui::DockWindowItems* setupDockWindows() const override;
};

}  // namespace McStudioGui
