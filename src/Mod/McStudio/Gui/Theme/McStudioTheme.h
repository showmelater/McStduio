// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "McStudioThemeTokens.h"

#include <QString>

#include <SARibbonGlobal.h>

class SARibbonBar;

namespace McStudioGui
{

McStudioThemeTokens currentMcStudioThemeTokens();
SARibbonTheme ribbonThemeForTokens(const McStudioThemeTokens& tokens);
QString loadMcStudioSARibbonOverlay(const McStudioThemeTokens& tokens);
void applyMcStudioSARibbonTheme(SARibbonBar* ribbon);

}  // namespace McStudioGui
