cmake_minimum_required(VERSION 3.20)

project(simple64-input-raphnetraw LANGUAGES C)

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

find_package(PkgConfig REQUIRED)

add_library(simple64-input-raphnetraw SHARED
    src/plugin_front.c
    src/plugin_back.c
    src/gcn64lib.c
    src/gcn64.c
    src/hexdump.c
)

if (WIN32)
    target_sources(simple64-input-raphnetraw PRIVATE
        src/osal_dynamiclib_win32.c
    )
    pkg_check_modules(HIDAPI REQUIRED hidapi)
endif (WIN32)

if (UNIX)
    target_sources(simple64-input-raphnetraw PRIVATE
        src/osal_dynamiclib_unix.c
    )
    pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)
endif (UNIX)

set_property(TARGET simple64-input-raphnetraw PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-input-raphnetraw PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET simple64-input-raphnetraw PROPERTY VISIBILITY_INLINES_HIDDEN ON)
set_target_properties(simple64-input-raphnetraw PROPERTIES PREFIX "")
target_compile_options(simple64-input-raphnetraw PRIVATE -march=x86-64-v3 ${HIDAPI_CFLAGS} -fno-semantic-interposition)
target_link_options(simple64-input-raphnetraw PRIVATE -march=x86-64-v3 -fno-semantic-interposition -Wl,-Bsymbolic -Wl,-Bsymbolic-functions)
target_link_directories(simple64-input-raphnetraw PRIVATE ${HIDAPI_LIBRARY_DIRS})
target_link_libraries(simple64-input-raphnetraw PRIVATE ${HIDAPI_LIBRARIES})
