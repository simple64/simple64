#ifdef _WIN32
#include <windows.h>
#else
#include "../winlnxdefs.h"
#endif
#include "../GBI.h"
#include "../Config.h"

#include "Settings.h"
#include "GlSettings.h"
#include "util.h"
#include <algorithm>
#include <memory>

static const char * strIniFileName = "GLideN64.ini";
static const char * strCustomSettingsFileName = "GLideN64.custom.ini";
static const char * strUserProfile = "User";

static
void _loadSettings(GlSettings & settings)
{
	config.version = settings.value("version").toInt();

	settings.beginGroup("video");
	config.video.fullscreenWidth = settings.value("fullscreenWidth", config.video.fullscreenWidth).toInt();
	config.video.fullscreenHeight = settings.value("fullscreenHeight", config.video.fullscreenHeight).toInt();
	config.video.windowedWidth = settings.value("windowedWidth", config.video.windowedWidth).toInt();
	config.video.windowedHeight = settings.value("windowedHeight", config.video.windowedHeight).toInt();
	config.video.fullscreenRefresh = settings.value("fullscreenRefresh", config.video.fullscreenRefresh).toInt();
	config.video.multisampling = settings.value("multisampling", config.video.multisampling).toInt();
	config.video.fxaa= settings.value("fxaa", config.video.fxaa).toInt();
	config.video.verticalSync = settings.value("verticalSync", config.video.verticalSync).toInt();
	config.video.threadedVideo = settings.value("threadedVideo", config.video.threadedVideo).toInt();
	settings.endGroup();

	settings.beginGroup("texture");
	config.texture.maxAnisotropy = settings.value("maxAnisotropy", config.texture.maxAnisotropy).toInt();
	config.texture.bilinearMode = settings.value("bilinearMode", config.texture.bilinearMode).toInt();
	config.texture.enableHalosRemoval = settings.value("enableHalosRemoval", config.texture.enableHalosRemoval).toInt();
	config.texture.screenShotFormat = settings.value("screenShotFormat", config.texture.screenShotFormat).toInt();
	settings.endGroup();

	settings.beginGroup("generalEmulation");
	config.generalEmulation.enableDitheringPattern = settings.value("enableDitheringPattern", config.generalEmulation.enableDitheringPattern).toInt();
	config.generalEmulation.enableDitheringQuantization = settings.value("enableDitheringQuantization", config.generalEmulation.enableDitheringQuantization).toInt();
	config.generalEmulation.enableHiresNoiseDithering = settings.value("enableHiresNoiseDithering", config.generalEmulation.enableHiresNoiseDithering).toInt();
	config.generalEmulation.rdramImageDitheringMode = settings.value("rdramImageDitheringMode", config.generalEmulation.rdramImageDitheringMode).toInt();
	config.generalEmulation.enableLOD = settings.value("enableLOD", config.generalEmulation.enableLOD).toInt();
	config.generalEmulation.enableHWLighting = settings.value("enableHWLighting", config.generalEmulation.enableHWLighting).toInt();
	config.generalEmulation.enableShadersStorage = settings.value("enableShadersStorage", config.generalEmulation.enableShadersStorage).toInt();
	config.generalEmulation.enableCustomSettings = settings.value("enableCustomSettings", config.generalEmulation.enableCustomSettings).toInt();
	settings.endGroup();

	settings.beginGroup("graphics2D");
	config.graphics2D.correctTexrectCoords = settings.value("correctTexrectCoords", config.graphics2D.correctTexrectCoords).toInt();
	config.graphics2D.enableNativeResTexrects = settings.value("enableNativeResTexrects", config.graphics2D.enableNativeResTexrects).toInt();
	config.graphics2D.bgMode = settings.value("bgMode", config.graphics2D.bgMode).toInt();
	settings.endGroup();

	settings.beginGroup("frameBufferEmulation");
	config.frameBufferEmulation.enable = settings.value("enable", config.frameBufferEmulation.enable).toInt();
	config.frameBufferEmulation.aspect = settings.value("aspect", config.frameBufferEmulation.aspect).toInt();
	config.frameBufferEmulation.nativeResFactor = settings.value("nativeResFactor", config.frameBufferEmulation.nativeResFactor).toInt();
	config.frameBufferEmulation.bufferSwapMode = settings.value("bufferSwapMode", config.frameBufferEmulation.bufferSwapMode).toInt();
	config.frameBufferEmulation.N64DepthCompare = settings.value("N64DepthCompare", config.frameBufferEmulation.N64DepthCompare).toInt();
	config.frameBufferEmulation.forceDepthBufferClear = settings.value("forceDepthBufferClear", config.frameBufferEmulation.forceDepthBufferClear).toInt();
	config.frameBufferEmulation.copyAuxToRDRAM = settings.value("copyAuxToRDRAM", config.frameBufferEmulation.copyAuxToRDRAM).toInt();
	config.frameBufferEmulation.copyToRDRAM = settings.value("copyToRDRAM", config.frameBufferEmulation.copyToRDRAM).toInt();
	config.frameBufferEmulation.copyDepthToRDRAM = settings.value("copyDepthToRDRAM", config.frameBufferEmulation.copyDepthToRDRAM).toInt();
	config.frameBufferEmulation.copyFromRDRAM = settings.value("copyFromRDRAM", config.frameBufferEmulation.copyFromRDRAM).toInt();
	config.frameBufferEmulation.fbInfoDisabled = settings.value("fbInfoDisabled", config.frameBufferEmulation.fbInfoDisabled).toInt();
	config.frameBufferEmulation.fbInfoReadColorChunk = settings.value("fbInfoReadColorChunk", config.frameBufferEmulation.fbInfoReadColorChunk).toInt();
	config.frameBufferEmulation.fbInfoReadDepthChunk = settings.value("fbInfoReadDepthChunk", config.frameBufferEmulation.fbInfoReadDepthChunk).toInt();
	config.frameBufferEmulation.copyDepthToMainDepthBuffer = settings.value("copyDepthToMainDepthBuffer", config.frameBufferEmulation.copyDepthToMainDepthBuffer).toInt();
	config.frameBufferEmulation.enableOverscan = settings.value("enableOverscan", config.frameBufferEmulation.enableOverscan).toInt();
	config.frameBufferEmulation.overscanPAL.left = settings.value("overscanPalLeft", config.frameBufferEmulation.overscanPAL.left).toInt();
	config.frameBufferEmulation.overscanPAL.right = settings.value("overscanPalRight", config.frameBufferEmulation.overscanPAL.right).toInt();
	config.frameBufferEmulation.overscanPAL.top = settings.value("overscanPalTop", config.frameBufferEmulation.overscanPAL.top).toInt();
	config.frameBufferEmulation.overscanPAL.bottom= settings.value("overscanPalBottom", config.frameBufferEmulation.overscanPAL.bottom).toInt();
	config.frameBufferEmulation.overscanNTSC.left = settings.value("overscanNtscLeft", config.frameBufferEmulation.overscanNTSC.left).toInt();
	config.frameBufferEmulation.overscanNTSC.right = settings.value("overscanNtscRight", config.frameBufferEmulation.overscanNTSC.right).toInt();
	config.frameBufferEmulation.overscanNTSC.top = settings.value("overscanNtscTop", config.frameBufferEmulation.overscanNTSC.top).toInt();
	config.frameBufferEmulation.overscanNTSC.bottom = settings.value("overscanNtscBottom", config.frameBufferEmulation.overscanNTSC.bottom).toInt();
	settings.endGroup();

	settings.beginGroup("textureFilter");
	config.textureFilter.txFilterMode = settings.value("txFilterMode", config.textureFilter.txFilterMode).toInt();
	config.textureFilter.txEnhancementMode = settings.value("txEnhancementMode", config.textureFilter.txEnhancementMode).toInt();
	config.textureFilter.txDeposterize = settings.value("txDeposterize", config.textureFilter.txDeposterize).toInt();
	config.textureFilter.txFilterIgnoreBG = settings.value("txFilterIgnoreBG", config.textureFilter.txFilterIgnoreBG).toInt();
	config.textureFilter.txCacheSize = settings.value("txCacheSize", config.textureFilter.txCacheSize).toInt();
	config.textureFilter.txHiresEnable = settings.value("txHiresEnable", config.textureFilter.txHiresEnable).toInt();
	config.textureFilter.txHiresFullAlphaChannel = settings.value("txHiresFullAlphaChannel", config.textureFilter.txHiresFullAlphaChannel).toInt();
	config.textureFilter.txHresAltCRC = settings.value("txHresAltCRC", config.textureFilter.txHresAltCRC).toInt();
	config.textureFilter.txDump = settings.value("txDump", config.textureFilter.txDump).toInt();
	config.textureFilter.txForce16bpp = settings.value("txForce16bpp", config.textureFilter.txForce16bpp).toInt();
	config.textureFilter.txCacheCompression = settings.value("txCacheCompression", config.textureFilter.txCacheCompression).toInt();
	config.textureFilter.txSaveCache = settings.value("txSaveCache", config.textureFilter.txSaveCache).toInt();
	config.textureFilter.txEnhancedTextureFileStorage = settings.value("txEnhancedTextureFileStorage", config.textureFilter.txEnhancedTextureFileStorage).toInt();
	config.textureFilter.txHiresTextureFileStorage = settings.value("txHiresTextureFileStorage", config.textureFilter.txHiresTextureFileStorage).toInt();
	wcscpy_s(config.textureFilter.txPath, ToUTF16(settings.value("txPath", FromUTF16(config.textureFilter.txPath).c_str()).toString().c_str()).c_str());
	wcscpy_s(config.textureFilter.txCachePath, ToUTF16(settings.value("txCachePath", FromUTF16(config.textureFilter.txPath).c_str()).toString().c_str()).c_str());
	wcscpy_s(config.textureFilter.txDumpPath, ToUTF16(settings.value("txDumpPath", FromUTF16(config.textureFilter.txPath).c_str()).toString().c_str()).c_str());
	settings.endGroup();

	settings.beginGroup("font");
	config.font.name = settings.value("name", config.font.name.c_str()).toString();
	if (config.font.name.empty())
		config.font.name = "arial.ttf";
	config.font.size = settings.value("size", config.font.size).toInt();
	GlColor fontColor = settings.value("color", GlColor(config.font.color[0], config.font.color[1], config.font.color[2])).toGlColor();
	config.font.color[0] = fontColor.Red();
	config.font.color[1] = fontColor.Green();
	config.font.color[2] = fontColor.Blue();
	config.font.color[3] = fontColor.Alpha();
	config.font.colorf[0] = _FIXED2FLOAT(config.font.color[0], 8);
	config.font.colorf[1] = _FIXED2FLOAT(config.font.color[1], 8);
	config.font.colorf[2] = _FIXED2FLOAT(config.font.color[2], 8);
	config.font.colorf[3] = config.font.color[3] == 0 ? 1.0f : _FIXED2FLOAT(config.font.color[3], 8);
	settings.endGroup();

	settings.beginGroup("gammaCorrection");
	config.gammaCorrection.force = settings.value("force", config.gammaCorrection.force).toInt();
	config.gammaCorrection.level = settings.value("level", config.gammaCorrection.level).toFloat();
	settings.endGroup();

	settings.beginGroup("onScreenDisplay");
	config.onScreenDisplay.fps = settings.value("showFPS", config.onScreenDisplay.fps).toInt();
	config.onScreenDisplay.vis = settings.value("showVIS", config.onScreenDisplay.vis).toInt();
	config.onScreenDisplay.percent = settings.value("showPercent", config.onScreenDisplay.percent).toInt();
	config.onScreenDisplay.internalResolution = settings.value("showInternalResolution", config.onScreenDisplay.internalResolution).toInt();
	config.onScreenDisplay.renderingResolution = settings.value("showRenderingResolution", config.onScreenDisplay.renderingResolution).toInt();
	config.onScreenDisplay.pos = settings.value("osdPos", config.onScreenDisplay.pos).toInt();
	settings.endGroup();

	settings.beginGroup("debug");
	config.debug.dumpMode = settings.value("dumpMode", config.debug.dumpMode).toInt();
	settings.endGroup();
}

