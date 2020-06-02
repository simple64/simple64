#pragma once
#include <string>
#include "wtl.h"
#include "config-tab.h"
#include "language.h"
#include "resource.h"
#include <vector>

class CFrameBufferTab;
class CVideoTab;
class CEmulationTab;
class COsdTab;

class CConfigDlg :
	public CDialogImpl<CConfigDlg>
{
public:
	CConfigDlg();
	~CConfigDlg();

	enum { IDD = IDD_CONFIG };

	BEGIN_MSG_MAP_EX(CConfigDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER_EX(IDC_TABS, TCN_SELCHANGE, OnTabChange)
		COMMAND_HANDLER(IDC_PROFILE, CBN_SELCHANGE, OnProfileChanged)
		COMMAND_HANDLER_EX(IDC_GAME_PROFILE, BN_CLICKED, OnGameProfile)
		COMMAND_HANDLER_EX(IDC_USE_PROFILE, BN_CLICKED, OnUseProfile)
		COMMAND_ID_HANDLER(IDC_REMOVE_PROFILE, OnRemoveProfile)		
		COMMAND_ID_HANDLER(ID_RESTORE_DEFAULTS, OnRestoreDefaults)
		COMMAND_ID_HANDLER(ID_SAVECLOSE, OnSaveClose)
		COMMAND_ID_HANDLER(ID_SAVE, OnSave)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	void setIniPath(const std::string & IniPath);
	void setRomName(const char * RomName);
	bool Saved(void) const { return m_Saved; }
	void OnCustomSettingsToggled(bool checked);
	void SetLanguage(const std::string & language);

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRemoveProfile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRestoreDefaults(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSaveClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTabChange(NMHDR* pNMHDR);
	LRESULT OnProfileChanged(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
	LRESULT OnGameProfile(UINT /*Code*/, int id, HWND /*ctl*/);
	LRESULT OnUseProfile(UINT /*Code*/, int id, HWND /*ctl*/);

	void Init(bool reInit = false, bool blockCustomSettings = false);
	void AddTab(languageStringID StringID, CConfigTab * tab);
	void ShowTab(int nPage);
	CRect GetTabRect();
	void SaveSettings();
	void ApplyLanguage();

	CTabCtrl m_Tabs;
	std::vector<CConfigTab *> m_TabWindows;
	std::string m_strIniPath;
	const char * m_romName;
	bool m_blockReInit;
	CFrameBufferTab * m_FrameBufferTab;
	CVideoTab * m_VideoTab;
	CEmulationTab * m_EmulationTab;
	COsdTab * m_OsdTab;
	uint32_t m_TabLeft, m_ProfileLeft;
	bool m_Saved;
};

#ifdef _WIN32
void ConfigInit(void * hinst);
void ConfigCleanup(void);
#endif