#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <list>
#include <vector>
#include <memory>

#include "gDP.h"
#include "Textures.h"
#include "Graphics/ObjectHandle.h"

struct gDPTile;
struct DepthBuffer;

const u32 fingerprint[4] = { 2, 6, 4, 3 };

namespace graphics {
	class ColorBufferReader;
}

struct FrameBuffer
{
	FrameBuffer();
	~FrameBuffer();
	void init(u32 _address, u16 _format, u16 _size, u16 _width, bool _cfb);
	void updateEndAddress();
	void resolveMultisampledTexture(bool _bForce = false);
	void copyDepthTexture();
	CachedTexture * getTexture(u32 _t);
	CachedTexture * getTextureBG();
	void setBufferClearParams(u32 _fillcolor, s32 _ulx, s32 _uly, s32 _lrx, s32 _lry);
	void copyRdram();
	void setDirty();
	bool isValid(bool _forceCheck) const;
	bool isAuxiliary() const;
	CachedTexture * getColorFbTexture();
	graphics::ObjectHandle getColorFbFbo();
	const u8 * readPixels(s32 _x0, s32 _y0, u32 _width, u32 _height, u32 _size, bool _sync);
	void cleanUp();

	u32 m_startAddress = 0;
	u32 m_endAddress = 0;
	u32 m_size = 0;
	u32 m_width = 0;
	u32 m_height = 0;
	u32 m_originX = 0;
	u32 m_originY = 0;
	u32 m_swapCount = 0;
	float m_scale = 0.0f;
	bool m_copiedToRdram = false;
	bool m_fingerprint = false;
	bool m_cleared = false;
	bool m_changed = false;
	bool m_cfb = false;
	bool m_isDepthBuffer = false;
	bool m_isPauseScreen = false;
	bool m_isOBScreen = false;
	bool m_isMainBuffer = false;
	bool m_readable = false;
	bool m_copied = false;

	struct {
		u32 uls = 0;
		u32 ult = 0;;
	} m_loadTileOrigin;
	u32 m_loadType = LOADTYPE_BLOCK;

	graphics::ObjectHandle m_FBO;
	CachedTexture *m_pTexture = nullptr;

	graphics::ObjectHandle m_depthFBO;
	CachedTexture *m_pDepthTexture = nullptr;

	std::unique_ptr<graphics::ColorBufferReader> m_bufferReader;
	graphics::ObjectHandle m_ColorBufferFBO;
	CachedTexture *m_pColorBufferTexture = nullptr;

	DepthBuffer *m_pDepthBuffer = nullptr;

	// multisampling
	graphics::ObjectHandle m_resolveFBO;
	CachedTexture *m_pResolveTexture = nullptr;
	bool m_resolved = false;

	// subtexture
	graphics::ObjectHandle m_SubFBO;
	CachedTexture *m_pSubTexture = nullptr;

	// copy FBO
	graphics::ObjectHandle m_copyFBO;
	CachedTexture * m_pFrameBufferCopyTexture = nullptr;

	std::vector<u8> m_RdramCopy;

private:
	struct {
		u32 fillcolor = 0;
		s32 ulx = 0;
		s32 uly = 0;
		s32 lrx = 0;
		s32 lry = 0;
	} m_clearParams;

	void _initTexture(u16 _width, u16 _height, u16 _format, u16 _size, CachedTexture *_pTexture);
	void _setAndAttachTexture(graphics::ObjectHandle _fbo, CachedTexture *_pTexture, u32 _t, bool _multisampling);
	bool _initSubTexture(u32 _t);
	void _initCopyTexture();
	CachedTexture * _copyFrameBufferTexture();
	CachedTexture * _getSubTexture(u32 _t);
	void _initColorFBTexture(int _width);
	void _destroyColorFBTexure();

	mutable u32 m_validityChecked = false;
};

class FrameBufferList
{
public:
	void init();
	void destroy();
	void saveBuffer(u32 _address, u16 _format, u16 _size, u16 _width, bool _cfb);
	void removeAux();
	void copyAux();
	void removeBuffer(u32 _address);
	void removeBuffers(u32 _width);
	void attachDepthBuffer();
	void clearDepthBuffer(DepthBuffer * _pDepthBuffer);
	FrameBuffer * findBuffer(u32 _startAddress);
	FrameBuffer * getBuffer(u32 _startAddress);
	FrameBuffer * findTmpBuffer(u32 _address);
	FrameBuffer * getCurrent() const {return m_pCurrent;}
	void setCurrent(FrameBuffer * _pCurrent) { m_pCurrent = _pCurrent; }
	void updateCurrentBufferEndAddress();
	void renderBuffer();
	void setBufferChanged(f32 _maxY);
	void clearBuffersChanged();
	void setCurrentDrawBuffer() const;
	void fillRDRAM(s32 ulx, s32 uly, s32 lrx, s32 lry);

