#include <stdio.h>
#include <cstdlib>
#include <Graphics/Context.h>
#include <Graphics/OpenGLContext/GLFunctions.h>
#include <Graphics/OpenGLContext/opengl_Utils.h>
#include <Graphics/OpenGLContext/ThreadedOpenGl/opengl_Wrapper.h>
#include <mupenplus/GLideN64_mupenplus.h>
#include <GLideN64.h>
#include <Config.h>
#include <N64.h>
#include <gSP.h>
#include <Log.h>
#include <Revision.h>
#include <FrameBuffer.h>
#include <GLideNUI/GLideNUI.h>
#include <DisplayWindow.h>

#ifdef VC
#include <bcm_host.h>
#endif

using namespace opengl;

class DisplayWindowMupen64plus : public DisplayWindow
{
public:
	DisplayWindowMupen64plus() {}

private:
	void _setAttributes();
	void _getDisplaySize();

	bool _start() override;
	void _stop() override;
	void _restart() override;
	void _swapBuffers() override;
	void _saveScreenshot() override;
	void _saveBufferContent(graphics::ObjectHandle _fbo, CachedTexture *_pTexture) override;
	bool _resizeWindow() override;
	void _changeWindow() override;
	void _readScreen(void **_pDest, long *_pWidth, long *_pHeight) override {}
	void _readScreen2(void * _dest, int * _width, int * _height, int _front) override;
#ifdef M64P_GLIDENUI
	bool _supportsWithRateFunctions = true;
#endif // M64P_GLIDENUI
	graphics::ObjectHandle _getDefaultFramebuffer() override;
};

DisplayWindow & DisplayWindow::get()
{
	static DisplayWindowMupen64plus video;
	return video;
}

void DisplayWindowMupen64plus::_setAttributes()
{
	LOG(LOG_VERBOSE, "_setAttributes");

	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_PROFILE_MASK, M64P_GL_CONTEXT_PROFILE_CORE);
	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MAJOR_VERSION, 3);
	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_CONTEXT_MINOR_VERSION, 3);

	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_DOUBLEBUFFER, 1);
	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_SWAP_CONTROL, config.video.verticalSync);
	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_BUFFER_SIZE, 32);
	FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_DEPTH_SIZE, 16);
	if (config.video.multisampling > 0 && config.frameBufferEmulation.enable == 0) {
		FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLEBUFFERS, 1);
		if (config.video.multisampling <= 2)
			FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 2);
		else if (config.video.multisampling <= 4)
			FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 4);
		else if (config.video.multisampling <= 8)
			FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 8);
		else
			FunctionWrapper::CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 16);
	}
}

bool DisplayWindowMupen64plus::_start()
{
	FunctionWrapper::setThreadedMode(config.video.threadedVideo);
	auto returnValue = FunctionWrapper::CoreVideo_Init();
	if (returnValue != M64ERR_SUCCESS) {
		LOG(LOG_ERROR, "Error in CoreVideo_Init. Error code: %d", returnValue);
		FunctionWrapper::CoreVideo_Quit();
		return false;
	}

	_setAttributes();

	m_bFullscreen = config.video.fullscreen > 0;
	m_screenWidth = config.video.windowedWidth;
	m_screenHeight = config.video.windowedHeight;
	m_screenRefresh = config.video.fullscreenRefresh;

	_getDisplaySize();
	_setBufferSize();

	LOG(LOG_VERBOSE, "Setting video mode %dx%d", m_screenWidth, m_screenHeight);
	const m64p_video_flags flags = M64VIDEOFLAG_SUPPORT_RESIZING;
#ifdef M64P_GLIDENUI
	returnValue = FunctionWrapper::CoreVideo_SetVideoModeWithRate(m_screenWidth, m_screenHeight, m_screenRefresh, 0, m_bFullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, flags);
	if (returnValue != M64ERR_SUCCESS)
	{
		_supportsWithRateFunctions = false;
#endif // M64P_GLIDENUI
		returnValue = FunctionWrapper::CoreVideo_SetVideoMode(m_screenWidth, m_screenHeight, 0, m_bFullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, flags);
#ifdef M64P_GLIDENUI
	}
#endif // M64P_GLIDENUI
	if (returnValue != M64ERR_SUCCESS) {
		LOG(LOG_ERROR, "Error setting videomode %dx%d @ %d. Error code: %d", m_screenWidth, m_screenHeight, m_screenRefresh, returnValue);
		FunctionWrapper::CoreVideo_Quit();
		return false;
	}

	char caption[128];
# ifdef _DEBUG
	sprintf(caption, "%s debug. Revision %s", pluginName, PLUGIN_REVISION);
# else // _DEBUG
	sprintf(caption, "%s. Revision %s", pluginName, PLUGIN_REVISION);
# endif // _DEBUG
	CoreVideo_SetCaption(caption);

	return true;
}

