#include "config-overscan.h"
#include "util.h"
#include "resource.h"

COverScanTab::COverScanTab()
{
	IDD = IDD_TAB_OVERSCAN;
}

BOOL COverScanTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	m_LeftTxt.Attach(GetDlgItem(IDC_OVERSCAN_LEFT_TXT));
	m_LeftSpin.Attach(GetDlgItem(IDC_OVERSCAN_LEFT_SPIN));
	m_LeftSpin.SetBase(10);
	m_LeftSpin.SetRange(-99, 99);
	m_LeftSpin.SetBuddy(m_LeftTxt);

	m_RightTxt.Attach(GetDlgItem(IDC_OVERSCAN_RIGHT_TXT));
	m_RightSpin.Attach(GetDlgItem(IDC_OVERSCAN_RIGHT_SPIN));
	m_RightSpin.SetBase(10);
	m_RightSpin.SetRange(-99, 99);
	m_RightSpin.SetBuddy(m_RightTxt);

	m_TopTxt.Attach(GetDlgItem(IDC_OVERSCAN_TOP_TXT));
	m_TopSpin.Attach(GetDlgItem(IDC_OVERSCAN_TOP_SPIN));
	m_TopSpin.SetBase(10);
	m_TopSpin.SetRange(-99, 99);
	m_TopSpin.SetBuddy(m_TopTxt);

	m_BottomTxt.Attach(GetDlgItem(IDC_OVERSCAN_BOTTOM_TXT));
	m_BottomSpin.Attach(GetDlgItem(IDC_OVERSCAN_BOTTOM_SPIN));
	m_BottomSpin.SetBase(10);
	m_BottomSpin.SetRange(-99, 99);
	m_BottomSpin.SetBuddy(m_BottomTxt);
	return true;
}

void COverScanTab::SetValue(int32_t Left, int32_t Right, int32_t Top, int32_t Bottom)
{
	m_LeftTxt.SetWindowText(FormatStrW(L"%d", Left).c_str());
	m_LeftSpin.SetPos(Left);
	m_RightTxt.SetWindowText(FormatStrW(L"%d", Right).c_str());
	m_RightSpin.SetPos(Right);
	m_TopTxt.SetWindowText(FormatStrW(L"%d", Top).c_str());
	m_TopSpin.SetPos(Top);
	m_BottomTxt.SetWindowText(FormatStrW(L"%d", Bottom).c_str());
	m_BottomSpin.SetPos(Bottom);
}

void COverScanTab::GetValue(int32_t & Left, int32_t & Right, int32_t & Top, int32_t & Bottom)
{
	Left = m_LeftSpin.GetPos();
	Right = m_RightSpin.GetPos();
	Top = m_TopSpin.GetPos();
	Bottom = m_BottomSpin.GetPos();
}

LRESULT COverScanTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}
