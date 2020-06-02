#include "../N64.h"
#include "../Config.h"
#include "../RSP.h"
#include "../PluginAPI.h"
#include "../GLideNUI/GLideNUI.h"
#include "../mupenplus/GLideN64_mupenplus.h"
#include <DisplayWindow.h>


Config config;

EXPORT void CALL Config_DoConfig(/*HWND hParent*/)
{
	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	mbstowcs(&strIniFolderPath[0], ConfigGetUserConfigPath(), PLUGIN_PATH_SIZE);
	wchar_t strCustomFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strCustomFolderPath);

	ConfigOpen = true;
	const bool bRestart = RunConfig(strIniFolderPath, api().isRomOpen() ? RSP.romname : nullptr);
	if (config.generalEmulation.enableCustomSettings != 0)
		LoadCustomRomSettings(strCustomFolderPath, RSP.romname);
	config.validate();
	if (bRestart)
		dwnd().restart();
	ConfigOpen = false;
}

void Config_LoadConfig()
{
	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	mbstowcs(&strIniFolderPath[0], ConfigGetUserConfigPath(), PLUGIN_PATH_SIZE);
	wchar_t strCustomFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strCustomFolderPath);
	LoadConfig(strIniFolderPath);
	if (config.generalEmulation.enableCustomSettings != 0)
		LoadCustomRomSettings(strCustomFolderPath, RSP.romname);
	config.validate();
}
