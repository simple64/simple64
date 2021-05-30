#include <assert.h>
#include <algorithm>

#include "ColorBufferToRDRAM.h"
#include "WriteToRDRAM.h"

#include <FrameBuffer.h>
#include <FrameBufferInfo.h>
#include <Config.h>
#include <N64.h>
#include <VI.h>
#include "Log.h"
#include "MemoryStatus.h"

#include <Graphics/Context.h>
#include <Graphics/Parameters.h>
#include <Graphics/ColorBufferReader.h>
#include <DisplayWindow.h>
#include "BlueNoiseTexture.h"

using namespace graphics;

ColorBufferToRDRAM::ColorBufferToRDRAM()
	: m_pCurFrameBuffer(nullptr)
{
}

u32 ColorBufferToRDRAM::m_blueNoiseIdx = 0;

ColorBufferToRDRAM::~ColorBufferToRDRAM()
{
}

void ColorBufferToRDRAM::init()
{
}

void ColorBufferToRDRAM::destroy() {

}

bool ColorBufferToRDRAM::_prepareCopy(u32& _startAddress)
{
	if (VI.width == 0 || frameBufferList().getCurrent() == nullptr)
		return false;

	FrameBuffer * pBuffer = frameBufferList().findBuffer(_startAddress);
	if (pBuffer == nullptr || pBuffer->m_isOBScreen)
		return false;

	DisplayWindow & wnd = dwnd();
	const u32 curFrame = wnd.getBuffersSwapCount();

	_startAddress &= ~0xfff;
	if (_startAddress < pBuffer->m_startAddress)
		_startAddress = pBuffer->m_startAddress;

	const u32 numPixels = pBuffer->m_width * pBuffer->m_height;
	if (numPixels == 0)
		return false;

	const u32 stride = pBuffer->m_width << pBuffer->m_size >> 1;
	const u32 bufferHeight = cutHeight(_startAddress, pBuffer->m_height, stride);
	if (bufferHeight == 0)
		return false;

	CachedTexture* colorBufferTexture = pBuffer->getColorFbTexture();

	m_pCurFrameBuffer = pBuffer;

	if ((config.generalEmulation.hacks & hack_subscreen) != 0 && m_pCurFrameBuffer->m_width == VI.width) {
		copyWhiteToRDRAM(m_pCurFrameBuffer);
		return false;
	}

	ObjectHandle readBuffer;

	if (config.video.multisampling != 0) {
		m_pCurFrameBuffer->resolveMultisampledTexture();
		readBuffer = m_pCurFrameBuffer->m_resolveFBO;
	} else {
		readBuffer = m_pCurFrameBuffer->m_FBO;
	}

	if (!m_pCurFrameBuffer->isAuxiliary()) {
		u32 x0 = 0;
		u32 width;
		if (config.frameBufferEmulation.nativeResFactor == 0 && m_pCurFrameBuffer->m_scale != 1.0f) {
			const u32 screenWidth = wnd.getWidth();
			width = screenWidth;
			if (wnd.isAdjustScreen()) {
				width = static_cast<u32>(screenWidth*wnd.getAdjustScale());
				x0 = (screenWidth - width) / 2;
			}
		} else {
			width = m_pCurFrameBuffer->m_pTexture->width;
		}
		u32 height = (u32)(bufferHeight * m_pCurFrameBuffer->m_scale);

		CachedTexture * pInputTexture = m_pCurFrameBuffer->m_pTexture;
		GraphicsDrawer::BlitOrCopyRectParams blitParams;
		blitParams.srcX0 = x0;
		blitParams.srcY0 = 0;
		blitParams.srcX1 = x0 + width;
		blitParams.srcY1 = height;
		blitParams.srcWidth = pInputTexture->width;
		blitParams.srcHeight = pInputTexture->height;
		blitParams.dstX0 = 0;
		blitParams.dstY0 = 0;
		blitParams.dstX1 = m_pCurFrameBuffer->m_width;
		blitParams.dstY1 = bufferHeight;
		blitParams.dstWidth = colorBufferTexture->width;
		blitParams.dstHeight = colorBufferTexture->height;
		blitParams.filter = m_pCurFrameBuffer->m_scale == 1.0f ? textureParameters::FILTER_NEAREST : textureParameters::FILTER_LINEAR;
		blitParams.tex[0] = pInputTexture;
		blitParams.combiner = CombinerInfo::get().getTexrectDownscaleCopyProgram();
		blitParams.readBuffer = readBuffer;
		blitParams.drawBuffer = pBuffer->getColorFbFbo();
		blitParams.mask = blitMask::COLOR_BUFFER;
		wnd.getDrawer().blitOrCopyTexturedRect(blitParams);

		gfxContext.bindFramebuffer(bufferTarget::READ_FRAMEBUFFER, pBuffer->getColorFbFbo());
	} else {
		gfxContext.bindFramebuffer(bufferTarget::READ_FRAMEBUFFER, readBuffer);
	}

	return true;
}

