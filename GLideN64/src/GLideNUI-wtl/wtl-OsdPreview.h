#pragma once
#include "wtl.h"
#include <string>

class COsdPreview :
	public CWindowImpl <COsdPreview>
{
public:
	BEGIN_MSG_MAP(CColorButton)
		MESSAGE_HANDLER(WM_PAINT, OnPaint);
	END_MSG_MAP()

	COsdPreview();
	void SetFont(const std::wstring & FontFile);
	void SetFontSize(uint32_t FontSize);
	void SetColor(uint8_t Red, uint8_t Green, uint8_t Blue);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/);

private:
	static HRESULT GetLogFontFromFileName(const wchar_t * fontFileName, LOGFONT* logFont);

	std::wstring m_FontFile;
	uint8_t m_Red, m_Green, m_Blue;
	uint32_t m_FontSize;
};
