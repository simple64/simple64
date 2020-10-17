#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T19:17:45
#
#-------------------------------------------------

QT       += widgets websockets

TARGET = mupen64plus-gui
TEMPLATE = app

RC_ICONS = mupen64plus.ico
ICON = mupen64plus.icns

DEFINES += "NETPLAY_VER=7"

INCLUDEPATH += \
        "../../mupen64plus-core/src/api"

SOURCES += main.cpp \
    mainwindow.cpp \
    vidext.cpp \
    settingsdialog.cpp \
    interface/common.cpp \
    plugindialog.cpp \
    oglwindow.cpp \
    workerthread.cpp \
    settingclasses.cpp \
    cheatdialog.cpp \
    interface/core_commands.cpp \
    interface/cheat.cpp \
    interface/sdl_key_converter.c \
    logviewer.cpp \
    keypressfilter.cpp \
    netplay/createroom.cpp \
    netplay/joinroom.cpp \
    netplay/waitroom.cpp

win32 {
SOURCES += osal/osal_dynamiclib_win32.c \
    osal/osal_files_win32.c

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
        DEFINES -= UNICODE
        LIBS += -Wl,-Bdynamic -lSDL2
        INCLUDEPATH += /mingw64/include/SDL2 /mingw32/include/SDL2
    }
}

!win32 {
QT += dbus

SOURCES += osal/osal_dynamiclib_unix.c \
    osal/osal_files_unix.c

LIBS += -L/usr/local/lib -ldl -lSDL2 -lz
INCLUDEPATH += /usr/local/include /usr/include/SDL2 /usr/local/include/SDL2
}

HEADERS  += mainwindow.h \
    vidext.h \
    interface/common.h \
    settingsdialog.h \
    workerthread.h \
    plugindialog.h \
    oglwindow.h \
    settingclasses.h \
    osal/osal_dynamiclib.h \
    cheatdialog.h \
    interface/cheat.h \
    interface/sdl_key_converter.h \
    logviewer.h \
    keypressfilter.h \
    netplay/createroom.h \
    netplay/joinroom.h \
    netplay/waitroom.h \
    version.h

FORMS    += mainwindow.ui

QMAKE_INCDIR += api interface

CONFIG += optimize_full

QMAKE_PROJECT_DEPTH = 0
