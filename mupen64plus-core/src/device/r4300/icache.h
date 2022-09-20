/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - icache.h                                                *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2022 loganmc10                                          *
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

#ifndef M64P_DEVICE_R4300_ICACHE_H
#define M64P_DEVICE_R4300_ICACHE_H

#include <stdint.h>

struct r4300_core;

struct instcache
{
    uint8_t valid;
    uint32_t tag;
    uint16_t index;
    uint32_t words[8];
};

static osal_inline uint8_t icache_hit(struct instcache *line, uint32_t address)
{
    return line->valid && (line->tag & UINT32_C(0x1ffffffc)) == (address & ~UINT32_C(0xFFF));
}

void poweron_icache(struct instcache *lines);
void icache_writeback(struct r4300_core* r4300, struct instcache *line);
void icache_fill(struct instcache *line, struct r4300_core* r4300, uint32_t address);
void icache_step(struct r4300_core* r4300);
uint32_t* icache_fetch(struct r4300_core* r4300, uint32_t address);

#endif
