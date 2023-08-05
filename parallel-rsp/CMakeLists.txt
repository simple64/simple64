cmake_minimum_required(VERSION 3.26)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(parallel-rsp LANGUAGES CXX C)
set(LIGHTNING_VERSION "2.2.2")

set(NAME_PLUGIN_SIMPLE64 "simple64-rsp-parallel")

include_directories(../mupen64plus-core/src/api)
add_definitions(-DM64P_PLUGIN_API)
add_definitions(-DPARALLEL_INTEGRATION)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    message("Interprocedural optimizations enabled")
endif()

add_library(${NAME_PLUGIN_SIMPLE64} SHARED
		parallel.cpp
		rsp/vfunctions.cpp
		rsp_jit.cpp rsp_jit.hpp
		jit_allocator.cpp jit_allocator.hpp
		rsp_disasm.cpp rsp_disasm.hpp
		rsp/ls.cpp rsp/pipeline.h
		rsp/reciprocal.cpp rsp/reciprocal.h
		rsp_1.1.h
		rsp/cp0.cpp rsp/cp2.cpp
		arch/simd/rsp/rsp_core.cpp
		arch/simd/rsp/clamp.h
		arch/simd/rsp/rsp_common.h
		arch/simd/rsp/SSE2NEON.h
		arch/simd/rsp/rsp_impl.h
		arch/simd/rsp/vcr.h
		arch/simd/rsp/vabs.h
		arch/simd/rsp/vadd.h
		arch/simd/rsp/vaddc.h
		arch/simd/rsp/vand.h
		arch/simd/rsp/vch.h
		arch/simd/rsp/vcl.h
		arch/simd/rsp/vcr.h
		arch/simd/rsp/vcmp.h
		arch/simd/rsp/vdivh.h
		arch/simd/rsp/vmac.h
		arch/simd/rsp/vmov.h
		arch/simd/rsp/vmrg.h
		arch/simd/rsp/vmudh.h
		arch/simd/rsp/vmul.h
		arch/simd/rsp/vmull.h
		arch/simd/rsp/vmulh.h
		arch/simd/rsp/vmuln.h
		arch/simd/rsp/vor.h
		arch/simd/rsp/vrcpsq.h
		arch/simd/rsp/vrsq.h
		arch/simd/rsp/vsub.h
		arch/simd/rsp/vsubc.h
		arch/simd/rsp/vxor.h
		arch/simd/rsp/vmulm.h)

target_sources(${NAME_PLUGIN_SIMPLE64} PRIVATE
		lightning-${LIGHTNING_VERSION}/lib/jit_disasm.c
		lightning-${LIGHTNING_VERSION}/lib/jit_memory.c
		lightning-${LIGHTNING_VERSION}/lib/jit_names.c
		lightning-${LIGHTNING_VERSION}/lib/jit_note.c
		lightning-${LIGHTNING_VERSION}/lib/jit_print.c
		lightning-${LIGHTNING_VERSION}/lib/jit_size.c
		lightning-${LIGHTNING_VERSION}/lib/lightning_patched.c)

if (WIN32)
		target_sources(${NAME_PLUGIN_SIMPLE64} PRIVATE win32/mman/sys/mman.c)
		target_include_directories(${NAME_PLUGIN_SIMPLE64} PRIVATE win32/mman)
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lightning-${LIGHTNING_VERSION}/lib/lightning_patched.c")
	execute_process(COMMAND patch
		${CMAKE_CURRENT_SOURCE_DIR}/lightning-${LIGHTNING_VERSION}/lib/lightning.c
		-i ${CMAKE_CURRENT_SOURCE_DIR}/lightning-${LIGHTNING_VERSION}/win32_mman.patch
		-o ${CMAKE_CURRENT_SOURCE_DIR}/lightning-${LIGHTNING_VERSION}/lib/lightning_patched.c
		COMMAND_ERROR_IS_FATAL ANY)
endif()

target_include_directories(${NAME_PLUGIN_SIMPLE64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lightning-${LIGHTNING_VERSION}/include)
target_include_directories(${NAME_PLUGIN_SIMPLE64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/arch/simd/rsp)

set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY VISIBILITY_INLINES_HIDDEN ON)
target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -DHAVE_MMAP -Ofast)
target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -DHAVE_MMAP -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=armv8-a)
    target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=armv8-a)
else()
    target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=x86-64-v3)
    target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=x86-64-v3)
endif()
if (NOT APPLE)
    target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif (NOT APPLE)
set_target_properties(${NAME_PLUGIN_SIMPLE64} PROPERTIES PREFIX "")
