#include "CRC.h"

#define CRC32_POLYNOMIAL     0x04C11DB7U

static unsigned int CRCTable[ 256 ];

static
u32 Reflect( u32 ref, u32 ch )
{
	 u32 value = 0;

	 // Swap bit 0 for bit 7
	 // bit 1 for bit 6, etc.
	 for (u32 i = 1; i < (ch + 1); ++i) {
		  if(ref & 1)
			value |= 1 << (ch - i);
		  ref >>= 1;
	 }
	 return value;
}

void CRC_Init()
{
	u32 crc;

	for (u32 i = 0; i < 256; ++i) {
		crc = Reflect( i, 8U ) << 24U;
		for (int j = 0; j < 8; ++j)
			crc = (crc << 1U) ^ (crc & (1U << 31U) ? CRC32_POLYNOMIAL : 0U);

		CRCTable[i] = Reflect( crc, 32U );
	}
}

u64 CRC_Calculate( u64 crc, const void * buffer, u32 count )
{
	u32 crc32 = static_cast<u32>(crc);
	u32 orig = crc32;

	const u8 *p = reinterpret_cast<const u8*>(buffer);
	while (count--)
		crc32 = (crc32 >> 8) ^ CRCTable[(crc32 & 0xFF) ^ *p++];

	return crc32 ^ orig;
}

u32 CRC_Calculate_Strict( u32 crc, const void * buffer, u32 count )
{
	return static_cast<u32>(CRC_Calculate(crc, buffer, count));
}

u64 CRC_CalculatePalette(u64 crc, const void * buffer, u32 count )
{
	u32 crc32 = static_cast<u32>(crc);
	u32 orig = crc32;

	const u8 *p = reinterpret_cast<const u8*>(buffer);
	while (count--) {
		crc32 = (crc32 >> 8) ^ CRCTable[(crc32 & 0xFF) ^ *p++];
		crc32 = (crc32 >> 8) ^ CRCTable[(crc32 & 0xFF) ^ *p++];

		p += 6;
	}

	return crc32 ^ orig;
}
