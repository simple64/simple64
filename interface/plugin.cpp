/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-ui-console - plugin.c                                     *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QCoreApplication>

#include "mainwindow.h"
#include "core_interface.h"
#include "m64p_common.h"
#include "m64p_types.h"
#include "common.h"  /* for the debug callback function */
extern "C" {
#include "osal_dynamiclib.h"
#include "osal_files.h"
#include "osal_preproc.h"
}
#include "plugin.h"
#include "version.h"

/* global variables */
QString qtPluginDir;
QString qtGfxPlugin;
QString qtAudioPlugin;
QString qtInputPlugin;
QString qtRspPlugin;

plugin_map_node g_PluginMap[] = {{M64PLUGIN_GFX,   "Video", NULL, "", "", 0 },
                                 {M64PLUGIN_AUDIO, "Audio", NULL, "", "", 0 },
                                 {M64PLUGIN_INPUT, "Input", NULL, "", "", 0 },
                                 {M64PLUGIN_RSP,   "RSP",   NULL, "", "", 0 } };

/* local functions */
static m64p_error PluginLoadTry(std::string filepath, int MapIndex)
{
    /* try to open a shared library at the given filepath */
    m64p_dynlib_handle handle;
    m64p_error rval = osal_dynlib_open(&handle, filepath.c_str());
    if (rval != M64ERR_SUCCESS)
        return rval;

    /* call the GetVersion function for the plugin and check compatibility */
    ptr_PluginGetVersion PluginGetVersion = (ptr_PluginGetVersion) osal_dynlib_getproc(handle, "PluginGetVersion");
    if (PluginGetVersion == NULL)
    {
        DebugMessage(M64MSG_ERROR, "library '%s' is not a Mupen64Plus library.", filepath.c_str());
        osal_dynlib_close(handle);
        return M64ERR_INCOMPATIBLE;
    }
    m64p_plugin_type PluginType = (m64p_plugin_type) 0;
    int PluginVersion = 0;
    const char *PluginName = NULL;
    (*PluginGetVersion)(&PluginType, &PluginVersion, NULL, &PluginName, NULL);
    if (PluginType != g_PluginMap[MapIndex].type)
    {
        /* the type of this plugin doesn't match with the type that was requested by the caller */
        osal_dynlib_close(handle);
        return M64ERR_INCOMPATIBLE;
    }
    /* the front-end doesn't talk to the plugins, so we don't care about the plugin version or api version */

    /* call the plugin's initialization function and make sure it starts okay */
    ptr_PluginStartup PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(handle, "PluginStartup");
    if (PluginStartup == NULL)
    {
        DebugMessage(M64MSG_ERROR, "library '%s' broken.  No PluginStartup() function found.", filepath.c_str());
        osal_dynlib_close(handle);
        return M64ERR_INCOMPATIBLE;
    }
    rval = (*PluginStartup)(CoreHandle, g_PluginMap[MapIndex].name, DebugCallback);  /* DebugCallback is in main.c */
    if (rval != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_ERROR, "Error: %s plugin library '%s' failed to start.", g_PluginMap[MapIndex].name, filepath.c_str());
        osal_dynlib_close(handle);
        return rval;
    }

    /* plugin loaded successfully, so set the plugin map's members */
    g_PluginMap[MapIndex].handle = handle;
    g_PluginMap[MapIndex].filename = filepath;
    g_PluginMap[MapIndex].libname = PluginName;
    g_PluginMap[MapIndex].libversion = PluginVersion;

    return M64ERR_SUCCESS;
}

/* global functions */
m64p_error PluginSearchLoad()
{
    osal_lib_search *lib_filelist = NULL;
    int i;

    /* start by checking the directory given on the command line */
    if (!qtPluginDir.isEmpty())
        lib_filelist = osal_library_search(qtPluginDir.toLatin1().data());
    if (lib_filelist == NULL)
        lib_filelist = osal_library_search(QCoreApplication::applicationDirPath().toLatin1().data());

    /* try to load one of each type of plugin */
    for (i = 0; i < 4; i++)
    {
        m64p_plugin_type type = g_PluginMap[i].type;
        std::string cmdline_path;
        switch (type)
        {
            case M64PLUGIN_RSP:    cmdline_path = qtRspPlugin.toStdString();   break;
            case M64PLUGIN_GFX:    cmdline_path = qtGfxPlugin.toStdString();   break;
            case M64PLUGIN_AUDIO:  cmdline_path = qtAudioPlugin.toStdString(); break;
            case M64PLUGIN_INPUT:  cmdline_path = qtInputPlugin.toStdString(); break;
            default:               cmdline_path = "";
        }

        /* if full path was given, try loading exactly this file */
        if (strchr(cmdline_path.c_str(), OSAL_DIR_SEPARATOR) != NULL)
        {
            PluginLoadTry(cmdline_path, i);
        }
        else /* otherwise search through the plugin directory to find a match with this name */
        {
            osal_lib_search *curr = lib_filelist;
            while (curr != NULL && g_PluginMap[i].handle == NULL)
            {
                if (curr->filename == cmdline_path)
                    PluginLoadTry(curr->filepath, i);
                curr = curr->next;
            }
        }

        /* print out the particular plugin used */
        if (g_PluginMap[i].handle == NULL)
        {
            QString name = g_PluginMap[i].name;
            if (w->getWorkerThread() != nullptr) {
                if (w->getWorkerThread()->isRunning())
                    w->getWorkerThread()->pluginWarning(name);
            }
        }
        else
        {
            DebugMessage(M64MSG_INFO, "using %s plugin: '%s' v%i.%i.%i", g_PluginMap[i].name,
                   g_PluginMap[i].libname.c_str(), VERSION_PRINTF_SPLIT(g_PluginMap[i].libversion));
            DebugMessage(M64MSG_VERBOSE, "%s plugin library: %s", g_PluginMap[i].name, g_PluginMap[i].filename.c_str());
        }
    }

    /* free up the list of library files in the plugin search directory */
    osal_free_lib_list(lib_filelist);
    return M64ERR_SUCCESS;
}

m64p_error PluginUnload(void)
{
    typedef m64p_error (*ptr_PluginShutdown)(void);
    ptr_PluginShutdown PluginShutdown;
    int i;

    /* shutdown each type of plugin */
    for (i = 0; i < 4; i++)
    {
        if (g_PluginMap[i].handle == NULL)
            continue;
        /* call the destructor function for the plugin and release the library */
        PluginShutdown = (ptr_PluginShutdown) osal_dynlib_getproc(g_PluginMap[i].handle, "PluginShutdown");
        if (PluginShutdown != NULL)
            (*PluginShutdown)();
        osal_dynlib_close(g_PluginMap[i].handle);
        /* clear out the plugin map's members */
        g_PluginMap[i].handle = NULL;
        g_PluginMap[i].filename.clear();
        g_PluginMap[i].libname.clear();
        g_PluginMap[i].libversion = 0;
    }

    return M64ERR_SUCCESS;
}