void loadSettings(const char * _strIniFolder)
{
	std::string IniFileName = _strIniFolder;
	IniFileName += "/";
	IniFileName += strIniFileName;

	bool rewriteSettings = false;
	{
		const u32 hacks = config.generalEmulation.hacks;
		GlSettings settings(IniFileName.c_str());
		const u32 configVersion = settings.value("version", 0).toInt();
		config.resetToDefaults();
		config.generalEmulation.hacks = hacks;
		config.translationFile = settings.value("translation", config.translationFile.c_str()).toString();
		if (configVersion < CONFIG_WITH_PROFILES) {
			_loadSettings(settings);
			config.version = CONFIG_VERSION_CURRENT;
			settings.clear();
			settings.setValue("version", CONFIG_VERSION_CURRENT);
			settings.setValue("profile", strUserProfile);
			settings.setValue("translation", config.translationFile.c_str());
			settings.beginGroup(strUserProfile);
			writeSettings(_strIniFolder);
			settings.endGroup();
		} else {
			std::string profile = settings.value("profile", strUserProfile).toString();
			GlSettings::sections childGroups = settings.childGroups();
			if (childGroups.find(profile.c_str()) != childGroups.end()) {
				settings.beginGroup(profile.c_str());
				_loadSettings(settings);
				settings.endGroup();
			} else
				rewriteSettings = true;
			if (config.version != CONFIG_VERSION_CURRENT)
				rewriteSettings = true;
		}
	}
	if (rewriteSettings) {
		// Keep settings up-to-date
		{
			GlSettings settings(IniFileName.c_str());
			std::string profile = settings.value("profile", strUserProfile).toString();
			settings.remove(profile.c_str());
		}
		config.version = CONFIG_VERSION_CURRENT;
		writeSettings(_strIniFolder);
	}
}

