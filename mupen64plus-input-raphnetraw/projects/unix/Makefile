#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Mupen64plus-input-raphnetraw - Makefile                               *
# *   Copyright (C) 2016 Raphael Assenat                                    *
# *                                                                         *
# *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# Makefile for raphnetraw Input plugin in Mupen64plus

# detect operating system
UNAME ?= $(shell uname -s)
OS := NONE
ifeq ("$(UNAME)","Linux")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","linux")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifneq ("$(filter GNU hurd,$(UNAME))","")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","Darwin")
  OS = OSX
  SO_EXTENSION = dylib
  SHARED = -bundle
endif
ifeq ("$(UNAME)","FreeBSD")
  OS = FREEBSD
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(UNAME)","OpenBSD")
  OS = FREEBSD
  SO_EXTENSION = so
  SHARED = -shared
endif
ifneq ("$(filter GNU/kFreeBSD kfreebsd,$(UNAME))","")
  OS = LINUX
  SO_EXTENSION = so
  SHARED = -shared
endif
ifeq ("$(patsubst MINGW%,MINGW,$(UNAME))","MINGW")
  OS = MINGW
  SO_EXTENSION = dll
  SHARED = -shared
  PIC = 0
endif
ifeq ("$(OS)","NONE")
  $(error OS type "$(UNAME)" not supported.  Please file bug report at 'http://code.google.com/p/mupen64plus/issues')
endif

# detect system architecture, only if it matters for build flags
HOST_CPU ?= $(shell uname -m)
CPU := OTHER
ifneq ("$(filter x86_64 amd64,$(HOST_CPU))","")
  CPU := X86
  ifeq ("$(BITS)", "32")
    ARCH_DETECTED := 64BITS_32
    PIC ?= 0
  else
    ARCH_DETECTED := 64BITS
    PIC ?= 1
  endif
endif
ifneq ("$(filter pentium i%86,$(HOST_CPU))","")
  CPU := X86
  ARCH_DETECTED := 32BITS
  PIC ?= 0
endif
ifeq ("$(CPU)","OTHER")
  PIC ?= 1
endif

# base CFLAGS, LDLIBS, and LDFLAGS
OPTFLAGS ?= -O3 -flto
WARNFLAGS ?= -Wall
CFLAGS += $(OPTFLAGS) $(WARNFLAGS) -ffast-math -fno-strict-aliasing -fvisibility=hidden -I../../src -D_GNU_SOURCE=1
LDFLAGS += $(SHARED)
LDLIBS += -lm

# Since we are building a shared library, we must compile with -fPIC on some architectures
# On 32-bit x86 systems we do not want to use -fPIC because we don't have to and it has a big performance penalty on this arch
ifeq ($(PIC), 1)
  CFLAGS += -fPIC
else
  CFLAGS += -fno-PIC
endif

# tweak flags for 32-bit build on 64-bit system
ifeq ($(ARCH_DETECTED), 64BITS_32)
  CFLAGS += -m32
  LDFLAGS += -Wl,-m,elf_i386
endif

# set special flags per-system
ifeq ($(OS), FREEBSD)
  ifeq ($(ARCH_DETECTED), 64BITS_32)
    $(error Do not use the BITS=32 option with FreeBSD, use -m32 and -m elf_i386)
  endif
endif
ifeq ($(OS), LINUX)
  LDLIBS += -ldl
endif
ifeq ($(OS), OSX)
  #xcode-select has been around since XCode 3.0, i.e. OS X 10.5
  OSX_SDK_ROOT = $(shell xcode-select -print-path)/Platforms/MacOSX.platform/Developer/SDKs
  OSX_SDK_PATH = $(OSX_SDK_ROOT)/$(shell ls $(OSX_SDK_ROOT) | tail -1)

  ifeq ($(CPU), X86)
    ifeq ($(ARCH_DETECTED), 64BITS)
      CFLAGS += -arch x86_64 -mmacosx-version-min=10.5 -isysroot $(OSX_SDK_PATH)
      LDLIBS += -ldl
    else
      CFLAGS += -mmmx -msse -fomit-frame-pointer -arch i686 -mmacosx-version-min=10.5 -isysroot $(OSX_SDK_PATH)
      LDLIBS += -ldl -read_only_relocs suppress
    endif
  endif
endif

ifeq ($(OS), LINUX)
    HIDAPI_NAME=hidapi-hidraw
else
    HIDAPI_NAME=hidapi
endif

# test for presence of HIDLIB
ifeq ($(origin HID_CFLAGS) $(origin HID_LDLIBS), undefined undefined)
  HIDAPI_CONFIG = $(CROSS_COMPILE)pkg-config $(HIDAPI_NAME)
  ifeq ($(shell which $(HIDAPI_CONFIG) 2>/dev/null),)
    HIDAPI_CONFIG = $(CROSS_COMPILE)pkg-config $(HIDAPI_NAME)
    ifeq ($(shell which $(HIDAPI_CONFIG) 2>/dev/null),)
      $(error No HIDAPI development libraries found!)
    else
      $(warning Using HIDAPI libraries)
    endif
  endif
  HID_CFLAGS  += $(shell $(HIDAPI_CONFIG) --cflags)
  HID_LDLIBS += $(shell $(HIDAPI_CONFIG) --libs)
endif
CFLAGS += $(HID_CFLAGS)
LDLIBS += $(HID_LDLIBS)

