#pragma once

#include "core/n64video.h"

#include <stdint.h>
#include <stdbool.h>

void vdac_init(struct n64video_config* config);
void vdac_read(struct n64video_frame_buffer* fb, bool alpha);
void vdac_write(struct n64video_frame_buffer* fb);
void vdac_sync(bool invaid);
void vdac_close(void);
