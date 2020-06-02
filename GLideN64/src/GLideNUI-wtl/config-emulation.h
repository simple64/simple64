#pragma once
#include "config-tab.h"
#include "wtl-BitmapPicture.h"
#include "wtl-tooltip.h"
#include "resource.h"

class CConfigDlg;

class CEmulationTab :
	public CConfigTab,
	public CToolTipDialog<CEmulationTab>
{
public:
	BEGIN_MSG_MAP(CEmulationTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		COMMAND_HANDLER_EX(IDC_CHK_GAMMA_CORRECTION, BN_CLICKED, OnGammaCorrection)
		COMMAND_HANDLER_EX(IDC_CHK_USE_PER_GAME, BN_CLICKED, OnPerGameSettings)
		CHAIN_MSG_MAP(CToolTipDialog<CEmulationTab>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CEmulationTab(CConfigDlg & Dlg);
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void OnGammaCorrection(UINT /*Code*/, int id, HWND /*ctl*/);
	void OnPerGameSettings(UINT /*Code*/, int id, HWND /*ctl*/);
	void ApplyLanguage(void);
	void LoadSettings(bool /*blockCustomSettings*/);
	void SaveSettings();

private:
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	CEdit m_GamaTxt, m_N64ResMultiplerTxt;
	CUpDownCtrl m_GamaSpin, m_N64ResMultiplerSpin;
	CBitmapPicture m_GammaIcon;
	CConfigDlg & m_Dlg;
};
