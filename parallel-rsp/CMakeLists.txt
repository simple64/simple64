cmake_minimum_required(VERSION 3.22)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
project(parallel-rsp LANGUAGES CXX C)


set(NAME_PLUGIN_SIMPLE64 "simple64-rsp-parallel")

include_directories(../mupen64plus-core/src/api)
add_definitions(-DM64P_PLUGIN_API)
add_definitions(-DPARALLEL_INTEGRATION)

include(CheckIPOSupported)
check_ipo_supported(RESULT ENABLE_IPO)
if(ENABLE_IPO)
	message("Interprocedural optimizations enabled")
endif(ENABLE_IPO)

if(ENABLE_IPO AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    set(PARALLEL_RSP_CXX_FLAGS -Wall -Wextra -Wno-missing-field-initializers -Wno-empty-body -ffast-math -Wno-unused-parameter)
elseif (MSVC)
    set(PARALLEL_RSP_CXX_FLAGS /D_CRT_SECURE_NO_WARNINGS /wd4267 /wd4244 /wd4309 /wd4005 /MP /DNOMINMAX)
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

set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET ${NAME_PLUGIN_SIMPLE64} PROPERTY VISIBILITY_INLINES_HIDDEN ON)
set_target_properties(${NAME_PLUGIN_SIMPLE64} PROPERTIES PREFIX "")
target_include_directories(${NAME_PLUGIN_SIMPLE64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME_PLUGIN_SIMPLE64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/arch/simd/rsp)
target_include_directories(${NAME_PLUGIN_SIMPLE64} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/api)
target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE ${PARALLEL_RSP_CXX_FLAGS} -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
	target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=armv8-a)
else()
	target_compile_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=x86-64-v3)
endif()

option(PARALLEL_RSP_BAKED_LIGHTNING "Use built-in Lightning." ON)

target_compile_definitions(${NAME_PLUGIN_SIMPLE64} PUBLIC PARALLEL_RSP_LIGHTNING)
if (PARALLEL_RSP_BAKED_LIGHTNING)
	add_library(lightning STATIC
			lightning/lib/jit_disasm.c
			lightning/lib/jit_memory.c
			lightning/lib/jit_names.c
			lightning/lib/jit_note.c
			lightning/lib/jit_print.c
			lightning/lib/jit_size.c
			lightning/lib/lightning.c)
	set_property(TARGET lightning PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_include_directories(lightning PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/lightning/include)
	if (WIN32)
		target_sources(lightning PRIVATE win32/mman/sys/mman.c)
		target_include_directories(lightning PRIVATE win32/mman)
	endif()
	set_property(TARGET lightning PROPERTY C_VISIBILITY_PRESET hidden)
	set_property(TARGET lightning PROPERTY CXX_VISIBILITY_PRESET hidden)
	set_property(TARGET lightning PROPERTY VISIBILITY_INLINES_HIDDEN ON)
	target_compile_options(lightning PRIVATE -DHAVE_MMAP -Ofast)
	target_link_options(lightning PRIVATE -DHAVE_MMAP -Ofast)
	if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
		target_compile_options(lightning PRIVATE -march=armv8-a)
		target_link_options(lightning PRIVATE -march=armv8-a)
	else()
		target_compile_options(lightning PRIVATE -march=x86-64-v3)
		target_link_options(lightning PRIVATE -march=x86-64-v3)
	endif()
	if (NOT APPLE)
		target_link_options(lightning PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
	endif (NOT APPLE)
endif()
target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
	target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=armv8-a)
else()
	target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -march=x86-64-v3)
endif()
if (NOT APPLE)
	target_link_options(${NAME_PLUGIN_SIMPLE64} PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif (NOT APPLE)
target_link_libraries(${NAME_PLUGIN_SIMPLE64} PUBLIC lightning)
