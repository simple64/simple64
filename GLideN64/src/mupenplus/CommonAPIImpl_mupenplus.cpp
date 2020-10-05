#include "GLideN64_mupenplus.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <Platform.h>
#include "../PluginAPI.h"
#include "../RSP.h"

#if defined(OS_WINDOWS)
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#elif defined(OS_MAC_OS_X)
#include <mach-o/dyld.h>
#endif

int PluginAPI::InitiateGFX(const GFX_INFO & _gfxInfo)
{
	_initiateGFX(_gfxInfo);

	int core_version;
	unsigned int gfx_info_version = 1;
	CoreGetVersion(NULL, &core_version, NULL, NULL, NULL);
	if (core_version >= 0x020501)
		gfx_info_version = _gfxInfo.version;
	if (gfx_info_version >= 2) {
		REG.SP_STATUS = _gfxInfo.SP_STATUS_REG;
		rdram_size = _gfxInfo.RDRAM_SIZE;
	}

	return TRUE;
}

static
void _cutLastPathSeparator(wchar_t * _strPath)
{
#ifdef OS_ANDROID
	const u32 bufSize = 512;
	char cbuf[bufSize];
	wcstombs(cbuf, _strPath, bufSize);
	std::string pluginPath(cbuf);
	std::string::size_type pos = pluginPath.find_last_of("/");
	mbstowcs(_strPath, pluginPath.c_str(), PLUGIN_PATH_SIZE);
#else
	std::wstring pluginPath(_strPath);
	std::replace(pluginPath.begin(), pluginPath.end(), L'\\', L'/');
	std::wstring::size_type pos = pluginPath.find_last_of(L"/");
	wcscpy(_strPath, pluginPath.substr(0, pos).c_str());
#endif
}

static
void _getWSPath(const char * _path, wchar_t * _strPath)
{
	::mbstowcs(_strPath, _path, PLUGIN_PATH_SIZE);
	_cutLastPathSeparator(_strPath);
}

void PluginAPI::GetUserDataPath(wchar_t * _strPath)
{
	_getWSPath(ConfigGetUserDataPath(), _strPath);
}

void PluginAPI::GetUserCachePath(wchar_t * _strPath)
{
	_getWSPath(ConfigGetUserCachePath(), _strPath);
}

void PluginAPI::FindPluginPath(wchar_t * _strPath)
{
	if (_strPath == nullptr)
		return;
#ifdef OS_WINDOWS
	GetModuleFileNameW((HINSTANCE)&__ImageBase, _strPath, PLUGIN_PATH_SIZE);
	_cutLastPathSeparator(_strPath);
#elif defined(OS_LINUX)
	std::ifstream maps;
	std::string line;
	std::size_t loc;
	maps.open("/proc/self/maps");

	if (maps.is_open())
	{
		while (getline(maps, line))
		{
			loc = line.find('/');
			if (loc == std::string::npos)
				continue;

			line = line.substr(loc);

			if (line.find("GLideN64") != std::string::npos)
			{
				_getWSPath(line.c_str(), _strPath);
				maps.close();
				return;
			}
		}

		maps.close();
	}

	char path[512];
	int res = readlink("/proc/self/exe", path, 510);
	if (res != -1) {
		path[res] = 0;
		_getWSPath(path, _strPath);
	}
#elif defined(OS_MAC_OS_X)
#define MAXPATHLEN 256
	char path[MAXPATHLEN];
	uint32_t pathLen = MAXPATHLEN * 2;
	if (_NSGetExecutablePath(path, &pathLen) == 0) {
		_getWSPath(path, _strPath);
	}
#elif defined(OS_ANDROID)
	GetUserCachePath(_strPath);
#endif
}
