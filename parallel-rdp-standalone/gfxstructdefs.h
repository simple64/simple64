#ifndef __Z64_H__
#define __Z64_H__

#include <stdio.h>
#include <stdint.h>

#define SP_INTERRUPT          0x1
#define SI_INTERRUPT          0x2
#define AI_INTERRUPT          0x4
#define VI_INTERRUPT          0x8
#define PI_INTERRUPT          0x10
#define DP_INTERRUPT          0x20

#define SP_STATUS_HALT        0x0001
#define SP_STATUS_BROKE       0x0002
#define SP_STATUS_DMABUSY     0x0004
#define SP_STATUS_DMAFULL     0x0008
#define SP_STATUS_IOFULL      0x0010
#define SP_STATUS_SSTEP       0x0020
#define SP_STATUS_INTR_BREAK  0x0040
#define SP_STATUS_SIGNAL0     0x0080
#define SP_STATUS_SIGNAL1     0x0100
#define SP_STATUS_SIGNAL2     0x0200
#define SP_STATUS_SIGNAL3     0x0400
#define SP_STATUS_SIGNAL4     0x0800
#define SP_STATUS_SIGNAL5     0x1000
#define SP_STATUS_SIGNAL6     0x2000
#define SP_STATUS_SIGNAL7     0x4000

#define DP_STATUS_XBUS_DMA    0x01
#define DP_STATUS_FREEZE      0x02
#define DP_STATUS_FLUSH       0x04
#define DP_STATUS_START_GCLK  0x008
#define DP_STATUS_TMEM_BUSY   0x010
#define DP_STATUS_PIPE_BUSY   0x020
#define DP_STATUS_CMD_BUSY    0x040
#define DP_STATUS_CBUF_READY  0x080
#define DP_STATUS_DMA_BUSY    0x100
#define DP_STATUS_END_VALID   0x200
#define DP_STATUS_START_VALID 0x400

#define GET_GFX_INFO(member) (gfx.member)

#define DRAM        GET_GFX_INFO(RDRAM)
#define SP_DMEM     GET_GFX_INFO(DMEM)
#define SP_IMEM     GET_GFX_INFO(IMEM)

#endif
