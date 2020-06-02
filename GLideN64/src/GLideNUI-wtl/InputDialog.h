#pragma once
#include <string>
#include "wtl.h"
#include "resource.h"

class CInputDialog :
	public CDialogImpl<CInputDialog>
{
public:
	static std::wstring getText(wchar_t * DlgTitle, wchar_t * Message, bool & ok);

	CInputDialog(wchar_t * DlgTitle, wchar_t * Message, bool & ok);

	enum { IDD = IDD_INPUT_DIALOG };

	BEGIN_MSG_MAP_EX(CInputDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	const std::wstring & GetInput() const { return m_Input; }

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	CInputDialog(void);

	std::wstring m_DlgTitle, m_Message, m_Input;
	bool & m_ok;
};