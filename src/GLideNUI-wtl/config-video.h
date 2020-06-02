#define _WTL_NO_CSTRING

#pragma once
#include "config-tab.h"
#include "config-overscan.h"
#include "config-framebuffer.h"
#include "wtl-BitmapPicture.h"
#include "wtl-tooltip.h"
#include "resource.h"
#include "Language.h"
#include <atlstr.h>
#include <vector>
#include <regex>

class CConfigDlg;
class CFrameBufferTab;

class CVideoTab :
	public CConfigTab,
	public CToolTipDialog<CVideoTab>
{
public:
	BEGIN_MSG_MAP(CVideoTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnColorStatic)
		NOTIFY_HANDLER_EX(IDC_TAB_OVERSCAN, TCN_SELCHANGE, OnOverscanTabChange)
		COMMAND_HANDLER_EX(IDC_CMB_FULL_SCREEN_RES, CBN_SELCHANGE, OnFullScreenChanged)
		COMMAND_HANDLER_EX(IDC_CHK_OVERSCAN, BN_CLICKED, OnOverscan)
		COMMAND_HANDLER(IDC_CMB_LANGUAGE, CBN_SELCHANGE, OnLanguageChanged)
		MESSAGE_HANDLER(WM_HSCROLL, OnScroll)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		CHAIN_MSG_MAP(CToolTipDialog<CVideoTab>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CVideoTab(CConfigDlg & Dlg, CFrameBufferTab & FrameBufferTab, const char * strIniPath);
	~CVideoTab();

	bool m_AAInfoWarning;
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	void OnOverscan(UINT /*Code*/, int id, HWND /*ctl*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOverscanTabChange(NMHDR* /*pNMHDR*/);
	void OnFullScreenChanged(UINT /*Code*/, int id, HWND /*ctl*/);
	LRESULT OnLanguageChanged(WORD wNotifyCode, WORD wID, HWND hwnd, BOOL& bHandled);
	void HideMSAADepthWarning(bool hide);
	void DisallowMSAA(bool disallow);
	void AddOverScanTab(languageStringID caption);
	void ShowOverScanTab(int nTab);
	void ApplyLanguage(void);
	void LoadSettings(bool blockCustomSettings);
	void SaveSettings();

	CTabCtrl m_OverScanTab;
	std::vector<COverScanTab *> m_OverscanTabs;
	CTrackBarCtrl m_AliasingSlider;
	CTrackBarCtrl m_AnisotropicSlider;
	CBitmapPicture m_AAInfoIcon;
	std::string m_strIniPath;
	LanguageList m_LangList;
	CConfigDlg & m_Dlg;
	CFrameBufferTab & m_FrameBufferTab;
};
