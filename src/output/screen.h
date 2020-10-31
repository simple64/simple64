#pragma once

#include "core/n64video.h"

#include <stdint.h>
#include <stdbool.h>

void screen_init(struct n64video_config* config);
void screen_adjust(int32_t width_out, int32_t height_out, int32_t* width, int32_t* height, int32_t* x, int32_t* y);
void screen_update(void);
void screen_toggle_fullscreen(void);
void screen_close(void);
