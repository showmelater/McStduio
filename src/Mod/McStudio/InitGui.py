# SPDX-License-Identifier: LGPL-2.1-or-later

import FreeCAD
import FreeCADGui


class McStudioWorkbench(Workbench):
    """McStudio C++ workbench entry point."""

    MenuText = "McStudio"
    ToolTip = "McStudio integrated CAD/CAM workbench"
    Icon = ""

    def Initialize(self):
        """Load McStudio's C++ workbench module and its native command providers."""
        FreeCAD.Console.PrintMessage("McStudio InitGui.Initialize: importing McStudioGui\n")
        try:
            import McStudioGui  # noqa: F401
        except Exception as exc:
            FreeCAD.Console.PrintError(
                "McStudio: failed to import McStudioGui: {}\n".format(exc)
            )
            raise

        FreeCAD.Console.PrintMessage("McStudio InitGui.Initialize: McStudioGui imported\n")

    def GetClassName(self):
        return "McStudioGui::Workbench"


FreeCAD.Console.PrintMessage("McStudio InitGui: registering McStudioWorkbench\n")
FreeCADGui.addWorkbench(McStudioWorkbench())
