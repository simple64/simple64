/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-ui-console - common.c                                     *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2007-2010 Richard42                                     *
 *   Copyright (C) 2008 Ebenblues Nmn Okaygo Tillin9                       *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __MINGW32__
#define _CRT_RAND_S
#endif

#include "common.h"
#include <SDL_keycode.h>
#include <QProcess>
#include <QFile>
#include "version.h"
#include "../mainwindow.h"
#include "../logviewer.h"
#include "core_commands.h"

/*********************************************************************************************************
 *  Callback functions from the core
 */

void DebugMessage(int level, const char *message, ...)
{
  char msgbuf[1024];
  va_list args;

  va_start(args, message);
  vsnprintf(msgbuf, 1024, message, args);

  DebugCallback((char*)"GUI", level, msgbuf);

  va_end(args);
}

void DebugCallback(void *Context, int level, const char *message)
{
    QString output;

    if (level == M64MSG_ERROR)
    {
        output = QString("%1 Error: %2").arg((const char *) Context, message);
        QString netplay = QString::fromUtf8(message);
        if (netplay.contains("Netplay"))
            w->getWorkerThread()->showMessage(netplay);
    }
    else if (level == M64MSG_WARNING)
        output = QString("%1 Warning: %2").arg((const char *) Context, message);
    else if (level == M64MSG_INFO)
            output = QString("%1: %2").arg((const char *) Context, message);
    else if (level == M64MSG_STATUS)
            output = QString("%1 Status: %2").arg((const char *) Context, message);
    else if (level == M64MSG_VERBOSE)
    {
        if (w->getVerbose())
            output = QString("%1: %2").arg((const char *) Context, message);
    }
    else
        output = QString("%1 Unknown: %2").arg((const char *) Context, message);
    if (w != nullptr && !output.isEmpty())
    {
        if (QThread::currentThread() == w->getWorkerThread())
            w->getWorkerThread()->addLog(output);
        else
            w->getLogViewer()->addLog(output);
    }
}

static char* media_loader_get_gb_cart_rom(void*, int control_id)
{
    QString pathname;
    if (control_id == 0)
        pathname = w->getSettings()->value("Player1GBROM").toString();
    else if (control_id == 1)
        pathname = w->getSettings()->value("Player2GBROM").toString();
    else if (control_id == 2)
        pathname = w->getSettings()->value("Player3GBROM").toString();
    else if (control_id == 3)
        pathname = w->getSettings()->value("Player4GBROM").toString();

    if (pathname.isEmpty())
        return NULL;
    else {
        char *path = strdup(pathname.toUtf8().constData());
        return path;
    }
}

static char* media_loader_get_gb_cart_ram(void*, int control_id)
{
    QString pathname;
    if (control_id == 0)
        pathname = w->getSettings()->value("Player1GBRAM").toString();
    else if (control_id == 1)
        pathname = w->getSettings()->value("Player2GBRAM").toString();
    else if (control_id == 2)
        pathname = w->getSettings()->value("Player3GBRAM").toString();
    else if (control_id == 3)
        pathname = w->getSettings()->value("Player4GBRAM").toString();

    if (pathname.isEmpty())
        return NULL;
    else {
        char *path = strdup(pathname.toUtf8().constData());
        return path;
    }
}

/*
static char* media_loader_get_dd_rom(void*)
{
    QString pathname = w->getSettings()->value("DD_ROM").toString();

    if (pathname.isEmpty())
        return NULL;
    else {
        char *path = strdup(pathname.toUtf8().constData());
        return path;
    }
}

static char* media_loader_get_dd_disk(void*)
{
    QString pathname = w->getSettings()->value("DD_DISK").toString();

    if (pathname.isEmpty())
        return NULL;
    else {
        char *path = strdup(pathname.toUtf8().constData());
        return path;
    }

}
*/

static m64p_media_loader media_loader =
{
    NULL,
    media_loader_get_gb_cart_rom,
    media_loader_get_gb_cart_ram,
    NULL,
    NULL,
    NULL
};