void writeSettings(const char * _strIniFolder)
{
	std::string IniFileName = _strIniFolder;
	IniFileName += "/";
	IniFileName += strIniFileName;

	GlSettings settings(IniFileName.c_str());
	settings.setValue("version", config.version);
	settings.setValue("translation", config.translationFile.c_str());
	std::string profile = settings.value("profile", strUserProfile).toString();
	if (profile.length() > 0)
	{
		settings.beginGroup(profile.c_str());
		settings.setValue("version", config.version);

		settings.beginGroup("video");
		settings.setValue("fullscreenWidth", config.video.fullscreenWidth);
		settings.setValue("fullscreenHeight", config.video.fullscreenHeight);
		settings.setValue("windowedWidth", config.video.windowedWidth);
		settings.setValue("windowedHeight", config.video.windowedHeight);
		settings.setValue("fullscreenRefresh", config.video.fullscreenRefresh);
		settings.setValue("multisampling", config.video.multisampling);
		settings.setValue("fxaa", config.video.fxaa);
		settings.setValue("verticalSync", config.video.verticalSync);
		settings.setValue("threadedVideo", config.video.threadedVideo);
		settings.endGroup();

		settings.beginGroup("texture");
		settings.setValue("maxAnisotropy", config.texture.maxAnisotropy);
		settings.setValue("bilinearMode", config.texture.bilinearMode);
		settings.setValue("enableHalosRemoval", config.texture.enableHalosRemoval);
		settings.setValue("screenShotFormat", config.texture.screenShotFormat);
		settings.endGroup();

		settings.beginGroup("generalEmulation");
		settings.setValue("enableDitheringPattern", config.generalEmulation.enableDitheringPattern);
		settings.setValue("enableDitheringQuantization", config.generalEmulation.enableDitheringQuantization);
		settings.setValue("enableHiresNoiseDithering", config.generalEmulation.enableHiresNoiseDithering);
		settings.setValue("rdramImageDitheringMode", config.generalEmulation.rdramImageDitheringMode);
		settings.setValue("enableLOD", config.generalEmulation.enableLOD);
		settings.setValue("enableHWLighting", config.generalEmulation.enableHWLighting);
		settings.setValue("enableShadersStorage", config.generalEmulation.enableShadersStorage);
		settings.setValue("enableCustomSettings", config.generalEmulation.enableCustomSettings);
		settings.endGroup();

		settings.beginGroup("graphics2D");
		settings.setValue("correctTexrectCoords", config.graphics2D.correctTexrectCoords);
		settings.setValue("enableNativeResTexrects", config.graphics2D.enableNativeResTexrects);
		settings.setValue("bgMode", config.graphics2D.bgMode);
		settings.endGroup();

		settings.beginGroup("frameBufferEmulation");
		settings.setValue("enable", config.frameBufferEmulation.enable);
		settings.setValue("aspect", config.frameBufferEmulation.aspect);
		settings.setValue("nativeResFactor", config.frameBufferEmulation.nativeResFactor);
		settings.setValue("bufferSwapMode", config.frameBufferEmulation.bufferSwapMode);
		settings.setValue("N64DepthCompare", config.frameBufferEmulation.N64DepthCompare);
		settings.setValue("forceDepthBufferClear", config.frameBufferEmulation.forceDepthBufferClear);
		settings.setValue("copyAuxToRDRAM", config.frameBufferEmulation.copyAuxToRDRAM);
		settings.setValue("copyFromRDRAM", config.frameBufferEmulation.copyFromRDRAM);
		settings.setValue("copyToRDRAM", config.frameBufferEmulation.copyToRDRAM);
		settings.setValue("copyDepthToRDRAM", config.frameBufferEmulation.copyDepthToRDRAM);
		settings.setValue("fbInfoDisabled", config.frameBufferEmulation.fbInfoDisabled);
		settings.setValue("fbInfoReadColorChunk", config.frameBufferEmulation.fbInfoReadColorChunk);
		settings.setValue("fbInfoReadDepthChunk", config.frameBufferEmulation.fbInfoReadDepthChunk);
		settings.setValue("copyDepthToMainDepthBuffer", config.frameBufferEmulation.copyDepthToMainDepthBuffer);
		settings.setValue("enableOverscan", config.frameBufferEmulation.enableOverscan);
		settings.setValue("overscanPalLeft", config.frameBufferEmulation.overscanPAL.left);
		settings.setValue("overscanPalRight", config.frameBufferEmulation.overscanPAL.right);
		settings.setValue("overscanPalTop", config.frameBufferEmulation.overscanPAL.top);
		settings.setValue("overscanPalBottom", config.frameBufferEmulation.overscanPAL.bottom);
		settings.setValue("overscanNtscLeft", config.frameBufferEmulation.overscanNTSC.left);
		settings.setValue("overscanNtscRight", config.frameBufferEmulation.overscanNTSC.right);
		settings.setValue("overscanNtscTop", config.frameBufferEmulation.overscanNTSC.top);
		settings.setValue("overscanNtscBottom", config.frameBufferEmulation.overscanNTSC.bottom);
		settings.endGroup();

		settings.beginGroup("textureFilter");
		settings.setValue("txFilterMode", config.textureFilter.txFilterMode);
		settings.setValue("txEnhancementMode", config.textureFilter.txEnhancementMode);
		settings.setValue("txDeposterize", config.textureFilter.txDeposterize);
		settings.setValue("txFilterIgnoreBG", config.textureFilter.txFilterIgnoreBG);
		settings.setValue("txCacheSize", config.textureFilter.txCacheSize);
		settings.setValue("txHiresEnable", config.textureFilter.txHiresEnable);
		settings.setValue("txHiresFullAlphaChannel", config.textureFilter.txHiresFullAlphaChannel);
		settings.setValue("txHresAltCRC", config.textureFilter.txHresAltCRC);
		settings.setValue("txDump", config.textureFilter.txDump);
		settings.setValue("txForce16bpp", config.textureFilter.txForce16bpp);
		settings.setValue("txCacheCompression", config.textureFilter.txCacheCompression);
		settings.setValue("txSaveCache", config.textureFilter.txSaveCache);
		settings.setValue("txEnhancedTextureFileStorage", config.textureFilter.txEnhancedTextureFileStorage);
		settings.setValue("txHiresTextureFileStorage", config.textureFilter.txHiresTextureFileStorage);
		settings.setValue("txPath", FromUTF16(config.textureFilter.txPath).c_str());
		settings.setValue("txCachePath", FromUTF16(config.textureFilter.txCachePath).c_str());
		settings.setValue("txDumpPath", FromUTF16(config.textureFilter.txDumpPath).c_str());
		settings.endGroup();

		settings.beginGroup("font");
		settings.setValue("name", config.font.name.c_str());
		settings.setValue("size", config.font.size);
		settings.setValue("color", GlColor(config.font.color[0], config.font.color[1], config.font.color[2], config.font.color[3]));
		settings.endGroup();

		settings.beginGroup("gammaCorrection");
		settings.setValue("force", config.gammaCorrection.force);
		settings.setValue("level", config.gammaCorrection.level);
		settings.endGroup();

		settings.beginGroup("onScreenDisplay");
		settings.setValue("showFPS", config.onScreenDisplay.fps);
		settings.setValue("showVIS", config.onScreenDisplay.vis);
		settings.setValue("showPercent", config.onScreenDisplay.percent);
		settings.setValue("showInternalResolution", config.onScreenDisplay.internalResolution);
		settings.setValue("showRenderingResolution", config.onScreenDisplay.renderingResolution);
		settings.setValue("osdPos", config.onScreenDisplay.pos);
		settings.endGroup();

		settings.beginGroup("debug");
		settings.setValue("dumpMode", config.debug.dumpMode);
		settings.endGroup();

		settings.endGroup();
	}
}

