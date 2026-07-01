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

    CreateMcStudioCommands();
    McStudioGui::Workbench::init();

    PyObject* mod = McStudioGui::initModule();
    Base::Console().log("Loading GUI of McStudio module... done\n");
    PyMOD_Return(mod);
}
