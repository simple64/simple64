cmake_minimum_required(VERSION 3.22)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(simple64-input-qt LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT ENABLE_IPO)
if(ENABLE_IPO)
    message("Interprocedural optimizations enabled")
endif(ENABLE_IPO)

if(ENABLE_IPO AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

include_directories(../mupen64plus-core/src/api)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x0602ff)

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

add_library(simple64-input-qt SHARED
    configdialog.cpp
    qt2sdl2.cpp
    main.cpp
)

if (WIN32)
    target_sources(simple64-input-qt PRIVATE
        osal/osal_dynamiclib_win32.c
    )
endif (WIN32)

if (UNIX)
    target_sources(simple64-input-qt PRIVATE
        osal/osal_dynamiclib_unix.c
    )
endif (UNIX)

set_property(TARGET simple64-input-qt PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-input-qt PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-input-qt PROPERTY VISIBILITY_INLINES_HIDDEN ON)
set_target_properties(simple64-input-qt PROPERTIES PREFIX "")
target_compile_options(simple64-input-qt PRIVATE -march=x86-64-v3 -Ofast)
target_link_options(simple64-input-qt PRIVATE -march=x86-64-v3 -Ofast)
if (NOT APPLE)
    target_link_options(simple64-input-qt PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif (NOT APPLE)
target_link_libraries(simple64-input-qt PRIVATE Qt6::Widgets Qt6::Network SDL2::SDL2)