static
std::string _getRomName(const char * _strRomName)
{
	std::string RomName;

	enum { CP_SHIFT_JIS = 932 };
	int utf16size = MultiByteToWideChar(CP_SHIFT_JIS, MB_ERR_INVALID_CHARS, _strRomName, -1, 0, 0);
	if (utf16size != 0)
	{
		std::unique_ptr<wchar_t> pUTF16(new WCHAR[utf16size]);
		if (MultiByteToWideChar(CP_SHIFT_JIS, 0, (LPCCH)_strRomName, -1, pUTF16.get(), utf16size) != 0)
		{
			std::wstring wsRomName(pUTF16.get(), utf16size);
			std::transform(wsRomName.begin(), wsRomName.end(), wsRomName.begin(), (int(*)(int)) toupper);
			std::string::size_type pos = wsRomName.find(L" ");
			std::wstring replace = L"%20";
			while (pos != std::string::npos)
			{
				wsRomName.replace(pos, 1, replace);
				pos = wsRomName.find(L" ", pos + replace.length());
			}

			int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, wsRomName.c_str(), -1, 0, 0, 0, 0);
			if (utf8size != 0)
			{
				std::unique_ptr<char> pUTF8(new char[utf8size]);
				if (::WideCharToMultiByte(CP_UTF8, 0, wsRomName.c_str(), -1, pUTF8.get(), utf8size, 0, 0) != 0)
				{
					RomName = std::string(pUTF8.get());
				}
			}
		}
	}
	return RomName;
}

