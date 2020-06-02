#pragma once
#include "wtl.h"
#include "wtl-WindowFont.h"
#include "wtl-BitmapPicture.h"
#include "resource.h"
#include <vector>


class CAboutTab :
	public CDialogImpl<CAboutTab>
{
public:
	CAboutTab(uint32_t _IDD);
	virtual ~CAboutTab();

	BEGIN_MSG_MAP(CAboutTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	uint32_t IDD;

protected:
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CWindowFont m_SubtitleFont;
};

class CAboutDlg :
	public CDialogImpl<CAboutDlg>
{
public:
	~CAboutDlg();

	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP_EX(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER_EX(IDC_TABS, TCN_SELCHANGE, OnTabChange)
	END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTabChange(NMHDR* pNMHDR);

	void AddTab(const wchar_t * caption, CAboutTab *);
	void ShowTab(int nPage);
	CRect GetTabRect();

	CWindowFont m_TitleFont;
	CBitmapPicture m_AboutIcon;
	CTabCtrl m_Tabs;
	std::vector<CAboutTab *> m_TabWindows;
};