# set mupen64plus core API header path
ifneq ("$(APIDIR)","")
  CFLAGS += "-I$(APIDIR)"
else
  TRYDIR = ../../../mupen64plus-core/src/api
  ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
    CFLAGS += -I$(TRYDIR)
  else
    TRYDIR = /usr/local/include/mupen64plus
    ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
      CFLAGS += -I$(TRYDIR)
    else
      TRYDIR = /usr/include/mupen64plus
      ifneq ("$(wildcard $(TRYDIR)/m64p_types.h)","")
        CFLAGS += -I$(TRYDIR)
      else
        $(error Mupen64Plus API header files not found! Use makefile parameter APIDIR to force a location.)
      endif
    endif
  endif
endif

# reduced compile output when running make without V=1
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
    Q_CC = @echo '    CC  '$@;
    Q_LD = @echo '    LD  '$@;
endif
endif

# set base program pointers and flags
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
RM       ?= rm -f
INSTALL  ?= install
MKDIR ?= mkdir -p
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# set special flags for given Makefile parameters
ifeq ($(DEBUG),1)
  CFLAGS += -g
  INSTALL_STRIP_FLAG ?= 
else
  ifneq ($(OS),OSX)
    INSTALL_STRIP_FLAG ?= -s
  endif
endif
ifeq ($(PLUGINDBG), 1)
  CFLAGS += -D_DEBUG
endif
ifeq ($(PORTS_1_AND_4), 1)
  CFLAGS += -DPORTS_1_AND_4
endif

# set installation options
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif
ifeq ($(SHAREDIR),)
  SHAREDIR := $(PREFIX)/share/mupen64plus
endif
ifeq ($(LIBDIR),)
  LIBDIR := $(PREFIX)/lib
endif
ifeq ($(PLUGINDIR),)
  PLUGINDIR := $(LIBDIR)/mupen64plus
endif

SRCDIR = ../../src
OBJDIR = _obj$(POSTFIX)

# list of source files to compile
SOURCE = \
	$(SRCDIR)/plugin_front.c \
	$(SRCDIR)/plugin_back.c \
	$(SRCDIR)/gcn64lib.c \
	$(SRCDIR)/gcn64.c \
	$(SRCDIR)/hexdump.c

ifeq ($(OS),MINGW)
SOURCE += $(SRCDIR)/osal_dynamiclib_win32.c
else
SOURCE += $(SRCDIR)/osal_dynamiclib_unix.c
endif

# generate a list of object files build, make a temporary directory for them
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter %.c, $(SOURCE)))
OBJDIRS = $(dir $(OBJECTS))
$(shell $(MKDIR) $(OBJDIRS))

# build targets
TARGET = mupen64plus-input-raphnetraw$(POSTFIX).$(SO_EXTENSION)

targets:
	@echo "Mupen64Plus-input-raphnetraw makefile. "
	@echo "  Targets:"
	@echo "    all           == Build Mupen64Plus raphnetraw input plugin"
	@echo "    clean         == remove object files"
	@echo "    rebuild       == clean and re-build all"
	@echo "    install       == Install Mupen64Plus raphnetraw input plugin"
	@echo "    uninstall     == Uninstall Mupen64Plus raphnetraw input plugin"
	@echo "  Options:"
	@echo "    BITS=32       == build 32-bit binaries on 64-bit machine"
	@echo "    APIDIR=path   == path to find Mupen64Plus Core headers"
	@echo "    OPTFLAGS=flag == compiler optimization (default: -O3 -flto)"
	@echo "    WARNFLAGS=flag == compiler warning levels (default: -Wall)"
	@echo "    PIC=(1|0)     == Force enable/disable of position independent code"
	@echo "    POSTFIX=name  == String added to the name of the the build (default: '')"
	@echo "  Install Options:"
	@echo "    PREFIX=path   == install/uninstall prefix (default: /usr/local)"
	@echo "    SHAREDIR=path == path to install shared data files (default: PREFIX/share/mupen64plus)"
	@echo "    LIBDIR=path   == library prefix (default: PREFIX/lib)"
	@echo "    PLUGINDIR=path == path to install plugin libraries (default: LIBDIR/mupen64plus)"
	@echo "    DESTDIR=path  == path to prepend to all installation paths (only for packagers)"
	@echo "  Debugging Options:"
	@echo "    DEBUG=1       == add debugging symbols"
	@echo "    PLUGINDBG=1   == print extra debugging information while running"
	@echo "    V=1           == show verbose compiler output"
	@echo "  Build Options, tweaks:"
	@echo "    PORTS_1_AND_4 == N64 port 4 -> Adapter port 2"

all: $(TARGET)

install: $(TARGET)
	$(INSTALL) -d "$(DESTDIR)$(PLUGINDIR)"
	$(INSTALL) -m 0644 $(INSTALL_STRIP_FLAG) $(TARGET) "$(DESTDIR)$(PLUGINDIR)"
	$(INSTALL) -d "$(DESTDIR)$(SHAREDIR)"

uninstall:
	$(RM) "$(DESTDIR)$(PLUGINDIR)/$(TARGET)"
	$(RM) "$(DESTDIR)$(SHAREDIR)/InputAutoCfg.ini"

clean:
	$(RM) -r $(OBJDIR) $(TARGET)

rebuild: clean all

# build dependency files
CFLAGS += -MD -MP
-include $(OBJECTS:.o=.d)

# standard build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: all clean install uninstall targets
