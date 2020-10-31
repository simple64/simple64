#include "gfx_1.3.h"

#include "wgl_ext.h"

#include "output/screen.h"
#include "output/gl_proc.h"
#include "core/common.h"
#include "core/msg.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

extern GFX_INFO gfx;

// default size of the window
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 480

// previous size of the window
static int32_t m_win_width;
static int32_t m_win_height;

// context states
static HDC m_dc;
static HGLRC m_glrc;
static HGLRC m_glrc_core;
static bool m_fullscreen;

// config states
static bool m_exclusive;

// Win32 helpers
void win32_client_resize(HWND hWnd, HWND hStatus, int32_t nWidth, int32_t nHeight)
{
    RECT rclient;
    if (!GetClientRect(hWnd, &rclient)) {
        return;
    }

    RECT rwin;
    if (!GetWindowRect(hWnd, &rwin)) {
        return;
    }

    RECT rstatus;
    if (hStatus && GetClientRect(hStatus, &rstatus)) {
        rclient.bottom -= rstatus.bottom;
    }

    POINT pdiff;
    pdiff.x = (rwin.right - rwin.left) - rclient.right;
    pdiff.y = (rwin.bottom - rwin.top) - rclient.bottom;

    MoveWindow(hWnd, rwin.left, rwin.top, nWidth + pdiff.x, nHeight + pdiff.y, TRUE);
}

static int TestPointer(const PROC pTest)
{
    if (!pTest) {
        return 0;
    }

    ptrdiff_t iTest = (ptrdiff_t)pTest;

    return iTest != 1 && iTest != 2 && iTest != 3 && iTest != -1;
}

funcptr IntGetProcAddress(const char* name)
{
    PROC pFunc = wglGetProcAddress((LPCSTR)name);
    if (TestPointer(pFunc)) {
        return (funcptr)pFunc;
    }
    HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
    if (glMod == NULL) {
        return (funcptr)glMod;
    } else {
        return (funcptr)GetProcAddress(glMod, (LPCSTR)name);
    }
}

