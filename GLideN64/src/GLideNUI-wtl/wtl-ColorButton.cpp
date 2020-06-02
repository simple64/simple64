#include "wtl-ColorButton.h"

CColorButton::CColorButton() :
	m_Red(0),
	m_Green(0),
	m_Blue(0)
{
	memset(m_CustClr, 0, sizeof(m_CustClr));
}

LRESULT CColorButton::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	CRect rect;
	GetClientRect(&rect);

	CPen pen;
	pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_GRAYTEXT));
	HPEN hpenOld = dc.SelectPen(pen);
	CBrush PaintBrush;
	PaintBrush.CreateSolidBrush(RGB(m_Red, m_Green, m_Blue));
	dc.SelectBrush(PaintBrush);
	dc.Rectangle(rect.left, rect.top, rect.Width(), rect.Height());
	dc.SelectPen(hpenOld);

	return 0;
}

void CColorButton::SetColor(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	m_Red = Red;
	m_Green = Green;
	m_Blue = Blue;
	SendNotification(CPN_SELCHANGE);
	Invalidate();
}

LRESULT CColorButton::OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CHOOSECOLOR cc = { 0 };
	static DWORD rgbCurrent;		// initial color selection
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = GetParent();
	cc.lpCustColors = (LPDWORD)m_CustClr;
	cc.rgbResult = RGB(m_Red, m_Green, m_Blue);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_PREVENTFULLOPEN;
	if (ChooseColor(&cc) == TRUE)
	{
		m_Red = GetRValue(cc.rgbResult);
		m_Green = GetGValue(cc.rgbResult);
		m_Blue = GetBValue(cc.rgbResult);
		SendNotification(CPN_SELCHANGE);
	}
	return TRUE;
}

void CColorButton::SendNotification(UINT nCode)
{
	NMCOLORBUTTON nmclr;
	nmclr.hdr.code = nCode;
	nmclr.hdr.hwndFrom = m_hWnd;
	nmclr.hdr.idFrom = GetDlgCtrlID();
	nmclr.clr = RGB(m_Red, m_Green, m_Blue);
	::SendMessage(GetParent(), WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&nmclr);
}
