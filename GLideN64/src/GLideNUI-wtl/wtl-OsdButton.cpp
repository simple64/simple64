#include "wtl-OsdButton.h"

COSDButton::COSDButton(uint32_t IconId, DWORD dwExtendedStyle, HIMAGELIST hImageList) :
	CBitmapButtonImpl<COSDButton>(dwExtendedStyle, hImageList),
	m_Checked(false)
{
	if (IconId != 0)
	{
		m_Icon.LoadIcon(IconId);
	}
}

void COSDButton::DoPaint(CDCHandle dc)
{
	CRect rect = { 0 };
	GetClientRect(&rect);

	COLORREF BackgroundColor = RGB(225, 225, 225);
	COLORREF BorderColor = RGB(173, 173, 173);
	if (!IsWindowEnabled())
	{
		BackgroundColor = RGB(204, 204, 204);
		BorderColor = RGB(191, 191, 191);
	}
	else if (m_fMouseOver)
	{
		BackgroundColor = RGB(229, 241, 251);
		BorderColor = RGB(0, 120, 215);
	}
	if (m_Checked)
	{
		BackgroundColor = RGB(204, 228, 247);
		BorderColor = RGB(0, 84, 153);
	}

	CBrush Background;
	Background.CreateSolidBrush(BackgroundColor);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, BorderColor);
	dc.SelectPen(pen);
	dc.SelectBrush(Background);
	dc.Rectangle(rect.left, rect.top, rect.right, rect.bottom);

	if (!m_Icon.IsNull())
	{
		uint32_t IconBorder = (uint32_t)(rect.Width() * 0.3);
		uint32_t IconSize = rect.Width() - (IconBorder * 2);
		dc.DrawIconEx(IconBorder, IconBorder, m_Icon, IconSize, IconSize);
	}
}

void COSDButton::SetChecked(bool checked)
{
	if (m_Checked != checked)
	{
		m_Checked = checked;
		Invalidate();
	}
}