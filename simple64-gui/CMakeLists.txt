cmake_minimum_required(VERSION 3.22)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(simple64-gui LANGUAGES CXX C)

include(CheckIPOSupported)
check_ipo_supported(RESULT ENABLE_IPO)

if(ENABLE_IPO AND (NOT CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    message("Interprocedural optimizations enabled")
endif()

include_directories(../mupen64plus-core/src/api)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x0602ff -DNETPLAY_VER=14)

if (DEFINED ENV{SIMPLE64_AUTOUPDATE})
    add_definitions(-D_AUTOUPDATE)
    message("Enabling auto updater")
else()
    message("Not enabling auto updater")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets)
if (UNIX)
    find_package(Qt6 REQUIRED COMPONENTS DBus)
endif (UNIX)

qt_standard_project_setup()

qt_add_resources(simple64-gui imageresources
    FILES icons/simple64.svg
)

add_executable(simple64-gui
    mainwindow.ui
    main.cpp
    mainwindow.cpp
    vidext.cpp
    settingsdialog.cpp
    interface/common.cpp
    hotkeydialog.cpp
    plugindialog.cpp
    vkwindow.cpp
    workerthread.cpp
    settingclasses.cpp
    interface/core_commands.cpp
    interface/sdl_key_converter.c
    logviewer.cpp
    keypressfilter.cpp
    netplay/createroom.cpp
    netplay/joinroom.cpp
    netplay/waitroom.cpp
)

if (WIN32)
    target_sources(simple64-gui PRIVATE
        osal/osal_dynamiclib_win32.c
    )
    set(app_icon_resource_windows "icons/icon.rc")
    target_sources(simple64-gui PRIVATE ${app_icon_resource_windows})
endif (WIN32)

if (UNIX)
    target_sources(simple64-gui PRIVATE
        osal/osal_dynamiclib_unix.c
    )
    target_link_libraries(simple64-gui PRIVATE Qt6::DBus)
endif (UNIX)

set_property(TARGET simple64-gui PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-gui PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-gui PROPERTY VISIBILITY_INLINES_HIDDEN ON)
target_compile_options(simple64-gui PRIVATE -Ofast)
target_link_options(simple64-gui PRIVATE -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_compile_options(simple64-gui PRIVATE -march=armv8-a)
    target_link_options(simple64-gui PRIVATE -march=armv8-a)
else()
    target_compile_options(simple64-gui PRIVATE -march=x86-64-v3)
    target_link_options(simple64-gui PRIVATE -march=x86-64-v3)
endif()
if (NOT APPLE)
    target_link_options(simple64-gui PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif (NOT APPLE)
target_link_libraries(simple64-gui PRIVATE Qt6::Widgets Qt6::WebSockets ${CMAKE_DL_LIBS})

set_target_properties(simple64-gui PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