u8 ColorBufferToRDRAM::_RGBAtoR8(u8 _c, u32 x, u32 y) {
	return _c;
}

u16 ColorBufferToRDRAM::_RGBAtoRGBA16(u32 _c, u32 x, u32 y) {
	// Precalculated 4x4 bayer matrix values for 5Bit
	static const s32 thresholdMapBayer[4][4] = {
		{ -4, 2, -3, 4 },
		{ 0, -2, 2, -1 },
		{ -3, 3, -4, 3 },
		{ 1, -1, 1, -2 }
	};

	// Precalculated 4x4 magic square matrix values for 5Bit
	static const s32 thresholdMapMagicSquare[4][4] = {
		{ -4, 2, 2, -1 },
		{ 3, -2, -3, 1 },
		{ -3, 0, 4, -2 },
		{ 3, -1, -4, 1 }
	};

	union RGBA c;
	c.raw = _c;

	if (config.generalEmulation.enableDitheringPattern == 0 || config.frameBufferEmulation.nativeResFactor != 1) {
		// Apply color dithering
		switch (config.generalEmulation.rdramImageDitheringMode) {
		case Config::BufferDitheringMode::bdmBayer:
		case Config::BufferDitheringMode::bdmMagicSquare:
		{
			s32 threshold = config.generalEmulation.rdramImageDitheringMode == Config::BufferDitheringMode::bdmBayer ?
				thresholdMapBayer[x & 3][y & 3] :
				thresholdMapMagicSquare[x & 3][y & 3];
			c.r = (u8)std::max(std::min((s32)c.r + threshold, 255), 0);
			c.g = (u8)std::max(std::min((s32)c.g + threshold, 255), 0);
			c.b = (u8)std::max(std::min((s32)c.b + threshold, 255), 0);
		}
		break;
		case Config::BufferDitheringMode::bdmBlueNoise:
		{
			const BlueNoiseItem& threshold = blueNoiseTex[m_blueNoiseIdx & 7][x & 63][y & 63];
			c.r = (u8)std::max(std::min((s32)c.r + threshold.r, 255), 0);
			c.g = (u8)std::max(std::min((s32)c.g + threshold.g, 255), 0);
			c.b = (u8)std::max(std::min((s32)c.b + threshold.b, 255), 0);
		}
		break;
		}
	}

	return ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a == 0 ? 0 : 1);
}

