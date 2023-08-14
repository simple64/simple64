#include "vkguts.h"
#include "gfx_simple64.h"
#include <stddef.h>
#include <stdbool.h>


/* definitions of pointers to Core video extension functions */
static ptr_VidExt_Init CoreVideo_Init = NULL;
static ptr_VidExt_Quit CoreVideo_Quit = NULL;
static ptr_VidExt_SetCaption CoreVideo_SetCaption = NULL;
static ptr_VidExt_ToggleFullScreen CoreVideo_ToggleFullScreen = NULL;
static ptr_VidExt_ResizeWindow CoreVideo_ResizeWindow = NULL;
static ptr_VidExt_VK_GetSurface CoreVideo_GetVkSurface = NULL;
static ptr_VidExt_VK_GetInstanceExtensions CoreVideo_GetVkInstExtensions = NULL;
static ptr_VidExt_SetVideoMode CoreVideo_SetVideoMode = NULL;
static ptr_VidExt_GL_SwapBuffers CoreVideo_SwapCounter = NULL;

void screen_init()
{
    /* Get the core Video Extension function pointers from the library handle */
    CoreVideo_Init = (ptr_VidExt_Init)DLSYM(CoreLibHandle, "VidExt_Init");
    CoreVideo_Quit = (ptr_VidExt_Quit)DLSYM(CoreLibHandle, "VidExt_Quit");
    CoreVideo_SetCaption = (ptr_VidExt_SetCaption)DLSYM(CoreLibHandle, "VidExt_SetCaption");
    CoreVideo_ToggleFullScreen = (ptr_VidExt_ToggleFullScreen)DLSYM(CoreLibHandle, "VidExt_ToggleFullScreen");
    CoreVideo_ResizeWindow = (ptr_VidExt_ResizeWindow)DLSYM(CoreLibHandle, "VidExt_ResizeWindow");
    CoreVideo_GetVkSurface = (ptr_VidExt_VK_GetSurface)DLSYM(CoreLibHandle, "VidExt_VK_GetSurface");
    CoreVideo_GetVkInstExtensions = (ptr_VidExt_VK_GetInstanceExtensions)DLSYM(CoreLibHandle, "VidExt_VK_GetInstanceExtensions");
    CoreVideo_SetVideoMode = (ptr_VidExt_SetVideoMode)DLSYM(CoreLibHandle, "VidExt_SetVideoMode");
    CoreVideo_SwapCounter = (ptr_VidExt_GL_SwapBuffers)DLSYM(CoreLibHandle, "VidExt_GL_SwapBuffers");

    CoreVideo_Init();
    CoreVideo_SetCaption("Simple64-Parallel");
}

void screen_set_mode(bool fullscreen)
{
    CoreVideo_SetVideoMode(window_width, window_height, 0, fullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, 0);
}

void screen_close()
{
    CoreVideo_Quit();
}

void screen_toggle_fullscreen()
{
    CoreVideo_ToggleFullScreen();
}

m64p_error screen_get_surface(VkSurfaceKHR* surface, VkInstance instance)
{
    return CoreVideo_GetVkSurface((void**)surface, (void*)instance);
}

m64p_error screen_get_instance_extensions(const char ** ext[], uint32_t* ext_num)
{
    return CoreVideo_GetVkInstExtensions(ext, ext_num);
}

void screen_frame_count()
{
    CoreVideo_SwapCounter();
    ++frame_counter;
}
