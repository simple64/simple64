#pragma once
#include "wtl.h"
#include <stdint.h>

class CColorButton :
	public CWindowImpl <CColorButton>
{
public:
	struct NMCOLORBUTTON
	{
		NMHDR hdr;
		COLORREF clr;
	};

	enum
	{
		CPN_SELCHANGE = 0x8000,	/* Colour Picker Selection change */
	};

	BEGIN_MSG_MAP(CColorButton)
		REFLECTED_COMMAND_CODE_HANDLER(BN_CLICKED, OnClicked)
		MESSAGE_HANDLER(WM_PAINT, OnPaint);
	END_MSG_MAP()

	CColorButton();
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/);
	void SetColor(uint8_t Red, uint8_t Green, uint8_t Blue);
	LRESULT OnClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	inline uint8_t Red() const { return m_Red; }
	inline uint8_t Green() const { return m_Green; }
	inline uint8_t Blue() const { return m_Blue; }

private:
	void SendNotification(UINT nCode);

	COLORREF m_CustClr[16];
	uint8_t m_Red, m_Green, m_Blue;
};
