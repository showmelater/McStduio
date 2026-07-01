# SPDX-License-Identifier: LGPL-2.1-or-later

if(BUILD_GUI)
    add_subdirectory(Gui)
endif()

set(McStudio_SRCS
    Init.py
    InitGui.py
)

set(McStudio_Header_SRCS
    McStudioGlobal.h
)

add_custom_target(McStudio ALL
    SOURCES
        ${McStudio_SRCS}
        ${McStudio_Header_SRCS}
)

fc_copy_sources(McStudio "${CMAKE_BINARY_DIR}/Mod/McStudio" ${McStudio_SRCS})

install(
    FILES
        ${McStudio_SRCS}
    DESTINATION
        Mod/McStudio
)