void loadCustomRomSettings(const char * _strIniFolder, const char * _strRomName)
{
	std::string CustomIniFileName = _strIniFolder;
	CustomIniFileName += "/";
	CustomIniFileName += strCustomSettingsFileName;

	GlSettings settings(CustomIniFileName.c_str());
	GlSettings::sections childGroups = settings.childGroups();
	const std::string romName = _getRomName(_strRomName);
	if (childGroups.find(romName.c_str()) == childGroups.end())
		return;

	settings.beginGroup(romName.c_str());
	_loadSettings(settings);
	settings.endGroup();
	config.version = CONFIG_VERSION_CURRENT;
}

void saveCustomRomSettings(const char * _strIniFolder, const char * _strRomName)
{
	Config origConfig;
	origConfig.resetToDefaults();
	std::swap(config, origConfig);
	loadSettings(_strIniFolder);
	std::swap(config, origConfig);

	std::string CustomIniFileName = _strIniFolder;
	CustomIniFileName += "/";
	CustomIniFileName += strCustomSettingsFileName;

	GlSettings settings(CustomIniFileName.c_str());
	const std::string romName = _getRomName(_strRomName);

#define WriteCustomSetting(G, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#S, config.G.S).toInt()) \
		settings.setValue(#S, config.G.S)
#define WriteCustomSetting2(G, N, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#N, config.G.S).toInt()) \
		settings.setValue(#N, config.G.S)
#define WriteCustomSettingF(G, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#S, config.G.S).toFloat()) \
		settings.setValue(#S, config.G.S)
#define WriteCustomSettingS(S) \
	const std::string new##S = FromUTF16(config.textureFilter.txPath); \
	const std::string orig##S = FromUTF16(origConfig.textureFilter.txPath); \
	if (orig##S != new##S || \
		orig##S != settings.value(#S, new##S.c_str()).toString()) \
		settings.setValue(#S, new##S.c_str())

	settings.beginGroup(romName.c_str());

	settings.beginGroup("video");
	WriteCustomSetting(video, fullscreenWidth);
	WriteCustomSetting(video, fullscreenHeight);
	WriteCustomSetting(video, windowedWidth);
	WriteCustomSetting(video, windowedHeight);
	WriteCustomSetting(video, fullscreenRefresh);
	WriteCustomSetting(video, multisampling);
	WriteCustomSetting(video, fxaa);
	WriteCustomSetting(video, verticalSync);
	settings.endGroup();

	settings.beginGroup("texture");
	WriteCustomSetting(texture, maxAnisotropy);
	WriteCustomSetting(texture, bilinearMode);
	WriteCustomSetting(texture, enableHalosRemoval);
	WriteCustomSetting(texture, screenShotFormat);
	settings.endGroup();

	settings.beginGroup("generalEmulation");
	WriteCustomSetting(generalEmulation, enableDitheringPattern);
	WriteCustomSetting(generalEmulation, enableDitheringQuantization);
	WriteCustomSetting(generalEmulation, enableHiresNoiseDithering);
	WriteCustomSetting(generalEmulation, rdramImageDitheringMode);
	WriteCustomSetting(generalEmulation, enableLOD);
	WriteCustomSetting(generalEmulation, enableHWLighting);
	WriteCustomSetting(generalEmulation, enableShadersStorage);
	settings.endGroup();

	settings.beginGroup("graphics2D");
	WriteCustomSetting(graphics2D, correctTexrectCoords);
	WriteCustomSetting(graphics2D, enableNativeResTexrects);
	WriteCustomSetting(graphics2D, bgMode);
	settings.endGroup();

	settings.beginGroup("frameBufferEmulation");
	WriteCustomSetting(frameBufferEmulation, enable);
	WriteCustomSetting(frameBufferEmulation, aspect);
	WriteCustomSetting(frameBufferEmulation, nativeResFactor);
	WriteCustomSetting(frameBufferEmulation, bufferSwapMode);
	WriteCustomSetting(frameBufferEmulation, N64DepthCompare);
	WriteCustomSetting(frameBufferEmulation, forceDepthBufferClear);
	WriteCustomSetting(frameBufferEmulation, copyAuxToRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyFromRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyToRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyDepthToRDRAM);
	WriteCustomSetting(frameBufferEmulation, fbInfoDisabled);
	WriteCustomSetting(frameBufferEmulation, fbInfoReadColorChunk);
	WriteCustomSetting(frameBufferEmulation, fbInfoReadDepthChunk);
	WriteCustomSetting(frameBufferEmulation, copyDepthToMainDepthBuffer);
	WriteCustomSetting(frameBufferEmulation, enableOverscan);
	WriteCustomSetting2(frameBufferEmulation, overscanPalLeft, overscanPAL.left);
	WriteCustomSetting2(frameBufferEmulation, overscanPalRight, overscanPAL.right);
	WriteCustomSetting2(frameBufferEmulation, overscanPalTop, overscanPAL.top);
	WriteCustomSetting2(frameBufferEmulation, overscanPalBottom, overscanPAL.bottom);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscLeft, overscanNTSC.left);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscRight, overscanNTSC.right);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscTop, overscanNTSC.top);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscBottom, overscanNTSC.bottom);
	settings.endGroup();

	settings.beginGroup("textureFilter");
	WriteCustomSetting(textureFilter, txFilterMode);
	WriteCustomSetting(textureFilter, txEnhancementMode);
	WriteCustomSetting(textureFilter, txDeposterize);
	WriteCustomSetting(textureFilter, txFilterIgnoreBG);
	WriteCustomSetting(textureFilter, txCacheSize);
	WriteCustomSetting(textureFilter, txEnhancedTextureFileStorage);
	WriteCustomSetting(textureFilter, txHiresTextureFileStorage);
	WriteCustomSetting(textureFilter, txHiresEnable);
	WriteCustomSetting(textureFilter, txHiresFullAlphaChannel);
	WriteCustomSetting(textureFilter, txHresAltCRC);
	WriteCustomSetting(textureFilter, txDump);
	WriteCustomSetting(textureFilter, txForce16bpp);
	WriteCustomSetting(textureFilter, txCacheCompression);
	WriteCustomSetting(textureFilter, txSaveCache);
	WriteCustomSettingS(txPath);
	WriteCustomSettingS(txCachePath);
	WriteCustomSettingS(txDumpPath);
	settings.endGroup();

	settings.beginGroup("gammaCorrection");
	WriteCustomSetting(gammaCorrection, force);
	WriteCustomSettingF(gammaCorrection, level);
	settings.endGroup();

	settings.beginGroup("onScreenDisplay");
	WriteCustomSetting2(onScreenDisplay, showFPS, fps);
	WriteCustomSetting2(onScreenDisplay, showVIS, vis);
	WriteCustomSetting2(onScreenDisplay, showPercent, percent);
	WriteCustomSetting2(onScreenDisplay, showInternalResolution, internalResolution);
	WriteCustomSetting2(onScreenDisplay, showRenderingResolution, renderingResolution);
	WriteCustomSetting2(onScreenDisplay, osdPos, pos);
	settings.endGroup();

	settings.endGroup();
}

std::string getTranslationFile()
{
	return config.translationFile.c_str();
}

ProfileList getProfiles(const char * _strIniFolder)
{
	std::string IniFileName = _strIniFolder;
	IniFileName += "/";
	IniFileName += strIniFileName;
	GlSettings settings(IniFileName.c_str());
	GlSettings::sections children = settings.childGroups();

	ProfileList profiles;
	for (GlSettings::sections::const_iterator itr = children.begin(); itr != children.end(); itr++)
	{
		profiles.insert(itr->c_str());
	}
	return profiles;
}

std::string getCurrentProfile(const char * _strIniFolder)
{
	std::string IniFileName = _strIniFolder;
	IniFileName += "/";
	IniFileName += strIniFileName;
	GlSettings settings(IniFileName.c_str());
	return settings.value("profile", strUserProfile).toString();
}

void changeProfile(const char * _strIniFolder, const char * _strProfile)
{
	{
		std::string IniFileName = _strIniFolder;
		IniFileName += "/";
		IniFileName += strIniFileName;

		GlSettings settings(IniFileName.c_str());
		settings.setValue("profile", _strProfile);
	}
	loadSettings(_strIniFolder);
}

void addProfile(const char * _strIniFolder, const char * _strProfile)
{
	{
		std::string IniFileName = _strIniFolder;
		IniFileName += "/";
		IniFileName += strIniFileName;

		GlSettings settings(IniFileName.c_str());
		settings.setValue("profile", _strProfile);
	}
	writeSettings(_strIniFolder);
}

void removeProfile(const char * _strIniFolder, const char * _strProfile)
{
	std::string IniFileName = _strIniFolder;
	IniFileName += "/";
	IniFileName += strIniFileName;

	GlSettings settings(IniFileName.c_str());
	settings.remove(_strProfile);
}
