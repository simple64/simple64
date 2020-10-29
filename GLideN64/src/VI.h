#ifndef VI_H
#define VI_H
#include "Types.h"

// VI status register
#define VI_STATUS_TYPE_16                0x000002    /* 16 bit 5/5/5/3 */
#define VI_STATUS_TYPE_32                0x000003    /* 32 bit 8/8/8/8 */
#define VI_STATUS_GAMMA_DITHER_ENABLED   0x000004
#define VI_STATUS_GAMMA_ENABLED          0x000008
#define VI_STATUS_DIVOT_ENABLED          0x000010
#define VI_STATUS_SERRATE_ENABLED        0x000040
#define VI_STATUS_AA_MASK                0x000300    /* see AA modes */
#define VI_STATUS_DITHER_FILTER_ENABLED  0x010000

// AA modes
#define AA_MODE_AA_RESAMPLE_ALWAYS_FETCH 0x000000    /* Always fetch lines */
#define AA_MODE_AA_RESAMPLE              0x000100    /* Fetch extra line if needed */
#define AA_MODE_RESAMPLE_ONLY            0x000200
#define AA_MODE_NEITHER                  0x000300    /* Replicate pixels, no interpolation */

struct VIInfo
{
	u32 width, widthPrev, height, real_height;
	f32 rwidth, rheight;
	u32 lastOrigin;
	bool interlaced;
	bool PAL;

	VIInfo() :
		width(0), widthPrev(0), height(0), real_height(0), rwidth(0), rheight(0),
		lastOrigin(-1), interlaced(false), PAL(false)
	{}
};

extern VIInfo VI;

void VI_UpdateSize();
void VI_UpdateScreen();
u16 VI_GetMaxBufferHeight(u16 _width);

#endif

