// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <FCGlobal.h>

#ifndef McStudioGuiExport
#  ifdef McStudioGui_EXPORTS
#    define McStudioGuiExport FREECAD_DECL_EXPORT
#  else
#    define McStudioGuiExport FREECAD_DECL_IMPORT
#  endif
#endif
