cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
project(parallel-rsp LANGUAGES CXX C)


set(NAME_PLUGIN_M64P "mupen64plus-rsp-parallel")

include_directories(../mupen64plus-core/src/api)
add_definitions(-DM64P_PLUGIN_API)
add_definitions(-DPARALLEL_INTEGRATION)

if (CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    set(PARALLEL_RSP_CXX_FLAGS -Wall -Wextra -Wno-missing-field-initializers -Wno-empty-body -ffast-math -Wno-unused-parameter)
elseif (MSVC)
    set(PARALLEL_RSP_CXX_FLAGS /D_CRT_SECURE_NO_WARNINGS /wd4267 /wd4244 /wd4309 /wd4005 /MP /DNOMINMAX)
endif()

add_library(${NAME_PLUGIN_M64P} SHARED
#		main.cpp
		parallel.cpp
#		debug_rsp.cpp
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

option(PARALLEL_RSP_DEBUG_JIT "Enable the debug JIT." OFF)

if (PARALLEL_RSP_DEBUG_JIT)
	if (ANDROID)
		message("Android does not support debug JIT. Disabling it.")
	elseif (NOT WIN32)
		target_sources(${NAME_PLUGIN_M64P} PRIVATE debug_rsp.cpp debug_rsp.hpp debug_jit.cpp debug_jit.hpp)
		target_compile_definitions(${NAME_PLUGIN_M64P} PUBLIC PARALLEL_RSP_DEBUG_JIT)
	endif()
endif()

target_include_directories(${NAME_PLUGIN_M64P} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME_PLUGIN_M64P} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/arch/simd/rsp)
target_include_directories(${NAME_PLUGIN_M64P} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/api)
target_compile_options(${NAME_PLUGIN_M64P} PRIVATE ${PARALLEL_RSP_CXX_FLAGS})

option(PARALLEL_RSP_BAKED_LIGHTNING "Use built-in Lightning." ON)

target_compile_definitions(${NAME_PLUGIN_M64P} PUBLIC PARALLEL_RSP_LIGHTNING)
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
endif()
target_link_libraries(${NAME_PLUGIN_M64P} PUBLIC lightning)
#target_compile_definitions(${NAME_PLUGIN_M64P} PUBLIC DEBUG_JIT)

if (NOT WIN32)
	target_link_libraries(${NAME_PLUGIN_M64P} PRIVATE dl)
endif()

add_executable(rsp-runner main.cpp)
target_link_libraries(rsp-runner PRIVATE ${NAME_PLUGIN_M64P})
target_compile_options(rsp-runner PRIVATE ${PARALLEL_RSP_CXX_FLAGS})

if (PARALLEL_RSP_DEBUG_JIT AND NOT WIN32)
	set_target_properties(rsp-runner PROPERTIES LINK_FLAGS "-rdynamic")
endif()

option(PARALLEL_RSP_TESTS "Enable unit tests for Lightning CPU." OFF)

add_executable(rsp-vu-fuzzer rsp_vu_fuzzer.cpp)
target_link_libraries(rsp-vu-fuzzer PRIVATE ${NAME_PLUGIN_M64P})
target_compile_options(rsp-vu-fuzzer PRIVATE ${PARALLEL_RSP_CXX_FLAGS})

if (PARALLEL_RSP_TESTS)
	enable_testing()

	if (ANDROID)
		add_test(NAME rsp-vu-fuzz
				COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/android_fuzz_runner.sh
				${CMAKE_CURRENT_BINARY_DIR}/rsp-vu-fuzzer)
	else()
		add_test(NAME rsp-vu-fuzz COMMAND $<TARGET_FILE:rsp-vu-fuzzer>)
	endif()
	add_custom_target(rsp-tests ALL)

	add_custom_target(rsp-test-crt
			COMMAND ${CMAKE_MAKE_PROGRAM} crt-obj -j1
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain)

	function(rsp_add_test NAME)
		add_custom_target(rsp-test-binary-${NAME}
				COMMAND ${CMAKE_MAKE_PROGRAM} RSP_TEST=${NAME} -j1
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain
				DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain/${NAME}.s)
		add_dependencies(rsp-test-binary-${NAME} rsp-test-crt)
		add_dependencies(rsp-tests rsp-test-binary-${NAME})

		if (ANDROID)
			add_test(NAME rsp-test-${NAME}
					COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/android_test_runner.sh
					${CMAKE_CURRENT_BINARY_DIR}/rsp-runner
					${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain ${NAME})
		else()
			add_test(NAME rsp-test-${NAME}
					COMMAND $<TARGET_FILE:rsp-runner>
					${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain/${NAME}.global.bin
					${CMAKE_CURRENT_SOURCE_DIR}/debug-toolchain/${NAME}.bin)
		endif()
	endfunction()

	rsp_add_test(sll)
	rsp_add_test(srl)
	rsp_add_test(sra)
	rsp_add_test(sllv)
	rsp_add_test(srlv)
	rsp_add_test(srav)
	rsp_add_test(add)
	rsp_add_test(sub)
	rsp_add_test(or)
	rsp_add_test(and)
	rsp_add_test(xor)
	rsp_add_test(nor)
	rsp_add_test(slt)
	rsp_add_test(sltu)
	rsp_add_test(addi)
	rsp_add_test(slti)
	rsp_add_test(sltiu)
	rsp_add_test(andi)
	rsp_add_test(ori)
	rsp_add_test(xori)
	rsp_add_test(lui)
	rsp_add_test(lb)
	rsp_add_test(lbu)
	rsp_add_test(lh)
	rsp_add_test(lhu)
	rsp_add_test(lh-unaligned)
	rsp_add_test(lhu-unaligned)
	rsp_add_test(lw)
	rsp_add_test(lw-unaligned)
	rsp_add_test(sb)
	rsp_add_test(sh)
	rsp_add_test(sw)
	rsp_add_test(sh-unaligned)
	rsp_add_test(sw-unaligned)
	rsp_add_test(beq-impossible-delay-slot-second-taken)
	rsp_add_test(beq-impossible-delay-slot-first-taken)
	#rsp_add_test(beq-impossible-delay-slot-both-taken) Hangs the reference implementation :D
	rsp_add_test(bne)
	rsp_add_test(blez)
	rsp_add_test(bltz)
	rsp_add_test(bltzal)
	rsp_add_test(bgtz)
	rsp_add_test(bgezal)
	rsp_add_test(j)
	rsp_add_test(jal)
	rsp_add_test(jr)
	rsp_add_test(jalr)
	rsp_add_test(lw-unaligned-in-branch-delay)
	rsp_add_test(delay-slot-before-break)
	rsp_add_test(delay-slot-before-new-block)
	rsp_add_test(delay-slot-before-new-block-illegal)
	rsp_add_test(delay-slot-before-new-block-not-taken)
	rsp_add_test(unconditional-delay-slot-before-break)
	rsp_add_test(cop0)
	rsp_add_test(cop2-basic)
	rsp_add_test(cop2-ls)
	rsp_add_test(cop2-vector)
	rsp_add_test(bug-shl-into-branch)
endif()
