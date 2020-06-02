#include "wtl-BitmapPicture.h"

CBitmapPicture::CBitmapPicture() :
	m_hBitmap(NULL),
	m_nResourceID(-1),
	m_ResourceIcon(false)
{
	memset(&m_bmInfo, 0, sizeof(m_bmInfo));
	m_BackgroundBrush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
}

LRESULT CBitmapPicture::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	CRect rect;
	GetClientRect(&rect);

	CBrush PaintBrush;
	HBRUSH OldBrush = dc.SelectBrush(m_BackgroundBrush);
	dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	if (m_ResourceIcon)
	{
		CIcon hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), m_nResourceID > 0 ? MAKEINTRESOURCE(m_nResourceID) : m_strResourceName.c_str(), IMAGE_ICON, m_IconWidth, m_IconHeight, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		if (!hIcon.IsNull())
		{
			dc.DrawIconEx(0, 0, hIcon, rect.Width(), rect.Height(), 0, NULL, DI_NORMAL);
		}
	}
	dc.SelectBrush(OldBrush);
	return 0;
}

bool CBitmapPicture::SetIcon(LPCWSTR lpszResourceName, uint32_t nWidth, uint32_t nHeight)
{
	CIcon hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), lpszResourceName > 0 ? MAKEINTRESOURCE(lpszResourceName) : lpszResourceName, IMAGE_ICON, nWidth, nHeight, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	if (hIcon.IsNull())
	{
		return false;
	}
	ICONINFO IconInfo;
	if (!hIcon.GetIconInfo(&IconInfo))
	{
		return false;
	}
	if (IS_INTRESOURCE(lpszResourceName))
	{
		m_nResourceID = (int)lpszResourceName;
	}
	else
	{
		m_strResourceName = lpszResourceName;
	}
	m_ResourceIcon = true;
	m_IconWidth = nWidth;
	m_IconHeight = nHeight;
	return true;
}

bool CBitmapPicture::SetBitmap(HBITMAP hBitmap)
{
	m_hBitmap.Attach(hBitmap);
	return ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo) != 0;
}

void CBitmapPicture::SetBackroundBrush(HBRUSH brush)
{
	m_BackgroundBrush.Attach(brush);
}
