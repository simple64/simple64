#-------------------------------------------------
#
# Project created by QtCreator 2020-02-07T20:15:01
#
#-------------------------------------------------

QT       += widgets network

greaterThan(QT_MAJOR_VERSION, 5) {
	QT	 += core5compat
}

TARGET = mupen64plus-input-qt
TEMPLATE = lib

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00

SOURCES += \
        configdialog.cpp \
        qt2sdl2.cpp \
        main.cpp

INCLUDEPATH += \
        "../mupen64plus-core/src/api"

HEADERS += \
        osal/osal_dynamiclib.h \
        configdialog.h \
        qt2sdl2.h \
        main.h \
        funcs.h

win32 {
SOURCES += osal/osal_dynamiclib_win32.c

    win32-msvc* {
        !contains(QMAKE_TARGET.arch, x86_64) {
            message("x86 build")
            LIBS += ../mupen64plus-win32-deps/SDL2-2.0.6/lib/x86/SDL2.lib
        } else {
            message("x86_64 build")
            LIBS += ../mupen64plus-win32-deps/SDL2-2.0.6/lib/x64/SDL2.lib
        }
        INCLUDEPATH += ../mupen64plus-win32-deps/SDL2-2.0.6/include
    } else {
        LIBS += -Wl,-Bdynamic -lSDL2
        INCLUDEPATH += /mingw64/include/SDL2 /mingw32/include/SDL2
    }
}

!win32 {
SOURCES += osal/osal_dynamiclib_unix.c

LIBS += -L/usr/local/lib -ldl -lSDL2
INCLUDEPATH += /usr/local/include /usr/include/SDL2 /usr/local/include/SDL2
}

CONFIG += optimize_full

QMAKE_PROJECT_DEPTH = 0