m64p_error loadROM(QString filename)
{
    char *ROM_buffer = NULL;
    size_t romlength = 0;

    if (filename.endsWith(".7z") || filename.endsWith(".zip") || filename.endsWith(".ZIP"))
    {
        QProcess process;
        process.setProgram("7za");
        QStringList arguments = { "e", "-so", filename, "*64"  };
        process.setArguments(arguments);
        process.start();
        process.waitForFinished(-1);
        QByteArray data = process.readAllStandardOutput();
        romlength = data.size();
        if (romlength == 0)
        {
            DebugMessage(M64MSG_ERROR, "couldn't open file '%s' for reading.", filename.toUtf8().constData());
            return M64ERR_INVALID_STATE;
        }
        ROM_buffer = (char *) malloc(romlength);
        memcpy(ROM_buffer, data.constData(), romlength);
    }
    else
    {
        /* load ROM image */
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            DebugMessage(M64MSG_ERROR, "couldn't open ROM file '%s' for reading.", filename.toUtf8().constData());
            return M64ERR_INVALID_STATE;
        }

        romlength = file.size();
        QDataStream in(&file);
        ROM_buffer = (char *) malloc(romlength);
        if (in.readRawData(ROM_buffer, romlength) == -1)
        {
            DebugMessage(M64MSG_ERROR, "couldn't read %li bytes from ROM image file '%s'.", romlength, filename.toUtf8().constData());
            free(ROM_buffer);
            file.close();
            return M64ERR_INVALID_STATE;
        }
        file.close();
    }

    /* Try to load the ROM image into the core */
    if ((*CoreDoCommand)(M64CMD_ROM_OPEN, (int) romlength, ROM_buffer) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_ERROR, "core failed to open ROM image file '%s'.", filename.toUtf8().constData());
        free(ROM_buffer);
        return M64ERR_INVALID_STATE;
    }
    free(ROM_buffer); /* the core copies the ROM image, so we can release this buffer immediately */

    return M64ERR_SUCCESS;
}

m64p_error launchGame(QString netplay_ip, int netplay_port, int netplay_player)
{
    /* attach plugins to core */
    (*CoreAttachPlugin)(M64PLUGIN_GFX, w->getGfxPlugin());
    (*CoreAttachPlugin)(M64PLUGIN_AUDIO, w->getAudioPlugin());
    (*CoreAttachPlugin)(M64PLUGIN_INPUT, w->getInputPlugin());
    (*CoreAttachPlugin)(M64PLUGIN_RSP, w->getRspPlugin());

    m64p_rom_header    l_RomHeader;
    /* get the ROM header for the currently loaded ROM image from the core */
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(l_RomHeader), &l_RomHeader) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_WARNING, "couldn't get ROM header information from core library");
        (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
        return M64ERR_INVALID_STATE;
    }

    if ((*CoreDoCommand)(M64CMD_SET_MEDIA_LOADER, sizeof(media_loader), &media_loader) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_WARNING, "Couldn't set media loader, transferpak and GB carts will not work.");
    }

    /* Save the configuration file again, just in case a plugin has altered it.
       This is the last opportunity to save changes before the relatively
       long-running game. */
    (*ConfigSaveFile)();

    if (netplay_port)
    {
        uint32_t version;
        if ((*CoreDoCommand)(M64CMD_NETPLAY_GET_VERSION, 0x010001, &version) == M64ERR_SUCCESS)
        {
            DebugMessage(M64MSG_INFO, "Netplay: using core version %u", version);

            if ((*CoreDoCommand)(M64CMD_NETPLAY_INIT, netplay_port, netplay_ip.toUtf8().data()) == M64ERR_SUCCESS)
                DebugMessage(M64MSG_INFO, "Netplay: init success");

            uint32_t reg_id = 0;
            while (reg_id == 0)
            {
#ifdef __MINGW32__
                rand_s(&reg_id);
#else
                reg_id = rand();
#endif
                reg_id &= ~0x7;
                reg_id |= netplay_player;
            }

            if ((*CoreDoCommand)(M64CMD_NETPLAY_CONTROL_PLAYER, netplay_player, &reg_id) == M64ERR_SUCCESS)
                DebugMessage(M64MSG_INFO, "Netplay: registered for player %d", netplay_player);
        }
    }

    /* run the game */
    (*CoreDoCommand)(M64CMD_EXECUTE, 0, NULL);

    if (netplay_port)
        (*CoreDoCommand)(M64CMD_NETPLAY_CLOSE, 0, NULL);

    (*CoreDetachPlugin)(M64PLUGIN_GFX);
    (*CoreDetachPlugin)(M64PLUGIN_AUDIO);
    (*CoreDetachPlugin)(M64PLUGIN_INPUT);
    (*CoreDetachPlugin)(M64PLUGIN_RSP);

    /* close the ROM image */
    (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);

    return M64ERR_SUCCESS;
}

