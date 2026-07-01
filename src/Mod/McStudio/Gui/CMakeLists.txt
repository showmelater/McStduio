# SPDX-License-Identifier: LGPL-2.1-or-later

set(McStudioGui_LIBS
    FreeCADGui
    SARibbonBar::SARibbonBar
)

# SARibbon is used as the long-term McStudio Ribbon framework.
# Keep it vendored and statically linked into McStudioGui; do not use SARibbonMainWindow.
set(SARIBBON_VERSION 2.9.1 CACHE STRING "SARibbon version" FORCE)
set(SARIBBON_BUILD_STATIC_LIBS ON CACHE BOOL "Build SARibbon as a static library" FORCE)
set(SARIBBON_BUILD_EXAMPLES OFF CACHE BOOL "Do not build SARibbon examples" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Do not build SARibbon tests" FORCE)
set(SARIBBON_USE_FRAMELESS_LIB OFF CACHE BOOL "Disable SARibbon frameless integration" FORCE)
set(_SARIBBON_USE_FRAMELESS_LIB OFF CACHE BOOL "Disable SARibbon frameless integration" FORCE)
set(QT_VERSION_MAJOR ${FREECAD_QT_MAJOR_VERSION})
# SARibbonBar/CMakeLists.txt expects CMAKE_DEBUG_POSTFIX to be non-empty because it is not quoted.
set(_MCSTUDIO_OLD_CMAKE_DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

if(NOT TARGET SARibbonBar::SARibbonBar)
    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/SARibbon/src/SARibbonBar
        ${CMAKE_BINARY_DIR}/src/3rdParty/SARibbonBar
    )
endif()
set(CMAKE_DEBUG_POSTFIX ${_MCSTUDIO_OLD_CMAKE_DEBUG_POSTFIX})


qt_add_resources(McStudioGui_QRC_SRCS Resources/McStudio.qrc)

set(McStudioGui_SRCS
    AppMcStudioGui.cpp
    Command.cpp
    PreCompiled.h
    RibbonModel.cpp
    RibbonModel.h
    RibbonWidget.cpp
    RibbonWidget.h
    RibbonBackends/McStudioRibbonBackend.h
    RibbonBackends/McStudioSARibbonBackend.cpp
    RibbonBackends/McStudioSARibbonBackend.h
    Theme/McStudioTheme.cpp
    Theme/McStudioTheme.h
    Theme/McStudioThemeTokens.h
    Workbench.cpp
    Workbench.h
    Resources/McStudio.qrc
    Resources/styles/McStudioRibbon.qss
    Resources/styles/McStudioSARibbon.qss
    ${McStudioGui_QRC_SRCS}
)

add_library(McStudioGui SHARED
    ${McStudioGui_SRCS}
)

if(FREECAD_USE_PCH)
    target_precompile_headers(McStudioGui PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:"${CMAKE_CURRENT_LIST_DIR}/PreCompiled.h">
    )
endif(FREECAD_USE_PCH)

target_include_directories(
    McStudioGui
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(McStudioGui ${McStudioGui_LIBS})

if (MSVC)
    target_compile_options(McStudioGui PRIVATE /wd4251)
endif()

if (FREECAD_WARN_ERROR)
    target_compile_warn_error(McStudioGui)
endif()

SET_BIN_DIR(McStudioGui McStudioGui /Mod/McStudio)
SET_PYTHON_PREFIX_SUFFIX(McStudioGui)

INSTALL(TARGETS McStudioGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