void screen_init(struct n64video_config* config)
{
    m_exclusive = config->vi.exclusive;

    // reset windowed size state
    m_win_width = 0;
    m_win_height = 0;

    // make window resizable for the user
    if (!m_fullscreen) {
        LONG style = GetWindowLong(gfx.hWnd, GWL_STYLE);

        if ((style & (WS_SIZEBOX | WS_MAXIMIZEBOX)) == 0) {
            style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
            SetWindowLong(gfx.hWnd, GWL_STYLE, style);

            // Fix client size after changing the window style, otherwise the PJ64
            // menu will be displayed incorrectly.
            // For some reason, this needs to be called twice, probably because the
            // style set above isn't applied immediately.
            for (int i = 0; i < 2; i++) {
                win32_client_resize(gfx.hWnd, gfx.hStatusBar, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
            }
        }
    }

    PIXELFORMATDESCRIPTOR win_pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
        PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
        32,            // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        24, // Number of bits for the depthbuffer
        8,  // Number of bits for the stencilbuffer
        0,  // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    m_dc = GetDC(gfx.hWnd);
    if (!m_dc) {
        msg_error("Can't get device context.");
        return;
    }

    int32_t win_pf = ChoosePixelFormat(m_dc, &win_pfd);
    if (!win_pf) {
        msg_error("Can't choose pixel format.");
        return;
    }
    SetPixelFormat(m_dc, win_pf, &win_pfd);

    // create legacy context, required for wglGetProcAddress to work properly
    m_glrc = wglCreateContext(m_dc);
    if (!m_glrc || !wglMakeCurrent(m_dc, m_glrc)) {
        msg_error("Can't create OpenGL context.");
        return;
    }

    // load wgl extension
    wgl_LoadFunctions(m_dc);

    // attributes for a 3.3 core profile without all the legacy stuff
    GLint attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    // create the actual context
    m_glrc_core = wglCreateContextAttribsARB(m_dc, m_glrc, attribs);
    if (!m_glrc_core || !wglMakeCurrent(m_dc, m_glrc_core)) {
        // rendering probably still works with the legacy context, so just send
        // a warning
        msg_warning("Can't create OpenGL 3.3 core context.");
    }

    // enable or disable vsync
    wglSwapIntervalEXT(config->vi.vsync ? 1 : 0);
}

void screen_adjust(int32_t width_out, int32_t height_out, int32_t* width, int32_t* height, int32_t* x, int32_t* y)
{
    UNUSED(width_out);
    UNUSED(height_out);

    // get size of window
    RECT rect;
    if (!GetClientRect(gfx.hWnd, &rect)) {
        // window handle invalid?
        *width = 0;
        *height = 0;
        *x = 0;
        *y = 0;
        return;
    }

    // status bar covers the client area, so exclude it from calculation
    RECT statusrect;
    SetRectEmpty(&statusrect);

    if (gfx.hStatusBar && GetClientRect(gfx.hStatusBar, &statusrect)) {
        rect.bottom -= statusrect.bottom;
    }

    int32_t win_width = rect.right - rect.left;
    int32_t win_height = rect.bottom - rect.top;

    // default to bottom left corner of the window above the status bar
    int32_t win_x = 0;
    int32_t win_y = statusrect.bottom;

    // adjust windowed size after the output size has changed so that
    // the output remains pixel-perfect until the user changes the window size
    if (win_width != win_width || win_height != win_height) {
        int32_t win_width_tmp = win_width = win_width;
        int32_t win_height_tmp = win_height = win_height;

        // double resolution for very small frame sizes, typically when
        // unfiltered option is enabled
        if (win_width_tmp <= 320) {
            win_width_tmp <<= 1;
            win_height_tmp <<= 1;
        }

        WINDOWPLACEMENT wndpl = { 0 };
        GetWindowPlacement(gfx.hWnd, &wndpl);

        // only fix size if windowed and not maximized
        if (!m_fullscreen && wndpl.showCmd != SW_MAXIMIZE) {
            win32_client_resize(gfx.hWnd, gfx.hStatusBar,
                win_width_tmp, win_height_tmp);
        }
    }

    *width = win_width;
    *height = win_height;
    *x = win_x;
    *y = win_y;
}

void screen_update(void)
{
    // don't render when the window is minimized
    if (!IsIconic(gfx.hWnd)) {
        // swap front and back buffers
        SwapBuffers(m_dc);
    }
}

void screen_toggle_fullscreen(void)
{
    static HMENU old_menu;
    static LONG old_style;
    static WINDOWPLACEMENT old_pos;

    m_fullscreen = !m_fullscreen;

    if (m_fullscreen) {
        // hide curser
        ShowCursor(FALSE);

        // hide status bar
        if (gfx.hStatusBar) {
            ShowWindow(gfx.hStatusBar, SW_HIDE);
        }

        // disable menu and save it to restore it later
        old_menu = GetMenu(gfx.hWnd);
        if (old_menu) {
            SetMenu(gfx.hWnd, NULL);
        }

        // save old window position and size
        GetWindowPlacement(gfx.hWnd, &old_pos);

        // use virtual screen dimensions for fullscreen mode
        int32_t vs_width = GetSystemMetrics(SM_CXSCREEN);
        int32_t vs_height = GetSystemMetrics(SM_CYSCREEN);

        // disable all styles to get a borderless window and save it to restore
        // it later
        old_style = GetWindowLong(gfx.hWnd, GWL_STYLE);
        LONG style = WS_VISIBLE;
        if (m_exclusive) {
            style |= WS_POPUP;
        }
        SetWindowLong(gfx.hWnd, GWL_STYLE, style);

        // resize window so it covers the entire virtual screen
        SetWindowPos(gfx.hWnd, HWND_TOP, 0, 0, vs_width, vs_height, SWP_SHOWWINDOW);
    }
    else {
        // restore cursor
        ShowCursor(TRUE);

        // restore status bar
        if (gfx.hStatusBar) {
            ShowWindow(gfx.hStatusBar, SW_SHOW);
        }

        // restore menu
        if (old_menu) {
            SetMenu(gfx.hWnd, old_menu);
            old_menu = NULL;
        }

        // restore style
        SetWindowLong(gfx.hWnd, GWL_STYLE, old_style);

        // restore window size and position
        SetWindowPlacement(gfx.hWnd, &old_pos);
    }
}

void screen_close(void)
{
    if (m_glrc_core) {
        wglDeleteContext(m_glrc_core);
    }

    wglDeleteContext(m_glrc);
}