void DisplayWindowMupen64plus::_stop()
{
	FunctionWrapper::CoreVideo_Quit();
}

void DisplayWindowMupen64plus::_restart()
{
#ifdef M64P_GLIDENUI
	if (_supportsWithRateFunctions && m_bFullscreen) {
		m_resizeWidth = config.video.fullscreenWidth;
		m_resizeHeight = config.video.fullscreenHeight;
	} else {
		m_resizeWidth = config.video.windowedWidth;
		m_resizeHeight = config.video.windowedHeight;
	}
#endif // M64P_GLIDENUI
}

void DisplayWindowMupen64plus::_swapBuffers()
{
	// if emulator defined a render callback function, call it before buffer swap
	if (renderCallback != nullptr) {
		gfxContext.resetShaderProgram();
		if (config.frameBufferEmulation.N64DepthCompare == Config::dcDisable) {
			gfxContext.setViewport(0, getHeightOffset(), getScreenWidth(), getScreenHeight());
			gSP.changed |= CHANGED_VIEWPORT;
		}
		gDP.changed |= CHANGED_COMBINE;
		(*renderCallback)((gDP.changed&CHANGED_CPU_FB_WRITE) == 0 ? 1 : 0);
	}

	//Don't let the command queue grow too big buy waiting on no more swap buffers being queued
	FunctionWrapper::WaitForSwapBuffersQueued();

	FunctionWrapper::CoreVideo_GL_SwapBuffers();
}

void DisplayWindowMupen64plus::_saveScreenshot()
{
}

void DisplayWindowMupen64plus::_saveBufferContent(graphics::ObjectHandle /*_fbo*/, CachedTexture* /*_pTexture*/)
{
}

bool DisplayWindowMupen64plus::_resizeWindow()
{
	_setAttributes();

#ifndef M64P_GLIDENUI
	m_bFullscreen = false;
#endif // M64P_GLIDENUI

#ifdef M64P_GLIDENUI
	m64p_error returnValue;

	if (!_supportsWithRateFunctions)
		m_bFullscreen = false;

	if (_supportsWithRateFunctions) {
		m64p_video_flags flags = {};

		m_width = m_screenWidth = m_resizeWidth;
		m_height = m_screenHeight = m_resizeHeight;

		returnValue = FunctionWrapper::CoreVideo_SetVideoModeWithRate(m_screenWidth, m_screenHeight, m_screenRefresh, 0, m_bFullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, flags);
		if (returnValue != M64ERR_SUCCESS) {
			LOG(LOG_ERROR, "Error setting videomode %dx%d @ %d. Error code: %d", m_screenWidth, m_screenHeight, m_screenRefresh, returnValue);
			FunctionWrapper::CoreVideo_Quit();
			return false;
		}

		_setBufferSize();
		opengl::Utils::isGLError(); // reset GL error.
		return true;
	}
#endif // M64P_GLIDENUI

	m_width = m_screenWidth = m_resizeWidth;
	m_height = m_screenHeight = m_resizeHeight;
	switch (CoreVideo_ResizeWindow(m_screenWidth, m_screenHeight)) {
		case M64ERR_INVALID_STATE:
			LOG(LOG_ERROR, "Error setting videomode %dx%d in fullscreen mode", m_screenWidth, m_screenHeight);
			m_width = m_screenWidth = config.video.windowedWidth;
			m_height = m_screenHeight = config.video.windowedHeight;
			break;
		case M64ERR_SUCCESS:
			break;
		default:
			LOG(LOG_ERROR, "Error setting videomode %dx%d", m_screenWidth, m_screenHeight);
			m_width = m_screenWidth = config.video.windowedWidth;
			m_height = m_screenHeight = config.video.windowedHeight;
			FunctionWrapper::CoreVideo_Quit();
			return false;
	}
	_setBufferSize();
	opengl::Utils::isGLError(); // reset GL error.
	return true;
}

