# Angrylion RDP Plus

This is a conservative fork of angrylion's RDP plugin that aims to improve performance and add new features while retaining the accuracy of the original plugin.

### Current features
* More maintainable code base by dividing the huge n64video.cpp into smaller pieces.
* Improved portability by separating the emulator plugin interface and window management from the RDP emulation core.
* Improved performance on multi-core CPUs by using multi-threaded rendering with scan line interleaving.
* Replaced deprecated DirectDraw interface with a modern OpenGL 3.3 implementation.
* Added fullscreen support and manual window sizing.
* Added BMP screenshot support.
* Added settings GUI.
* Added Mupen64Plus support.

Tested with Project64 2.3+. May also work with Project64 1.7 with a RSP plugin of newer builds (1.7.1+).

The Mupen64Plus plugin was tested with Mupen64Plus 2.5 using the [mupen64plus-rsp-cxd4](https://github.com/mupen64plus/mupen64plus-rsp-cxd4) plugin.

### Building

#### Visual Studio

To build the project with Visual Studio (2015+), you currently need to have Python 3 installed, which is used to fetch Git metadata and write it to `version.h`.
You can also build without, but then you have to copy `version.h.in` to `version.h` and disable the custom build event in the core project.

The glLoadGen files (`gl_core_3_3` and `wgl_ext`) were generated using the following parameters:

    lua LoadGen.lua core_3_3 -style=pointer_c -spec=gl -version=3.3 -profile=core
    lua LoadGen.lua ext -style=pointer_c -spec=wgl -ext WGL_EXT_swap_control -ext ARB_create_context -ext ARB_create_context_profile

#### CMake

Install dependencies:

    apt install cmake freeglut3-dev

Building:

    mkdir build
    cd build
    cmake ..
    make

To create an OpenGL ES 3 build, add ``-DGLES=ON`` to the cmake arguments.

### Credits
* Angrylion, Ville Linde, MooglyGuy and others involved for creating an awesome N64 RDP reference software.
* theboy181 - Testing. Lots of testing.
* fallaha56 - Donator.
* loganmc10 - Mupen64Plus plugin implementation.
