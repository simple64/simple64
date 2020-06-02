/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-ui-console - plugin.h                                     *
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

#if !defined(PLUGIN_H)
#define PLUGIN_H

#include <QString>
#include <limits.h>

#include "m64p_types.h"
#include "osal_preproc.h"

extern m64p_error PluginSearchLoad(void);
extern m64p_error PluginUnload(void);

extern QString qtPluginDir;
extern QString qtGfxPlugin;
extern QString qtAudioPlugin;
extern QString qtInputPlugin;
extern QString qtRspPlugin;

typedef struct {
  m64p_plugin_type    type;
  char                name[8];
  m64p_dynlib_handle  handle;
  std::string         filename;
  std::string         libname;
  int                 libversion;
  } plugin_map_node;

extern plugin_map_node g_PluginMap[4];

#endif /* #define PLUGIN_H */


