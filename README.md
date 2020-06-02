# Input plugin with direct N64 controller access for raphnet adapters

This mupen64plus plugin uses the direct controller access feature offered by my N64 to USB
adapters (versions 3 and up) to let the emulated game communicate with the controllers directly.

* Perfect accuracy: The controller works exactly as it would on a N64 system, without any calibration or tweaking.
* Rumble Pak and Controller Pak (mempak) support (and potentially other accesories too [not confirmed])
* Support for other peripherals that connect to controller ports (eg: N64 mouse) [not confirmed]
* Low latency

## Project homepage

* English: [Direct N64 controller access plugin mupen64plus](http://raphnet.net/programmation/mupen64plus-input-raphnetraw/index_en.php)
* French: [Plugin d'accès direct aux manettes N64 pour mupen64plus](http://raphnet.net/programmation/mupen64plus-input-raphnetraw/index.php)

## License

This project is licensed under the terms of the GNU General Public License, version 2.
Source code is available on the project homepage and on GitHub.

## How to use the plugin

Regardless of your operating system, you will need to install the plugin file(s)
in a directory mupen64plus will search for plugins (eg: Along the other plugin files).
Then, you must tell mupen64plus to use it. eg:

```
mupen64plus --input mupen64plus-input-raphnetraw
```

Where to obtain the plugin files is described below.

### Linux

Please compile it from sources (howto below). This will generate mupen64plus-input-raphnetraw.so
for your architecture.

### Windows (32 and 64 bit)

You may use the releases distributed by raphnet in a zip file. The file contents will look
like this:

```
mupen64plus-input-raphnetraw-windows-0.9/
mupen64plus-input-raphnetraw-windows-0.9/README.md
mupen64plus-input-raphnetraw-windows-0.9/dist_win64/
mupen64plus-input-raphnetraw-windows-0.9/dist_win64/libhidapi-0.dll
mupen64plus-input-raphnetraw-windows-0.9/dist_win64/mupen64plus-input-raphnetraw.dll
mupen64plus-input-raphnetraw-windows-0.9/dist_win32/
mupen64plus-input-raphnetraw-windows-0.9/dist_win32/libhidapi-0.dll
mupen64plus-input-raphnetraw-windows-0.9/dist_win32/mupen64plus-input-raphnetraw.dll
```

If you use a 64-bit version of mupen64plus, copy the files from the `dist_win64` directory. For
the 32-bit version, copy the files from the `dist_win32` directory.

## Compiling the Linux / Unix version

### Prerequesites

A Linux system such as Debian with the following packages installed: (non-exhaustive list)

* gcc
* make
* libhidapi-hidraw0
* libhidapi-dev

### Environment

#### Directory layout

The input-raphnetraw plugin requires a checkout of mupen64plus-core (for the plugin api headers).

* ...somedir/mupen64plus-input-raphnetraw
* ...somedir/mupen64plus-core

#### Compilation

```sh
$ cd .../somedir/
$ cd mupen64plus-input-raphnetraw/projects/unix
$ make all

# then, assuming all went well:
$ cp mupen64plus-input-raphnetraw.so /your/mupen64plus-plugin-directory

# or, if you passed an appropriate PREFIX to make, you can just:
$ make install
```

## Win32/64 cross-compilation under Linux

### System requirements

A Linux system such as Debian with the following packages installed: (non-exhaustive list)

* mingw-w64
* autotools-dev
* autoconf
* automake
* libtool
* git
* make

### Environment

#### Directory layout

The input-raphnetraw plugin requires a checkout of mupen64plus-core (for the plugin api headers) in the
parent directory. Also, each version (32 or 64 bit) requires a matching hidapi build.

* ...somedir/mupen64plus-input-raphnetraw
* ...somedir/mupen64plus-core
* ...somedir/hidapi
* ...somedir/hidapi64

#### preparing the hidapi build

The buildwin32.sh and buildwin64.sh scripts assume the following:

* For the 32-bit build an hidapi configured and compiled with --host i686-w64-mingw32 is required here: ../../../hidapi
* For the 64-bit build an hidapi configured and compiled with --host x86_64-w64-mingw32 is required here: ../../../hidapi64

hidapi can be obtained like this:

```sh
$ cd ...somedir
$ git clone https://github.com/signal11/hidapi.git
$ git clone hidapi hidapi64
```

And then to compile the win32 version:
```sh
cd ...somedir/hidapi
./bootstrap
./configure --host i686-w64-mingw32
make
```

And the win64 version:
```sh
cd ...somedir/hidapi64
./bootstrap
./configure --host x86_64-w64-mingw32
make
```

#### Compilation

This would cross-compile the 32 and 64-bit windows versions.

```bash
$ cd .../somedir/
$ cd mupen64plus-input-raphnetraw/projects/unix

# 32-bit version
$./buildwin32.sh
$ ./buildwin64.sh
```
