#pragma once
#include "config-tab.h"

class CDebugTab :
	public CConfigTab
{
public:
	BEGIN_MSG_MAP(CEmulationTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	CDebugTab();

	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void ApplyLanguage(void);
	void LoadSettings(bool /*blockCustomSettings*/);
	void SaveSettings();
};
