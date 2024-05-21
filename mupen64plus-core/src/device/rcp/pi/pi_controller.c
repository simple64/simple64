/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - pi_controller.c                                         *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2014 Bobby Smiles                                       *
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

#include "pi_controller.h"

#define M64P_CORE_PROTOTYPES 1
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "api/callbacks.h"
#include "api/m64p_types.h"
#include "device/device.h"
#include "device/dd/dd_controller.h"
#include "device/memory/memory.h"
#include "device/r4300/r4300_core.h"
#include "device/rcp/mi/mi_controller.h"
#include "device/rcp/rdp/rdp_core.h"
#include "device/rcp/ri/ri_controller.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int validate_pi_request(struct pi_controller* pi)
{
    if (pi->regs[PI_STATUS_REG] & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
        pi->regs[PI_STATUS_REG] |= PI_STATUS_ERROR;
        return 0;
    }

    return 1;
}

uint32_t pi_calculate_cycles(struct pi_controller* pi, uint8_t domain, uint32_t length)
{
    if (!domain)
    {
        DebugMessage(M64MSG_WARNING, "Unknown PI DMA domain: 0x%x", pi->regs[PI_CART_ADDR_REG]);
        return length / 8;
    }
    uint32_t cycles = 0;
    uint32_t latency = 0;
    uint32_t pulse_width = 0;
    uint32_t release = 0;
    uint32_t page_size = 0;
    uint32_t pages = 0;

    if (domain == 1)
    {
        latency = pi->regs[PI_BSD_DOM1_LAT_REG] + 1;
        pulse_width = pi->regs[PI_BSD_DOM1_PWD_REG] + 1;
        release = pi->regs[PI_BSD_DOM1_RLS_REG] + 1;
        page_size = pow(2, (pi->regs[PI_BSD_DOM1_PGS_REG] + 2));
    }
    else if (domain == 2)
    {
        latency = pi->regs[PI_BSD_DOM2_LAT_REG] + 1;
        pulse_width = pi->regs[PI_BSD_DOM2_PWD_REG] + 1;
        release = pi->regs[PI_BSD_DOM2_RLS_REG] + 1;
        page_size = pow(2, (pi->regs[PI_BSD_DOM2_PGS_REG] + 2));
    }
    pages = ceil((double)length / page_size);

    cycles += (14 + latency) * pages;
    cycles += (pulse_width + release) * (length / 2);
    cycles += 5 * pages;
    return cycles * 1.5; // Converting RCP clock speed to CPU clock speed
}

static void dma_pi_read(struct pi_controller* pi)
{
    if (!validate_pi_request(pi))
        return;

    uint32_t cart_addr = pi->regs[PI_CART_ADDR_REG] & ~UINT32_C(1);
    uint32_t dram_addr = pi->regs[PI_DRAM_ADDR_REG] & 0xfffffe;
    uint32_t length = (pi->regs[PI_RD_LEN_REG] & UINT32_C(0x00ffffff)) + 1;
    const uint8_t* dram = (uint8_t*)pi->ri->rdram->dram;

    const struct pi_dma_handler* handler = NULL;
    void* opaque = NULL;

    pi->get_pi_dma_handler(pi->cart, pi->dd, cart_addr, &opaque, &handler);

    if (handler == NULL) {
        DebugMessage(M64MSG_WARNING, "Unknown PI DMA read: 0x%" PRIX32 " -> 0x%" PRIX32 " (0x%" PRIX32 ")", dram_addr, cart_addr, length);
        return;
    }

    /* PI seems to treat the first 128 bytes differently, see https://n64brew.dev/wiki/Peripheral_Interface#Unaligned_DMA_transfer */
    if (length >= 0x7f && (length & 1))
        length += 1;
    uint32_t cycles = handler->dma_read(opaque, dram, dram_addr, cart_addr, length);

    /* Mark DMA as busy */
    pi->regs[PI_STATUS_REG] |= PI_STATUS_DMA_BUSY;
    /* Update PI_DRAM_ADDR_REG and PI_CART_ADDR_REG */
    pi->regs[PI_DRAM_ADDR_REG] = (pi->regs[PI_DRAM_ADDR_REG] + length + 7) & ~7;
    pi->regs[PI_CART_ADDR_REG] = (pi->regs[PI_CART_ADDR_REG] + length + 1) & ~1;

    /* schedule end of dma interrupt event */
    add_interrupt_event(&pi->mi->r4300->cp0, PI_INT, cycles / 2);
}

