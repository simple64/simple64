#include "About.h"

#include <thread>

#include "GLideNUI.h"
#include "Settings.h"
#include "../Config.h"
#include "ConfigDlg.h"

#ifdef QT_STATICPLUGIN
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

//#define RUN_DIALOG_IN_THREAD

static
int openConfigDialog(const wchar_t * _strFileName, const char * _romName, bool & _accepted)
{
	std::string IniFolder;
	uint32_t slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, NULL, 0, NULL, NULL);
	IniFolder.resize(slength);
	slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, (LPSTR)IniFolder.c_str(), slength, NULL, NULL);
	IniFolder.resize(slength - 1); //Remove null end char

	loadSettings(IniFolder.c_str());
	if (config.generalEmulation.enableCustomSettings != 0 && _romName != nullptr && strlen(_romName) != 0)
		loadCustomRomSettings(IniFolder.c_str(), _romName);

	LoadCurrentStrings(IniFolder.c_str(), config.translationFile);

	CConfigDlg Dlg;
	Dlg.setIniPath(IniFolder.c_str());
	Dlg.setRomName(_romName);
	Dlg.DoModal();
	_accepted = Dlg.Saved();
	return 0;
}

bool runConfigThread(const wchar_t * _strFileName, const char * _romName) {
	bool accepted = false;
#ifdef RUN_DIALOG_IN_THREAD
	std::thread configThread(openConfigDialog, _strFileName, std::ref(accepted));
	configThread.join();
#else
	openConfigDialog(_strFileName, _romName, accepted);
#endif
	return accepted;

}

EXPORT bool CALL RunConfig(const wchar_t * _strFileName, const char * _romName)
{
	return runConfigThread(_strFileName, _romName);
}

EXPORT int CALL RunAbout(const wchar_t * _strFileName)
{
	std::string IniFolder;
	uint32_t slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, NULL, 0, NULL, NULL);
	IniFolder.resize(slength);
	slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, (LPSTR)IniFolder.c_str(), slength, NULL, NULL);
	IniFolder.resize(slength - 1); //Remove null end char

	LoadCurrentStrings(IniFolder.c_str(), config.translationFile);

	CAboutDlg Dlg;
	Dlg.DoModal();
	return 0;
}

EXPORT void CALL LoadConfig(const wchar_t * _strFileName)
{
	std::string IniFolder;
	uint32_t slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, NULL, 0, NULL, NULL);
	IniFolder.resize(slength);
	slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, (LPSTR)IniFolder.c_str(), slength, NULL, NULL);
	IniFolder.resize(slength - 1); //Remove null end char

	loadSettings(IniFolder.c_str());
}

EXPORT void CALL LoadCustomRomSettings(const wchar_t * _strFileName, const char * _romName)
{
	std::string IniFolder;
	uint32_t slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, NULL, 0, NULL, NULL);
	IniFolder.resize(slength);
	slength = WideCharToMultiByte(CP_ACP, 0, _strFileName, -1, (LPSTR)IniFolder.c_str(), slength, NULL, NULL);
	IniFolder.resize(slength - 1); //Remove null end char

	loadCustomRomSettings(IniFolder.c_str(), _romName);
}
