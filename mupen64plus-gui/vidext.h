#ifndef VIDEXT_H
#define VIDEXT_H

#include "m64p_types.h"

#ifdef __cplusplus

#include "oglwindow.h"

extern "C" {
#endif
m64p_error qtVidExtFuncInit(void);
m64p_error qtVidExtFuncQuit(void);
m64p_error qtVidExtFuncListModes(m64p_2d_size *SizeArray, int *NumSizes);
m64p_error qtVidExtFuncListRates(m64p_2d_size Size, int *NumRates, int *Rates);
m64p_error qtVidExtFuncSetMode(int Width, int Height, int, int, int);
m64p_error qtVidExtFuncSetModeWithRate(int Width, int Height, int RefreshRate, int BitsPerPixel, m64p_video_mode ScreenMode, m64p_video_flags Flags);
m64p_function     qtVidExtFuncGLGetProc(const char*Proc);
m64p_error qtVidExtFuncGLSetAttr(m64p_GLattr Attr, int Value);
m64p_error qtVidExtFuncGLGetAttr(m64p_GLattr Attr, int *pValue);
m64p_error qtVidExtFuncGLSwapBuf(void);
m64p_error qtVidExtFuncSetCaption(const char *_title);
m64p_error qtVidExtFuncToggleFS(void);
m64p_error qtVidExtFuncResizeWindow(int, int);
uint32_t   qtVidExtFuncGLGetDefaultFramebuffer(void);
#ifdef __cplusplus
}
#endif

#endif // VIDEXT_H
