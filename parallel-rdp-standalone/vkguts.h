#pragma once

#include "m64p_vidext.h"
#include "volk.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void screen_init();
    void screen_close();
    void screen_toggle_fullscreen();
    void screen_set_mode(bool fullscreen);
    VkSurfaceKHR screen_get_surface(VkInstance instance);
    m64p_error screen_get_instance_extensions(const char** ext[], uint32_t* ext_num);

    extern bool window_fullscreen;
    extern bool window_widescreen;
    extern uint32_t window_width;
    extern uint32_t window_height;
    extern bool window_vsync;

#ifdef __cplusplus
}
#endif
