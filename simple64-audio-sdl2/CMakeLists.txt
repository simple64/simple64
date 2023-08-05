cmake_minimum_required(VERSION 3.26)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(simple64-audio-sdl2 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    message("Interprocedural optimizations enabled")
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
target_compile_options(simple64-audio-sdl2 PRIVATE -Ofast)
target_link_options(simple64-audio-sdl2 PRIVATE -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_compile_options(simple64-audio-sdl2 PRIVATE -march=armv8-a)
    target_link_options(simple64-audio-sdl2 PRIVATE -march=armv8-a)
else()
    target_compile_options(simple64-audio-sdl2 PRIVATE -march=x86-64-v3)
    target_link_options(simple64-audio-sdl2 PRIVATE -march=x86-64-v3)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_link_options(simple64-audio-sdl2 PRIVATE -fuse-ld=lld)
endif()

if (NOT APPLE AND NOT WIN32)
    target_link_options(simple64-audio-sdl2 PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif ()

target_link_libraries(simple64-audio-sdl2 PRIVATE SDL2::SDL2)
