#include "wtl-OsdPreview.h"
#include "FontInfo.h"
#include "util.h"
#include "Language.h"
#include <DWrite.h>

#pragma comment(lib, "Dwrite.lib")

COsdPreview::COsdPreview() :
	m_FontFile(L"arial.ttf"),
	m_FontSize(16),
	m_Red(0),
	m_Green(0),
	m_Blue(0)
{
}

void COsdPreview::SetFont(const std::wstring & FontFile)
{
	m_FontFile = FontFile;
	Invalidate();
}

void COsdPreview::SetFontSize(uint32_t FontSize)
{
	m_FontSize = FontSize;
	Invalidate();
}

void COsdPreview::SetColor(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	m_Red = Red;
	m_Green = Green;
	m_Blue = Blue;
	Invalidate();
}


LRESULT COsdPreview::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
	CRect rect;
	GetClientRect(&rect);

	CPaintDC dc(m_hWnd);
	CBrush PaintBrush;
	PaintBrush.CreateSolidBrush(RGB(0, 0, 0));
	dc.SelectBrush(PaintBrush);
	dc.Rectangle(rect.left, rect.top, rect.Width(), rect.Height());
	dc.SetTextColor(RGB(m_Red, m_Green, m_Blue));
	dc.SetBkColor(RGB(0, 0, 0));

	std::wstring fontFileName = ToUTF16(GetFontFolder().c_str());
	fontFileName += L"\\" + m_FontFile;

	AddFontResourceEx(fontFileName.c_str(), FR_PRIVATE, 0);
	LOGFONT logFont;
	if (SUCCEEDED(GetLogFontFromFileName(fontFileName.c_str(), &logFont)))
	{
		int desiredPpem = (int)m_FontSize;
		logFont.lfHeight = -long(desiredPpem);
		HFONT hf = CreateFontIndirect(&logFont);
		HFONT hOldFont = (HFONT)dc.SelectFont(hf);
		if (hOldFont)
		{
			dc.DrawText(wGS(OSD_PREVIEW).c_str(), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			dc.SelectFont(hOldFont);
		}
	}
	return 0;
}

HRESULT COsdPreview::GetLogFontFromFileName(const wchar_t * fontFileName, LOGFONT* logFont)
{
	CComPtr<IDWriteFactory> dwriteFactory;
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory))))
	{
		return S_FALSE;
	}

	CComPtr<IDWriteGdiInterop> gdiInterop;
	if (FAILED(dwriteFactory->GetGdiInterop(&gdiInterop)))
	{
		return S_FALSE;
	}

	CComPtr<IDWriteFontFile> fontFile;
	if (FAILED(dwriteFactory->CreateFontFileReference(fontFileName, nullptr, &fontFile)))
	{
		return S_FALSE;
	}

	BOOL isSupportedFontType = false;
	DWRITE_FONT_FILE_TYPE fontFileType;
	DWRITE_FONT_FACE_TYPE fontFaceType;
	UINT32 numberOfFaces = 0;
	if (FAILED(fontFile->Analyze(&isSupportedFontType, &fontFileType, &fontFaceType, &numberOfFaces)))
	{
		return S_FALSE;
	}
	if (!isSupportedFontType)
	{
		return DWRITE_E_FILEFORMAT;
	}

	CComPtr<IDWriteFontFace> fontFace;
	IDWriteFontFile * const fontFiles[] =
	{
		fontFile.p
	};
	if (FAILED(dwriteFactory->CreateFontFace(fontFaceType, ARRAYSIZE(fontFiles), &fontFiles[0], 0, DWRITE_FONT_SIMULATIONS_NONE, &fontFace)))
	{
		return S_FALSE;
	}

	if (FAILED(gdiInterop->ConvertFontFaceToLOGFONT(fontFace, OUT logFont)))
	{
		return S_FALSE;
	}
	return S_OK;
}
