#pragma once
#include "wtl.h"

// Wrapper for the Win32 LOGFONT structure
class CLogFont : public LOGFONT
{
public:
	CLogFont()
	{
		memset(static_cast<LOGFONT*>(this), 0, sizeof(LOGFONT));
	}
};

// Class used to create a font based on the font used by a specific window
class CWindowFont : public CFont  
{
public:
	// Font styles
	typedef enum tagEType
	{
		typeNormal		= 0x00,
		typeBold		= 0x01,
		typeItalic		= 0x02,
		typeUnderline	= 0x04,
		typeSubheading	= 0x08,
		typeHeading		= 0x10,
	} EType;
public:
	CWindowFont() : CFont()
	{
	}
	
	/// hWnd	- The window to use for the base font
	/// nType	- Font style flags
	CWindowFont(HWND hWnd, int nType)
	{
		// We need a HWND
		ATLASSERT(hWnd != NULL);
		// Create the font
		Create(hWnd, nType);
	}
	
	virtual ~CWindowFont()
	{
	}
public:
	// Create the font
	// hWnd		- The window to use for the base font
	// nType	- Font style flags
	// return true on success
	bool Create(HWND hWnd, int nType)
	{
		// Be defensive
		ATLASSERT(hWnd != NULL);
		ATLASSERT(::IsWindow(hWnd) != FALSE);
		// Get the font the window is currently using
		HFONT hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
		// Do we have a font?
		if (hFont == NULL)
			return false;
		
		::CLogFont lf;
		// Fill the LOGFONT
		if (::GetObject(hFont, sizeof(lf), &lf) == 0)
			return false;
		// Amend the LOGFONT members
		if (nType & typeBold)
			lf.lfWeight = FW_BOLD;
		if (nType & typeItalic)
			lf.lfItalic = TRUE;
		if (nType & typeUnderline)
			lf.lfUnderline = TRUE;
		if (nType & typeSubheading)
			lf.lfHeight = static_cast<LONG>(lf.lfHeight * 1.25);
		if (nType & typeHeading)
			lf.lfHeight = static_cast<LONG>(lf.lfHeight * 2.5);
		
		// Create the new font
		return CreateFontIndirect(&lf) ? true : false;
	}
	
	// Create the font and apply to a nominate dialog control
	bool Apply(HWND hWnd, int nType, UINT nControlID)
	{
		// First create the font
		if (IsNull() && !Create(hWnd, nType))
		{
			return false;
		}
		// Apply to the nominated control
		CWindow wndControl = ::GetDlgItem(hWnd, nControlID);
		ATLASSERT(wndControl != NULL);
		// Apply
		wndControl.SetFont(m_hFont);
		return true;
	}
};
