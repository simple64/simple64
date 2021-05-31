#-------------------------------------------------
#
# Project created by QtCreator 2020-02-07T20:15:01
#
#-------------------------------------------------

QT       += widgets

TARGET = mupen64plus-input-qt
TEMPLATE = lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        configdialog.cpp \
        qt2sdl2.cpp \
        main.cpp

INCLUDEPATH += \
        "../../mupen64plus-core/src/api"

HEADERS += \
        osal/osal_dynamiclib.h \
        configdialog.h \
        qt2sdl2.h \
        main.h

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
