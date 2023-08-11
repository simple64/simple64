/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - dcache.c                                                *
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

#include <string.h>

#include "device/r4300/r4300_core.h"
#include "device/rdram/rdram.h"
#include "device/memory/memory.h"

void poweron_dcache(struct datacache *lines)
{
    memset(lines, 0, 512 * sizeof(lines[0]));
    for (uint32_t i = 0; i < 512; ++i)
        lines[i].index = i << 4 & UINT32_C(0xFF0);
}

void dcache_writeback(struct r4300_core* r4300, struct datacache *line)
{
    cp0_dcb_interlock(r4300, rdram_calculate_cycles(16));
    line->dirty = 0;
    uint32_t cache_address = (line->tag | line->index) & UINT32_C(0x1ffffffc);
    invalidate_r4300_cached_code(r4300, R4300_KSEG0 + cache_address, 16);
    invalidate_r4300_cached_code(r4300, R4300_KSEG1 + cache_address, 16);
    const struct mem_handler* handler = mem_get_handler(r4300->mem, cache_address);
    mem_write32(handler, cache_address | UINT32_C(0x0), line->words[0], ~UINT32_C(0));
    mem_write32(handler, cache_address | UINT32_C(0x4), line->words[1], ~UINT32_C(0));
    mem_write32(handler, cache_address | UINT32_C(0x8), line->words[2], ~UINT32_C(0));
    mem_write32(handler, cache_address | UINT32_C(0xC), line->words[3], ~UINT32_C(0));
}

static void dcache_fill(struct datacache *line, struct r4300_core* r4300, uint32_t address)
{
    cp0_dcb_interlock(r4300, 7);
    line->valid = 1;
    line->dirty = 0;
    line->tag = address & ~UINT32_C(0xFFF);
    r4300->current_access_size = ACCESS_DCACHE;
    uint32_t cache_address = (line->tag | line->index) & UINT32_C(0x1ffffffc);
    const struct mem_handler* handler = mem_get_handler(r4300->mem, cache_address);
    mem_read32(handler, cache_address | UINT32_C(0x0), &line->words[0]);
    mem_read32(handler, cache_address | UINT32_C(0x4), &line->words[1]);
    mem_read32(handler, cache_address | UINT32_C(0x8), &line->words[2]);
    mem_read32(handler, cache_address | UINT32_C(0xC), &line->words[3]);
}

void dcache_read32(struct r4300_core* r4300, uint32_t address, uint32_t *value)
{
    struct datacache *line = &r4300->dcache[(address >> 4) & UINT32_C(0x1FF)];
    if(!dcache_hit(line, address))
    {
        if(line->valid && line->dirty)
            dcache_writeback(r4300, line);
        dcache_fill(line, r4300, address);
    }
    else
        cp0_dcb_interlock(r4300, 1);
    *value = line->words[(address >> 2) & UINT32_C(3)];
}

void dcache_write32(struct r4300_core* r4300, uint32_t address, uint32_t value, uint32_t mask)
{
    struct datacache *line = &r4300->dcache[(address >> 4) & UINT32_C(0x1FF)];
    if(!dcache_hit(line, address))
    {
        if(line->valid && line->dirty)
            dcache_writeback(r4300, line);
        dcache_fill(line, r4300, address);
    }
    else
        cp0_dcb_interlock(r4300, 1);
    masked_write(&line->words[(address >> 2) & UINT32_C(3)], value, mask);
    line->dirty = 1;
}