void DisplayWindowMupen64plus::_changeWindow()
{
#ifdef M64P_GLIDENUI
	if (_supportsWithRateFunctions) {
		m64p_error returnValue;
		m_bFullscreen = !m_bFullscreen;
		if (m_bFullscreen) {
			m_screenWidth = config.video.fullscreenWidth;
			m_screenHeight = config.video.fullscreenHeight;
			m_screenRefresh = config.video.fullscreenRefresh;
		} else {
			m_screenWidth = config.video.windowedWidth;
			m_screenHeight = config.video.windowedHeight;
		}

		m64p_video_flags flags = {};
		returnValue = FunctionWrapper::CoreVideo_SetVideoModeWithRate(m_screenWidth, m_screenHeight, m_screenRefresh, 0, m_bFullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, flags);

		if (returnValue != M64ERR_SUCCESS) {
			LOG(LOG_ERROR, "Error setting videomode %dx%d @ %d. Error code: %d", m_screenWidth, m_screenHeight, m_screenRefresh, returnValue);
			FunctionWrapper::CoreVideo_Quit();
		}
	} else {
#endif // M64P_GLIDENUI
		CoreVideo_ToggleFullScreen();
#ifdef M64P_GLIDENUI
	}
#endif // M64P_GLIDENUI
}

void DisplayWindowMupen64plus::_getDisplaySize()
{
#ifdef VC
	if( m_bFullscreen ) {
		// Use VC get_display_size function to get the current screen resolution
		u32 fb_width;
		u32 fb_height;
		auto returnValue = graphics_get_display_size(0 /* LCD */, &fb_width, &fb_height);
		if (returnValue < 0)
			LOG(LOG_ERROR, "Failed to get display size. Error code: %d", returnValue);
		else {
			LOG(LOG_VERBOSE, "Display size %dx%d", fb_width, fb_height);
			m_screenWidth = fb_width;
			m_screenHeight = fb_height;
		}
	}
#endif
}

void DisplayWindowMupen64plus::_readScreen2(void * _dest, int * _width, int * _height, int _front)
{
	if (_width == nullptr || _height == nullptr)
		return;

	*_width = m_screenWidth;
	*_height = m_screenHeight;

	if (_dest == nullptr)
		return;

	u8 *pBufferData = (u8*)malloc((*_width)*(*_height) * 4);
	if (pBufferData == nullptr)
		return;
	u8 *pDest = (u8*)_dest;

#if !defined(OS_ANDROID) && !defined(OS_IOS)
	GLint oldMode;
	glGetIntegerv(GL_READ_BUFFER, &oldMode);
	if (_front != 0)
		glReadBuffer(GL_FRONT);
	else
		glReadBuffer(GL_BACK);
	glReadPixels(0, m_heightOffset, m_screenWidth, m_screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pBufferData);
	glReadBuffer(oldMode);
#else
	glReadPixels(0, m_heightOffset, m_screenWidth, m_screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pBufferData);
#endif

	//Convert RGBA to RGB
	for (s32 y = 0; y < *_height; ++y) {
		u8 *ptr = pBufferData + ((*_width) * 4 * y);
		for (s32 x = 0; x < *_width; ++x) {
			pDest[x * 3] = ptr[0]; // red
			pDest[x * 3 + 1] = ptr[1]; // green
			pDest[x * 3 + 2] = ptr[2]; // blue
			ptr += 4;
		}
		pDest += (*_width) * 3;
	}

	free(pBufferData);
}

graphics::ObjectHandle DisplayWindowMupen64plus::_getDefaultFramebuffer()
{
	if (CoreVideo_GL_GetDefaultFramebuffer != nullptr)
		return graphics::ObjectHandle(CoreVideo_GL_GetDefaultFramebuffer());
	return graphics::ObjectHandle::null;
}
