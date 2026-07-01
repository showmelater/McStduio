// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <vector>

#include <Mod/McStudio/McStudioGlobal.h>

namespace McStudioGui
{

enum class RibbonCommandSize
{
    Large,
    Medium,
    Small,
};

enum class RibbonCommandStyle
{
    TextUnderIcon,
    TextBesideIcon,
    IconOnly,
};

struct McStudioGuiExport RibbonCommandDefinition
{
    const char* command;
    RibbonCommandSize size;
    RibbonCommandStyle style;
    const char* label {nullptr};
};

struct McStudioGuiExport RibbonGroupDefinition
{
    const char* title;
    std::vector<RibbonCommandDefinition> commands;
};

struct McStudioGuiExport RibbonTabDefinition
{
    const char* title;
    std::vector<RibbonGroupDefinition> groups;
};

struct McStudioGuiExport RibbonContextDefinition
{
    const char* id;
    const char* title;
    const char* color;
    std::vector<RibbonTabDefinition> categories;
};

McStudioGuiExport RibbonCommandDefinition largeCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition largeCommand(const char* command, const char* label);
McStudioGuiExport RibbonCommandDefinition mediumCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition mediumCommand(const char* command, const char* label);
McStudioGuiExport RibbonCommandDefinition smallCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition smallCommand(const char* command, const char* label);
McStudioGuiExport RibbonCommandDefinition iconCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition dropDownCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition dropDownCommand(const char* command, const char* label);
McStudioGuiExport RibbonCommandDefinition mediumDropDownCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition mediumDropDownCommand(const char* command, const char* label);
McStudioGuiExport RibbonCommandDefinition smallDropDownCommand(const char* command);
McStudioGuiExport RibbonCommandDefinition smallDropDownCommand(const char* command, const char* label);

McStudioGuiExport const std::vector<RibbonTabDefinition>& ribbonTabs();
McStudioGuiExport const std::vector<RibbonContextDefinition>& ribbonContexts();

}  // namespace McStudioGui
