#pragma once
#include "config-tab.h"
#include "wtl-OsdButton.h"
#include "wtl-ColorButton.h"
#include "wtl-OsdPreview.h"
#include "resource.h"
#include <string>

class COsdTab :
	public CConfigTab
{
public:
	BEGIN_MSG_MAP(COsdTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		NOTIFY_HANDLER_EX(IDC_FONTS, TVN_SELCHANGED, OnFontItemChanged)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		COMMAND_ID_HANDLER(IDC_OSD_TOP_LEFT, OnOsdTopLeft)
		COMMAND_ID_HANDLER(IDC_OSD_TOP, OnOsdTop)
		COMMAND_ID_HANDLER(IDC_OSD_TOP_RIGHT, OnOsdTopRight)
		COMMAND_ID_HANDLER(IDC_OSD_BOTTOM_LEFT, OnOsdBottomLeft)
		COMMAND_ID_HANDLER(IDC_OSD_BOTTOM, OnOsdBottom)
		COMMAND_ID_HANDLER(IDC_OSD_BOTTOM_RIGHT, OnOsdBottomRight)
		NOTIFY_HANDLER_EX(IDC_OSD_COLOR, CColorButton::CPN_SELCHANGE, OnNotifyOsdColor)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	COsdTab();

	bool m_FontsLoaded;
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	void ClearOsdChecked();
	LRESULT OnOsdTopLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOsdTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOsdTopRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOsdBottomLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOsdBottom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOsdBottomRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFontItemChanged(NMHDR* /*phdr*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyOsdColor(LPNMHDR pnmh);
	void ApplyLanguage(void);
	void LoadFonts(void);
	void SelCurrentFont(void);
	void LoadSettings(bool blockCustomSettings);
	void SaveSettings();

private:
	std::wstring GetSelectedFont();

	COSDButton m_PosTopLeft, m_PosTop, m_PosTopRight;
	COSDButton m_PosCenterLeft, m_PosCenter, m_PosCenterRight;
	COSDButton m_PosBottomLeft, m_PosBottom, m_PosBottomRight;
	CTreeViewCtrl m_Fonts;
	CEdit m_FontSizeTxt;
	CUpDownCtrl m_FontSizeSpin;
	CColorButton m_OsdColor;
	COsdPreview m_OsdPreview;
};
