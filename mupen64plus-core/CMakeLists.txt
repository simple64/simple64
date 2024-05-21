cmake_minimum_required(VERSION 3.26)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(mupen64plus LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    message("Interprocedural optimizations enabled")
endif()

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL_NET REQUIRED SDL2_net)

include_directories(
    src
    subprojects/md5
    subprojects/xxhash
    subprojects/minizip
    subprojects/xdelta
    ${SDL2_INCLUDE_DIRS}
)

add_definitions(-DM64P_PARALLEL -DM64P_NETPLAY -DNO_ASM)

add_library(mupen64plus SHARED
    src/api/callbacks.c
    src/api/common.c
    src/api/config.c
    src/api/debugger.c
    src/api/frontend.c
    src/api/vidext.c
    src/backends/api/video_capture_backend.c
    src/backends/plugins_compat/audio_plugin_compat.c
    src/backends/plugins_compat/input_plugin_compat.c
    src/backends/clock_ctime_plus_delta.c
    src/backends/dummy_video_capture.c
    src/backends/file_storage.c
    src/device/cart/cart.c
    src/device/cart/af_rtc.c
    src/device/cart/cart_rom.c
    src/device/cart/eeprom.c
    src/device/cart/flashram.c
    src/device/cart/is_viewer.c
    src/device/cart/sram.c
    src/device/controllers/game_controller.c
    src/device/controllers/vru_controller.c
    src/device/controllers/paks/biopak.c
    src/device/controllers/paks/mempak.c
    src/device/controllers/paks/rumblepak.c
    src/device/controllers/paks/transferpak.c
    src/device/dd/dd_controller.c
    src/device/dd/disk.c
    src/device/device.c
    src/device/gb/gb_cart.c
    src/device/gb/mbc3_rtc.c
    src/device/gb/m64282fp.c
    src/device/memory/memory.c
    src/device/pif/bootrom_hle.c
    src/device/pif/cic.c
    src/device/pif/n64_cic_nus_6105.c
    src/device/pif/pif.c
    src/device/r4300/cached_interp.c
    src/device/r4300/cp0.c
    src/device/r4300/cp1.c
    src/device/r4300/cp2.c
    src/device/r4300/idec.c
    src/device/r4300/interrupt.c
    src/device/r4300/pure_interp.c
    src/device/r4300/r4300_core.c
    src/device/r4300/dcache.c
    src/device/r4300/icache.c
    src/device/r4300/tlb.c
    src/device/rcp/ai/ai_controller.c
    src/device/rcp/mi/mi_controller.c
    src/device/rcp/pi/pi_controller.c
    src/device/rcp/rdp/rdp_core.c
    src/device/rcp/ri/ri_controller.c
    src/device/rcp/rsp/rsp_core.c
    src/device/rcp/si/si_controller.c
    src/device/rcp/vi/vi_controller.c
    src/device/rdram/rdram.c
    src/main/main.c
    src/main/util.c
    src/main/cheat.c
    src/main/eventloop.c
    src/main/rom.c
    src/main/savestates.c
    src/main/screenshot.c
    src/main/sdl_key_converter.c
    src/main/workqueue.c
    src/plugin/plugin.c
    src/plugin/dummy_video.c
    src/plugin/dummy_audio.c
    src/plugin/dummy_input.c
    src/plugin/dummy_rsp.c
    src/main/netplay.c
    subprojects/md5/md5.c
    subprojects/minizip/ioapi.c
    subprojects/minizip/unzip.c
    subprojects/minizip/zip.c
)

if (WIN32)
    target_sources(mupen64plus PRIVATE
        src/osal/dynamiclib_win32.c
        src/osal/files_win32.c
    )
elseif (APPLE)
    target_sources(mupen64plus PRIVATE
        src/osal/dynamiclib_unix.c
        src/osal/files_macos.c
    )
elseif (UNIX)
    target_sources(mupen64plus PRIVATE
        src/osal/dynamiclib_unix.c
        src/osal/files_unix.c
    )
endif ()

set_property(TARGET mupen64plus PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET mupen64plus PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET mupen64plus PROPERTY VISIBILITY_INLINES_HIDDEN ON)
target_compile_options(mupen64plus PRIVATE -Ofast)
target_link_options(mupen64plus PRIVATE -Ofast)
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_compile_options(mupen64plus PRIVATE -march=armv8-a)
    target_link_options(mupen64plus PRIVATE -march=armv8-a)
else()
    target_compile_options(mupen64plus PRIVATE -march=x86-64-v3)
    target_link_options(mupen64plus PRIVATE -march=x86-64-v3)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_link_options(mupen64plus PRIVATE -fuse-ld=lld)
endif()

if (NOT APPLE AND NOT WIN32)
    target_link_options(mupen64plus PRIVATE -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
endif ()

target_link_directories(mupen64plus PRIVATE ${SDL_NET_LIBRARY_DIRS})
target_link_libraries(mupen64plus PRIVATE SDL2::SDL2 ${SDL_NET_LIBRARIES} ZLIB::ZLIB PNG::PNG)
if (APPLE)
    target_link_libraries(mupen64plus PRIVATE "-framework CoreFoundation")
endif (APPLE)
