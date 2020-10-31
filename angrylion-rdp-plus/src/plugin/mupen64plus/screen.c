#include "gfx_m64p.h"
#include "api/m64p_vidext.h"

#include "core/common.h"
#include "core/msg.h"

#include "output/screen.h"

#include <stdlib.h>

/* definitions of pointers to Core video extension functions */
static ptr_VidExt_Init                  CoreVideo_Init = NULL;
static ptr_VidExt_Quit                  CoreVideo_Quit = NULL;
static ptr_VidExt_ListFullscreenModes   CoreVideo_ListFullscreenModes = NULL;
static ptr_VidExt_SetVideoMode          CoreVideo_SetVideoMode = NULL;
static ptr_VidExt_SetCaption            CoreVideo_SetCaption = NULL;
static ptr_VidExt_ToggleFullScreen      CoreVideo_ToggleFullScreen = NULL;
static ptr_VidExt_ResizeWindow          CoreVideo_ResizeWindow = NULL;
static ptr_VidExt_GL_GetProcAddress     CoreVideo_GL_GetProcAddress = NULL;
static ptr_VidExt_GL_SetAttribute       CoreVideo_GL_SetAttribute = NULL;
static ptr_VidExt_GL_GetAttribute       CoreVideo_GL_GetAttribute = NULL;
static ptr_VidExt_GL_SwapBuffers        CoreVideo_GL_SwapBuffers = NULL;

// framebuffer texture states
int32_t win_width;
int32_t win_height;
int32_t win_fullscreen;

void* IntGetProcAddress(const char *name)
{
    return CoreVideo_GL_GetProcAddress(name);
}

void screen_init(struct n64video_config* config)
{
    UNUSED(config);

    /* Get the core Video Extension function pointers from the library handle */
    CoreVideo_Init = (ptr_VidExt_Init) DLSYM(CoreLibHandle, "VidExt_Init");
    CoreVideo_Quit = (ptr_VidExt_Quit) DLSYM(CoreLibHandle, "VidExt_Quit");
    CoreVideo_ListFullscreenModes = (ptr_VidExt_ListFullscreenModes) DLSYM(CoreLibHandle, "VidExt_ListFullscreenModes");
    CoreVideo_SetVideoMode = (ptr_VidExt_SetVideoMode) DLSYM(CoreLibHandle, "VidExt_SetVideoMode");
    CoreVideo_SetCaption = (ptr_VidExt_SetCaption) DLSYM(CoreLibHandle, "VidExt_SetCaption");
    CoreVideo_ToggleFullScreen = (ptr_VidExt_ToggleFullScreen) DLSYM(CoreLibHandle, "VidExt_ToggleFullScreen");
    CoreVideo_ResizeWindow = (ptr_VidExt_ResizeWindow) DLSYM(CoreLibHandle, "VidExt_ResizeWindow");
    CoreVideo_GL_GetProcAddress = (ptr_VidExt_GL_GetProcAddress) DLSYM(CoreLibHandle, "VidExt_GL_GetProcAddress");
    CoreVideo_GL_SetAttribute = (ptr_VidExt_GL_SetAttribute) DLSYM(CoreLibHandle, "VidExt_GL_SetAttribute");
    CoreVideo_GL_GetAttribute = (ptr_VidExt_GL_GetAttribute) DLSYM(CoreLibHandle, "VidExt_GL_GetAttribute");
    CoreVideo_GL_SwapBuffers = (ptr_VidExt_GL_SwapBuffers) DLSYM(CoreLibHandle, "VidExt_GL_SwapBuffers");

    CoreVideo_Init();

#ifndef GLES
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_PROFILE_MASK, M64P_GL_CONTEXT_PROFILE_CORE);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MAJOR_VERSION, 3);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MINOR_VERSION, 3);
#else
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_PROFILE_MASK, M64P_GL_CONTEXT_PROFILE_ES);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MAJOR_VERSION, 3);
    CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    CoreVideo_SetVideoMode(win_width, win_height, 0, win_fullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, M64VIDEOFLAG_SUPPORT_RESIZING);
}

void screen_adjust(int32_t width_out, int32_t height_out, int32_t* width, int32_t* height, int32_t* x, int32_t* y)
{
    UNUSED(width_out);
    UNUSED(height_out);

    *width = win_width;
    *height = win_height;
    *x = 0;
    *y = 0;
}

void screen_update(void)
{
    (*render_callback)(1);
    CoreVideo_GL_SwapBuffers();
}

void screen_toggle_fullscreen(void)
{
    CoreVideo_ToggleFullScreen();
}

void screen_close(void)
{
    CoreVideo_Quit();
}
