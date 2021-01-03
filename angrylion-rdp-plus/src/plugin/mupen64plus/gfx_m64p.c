/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-video-angrylionplus - plugin.c                            *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2014 Bobby Smiles                                       *
 *   Copyright (C) 2009 Richard Goedeken                                   *
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

#define M64P_PLUGIN_PROTOTYPES 1

#define KEY_FULLSCREEN "Fullscreen"
#define KEY_SCREEN_WIDTH "ScreenWidth"
#define KEY_SCREEN_HEIGHT "ScreenHeight"
#define KEY_PARALLEL "Parallel"
#define KEY_NUM_WORKERS "NumWorkers"

#define KEY_VI_MODE "ViMode"
#define KEY_VI_INTERP "ViInterpolation"
#define KEY_VI_WIDESCREEN "ViWidescreen"
#define KEY_VI_HIDE_OVERSCAN "ViHideOverscan"
#define KEY_VI_INTEGER_SCALING "ViIntegerScaling"

#define KEY_DP_COMPAT "DpCompat"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gfx_m64p.h"

#include "api/m64p_types.h"
#include "api/m64p_config.h"

#include "core/common.h"
#include "core/version.h"
#include "core/msg.h"

#include "output/screen.h"
#include "output/vdac.h"

static ptr_ConfigOpenSection      ConfigOpenSection = NULL;
static ptr_ConfigSaveSection      ConfigSaveSection = NULL;
static ptr_ConfigSetDefaultInt    ConfigSetDefaultInt = NULL;
static ptr_ConfigSetDefaultBool   ConfigSetDefaultBool = NULL;
static ptr_ConfigGetParamInt      ConfigGetParamInt = NULL;
static ptr_ConfigGetParamBool     ConfigGetParamBool = NULL;
static ptr_PluginGetVersion       CoreGetVersion = NULL;

static bool warn_hle;
static bool plugin_initialized;
void (*debug_callback)(void *, int, const char *);
void *debug_call_context;
static struct n64video_config config;

m64p_dynlib_handle CoreLibHandle;
GFX_INFO gfx;
void (*render_callback)(int);

static m64p_handle configVideoGeneral = NULL;
static m64p_handle configVideoAngrylionPlus = NULL;

#define PLUGIN_VERSION              0x000100
#define VIDEO_PLUGIN_API_VERSION    0x020200

extern int32_t win_width;
extern int32_t win_height;
extern int32_t win_fullscreen;

EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle _CoreLibHandle, void *Context,
                                     void (*DebugCallback)(void *, int, const char *))
{
    if (plugin_initialized) {
        return M64ERR_ALREADY_INIT;
    }

    /* first thing is to set the callback function for debug info */
    debug_callback = DebugCallback;
    debug_call_context = Context;

    CoreLibHandle = _CoreLibHandle;

    ConfigOpenSection = (ptr_ConfigOpenSection)DLSYM(CoreLibHandle, "ConfigOpenSection");
    ConfigSaveSection = (ptr_ConfigSaveSection)DLSYM(CoreLibHandle, "ConfigSaveSection");
    ConfigSetDefaultInt = (ptr_ConfigSetDefaultInt)DLSYM(CoreLibHandle, "ConfigSetDefaultInt");
    ConfigSetDefaultBool = (ptr_ConfigSetDefaultBool)DLSYM(CoreLibHandle, "ConfigSetDefaultBool");
    ConfigGetParamInt = (ptr_ConfigGetParamInt)DLSYM(CoreLibHandle, "ConfigGetParamInt");
    ConfigGetParamBool = (ptr_ConfigGetParamBool)DLSYM(CoreLibHandle, "ConfigGetParamBool");

    ConfigOpenSection("Video-General", &configVideoGeneral);
    ConfigOpenSection("Video-Angrylion-Plus", &configVideoAngrylionPlus);

    ConfigSetDefaultBool(configVideoGeneral, KEY_FULLSCREEN, 0, "Use fullscreen mode if True, or windowed mode if False");
    ConfigSetDefaultInt(configVideoGeneral, KEY_SCREEN_WIDTH, 640, "Width of output window or fullscreen width");
    ConfigSetDefaultInt(configVideoGeneral, KEY_SCREEN_HEIGHT, 480, "Height of output window or fullscreen height");

    CoreGetVersion = (ptr_PluginGetVersion)DLSYM(CoreLibHandle, "PluginGetVersion");

    n64video_config_init(&config);

    ConfigSetDefaultBool(configVideoAngrylionPlus, KEY_PARALLEL, config.parallel, "Distribute rendering between multiple processors if True");
    ConfigSetDefaultInt(configVideoAngrylionPlus, KEY_NUM_WORKERS, config.num_workers, "Rendering Workers (0=Use all logical processors)");
    ConfigSetDefaultInt(configVideoAngrylionPlus, KEY_VI_MODE, config.vi.mode, "VI mode (0=Filtered, 1=Unfiltered, 2=Depth, 3=Coverage)");
    ConfigSetDefaultInt(configVideoAngrylionPlus, KEY_VI_INTERP, config.vi.interp, "Scaling interpolation type (0=NN, 1=Linear)");
    ConfigSetDefaultBool(configVideoAngrylionPlus, KEY_VI_WIDESCREEN, config.vi.widescreen, "Use anamorphic 16:9 output mode if True");
    ConfigSetDefaultBool(configVideoAngrylionPlus, KEY_VI_HIDE_OVERSCAN, config.vi.hide_overscan, "Hide overscan area in filteded mode if True");
    ConfigSetDefaultBool(configVideoAngrylionPlus, KEY_VI_INTEGER_SCALING, config.vi.integer_scaling, "Display upscaled pixels as groups of 1x1, 2x2, 3x3, etc. if True");
    ConfigSetDefaultInt(configVideoAngrylionPlus, KEY_DP_COMPAT, config.dp.compat, "Compatibility mode (0=Fast 1=Moderate 2=Slow");

    ConfigSaveSection("Video-General");
    ConfigSaveSection("Video-Angrylion-Plus");

    plugin_initialized = true;
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
    if (!plugin_initialized) {
        return M64ERR_NOT_INIT;
    }

    /* reset some local variable */
    debug_callback = NULL;
    debug_call_context = NULL;

    plugin_initialized = false;
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    /* set version info */
    if (PluginType != NULL) {
        *PluginType = M64PLUGIN_GFX;
    }

    if (PluginVersion != NULL) {
        *PluginVersion = PLUGIN_VERSION;
    }

    if (APIVersion != NULL) {
        *APIVersion = VIDEO_PLUGIN_API_VERSION;
    }

    if (PluginNamePtr != NULL) {
        *PluginNamePtr = CORE_NAME;
    }

    if (Capabilities != NULL) {
        *Capabilities = 0;
    }

    return M64ERR_SUCCESS;
}

