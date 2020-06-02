/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - common.h                                                *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2009 Richard42                                          *
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

#ifndef __COMMON_H__
#define __COMMON_H__

#include "m64p_types.h"

#ifdef __cplusplus
#include <Qt>
#include <string>
m64p_error loadROM(std::string filename);
m64p_error launchGame(QString netplay_ip, int netplay_port, int netplay_player);
int QT2SDL2MOD(Qt::KeyboardModifiers modifiers);
extern "C" {
#endif
void DebugMessage(int level, const char *message, ...);
void DebugCallback(void *Context, int level, const char *message);
int QT2SDL2(int qtKey);
#ifdef __cplusplus
}
#endif

extern int  g_Verbose;

#endif /* __COMMON_H__ */

