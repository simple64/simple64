cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
project(parallel-rdp LANGUAGES CXX C)

set(NAME_PLUGIN_M64P "mupen64plus-video-parallel")
include_directories(../mupen64plus-core/src/api)
add_definitions(-DM64P_PLUGIN_API -DGRANITE_VULKAN_MT)

# check for INTERPROCEDURAL_OPTIMIZATION support
if((${CMAKE_VERSION} VERSION_EQUAL 3.9) OR (${CMAKE_VERSION} VERSION_GREATER 3.9))
    cmake_policy(SET CMP0069 NEW)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT ENABLE_IPO)
    if(ENABLE_IPO)
        message("Interprocedural optimizations enabled")
    endif(ENABLE_IPO)
endif()

if(ENABLE_IPO AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(WIN32)
    find_package(OpenGL REQUIRED)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
endif()

FILE(GLOB RDPSources parallel-rdp/*.cpp)

add_library(${NAME_PLUGIN_M64P} SHARED
    ${RDPSources}
    vulkan/buffer.cpp
    vulkan/buffer_pool.cpp
    vulkan/command_buffer.cpp
    vulkan/command_pool.cpp
    vulkan/context.cpp
    vulkan/cookie.cpp
    vulkan/descriptor_set.cpp
    vulkan/device.cpp
    vulkan/event_manager.cpp
    vulkan/fence.cpp
    vulkan/fence_manager.cpp
    vulkan/image.cpp
    vulkan/memory_allocator.cpp
    vulkan/pipeline_event.cpp
    vulkan/query_pool.cpp
    vulkan/render_pass.cpp
    vulkan/sampler.cpp
    vulkan/semaphore.cpp
    vulkan/semaphore_manager.cpp
    vulkan/shader.cpp
    vulkan/texture_format.cpp
    util/logging.cpp
    util/thread_id.cpp
    util/aligned_alloc.cpp
    util/timer.cpp
    util/timeline_trace_file.cpp
    util/thread_name.cpp
    volk/volk.c
    gfx_m64p.c
    glguts.c
    parallel_imp.cpp)

target_include_directories(${NAME_PLUGIN_M64P} PUBLIC
    parallel-rdp
    volk
    vulkan
    vulkan-headers/include
    util)

if(WIN32)
    target_link_libraries(${NAME_PLUGIN_M64P} ${OPENGL_LIBRARIES} -lwinmm)
endif()

target_compile_options(${NAME_PLUGIN_M64P} PRIVATE -march=x86-64-v3)
target_link_options(${NAME_PLUGIN_M64P} PRIVATE -march=x86-64-v3)
set_target_properties(${NAME_PLUGIN_M64P} PROPERTIES PREFIX "")