static void dma_pi_write(struct pi_controller* pi)
{
    if (!validate_pi_request(pi))
        return;

    uint32_t cart_addr = pi->regs[PI_CART_ADDR_REG] & ~UINT32_C(1);
    uint32_t dram_addr = pi->regs[PI_DRAM_ADDR_REG] & 0xfffffe;
    uint32_t length = (pi->regs[PI_WR_LEN_REG] & UINT32_C(0x00ffffff)) + 1;
    uint8_t* dram = (uint8_t*)pi->ri->rdram->dram;

    const struct pi_dma_handler* handler = NULL;
    void* opaque = NULL;

    pi->get_pi_dma_handler(pi->cart, pi->dd, cart_addr, &opaque, &handler);

    if (handler == NULL) {
        DebugMessage(M64MSG_WARNING, "Unknown PI DMA write: 0x%" PRIX32 " -> 0x%" PRIX32 " (0x%" PRIX32 ")", cart_addr, dram_addr, length);
        return;
    }

    /* PI seems to treat the first 128 bytes differently, see https://n64brew.dev/wiki/Peripheral_Interface#Unaligned_DMA_transfer */
    if (length >= 0x7f && (length & 1))
        length += 1;
    if (length <= 0x80)
        length -= dram_addr & 0x7;
    uint32_t cycles = handler->dma_write(opaque, dram, dram_addr, cart_addr, length);

    /* Mark DMA as busy */
    pi->regs[PI_STATUS_REG] |= PI_STATUS_DMA_BUSY;
    /* Update PI_DRAM_ADDR_REG and PI_CART_ADDR_REG */
    pi->regs[PI_DRAM_ADDR_REG] = (pi->regs[PI_DRAM_ADDR_REG] + length + 7) & ~7;
    pi->regs[PI_CART_ADDR_REG] = (pi->regs[PI_CART_ADDR_REG] + length + 1) & ~1;

    /* schedule end of dma interrupt event */
    add_interrupt_event(&pi->mi->r4300->cp0, PI_INT, cycles / 2);
}


void init_pi(struct pi_controller* pi,
             pi_dma_handler_getter get_pi_dma_handler,
             struct cart* cart,
             struct dd_controller* dd,
             struct mi_controller* mi,
             struct ri_controller* ri,
             struct rdp_core* dp)
{
    pi->get_pi_dma_handler = get_pi_dma_handler;
    pi->cart = cart;
    pi->dd = dd;
    pi->mi = mi;
    pi->ri = ri;
    pi->dp = dp;
}

void poweron_pi(struct pi_controller* pi)
{
    memset(pi->regs, 0, PI_REGS_COUNT*sizeof(uint32_t));
    pi->regs[PI_BSD_DOM1_LAT_REG] = 0xFF;
    pi->regs[PI_BSD_DOM1_PWD_REG] = 0xFF;
    pi->regs[PI_BSD_DOM1_RLS_REG] = 0x03;
    pi->regs[PI_BSD_DOM2_LAT_REG] = 0xFF;
    pi->regs[PI_BSD_DOM2_PWD_REG] = 0xFF;
    pi->regs[PI_BSD_DOM2_RLS_REG] = 0x03;
}

void read_pi_regs(void* opaque, uint32_t address, uint32_t* value)
{
    struct pi_controller* pi = (struct pi_controller*)opaque;
    uint32_t reg = pi_reg(address);

    *value = pi->regs[reg];

    if (reg == PI_WR_LEN_REG || reg == PI_RD_LEN_REG)
        *value = 0x7F;
    else if (reg == PI_CART_ADDR_REG)
        *value &= 0xFFFFFFFE;
    else if (reg == PI_DRAM_ADDR_REG)
        *value &= 0xFFFFFE;
}

void write_pi_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask)
{
    struct pi_controller* pi = (struct pi_controller*)opaque;
    uint32_t reg = pi_reg(address);

    switch (reg)
    {
    case PI_CART_ADDR_REG:
        if (pi->dd != NULL) {
            masked_write(&pi->regs[PI_CART_ADDR_REG], value, mask);
            return;
        }
        break;

    case PI_RD_LEN_REG:
        masked_write(&pi->regs[PI_RD_LEN_REG], value, mask);
        dma_pi_read(pi);
        return;

    case PI_WR_LEN_REG:
        masked_write(&pi->regs[PI_WR_LEN_REG], value, mask);
        dma_pi_write(pi);
        return;

    case PI_STATUS_REG:
        if (value & mask & PI_STATUS_CLR_INTR)
        {
            pi->regs[reg] &= ~PI_STATUS_INTERRUPT;
            clear_rcp_interrupt(pi->mi, MI_INTR_PI);
        }
        if (value & mask & PI_STATUS_RESET)
            pi->regs[PI_STATUS_REG] = 0;
        return;

    case PI_BSD_DOM1_LAT_REG:
    case PI_BSD_DOM1_PWD_REG:
    case PI_BSD_DOM1_PGS_REG:
    case PI_BSD_DOM1_RLS_REG:
    case PI_BSD_DOM2_LAT_REG:
    case PI_BSD_DOM2_PWD_REG:
    case PI_BSD_DOM2_PGS_REG:
    case PI_BSD_DOM2_RLS_REG:
        masked_write(&pi->regs[reg], value & 0xff, mask);
        return;
    }

    masked_write(&pi->regs[reg], value, mask);
}

void pi_end_of_dma_event(void* opaque)
{
    struct pi_controller* pi = (struct pi_controller*)opaque;
    pi->regs[PI_STATUS_REG] &= ~(PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY);
    pi->regs[PI_STATUS_REG] |= PI_STATUS_INTERRUPT;

    raise_rcp_interrupt(pi->mi, MI_INTR_PI);
}
