#pragma once

#include "gfx_m64p.h"
#include "gfxstructdefs.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern int32_t vk_rescaling;
    extern bool vk_ssreadbacks;
    extern bool vk_ssdither;

    extern unsigned vk_overscan;
    extern unsigned vk_vertical_stretch;
    extern unsigned vk_downscaling_steps;
    extern bool vk_native_texture_lod;
    extern bool vk_native_tex_rect;
    extern bool vk_divot_filter, vk_gamma_dither;
    extern bool vk_vi_aa, vk_vi_scale, vk_dither_filter;
    extern bool vk_interlacing;
    extern bool vk_synchronous;
    extern bool skip_swap_clear;

    void vk_rasterize();
    void vk_process_commands();
    bool vk_init();
    void vk_destroy();
    void vk_resize();
    void vk_full_sync();
    void vk_read_screen(unsigned char* dest);

#ifdef __cplusplus
}
#endif
