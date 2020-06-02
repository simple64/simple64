#pragma once
#include <stdint.h>
#include "wtl.h"

class COverScanTab :
	public CDialogImpl<COverScanTab>
{
public:
	uint32_t IDD;

	BEGIN_MSG_MAP(CAboutCreditsTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	COverScanTab();
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void SetValue(int32_t Left, int32_t Right, int32_t Top, int32_t Bottom);
	void GetValue(int32_t & Left, int32_t & Right, int32_t & Top, int32_t & Bottom);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CEdit m_LeftTxt, m_RightTxt, m_TopTxt, m_BottomTxt;
	CUpDownCtrl m_LeftSpin, m_RightSpin, m_TopSpin, m_BottomSpin;
};

