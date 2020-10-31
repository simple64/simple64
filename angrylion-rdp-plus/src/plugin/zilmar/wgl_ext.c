#include "wgl_ext.h"
#include "output/gl_proc.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

int wgl_ext_EXT_swap_control = wgl_LOAD_FAILED;
int wgl_ext_ARB_create_context = wgl_LOAD_FAILED;
int wgl_ext_ARB_create_context_profile = wgl_LOAD_FAILED;

int (CODEGEN_FUNCPTR *_ptrc_wglGetSwapIntervalEXT)(void) = NULL;
BOOL (CODEGEN_FUNCPTR *_ptrc_wglSwapIntervalEXT)(int interval) = NULL;

static int Load_EXT_swap_control(void)
{
	int numFailed = 0;
	_ptrc_wglGetSwapIntervalEXT = (int (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("wglGetSwapIntervalEXT");
	if(!_ptrc_wglGetSwapIntervalEXT) numFailed++;
	_ptrc_wglSwapIntervalEXT = (BOOL (CODEGEN_FUNCPTR *)(int))IntGetProcAddress("wglSwapIntervalEXT");
	if(!_ptrc_wglSwapIntervalEXT) numFailed++;
	return numFailed;
}

HGLRC (CODEGEN_FUNCPTR *_ptrc_wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int * attribList) = NULL;

static int Load_ARB_create_context(void)
{
	int numFailed = 0;
	_ptrc_wglCreateContextAttribsARB = (HGLRC (CODEGEN_FUNCPTR *)(HDC, HGLRC, const int *))IntGetProcAddress("wglCreateContextAttribsARB");
	if(!_ptrc_wglCreateContextAttribsARB) numFailed++;
	return numFailed;
}


static const char * (CODEGEN_FUNCPTR *_ptrc_wglGetExtensionsStringARB)(HDC hdc) = NULL;

typedef int (*PFN_LOADFUNCPOINTERS)(void);
typedef struct wgl_StrToExtMap_s
{
	char *extensionName;
	int *extensionVariable;
	PFN_LOADFUNCPOINTERS LoadExtension;
} wgl_StrToExtMap;

static wgl_StrToExtMap ExtensionMap[3] = {
	{"WGL_EXT_swap_control", &wgl_ext_EXT_swap_control, Load_EXT_swap_control},
	{"WGL_ARB_create_context", &wgl_ext_ARB_create_context, Load_ARB_create_context},
	{"WGL_ARB_create_context_profile", &wgl_ext_ARB_create_context_profile, NULL},
};

static int g_extensionMapSize = 3;

static wgl_StrToExtMap *FindExtEntry(const char *extensionName)
{
	int loop;
	wgl_StrToExtMap *currLoc = ExtensionMap;
	for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
	{
		if(strcmp(extensionName, currLoc->extensionName) == 0)
			return currLoc;
	}
	
	return NULL;
}

static void ClearExtensionVars(void)
{
	wgl_ext_EXT_swap_control = wgl_LOAD_FAILED;
	wgl_ext_ARB_create_context = wgl_LOAD_FAILED;
	wgl_ext_ARB_create_context_profile = wgl_LOAD_FAILED;
}


static void LoadExtByName(const char *extensionName)
{
	wgl_StrToExtMap *entry = NULL;
	entry = FindExtEntry(extensionName);
	if(entry)
	{
		if(entry->LoadExtension)
		{
			int numFailed = entry->LoadExtension();
			if(numFailed == 0)
			{
				*(entry->extensionVariable) = wgl_LOAD_SUCCEEDED;
			}
			else
			{
				*(entry->extensionVariable) = wgl_LOAD_SUCCEEDED + numFailed;
			}
		}
		else
		{
			*(entry->extensionVariable) = wgl_LOAD_SUCCEEDED;
		}
	}
}


static void ProcExtsFromExtString(const char *strExtList)
{
	size_t iExtListLen = strlen(strExtList);
	const char *strExtListEnd = strExtList + iExtListLen;
	const char *strCurrPos = strExtList;
	char strWorkBuff[256];

	while(*strCurrPos)
	{
		/*Get the extension at our position.*/
		int iStrLen = 0;
		const char *strEndStr = strchr(strCurrPos, ' ');
		int iStop = 0;
		if(strEndStr == NULL)
		{
			strEndStr = strExtListEnd;
			iStop = 1;
		}

		iStrLen = (int)((ptrdiff_t)strEndStr - (ptrdiff_t)strCurrPos);

		if(iStrLen > 255)
			return;

		strncpy(strWorkBuff, strCurrPos, iStrLen);
		strWorkBuff[iStrLen] = '\0';

		LoadExtByName(strWorkBuff);

		strCurrPos = strEndStr + 1;
		if(iStop) break;
	}
}

int wgl_LoadFunctions(HDC hdc)
{
	ClearExtensionVars();
	
	_ptrc_wglGetExtensionsStringARB = (const char * (CODEGEN_FUNCPTR *)(HDC))IntGetProcAddress("wglGetExtensionsStringARB");
	if(!_ptrc_wglGetExtensionsStringARB) return wgl_LOAD_FAILED;
	
	ProcExtsFromExtString((const char *)_ptrc_wglGetExtensionsStringARB(hdc));
	return wgl_LOAD_SUCCEEDED;
}