u32 ColorBufferToRDRAM::_RGBAtoRGBA32(u32 _c, u32 x, u32 y) {
	RGBA c;
	c.raw = _c;
	return (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a;
}

void ColorBufferToRDRAM::_copy(u32 _startAddress, u32 _endAddress, bool _sync)
{
	const u32 stride = m_pCurFrameBuffer->m_width << m_pCurFrameBuffer->m_size >> 1;
	const u32 max_height = std::min((u32)VI_GetMaxBufferHeight(m_pCurFrameBuffer->m_width), cutHeight(_startAddress, m_pCurFrameBuffer->m_height, stride));

	u32 numPixels = (_endAddress - _startAddress) >> (m_pCurFrameBuffer->m_size - 1);
	if (numPixels / m_pCurFrameBuffer->m_width > max_height) {
		_endAddress = _startAddress + (max_height * stride);
		numPixels = (_endAddress - _startAddress) >> (m_pCurFrameBuffer->m_size - 1);
	}

	const u32 width = m_pCurFrameBuffer->m_width;
	const s32 x0 = 0;
	const s32 y0 = (_startAddress - m_pCurFrameBuffer->m_startAddress) / stride;
	const u32 y1 = (_endAddress - m_pCurFrameBuffer->m_startAddress) / stride;
	const u32 height = std::min(max_height, 1u + y1 - y0);

	const u8* pPixels = m_pCurFrameBuffer->readPixels(x0, y0, width, height, m_pCurFrameBuffer->m_size, _sync);
	frameBufferList().setCurrentDrawBuffer();
	if (pPixels == nullptr)
		return;

	if (m_pCurFrameBuffer->m_size == G_IM_SIZ_32b) {
		u32 *ptr_src = (u32*)pPixels;
		u32 *ptr_dst = (u32*)(RDRAM + _startAddress);
		writeToRdram<u32, u32>(ptr_src, ptr_dst, &ColorBufferToRDRAM::_RGBAtoRGBA32, valueTester<u32, 0>, 0, width, height, numPixels, _startAddress, m_pCurFrameBuffer->m_startAddress, m_pCurFrameBuffer->m_size);
	} else if (m_pCurFrameBuffer->m_size == G_IM_SIZ_16b) {
		u32 *ptr_src = (u32*)pPixels;
		u16 *ptr_dst = (u16*)(RDRAM + _startAddress);
		m_blueNoiseIdx++;
		writeToRdram<u32, u16>(ptr_src, ptr_dst, &ColorBufferToRDRAM::_RGBAtoRGBA16, valueTester<u32, 0>, 1, width, height, numPixels, _startAddress, m_pCurFrameBuffer->m_startAddress, m_pCurFrameBuffer->m_size);
	} else if (m_pCurFrameBuffer->m_size == G_IM_SIZ_8b) {
		u8 *ptr_src = (u8*)pPixels;
		u8 *ptr_dst = RDRAM + _startAddress;
		writeToRdram<u8, u8>(ptr_src, ptr_dst, &ColorBufferToRDRAM::_RGBAtoR8, dummyTester<u8>, 3, width, height, numPixels, _startAddress, m_pCurFrameBuffer->m_startAddress, m_pCurFrameBuffer->m_size);
	}

	m_pCurFrameBuffer->m_copiedToRdram = true;
	m_pCurFrameBuffer->copyRdram();

	m_pCurFrameBuffer->cleanUp();

	gDP.changed |= CHANGED_SCISSOR;
}

void ColorBufferToRDRAM::copyToRDRAM(u32 _address, bool _sync)
{
	if (!isMemoryWritable(RDRAM + _address, gDP.colorImage.width << gDP.colorImage.size >> 1))
		return;
	if (!_prepareCopy(_address))
		return;
	if (config.frameBufferEmulation.copyToRDRAM == Config::CopyToRDRAM::ctDisable)
		return;

	const u32 numBytes = (m_pCurFrameBuffer->m_width*m_pCurFrameBuffer->m_height) << m_pCurFrameBuffer->m_size >> 1;
	_copy(m_pCurFrameBuffer->m_startAddress, m_pCurFrameBuffer->m_startAddress + numBytes, _sync);
}

void ColorBufferToRDRAM::copyChunkToRDRAM(u32 _startAddress)
{
	const u32 endAddress = (_startAddress & ~0xfff) + 0x1000;

	if (!isMemoryWritable(RDRAM + _startAddress, endAddress - _startAddress))
		return;
	if (!_prepareCopy(_startAddress))
		return;
	_copy(_startAddress, endAddress, true);
}


ColorBufferToRDRAM & ColorBufferToRDRAM::get()
{
	static ColorBufferToRDRAM cbCopy;
	return cbCopy;
}

void copyWhiteToRDRAM(FrameBuffer * _pBuffer)
{
	if (_pBuffer->m_size == G_IM_SIZ_32b) {
		u32 *ptr_dst = (u32*)(RDRAM + _pBuffer->m_startAddress);

		for (u32 y = 0; y < VI.height; ++y) {
			for (u32 x = 0; x < VI.width; ++x)
				ptr_dst[x + y*VI.width] = 0xFFFFFFFF;
		}
	} else {
		u16 *ptr_dst = (u16*)(RDRAM + _pBuffer->m_startAddress);

		for (u32 y = 0; y < VI.height; ++y) {
			for (u32 x = 0; x < VI.width; ++x) {
				ptr_dst[(x + y*VI.width) ^ 1] = 0xFFFF;
			}
		}
	}
	_pBuffer->m_copiedToRdram = true;
	_pBuffer->copyRdram();
}
