cmake_minimum_required(VERSION 3.24)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(simple64-audio-sdl2 LANGUAGES C)

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

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(../mupen64plus-core/src/api)

add_library(simple64-audio-sdl2 SHARED
    src/main.c
)

set_property(TARGET simple64-audio-sdl2 PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-audio-sdl2 PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-audio-sdl2 PROPERTY VISIBILITY_INLINES_HIDDEN ON)
set_target_properties(simple64-audio-sdl2 PROPERTIES PREFIX "")
target_compile_options(simple64-audio-sdl2 PRIVATE -march=x86-64-v3 -Ofast)
target_link_options(simple64-audio-sdl2 PRIVATE -march=x86-64-v3 -Ofast)
if (NOT APPLE)
    target_link_options(simple64-audio-sdl2 PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif (NOT APPLE)
target_link_libraries(simple64-audio-sdl2 PRIVATE SDL2::SDL2)