int QT2SDL2MOD(Qt::KeyboardModifiers modifiers)
{
    int value = 0;
    if (modifiers & Qt::ShiftModifier)
        value |= KMOD_SHIFT;
    if (modifiers & Qt::ControlModifier)
        value |= KMOD_CTRL;
    if (modifiers & Qt::AltModifier)
        value |= KMOD_ALT;
    if (modifiers & Qt::MetaModifier)
        value |= KMOD_GUI;
    return value;
}

int QT2SDL2(int qtKey)
{
    SDL_Scancode returnValue;
    switch (qtKey) {
    case Qt::Key_Escape:
        returnValue = SDL_SCANCODE_ESCAPE;
        break;
    case Qt::Key_Tab:
        returnValue = SDL_SCANCODE_TAB;
        break;
    case Qt::Key_Backspace:
        returnValue = SDL_SCANCODE_BACKSPACE;
        break;
    case Qt::Key_Return:
        returnValue = SDL_SCANCODE_RETURN;
        break;
    case Qt::Key_Enter:
        returnValue = SDL_SCANCODE_KP_ENTER;
        break;
    case Qt::Key_Insert:
        returnValue = SDL_SCANCODE_INSERT;
        break;
    case Qt::Key_Delete:
        returnValue = SDL_SCANCODE_DELETE;
        break;
    case Qt::Key_Pause:
        returnValue = SDL_SCANCODE_PAUSE;
        break;
    case Qt::Key_Print:
        returnValue = SDL_SCANCODE_PRINTSCREEN;
        break;
    case Qt::Key_SysReq:
        returnValue = SDL_SCANCODE_SYSREQ;
        break;
    case Qt::Key_Clear:
        returnValue = SDL_SCANCODE_CLEAR;
        break;
    case Qt::Key_Home:
        returnValue = SDL_SCANCODE_HOME;
        break;
    case Qt::Key_End:
        returnValue = SDL_SCANCODE_END;
        break;
    case Qt::Key_Left:
        returnValue = SDL_SCANCODE_LEFT;
        break;
    case Qt::Key_Right:
        returnValue = SDL_SCANCODE_RIGHT;
        break;
    case Qt::Key_Up:
        returnValue = SDL_SCANCODE_UP;
        break;
    case Qt::Key_Down:
        returnValue = SDL_SCANCODE_DOWN;
        break;
    case Qt::Key_PageUp:
        returnValue = SDL_SCANCODE_PAGEUP;
        break;
    case Qt::Key_PageDown:
        returnValue = SDL_SCANCODE_PAGEDOWN;
        break;
    case Qt::Key_Shift:
        returnValue = SDL_SCANCODE_LSHIFT;
        break;
    case Qt::Key_Control:
        returnValue = SDL_SCANCODE_LCTRL;
        break;
    case Qt::Key_Meta:
        returnValue = SDL_SCANCODE_LGUI;
        break;
    case Qt::Key_Alt:
        returnValue = SDL_SCANCODE_LALT;
        break;
    case Qt::Key_AltGr:
        returnValue = SDL_SCANCODE_RALT;
        break;
    case Qt::Key_CapsLock:
        returnValue = SDL_SCANCODE_CAPSLOCK;
        break;
    case Qt::Key_NumLock:
        returnValue = SDL_SCANCODE_NUMLOCKCLEAR;
        break;
    case Qt::Key_ScrollLock:
        returnValue = SDL_SCANCODE_SCROLLLOCK;
        break;
    case Qt::Key_F1:
        returnValue = SDL_SCANCODE_F1;
        break;
    case Qt::Key_F2:
        returnValue = SDL_SCANCODE_F2;
        break;
    case Qt::Key_F3:
        returnValue = SDL_SCANCODE_F3;
        break;
    case Qt::Key_F4:
        returnValue = SDL_SCANCODE_F4;
        break;
    case Qt::Key_F5:
        returnValue = SDL_SCANCODE_F5;
        break;
    case Qt::Key_F6:
        returnValue = SDL_SCANCODE_F6;
        break;
    case Qt::Key_F7:
        returnValue = SDL_SCANCODE_F7;
        break;
    case Qt::Key_F8:
        returnValue = SDL_SCANCODE_F8;
        break;
    case Qt::Key_F9:
        returnValue = SDL_SCANCODE_F9;
        break;
    case Qt::Key_F10:
        returnValue = SDL_SCANCODE_F10;
        break;
    case Qt::Key_F11:
        returnValue = SDL_SCANCODE_F11;
        break;
    case Qt::Key_F12:
        returnValue = SDL_SCANCODE_F12;
        break;
    case Qt::Key_F13:
        returnValue = SDL_SCANCODE_F13;
        break;
    case Qt::Key_F14:
        returnValue = SDL_SCANCODE_F14;
        break;
    case Qt::Key_F15:
        returnValue = SDL_SCANCODE_F15;
        break;
    case Qt::Key_F16:
        returnValue = SDL_SCANCODE_F16;
        break;
    case Qt::Key_F17:
        returnValue = SDL_SCANCODE_F17;
        break;
    case Qt::Key_F18:
        returnValue = SDL_SCANCODE_F18;
        break;
    case Qt::Key_F19:
        returnValue = SDL_SCANCODE_F19;
        break;
    case Qt::Key_F20:
        returnValue = SDL_SCANCODE_F20;
        break;
    case Qt::Key_F21:
        returnValue = SDL_SCANCODE_F21;
        break;
    case Qt::Key_F22:
        returnValue = SDL_SCANCODE_F22;
        break;
    case Qt::Key_F23:
        returnValue = SDL_SCANCODE_F23;
        break;
    case Qt::Key_F24:
        returnValue = SDL_SCANCODE_F24;
        break;
    case Qt::Key_Space:
        returnValue = SDL_SCANCODE_SPACE;
        break;
    case Qt::Key_0:
        returnValue = SDL_SCANCODE_0;
        break;
    case Qt::Key_1:
        returnValue = SDL_SCANCODE_1;
        break;
    case Qt::Key_2:
        returnValue = SDL_SCANCODE_2;
        break;
    case Qt::Key_3:
        returnValue = SDL_SCANCODE_3;
        break;
    case Qt::Key_4:
        returnValue = SDL_SCANCODE_4;
        break;
    case Qt::Key_5:
        returnValue = SDL_SCANCODE_5;
        break;
    case Qt::Key_6:
        returnValue = SDL_SCANCODE_6;
        break;
    case Qt::Key_7:
        returnValue = SDL_SCANCODE_7;
        break;
    case Qt::Key_8:
        returnValue = SDL_SCANCODE_8;
        break;
    case Qt::Key_9:
        returnValue = SDL_SCANCODE_9;
        break;
    case Qt::Key_A:
        returnValue = SDL_SCANCODE_A;
        break;
    case Qt::Key_B:
        returnValue = SDL_SCANCODE_B;
        break;
    case Qt::Key_C:
        returnValue = SDL_SCANCODE_C;
        break;
    case Qt::Key_D:
        returnValue = SDL_SCANCODE_D;
        break;
    case Qt::Key_E:
        returnValue = SDL_SCANCODE_E;
        break;
    case Qt::Key_F:
        returnValue = SDL_SCANCODE_F;
        break;
    case Qt::Key_G:
        returnValue = SDL_SCANCODE_G;
        break;
    case Qt::Key_H:
        returnValue = SDL_SCANCODE_H;
        break;
    case Qt::Key_I:
        returnValue = SDL_SCANCODE_I;
        break;
    case Qt::Key_J:
        returnValue = SDL_SCANCODE_J;
        break;
    case Qt::Key_K:
        returnValue = SDL_SCANCODE_K;
        break;
    case Qt::Key_L:
        returnValue = SDL_SCANCODE_L;
        break;
    case Qt::Key_M:
        returnValue = SDL_SCANCODE_M;
        break;
    case Qt::Key_N:
        returnValue = SDL_SCANCODE_N;
        break;
    case Qt::Key_O:
        returnValue = SDL_SCANCODE_O;
        break;
    case Qt::Key_P:
        returnValue = SDL_SCANCODE_P;
        break;
    case Qt::Key_Q:
        returnValue = SDL_SCANCODE_Q;
        break;
    case Qt::Key_R:
        returnValue = SDL_SCANCODE_R;
        break;
    case Qt::Key_S:
        returnValue = SDL_SCANCODE_S;
        break;
    case Qt::Key_T:
        returnValue = SDL_SCANCODE_T;
        break;
    case Qt::Key_U:
        returnValue = SDL_SCANCODE_U;
        break;
    case Qt::Key_V:
        returnValue = SDL_SCANCODE_V;
        break;
    case Qt::Key_W:
        returnValue = SDL_SCANCODE_W;
        break;
    case Qt::Key_X:
        returnValue = SDL_SCANCODE_X;
        break;
    case Qt::Key_Y:
        returnValue = SDL_SCANCODE_Y;
        break;
    case Qt::Key_Z:
        returnValue = SDL_SCANCODE_Z;
        break;
    case Qt::Key_BracketLeft:
        returnValue = SDL_SCANCODE_LEFTBRACKET;
        break;
    case Qt::Key_BracketRight:
        returnValue = SDL_SCANCODE_RIGHTBRACKET;
        break;
    case Qt::Key_Minus:
        returnValue = SDL_SCANCODE_MINUS;
        break;
    case Qt::Key_Semicolon:
        returnValue = SDL_SCANCODE_SEMICOLON;
        break;
    case Qt::Key_Slash:
        returnValue = SDL_SCANCODE_SLASH;
        break;
    case Qt::Key_Backslash:
        returnValue = SDL_SCANCODE_BACKSLASH;
        break;
    case Qt::Key_Apostrophe:
        returnValue = SDL_SCANCODE_APOSTROPHE;
        break;
    case Qt::Key_Comma:
        returnValue = SDL_SCANCODE_COMMA;
        break;
    case Qt::Key_Period:
        returnValue = SDL_SCANCODE_PERIOD;
        break;
    case Qt::Key_Equal:
        returnValue = SDL_SCANCODE_EQUALS;
        break;
    case Qt::Key_QuoteLeft:
        returnValue = SDL_SCANCODE_GRAVE;
        break;
    default:
        returnValue = SDL_SCANCODE_UNKNOWN;
        break;
    }

    return returnValue;
}

