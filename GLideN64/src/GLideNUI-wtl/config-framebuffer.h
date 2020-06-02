#pragma once
#include "config-tab.h"
#include "wtl-BitmapPicture.h"
#include "wtl-tooltip.h"
#include "resource.h"

class CFrameBufferTab :
	public CConfigTab,
	public CToolTipDialog<CFrameBufferTab>
{
public:
	BEGIN_MSG_MAP(CFrameBufferTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
		COMMAND_HANDLER_EX(IDC_CHK_ENABLE_FRAMEBUFFER, BN_CLICKED, OnEnableFramebuffer)
		COMMAND_HANDLER_EX(IDC_CHK_FB_INFO_ENABLE, BN_CLICKED, OnFbInfoEnable)
		CHAIN_MSG_MAP(CToolTipDialog<CFrameBufferTab>)
	END_MSG_MAP()
	
	CFrameBufferTab();
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnableFramebuffer(UINT /*Code*/, int id, HWND /*ctl*/);
	LRESULT OnFbInfoEnable(UINT /*Code*/, int id, HWND /*ctl*/);
	void ApplyLanguage(void);
	void LoadSettings(bool blockCustomSettings);
	void SaveSettings();

private:
	CBitmapPicture m_EmulateFBIcon;
};

