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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "common.h"
#include <SDL_keycode.h>
#include <QProcess>
#include "version.h"
#include "mainwindow.h"
#include "logviewer.h"
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
        output = QString("%1 Error: %2\n").arg((const char *) Context, message);
        QString netplay = QString::fromUtf8(message);
        if (netplay.contains("Netplay"))
            w->getWorkerThread()->showMessage(netplay);
    }
    else if (level == M64MSG_WARNING)
        output = QString("%1 Warning: %2\n").arg((const char *) Context, message);
    else if (level == M64MSG_INFO)
            output = QString("%1: %2\n").arg((const char *) Context, message);
    else if (level == M64MSG_STATUS)
            output = QString("%1 Status: %2\n").arg((const char *) Context, message);
    else if (level == M64MSG_VERBOSE)
    {
        if (w->getVerbose())
            output = QString("%1: %2\n").arg((const char *) Context, message);
    }
    else
        output = QString("%1 Unknown: %2\n").arg((const char *) Context, message);
    if (w != nullptr && !output.isEmpty())
        w->getLogViewer()->addLog(output);
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

static m64p_media_loader media_loader =
{
    NULL,
    media_loader_get_gb_cart_rom,
    media_loader_get_gb_cart_ram,
    media_loader_get_dd_rom,
    media_loader_get_dd_disk
};

m64p_error loadROM(QString filename)
{
    char *ROM_buffer = NULL;
    size_t romlength = 0;

    if (filename.endsWith(".7z") || filename.endsWith(".zip") || filename.endsWith(".ZIP"))
    {
        QProcess process;
        QString command = "7za e -so \"";
        command += filename;
        command += "\" *64";
        process.start(command);
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

static void loadPif()
{
    /* Try to load the PIF image into the core */
    if (!w->getSettings()->value("PIF_ROM").toString().isEmpty()) {
        QFile pifFile(w->getSettings()->value("PIF_ROM").toString());
        if (pifFile.open(QIODevice::ReadOnly)) {
            QDataStream inPif(&pifFile);
            char *PIF_buffer = (char *) malloc(2048);
            if (inPif.readRawData(PIF_buffer, 2048) == 2048) {
                if ((*CoreDoCommand)(M64CMD_PIF_OPEN, 2048, PIF_buffer) != M64ERR_SUCCESS)
                {
                    DebugMessage(M64MSG_ERROR, "core failed to open PIF image file.");
                }
            }
            else
            {
                DebugMessage(M64MSG_ERROR, "core failed to open PIF image file.");
            }
            free(PIF_buffer);
            pifFile.close();
        }
    }
}

m64p_error launchGame(QString netplay_ip, int netplay_port, int netplay_player)
{
    if (!netplay_port)
        loadPif();

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
        if ((*CoreDoCommand)(M64CMD_NETPLAY_GET_VERSION, 0x010000, &version) == M64ERR_SUCCESS)
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
                reg_id += netplay_player;
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