	FrameBuffer * getCopyBuffer() const { return m_pCopy; }
	void setCopyBuffer(FrameBuffer * _pBuffer) { m_pCopy = _pBuffer; }
	void depthBufferCopyRdram();

	void fillBufferInfo(void * _pinfo, u32 _size);

	static FrameBufferList & get();

private:
	FrameBufferList() : m_pCurrent(nullptr), m_pCopy(nullptr), m_prevColorImageHeight(0) {}
	FrameBufferList(const FrameBufferList &) = delete;

	void removeIntersections();

	void _createScreenSizeBuffer();
	void _renderScreenSizeBuffer();

	class OverscanBuffer
	{
	public:
		void init();
		void destroy();

		void setInputBuffer(const FrameBuffer *  _pBuffer);
		void activate();
		void draw(u32 _fullHeight, bool _PAL);

		s32 getHOffset() const;
		s32 getVOffset() const;
		f32 getScaleX() const;
		f32 getScaleY(u32 _fullHeight) const;
		u32 getDrawingWidth() const { return m_drawingWidth; }
		u32 getBufferWidth() const { return m_bufferWidth; }
		u32 getBufferHeight() const { return m_bufferHeight; }

	private:
		s32 m_hOffset = 0;
		s32 m_vOffset = 0;
		f32 m_scale = 1.0f;
		u32 m_drawingWidth = 0U;
		u32 m_bufferWidth = 0U;
		u32 m_bufferHeight = 0U;
		bool m_enabled = false;

		graphics::ObjectHandle m_FBO;
		CachedTexture *m_pTexture = nullptr;
		CachedTexture *m_pDepthTexture = nullptr;
	};

	typedef std::list<FrameBuffer> FrameBuffers;
	FrameBuffers m_list;
	FrameBuffer * m_pCurrent;
	FrameBuffer * m_pCopy;
	u32 m_prevColorImageHeight;
	OverscanBuffer m_overscan;

	struct RdpUpdateResult {
		u32 vi_vres;
		u32 vi_hres;
		u32 vi_v_start;
		u32 vi_h_start;
		u32 vi_x_start;
		u32 vi_y_start;
		u32 vi_x_add;
		u32 vi_y_add;
		u32 vi_width;
		u32 vi_origin;
		u32 vi_minhpass;
		u32 vi_maxhpass;
		bool vi_lowerfield;
		bool vi_fsaa;
		bool vi_divot;
		bool vi_ispal;
	};

	class RdpUpdate
	{
	public:
		void init();
		bool update(RdpUpdateResult & _result);

	private:
		s32 oldvstart = 0;
		u32 prevvicurrent = 0U;
		bool prevwasblank = false;
		bool prevserrate = false;
		bool oldlowerfield = false;
		s32 emucontrolsvicurrent = -1;
	} m_rdpUpdate;
};

inline
FrameBufferList & frameBufferList()
{
	return FrameBufferList::get();
}

u32 cutHeight(u32 _address, u32 _height, u32 _stride);
void calcCoordsScales(const FrameBuffer * _pBuffer, f32 & _scaleX, f32 & _scaleY);

void FrameBuffer_Init();
void FrameBuffer_Destroy();
void FrameBuffer_CopyToRDRAM( u32 _address , bool _sync );
void FrameBuffer_CopyChunkToRDRAM(u32 _address);
void FrameBuffer_CopyFromRDRAM(u32 address, bool bUseAlpha);
void FrameBuffer_AddAddress(u32 address, u32 _size);
bool FrameBuffer_CopyDepthBuffer(u32 address);
bool FrameBuffer_CopyDepthBufferChunk(u32 address);
void FrameBuffer_ActivateBufferTexture(u32 t, u32 _frameBufferAddress);
void FrameBuffer_ActivateBufferTextureBG(u32 t, u32 _frameBufferAddress);

#endif
