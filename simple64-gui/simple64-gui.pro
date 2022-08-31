#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T19:17:45
#
#-------------------------------------------------

QT       += widgets websockets

TARGET = simple64-gui
TEMPLATE = app

RC_ICONS = simple64.ico

RESOURCES = simple64-gui.qrc

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x0602ff

DEFINES += "NETPLAY_VER=10"

_AUTOUPDATE = $$(SIMPLE64_AUTOUPDATE)
isEmpty(_AUTOUPDATE) {
message("Not enabling auto updater")
}
else {
message("Enabling auto updater")
DEFINES += _AUTOUPDATE
}

INCLUDEPATH += \
        "../mupen64plus-core/src/api"

SOURCES += main.cpp \
    mainwindow.cpp \
    vidext.cpp \
    settingsdialog.cpp \
    interface/common.cpp \
    plugindialog.cpp \
    vkwindow.cpp \
    workerthread.cpp \
    settingclasses.cpp \
    interface/core_commands.cpp \
    interface/sdl_key_converter.c \
    logviewer.cpp \
    keypressfilter.cpp \
    netplay/createroom.cpp \
    netplay/joinroom.cpp \
    netplay/waitroom.cpp

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
QT += dbus

CONFIG += ltcg

SOURCES += osal/osal_dynamiclib_unix.c

LIBS += -L/usr/local/lib -ldl -lSDL2 -lz
INCLUDEPATH += /usr/local/include /usr/include/SDL2 /usr/local/include/SDL2
}

HEADERS  += mainwindow.h \
    vidext.h \
    interface/common.h \
    settingsdialog.h \
    workerthread.h \
    plugindialog.h \
    vkwindow.h \
    settingclasses.h \
    osal/osal_dynamiclib.h \
    interface/sdl_key_converter.h \
    logviewer.h \
    keypressfilter.h \
    netplay/createroom.h \
    netplay/joinroom.h \
    netplay/waitroom.h \
    version.h \
    discord/discord_game_sdk.h

FORMS    += mainwindow.ui

QMAKE_INCDIR += api interface

QMAKE_LFLAGS += -no-pie

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=x86-64-v3
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -march=x86-64-v3
QMAKE_LFLAGS_RELEASE -= -O2
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -march=x86-64-v3

QMAKE_PROJECT_DEPTH = 0
