#ifndef ColorBufferToRDRAM_H
#define ColorBufferToRDRAM_H

#include <memory>
#include <array>
#include <vector>
#include <Graphics/ObjectHandle.h>

namespace graphics {
	class ColorBufferReader;
}

struct CachedTexture;
struct FrameBuffer;

class ColorBufferToRDRAM
{
public:
	void init();
	void destroy();

	void copyToRDRAM(u32 _address, bool _sync);
	void copyChunkToRDRAM(u32 _startAddress);

	static ColorBufferToRDRAM & get();

private:
	ColorBufferToRDRAM();
	ColorBufferToRDRAM(const ColorBufferToRDRAM &) = delete;
	virtual ~ColorBufferToRDRAM();

	union RGBA {
		struct {
			u8 r, g, b, a;
		};
		u32 raw;
	};

	bool _prepareCopy(u32& _startAddress);

	void _copy(u32 _startAddress, u32 _endAddress, bool _sync);

	// Convert pixel from video memory to N64 buffer format.
	static u8 _RGBAtoR8(u8 _c, u32 x, u32 y);
	static u16 _RGBAtoRGBA16(u32 _c, u32 x, u32 y);
	static u32 _RGBAtoRGBA32(u32 _c, u32 x, u32 y);

	FrameBuffer * m_pCurFrameBuffer;

	static u32 m_blueNoiseIdx;
};

void copyWhiteToRDRAM(FrameBuffer * _pBuffer);

#endif // ColorBufferToRDRAM
