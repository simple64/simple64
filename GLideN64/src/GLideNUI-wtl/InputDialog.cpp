#include "InputDialog.h"

CInputDialog::CInputDialog(wchar_t * DlgTitle, wchar_t * Message, bool & ok) :
	m_DlgTitle(DlgTitle != NULL ? DlgTitle : L""),
	m_Message(Message != NULL ? Message : L""),
	m_ok(ok)
{
	m_ok = false;
}

std::wstring CInputDialog::getText(wchar_t * DlgTitle, wchar_t * Message, bool & ok)
{
	CInputDialog Dlg(DlgTitle, Message, ok);
	Dlg.DoModal();
	return Dlg.GetInput();
}

LRESULT CInputDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HICON hIcon = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	
	SetWindowText(m_DlgTitle.c_str());
	GetDlgItem(IDC_INFO).SetWindowText(m_Message.c_str());
	GetDlgItem(IDC_INPUT).SetFocus();
	return 0;
}

LRESULT CInputDialog::OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CWindow InputWnd = GetDlgItem(IDC_INPUT);
	m_Input.resize(InputWnd.GetWindowTextLength() + 1);
	InputWnd.GetWindowText((wchar_t *)m_Input.data(), m_Input.size());
	
	m_ok = true;
	EndDialog(wID);
	return 0;
}

LRESULT CInputDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