EXPORT int CALL InitiateGFX (GFX_INFO Gfx_Info)
{
    gfx = Gfx_Info;

    return 1;
}

EXPORT void CALL MoveScreen (int xpos, int ypos)
{
    UNUSED(xpos);
    UNUSED(ypos);
}

EXPORT void CALL ProcessDList(void)
{
    if (!warn_hle) {
        msg_warning("HLE video emulation not supported, please use a LLE RSP plugin like mupen64plus-rsp-cxd4");
        warn_hle = true;
    }
}

EXPORT void CALL ProcessRDPList(void)
{
    n64video_process_list();
}

EXPORT int CALL RomOpen (void)
{
    win_fullscreen = ConfigGetParamBool(configVideoGeneral, KEY_FULLSCREEN);
    win_width = ConfigGetParamInt(configVideoGeneral, KEY_SCREEN_WIDTH);
    win_height = ConfigGetParamInt(configVideoGeneral, KEY_SCREEN_HEIGHT);

    config.parallel = ConfigGetParamBool(configVideoAngrylionPlus, KEY_PARALLEL);
    config.num_workers = ConfigGetParamInt(configVideoAngrylionPlus, KEY_NUM_WORKERS);
    config.vi.mode = ConfigGetParamInt(configVideoAngrylionPlus, KEY_VI_MODE);
    config.vi.interp = ConfigGetParamInt(configVideoAngrylionPlus, KEY_VI_INTERP);
    config.vi.widescreen = ConfigGetParamBool(configVideoAngrylionPlus, KEY_VI_WIDESCREEN);
    config.vi.hide_overscan = ConfigGetParamBool(configVideoAngrylionPlus, KEY_VI_HIDE_OVERSCAN);
    config.vi.integer_scaling = ConfigGetParamBool(configVideoAngrylionPlus, KEY_VI_INTEGER_SCALING);

    config.dp.compat = ConfigGetParamInt(configVideoAngrylionPlus, KEY_DP_COMPAT);

    config.gfx.rdram = gfx.RDRAM;

    int core_version;
    CoreGetVersion(NULL, &core_version, NULL, NULL, NULL);
    if (core_version >= 0x020501) {
        config.gfx.rdram_size = *gfx.RDRAM_SIZE;
    } else {
        config.gfx.rdram_size = RDRAM_MAX_SIZE;
    }

    config.gfx.dmem = gfx.DMEM;
    config.gfx.mi_intr_reg = (uint32_t*)gfx.MI_INTR_REG;
    config.gfx.mi_intr_cb = gfx.CheckInterrupts;

    config.gfx.vi_reg = (uint32_t**)&gfx.VI_STATUS_REG;
    config.gfx.dp_reg = (uint32_t**)&gfx.DPC_START_REG;

    n64video_init(&config);
    vdac_init(&config);

    return 1;
}

EXPORT void CALL RomClosed (void)
{
    vdac_close();
    n64video_close();
}

EXPORT void CALL ShowCFB (void)
{
}

EXPORT void CALL UpdateScreen (void)
{
    struct n64video_frame_buffer fb;
    n64video_update_screen(&fb);

    if (fb.valid) {
        vdac_write(&fb);
    }

    vdac_sync(fb.valid);
}

EXPORT void CALL ViStatusChanged (void)
{
}

EXPORT void CALL ViWidthChanged (void)
{
}

EXPORT void CALL ChangeWindow(void)
{
    screen_toggle_fullscreen();
}

EXPORT void CALL ReadScreen2(void *dest, int *width, int *height, int front)
{
    UNUSED(front);

    struct n64video_frame_buffer fb = { 0 };
    fb.pixels = dest;
    vdac_read(&fb, false);

    *width = fb.width;
    *height = fb.height;
}

EXPORT void CALL SetRenderingCallback(void (*callback)(int))
{
    render_callback = callback;
}

EXPORT void CALL ResizeVideoOutput(int width, int height)
{
    win_width = width;
    win_height = height;
}

EXPORT void CALL FBWrite(unsigned int addr, unsigned int size)
{
    UNUSED(addr);
    UNUSED(size);
}

EXPORT void CALL FBRead(unsigned int addr)
{
    UNUSED(addr);
}

EXPORT void CALL FBGetFrameBufferInfo(void *pinfo)
{
    UNUSED(pinfo);
}
