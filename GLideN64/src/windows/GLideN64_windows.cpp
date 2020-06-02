#include "GLideN64_Windows.h"

HWND		hWnd;
HWND		hStatusBar;
HWND		hToolBar;
HINSTANCE	hInstance;

#ifdef WTL_UI
void ConfigInit(void * hinst);
void ConfigCleanup(void);
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/)
{
#ifdef WTL_UI
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        hInstance = hinstDLL;
        ConfigInit(hinstDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        ConfigCleanup();
    }
#else
    hInstance = hinstDLL;
#endif
	return TRUE;
}
