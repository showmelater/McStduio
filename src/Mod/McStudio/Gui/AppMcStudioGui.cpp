// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PreCompiled.h"

#include <CXX/Extensions.hxx>
#include <CXX/Objects.hxx>

#include <Base/Console.h>
#include <Base/Interpreter.h>
#include <Base/PyObjectBase.h>
#include <Gui/Application.h>

#include "Workbench.h"

void CreateMcStudioCommands();

namespace McStudioGui
{
class Module: public Py::ExtensionModule<Module>
{
public:
    Module()
        : Py::ExtensionModule<Module>("McStudioGui")
    {
        initialize("This module is the McStudioGui module.");
    }
};

PyObject* initModule()
{
    return Base::Interpreter().addModule(new Module);
}
}  // namespace McStudioGui

PyMOD_INIT_FUNC(McStudioGui)
{
    if (!Gui::Application::Instance) {
        PyErr_SetString(PyExc_ImportError, "Cannot load McStudioGui module in console application.");
        PyMOD_Return(nullptr);
    }

    try {
        Base::Interpreter().runString("import PartGui");
        Base::Interpreter().runString("import PartDesignGui");
        Base::Interpreter().runString("import SketcherGui");
        Base::Interpreter().runString("import SurfaceGui");
        Base::Interpreter().runString("import MeasureGui");
    }
    catch (const Base::Exception& e) {
        PyErr_SetString(PyExc_ImportError, e.what());
        PyMOD_Return(nullptr);
    }

    // Optional modules: preloaded so their (Python-registered) commands are
    // available to the McStudio Ribbon Annotate/Assembly tabs. These are not
    // hard dependencies — if a module is unavailable in a given build we log a
    // warning and continue, rather than failing the whole McStudioGui load
    // (the Ribbon tolerates missing commands by skipping their buttons).
    try {
        Base::Interpreter().runString("import DraftGui");
    }
    catch (const Base::Exception& e) {
        Base::Console().warning("McStudio: DraftGui preload failed (Annotate commands may be missing): %s\n", e.what());
    }
    try {
        Base::Interpreter().runString("import AssemblyGui");
    }
    catch (const Base::Exception& e) {
        Base::Console().warning("McStudio: AssemblyGui preload failed (Assembly commands may be missing): %s\n", e.what());
    }

    CreateMcStudioCommands();
    McStudioGui::Workbench::init();

    PyObject* mod = McStudioGui::initModule();
    Base::Console().log("Loading GUI of McStudio module... done\n");
    PyMOD_Return(mod);
}
