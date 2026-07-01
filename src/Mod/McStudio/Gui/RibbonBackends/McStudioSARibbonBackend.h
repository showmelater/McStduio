// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "McStudioRibbonBackend.h"

class QWidget;

namespace McStudioGui
{

McStudioRibbonBackend* createSARibbonBackend(QWidget* parent);

}  // namespace McStudioGui
