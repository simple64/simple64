#include "../N64.h"
#include "../Config.h"
#include "../RSP.h"
#include "../PluginAPI.h"
#include "../GLideNUI/GLideNUI.h"
#include "../mupenplus/GLideN64_mupenplus.h"
#include <DisplayWindow.h>
#include <cstring>

Config config;

void Config_DoConfig(/*HWND hParent*/)
{
	if (config.netplay)
		return;

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
	config.netplay = 0;

	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	mbstowcs(&strIniFolderPath[0], ConfigGetUserConfigPath(), PLUGIN_PATH_SIZE);
	wchar_t strCustomFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strCustomFolderPath);
	LoadConfig(strIniFolderPath);
	if (config.generalEmulation.enableCustomSettings != 0)
		LoadCustomRomSettings(strCustomFolderPath, RSP.romname);

	char buffer[93];
	buffer[0] = 8; //send settings
	memcpy(&buffer[1], &config.frameBufferEmulation.enable, 4);
	memcpy(&buffer[5], &config.frameBufferEmulation.aspect, 4);
	//memcpy(&buffer[9], &config.frameBufferEmulation.nativeResFactor, 4);
	memcpy(&buffer[13], &config.frameBufferEmulation.bufferSwapMode, 4);
	memcpy(&buffer[17], &config.frameBufferEmulation.N64DepthCompare, 4);
	memcpy(&buffer[21], &config.frameBufferEmulation.forceDepthBufferClear, 4);
	memcpy(&buffer[25], &config.frameBufferEmulation.copyAuxToRDRAM, 4);
	memcpy(&buffer[29], &config.frameBufferEmulation.copyToRDRAM, 4);
	memcpy(&buffer[33], &config.frameBufferEmulation.copyDepthToRDRAM, 4);
	memcpy(&buffer[37], &config.frameBufferEmulation.copyFromRDRAM, 4);
	memcpy(&buffer[41], &config.frameBufferEmulation.fbInfoDisabled, 4);
	memcpy(&buffer[45], &config.frameBufferEmulation.fbInfoReadColorChunk, 4);
	memcpy(&buffer[49], &config.frameBufferEmulation.fbInfoReadDepthChunk, 4);
	memcpy(&buffer[53], &config.frameBufferEmulation.copyDepthToMainDepthBuffer, 4);
	memcpy(&buffer[57], &config.frameBufferEmulation.enableOverscan, 4);
	memcpy(&buffer[61], &config.frameBufferEmulation.overscanPAL.left, 4);
	memcpy(&buffer[65], &config.frameBufferEmulation.overscanPAL.right, 4);
	memcpy(&buffer[69], &config.frameBufferEmulation.overscanPAL.top, 4);
	memcpy(&buffer[73], &config.frameBufferEmulation.overscanPAL.bottom, 4);
	memcpy(&buffer[77], &config.frameBufferEmulation.overscanNTSC.left, 4);
	memcpy(&buffer[81], &config.frameBufferEmulation.overscanNTSC.right, 4);
	memcpy(&buffer[85], &config.frameBufferEmulation.overscanNTSC.top, 4);
	memcpy(&buffer[89], &config.frameBufferEmulation.overscanNTSC.bottom, 4);
	m64p_error netplay_init = ConfigSendNetplayConfig(&buffer[0], 93);

	if (netplay_init == M64ERR_INVALID_STATE)
	{
		buffer[0] = 9; //get gettings;
		ConfigSendNetplayConfig(&buffer[0], 1);
		ConfigReceiveNetplayConfig(&buffer[0], 92);
		memcpy(&config.frameBufferEmulation.enable, &buffer[0], 4);
		memcpy(&config.frameBufferEmulation.aspect, &buffer[4], 4);
		//memcpy(&config.frameBufferEmulation.nativeResFactor, &buffer[8], 4);
		memcpy(&config.frameBufferEmulation.bufferSwapMode, &buffer[12], 4);
		memcpy(&config.frameBufferEmulation.N64DepthCompare, &buffer[16], 4);
		memcpy(&config.frameBufferEmulation.forceDepthBufferClear, &buffer[20], 4);
		memcpy(&config.frameBufferEmulation.copyAuxToRDRAM, &buffer[24], 4);
		memcpy(&config.frameBufferEmulation.copyToRDRAM, &buffer[28], 4);
		memcpy(&config.frameBufferEmulation.copyDepthToRDRAM, &buffer[32], 4);
		memcpy(&config.frameBufferEmulation.copyFromRDRAM, &buffer[36], 4);
		memcpy(&config.frameBufferEmulation.fbInfoDisabled, &buffer[40], 4);
		memcpy(&config.frameBufferEmulation.fbInfoReadColorChunk, &buffer[44], 4);
		memcpy(&config.frameBufferEmulation.fbInfoReadDepthChunk, &buffer[48], 4);
		memcpy(&config.frameBufferEmulation.copyDepthToMainDepthBuffer, &buffer[52], 4);
		memcpy(&config.frameBufferEmulation.enableOverscan, &buffer[56], 4);
		memcpy(&config.frameBufferEmulation.overscanPAL.left, &buffer[60], 4);
		memcpy(&config.frameBufferEmulation.overscanPAL.right, &buffer[64], 4);
		memcpy(&config.frameBufferEmulation.overscanPAL.top, &buffer[68], 4);
		memcpy(&config.frameBufferEmulation.overscanPAL.bottom, &buffer[72], 4);
		memcpy(&config.frameBufferEmulation.overscanNTSC.left, &buffer[76], 4);
		memcpy(&config.frameBufferEmulation.overscanNTSC.right, &buffer[80], 4);
		memcpy(&config.frameBufferEmulation.overscanNTSC.top, &buffer[84], 4);
		memcpy(&config.frameBufferEmulation.overscanNTSC.bottom, &buffer[88], 4);
	}

	if (netplay_init != M64ERR_NOT_INIT && netplay_init != M64ERR_INCOMPATIBLE)
	{
		config.netplay = 1;
		config.video.verticalSync = 0;
		if (strstr(RSP.romname, (const char *)"GOLDENEYE") != nullptr)
			config.frameBufferEmulation.copyToRDRAM = 0;
	}

	config.validate();
}