int SDL22QT(int scanCode)
{
    int returnValue;
    switch (scanCode) {
    case SDL_SCANCODE_ESCAPE:
        returnValue = Qt::Key_Escape;
        break;
    case SDL_SCANCODE_TAB:
        returnValue = Qt::Key_Tab;
        break;
    case SDL_SCANCODE_BACKSPACE:
        returnValue = Qt::Key_Backspace;
        break;
    case SDL_SCANCODE_RETURN:
        returnValue = Qt::Key_Return;
        break;
    case SDL_SCANCODE_KP_ENTER:
        returnValue = Qt::Key_Enter;
        break;
    case SDL_SCANCODE_INSERT:
        returnValue = Qt::Key_Insert;
        break;
    case SDL_SCANCODE_DELETE:
        returnValue = Qt::Key_Delete;
        break;
    case SDL_SCANCODE_PAUSE:
        returnValue = Qt::Key_Pause;
        break;
    case SDL_SCANCODE_PRINTSCREEN:
        returnValue = Qt::Key_Print;
        break;
    case SDL_SCANCODE_SYSREQ:
        returnValue = Qt::Key_SysReq;
        break;
    case SDL_SCANCODE_CLEAR:
        returnValue = Qt::Key_Clear;
        break;
    case SDL_SCANCODE_HOME:
        returnValue = Qt::Key_Home;
        break;
    case SDL_SCANCODE_END:
        returnValue = Qt::Key_End;
        break;
    case SDL_SCANCODE_LEFT:
        returnValue = Qt::Key_Left;
        break;
    case SDL_SCANCODE_RIGHT:
        returnValue = Qt::Key_Right;
        break;
    case SDL_SCANCODE_UP:
        returnValue = Qt::Key_Up;
        break;
    case SDL_SCANCODE_DOWN:
        returnValue = Qt::Key_Down;
        break;
    case SDL_SCANCODE_PAGEUP:
        returnValue = Qt::Key_PageUp;
        break;
    case SDL_SCANCODE_PAGEDOWN:
        returnValue = Qt::Key_PageDown;
        break;
    case SDL_SCANCODE_LSHIFT:
        returnValue = Qt::Key_Shift;
        break;
    case SDL_SCANCODE_LCTRL:
        returnValue = Qt::Key_Control;
        break;
    case SDL_SCANCODE_LGUI:
        returnValue = Qt::Key_Meta;
        break;
    case SDL_SCANCODE_LALT:
        returnValue = Qt::Key_Alt;
        break;
    case SDL_SCANCODE_RALT:
        returnValue = Qt::Key_AltGr;
        break;
    case SDL_SCANCODE_CAPSLOCK:
        returnValue = Qt::Key_CapsLock;
        break;
    case SDL_SCANCODE_NUMLOCKCLEAR:
        returnValue = Qt::Key_NumLock;
        break;
    case SDL_SCANCODE_SCROLLLOCK:
        returnValue = Qt::Key_ScrollLock;
        break;
    case SDL_SCANCODE_F1:
        returnValue = Qt::Key_F1;
        break;
    case SDL_SCANCODE_F2:
        returnValue = Qt::Key_F2;
        break;
    case SDL_SCANCODE_F3:
        returnValue = Qt::Key_F3;
        break;
    case SDL_SCANCODE_F4:
        returnValue = Qt::Key_F4;
        break;
    case SDL_SCANCODE_F5:
        returnValue = Qt::Key_F5;
        break;
    case SDL_SCANCODE_F6:
        returnValue = Qt::Key_F6;
        break;
    case SDL_SCANCODE_F7:
        returnValue = Qt::Key_F7;
        break;
    case SDL_SCANCODE_F8:
        returnValue = Qt::Key_F8;
        break;
    case SDL_SCANCODE_F9:
        returnValue = Qt::Key_F9;
        break;
    case SDL_SCANCODE_F10:
        returnValue = Qt::Key_F10;
        break;
    case SDL_SCANCODE_F11:
        returnValue = Qt::Key_F11;
        break;
    case SDL_SCANCODE_F12:
        returnValue = Qt::Key_F12;
        break;
    case SDL_SCANCODE_F13:
        returnValue = Qt::Key_F13;
        break;
    case SDL_SCANCODE_F14:
        returnValue = Qt::Key_F14;
        break;
    case SDL_SCANCODE_F15:
        returnValue = Qt::Key_F15;
        break;
    case SDL_SCANCODE_F16:
        returnValue = Qt::Key_F16;
        break;
    case SDL_SCANCODE_F17:
        returnValue = Qt::Key_F17;
        break;
    case SDL_SCANCODE_F18:
        returnValue = Qt::Key_F18;
        break;
    case SDL_SCANCODE_F19:
        returnValue = Qt::Key_F19;
        break;
    case SDL_SCANCODE_F20:
        returnValue = Qt::Key_F20;
        break;
    case SDL_SCANCODE_F21:
        returnValue = Qt::Key_F21;
        break;
    case SDL_SCANCODE_F22:
        returnValue = Qt::Key_F22;
        break;
    case SDL_SCANCODE_F23:
        returnValue = Qt::Key_F23;
        break;
    case SDL_SCANCODE_F24:
        returnValue = Qt::Key_F24;
        break;
    case SDL_SCANCODE_SPACE:
        returnValue = Qt::Key_Space;
        break;
    case SDL_SCANCODE_0:
        returnValue = Qt::Key_0;
        break;
    case SDL_SCANCODE_1:
        returnValue = Qt::Key_1;
        break;
    case SDL_SCANCODE_2:
        returnValue = Qt::Key_2;
        break;
    case SDL_SCANCODE_3:
        returnValue = Qt::Key_3;
        break;
    case SDL_SCANCODE_4:
        returnValue = Qt::Key_4;
        break;
    case SDL_SCANCODE_5:
        returnValue = Qt::Key_5;
        break;
    case SDL_SCANCODE_6:
        returnValue = Qt::Key_6;
        break;
    case SDL_SCANCODE_7:
        returnValue = Qt::Key_7;
        break;
    case SDL_SCANCODE_8:
        returnValue = Qt::Key_8;
        break;
    case SDL_SCANCODE_9:
        returnValue = Qt::Key_9;
        break;
    case SDL_SCANCODE_A:
        returnValue = Qt::Key_A;
        break;
    case SDL_SCANCODE_B:
        returnValue = Qt::Key_B;
        break;
    case SDL_SCANCODE_C:
        returnValue = Qt::Key_C;
        break;
    case SDL_SCANCODE_D:
        returnValue = Qt::Key_D;
        break;
    case SDL_SCANCODE_E:
        returnValue = Qt::Key_E;
        break;
    case SDL_SCANCODE_F:
        returnValue = Qt::Key_F;
        break;
    case SDL_SCANCODE_G:
        returnValue = Qt::Key_G;
        break;
    case SDL_SCANCODE_H:
        returnValue = Qt::Key_H;
        break;
    case SDL_SCANCODE_I:
        returnValue = Qt::Key_I;
        break;
    case SDL_SCANCODE_J:
        returnValue = Qt::Key_J;
        break;
    case SDL_SCANCODE_K:
        returnValue = Qt::Key_K;
        break;
    case SDL_SCANCODE_L:
        returnValue = Qt::Key_L;
        break;
    case SDL_SCANCODE_M:
        returnValue = Qt::Key_M;
        break;
    case SDL_SCANCODE_N:
        returnValue = Qt::Key_N;
        break;
    case SDL_SCANCODE_O:
        returnValue = Qt::Key_O;
        break;
    case SDL_SCANCODE_P:
        returnValue = Qt::Key_P;
        break;
    case SDL_SCANCODE_Q:
        returnValue = Qt::Key_Q;
        break;
    case SDL_SCANCODE_R:
        returnValue = Qt::Key_R;
        break;
    case SDL_SCANCODE_S:
        returnValue = Qt::Key_S;
        break;
    case SDL_SCANCODE_T:
        returnValue = Qt::Key_T;
        break;
    case SDL_SCANCODE_U:
        returnValue = Qt::Key_U;
        break;
    case SDL_SCANCODE_V:
        returnValue = Qt::Key_V;
        break;
    case SDL_SCANCODE_W:
        returnValue = Qt::Key_W;
        break;
    case SDL_SCANCODE_X:
        returnValue = Qt::Key_X;
        break;
    case SDL_SCANCODE_Y:
        returnValue = Qt::Key_Y;
        break;
    case SDL_SCANCODE_Z:
        returnValue = Qt::Key_Z;
        break;
    case SDL_SCANCODE_LEFTBRACKET:
        returnValue = Qt::Key_BracketLeft;
        break;
    case SDL_SCANCODE_RIGHTBRACKET:
        returnValue = Qt::Key_BracketRight;
        break;
    case SDL_SCANCODE_MINUS:
        returnValue = Qt::Key_Minus;
        break;
    case SDL_SCANCODE_SEMICOLON:
        returnValue = Qt::Key_Semicolon;
        break;
    case SDL_SCANCODE_SLASH:
        returnValue = Qt::Key_Slash;
        break;
    case SDL_SCANCODE_BACKSLASH:
        returnValue = Qt::Key_Backslash;
        break;
    case SDL_SCANCODE_APOSTROPHE:
        returnValue = Qt::Key_Apostrophe;
        break;
    case SDL_SCANCODE_COMMA:
        returnValue = Qt::Key_Comma;
        break;
    case SDL_SCANCODE_PERIOD:
        returnValue = Qt::Key_Period;
        break;
    case SDL_SCANCODE_EQUALS:
        returnValue = Qt::Key_Equal;
        break;
    case SDL_SCANCODE_GRAVE:
        returnValue = Qt::Key_QuoteLeft;
        break;
    default:
        returnValue = Qt::Key_unknown;
        break;
    }
    return returnValue;
}
