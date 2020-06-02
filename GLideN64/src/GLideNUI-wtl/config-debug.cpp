#include "config-debug.h"
#include "../Config.h"
#include "../DebugDump.h"
#include "Language.h"
#include "resource.h"

CDebugTab::CDebugTab() :
	CConfigTab(IDD_TAB_DEBUG)
{
}

BOOL CDebugTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/) {
	return true;
}

void CDebugTab::ApplyLanguage(void) {
	SetDlgItemTextW(IDC_CHK_DUMP_LOW, wGS(DEBUG_DUMP_LOWLEVEL_INFO).c_str());
	SetDlgItemTextW(IDC_CHK_DUMP_NORMAL, wGS(DEBUG_DUMP_DISPLAY_LIST).c_str());
	SetDlgItemTextW(IDC_CHK_DUMP_DETAIL, wGS(DEBUG_DUMP_DETAILED_INFO).c_str());
}

LRESULT CDebugTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}

void CDebugTab::LoadSettings(bool /*blockCustomSettings*/) {
	CButton(GetDlgItem(IDC_CHK_DUMP_LOW)).SetCheck((config.debug.dumpMode & DEBUG_LOW) != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_DUMP_NORMAL)).SetCheck((config.debug.dumpMode & DEBUG_NORMAL) != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_DUMP_DETAIL)).SetCheck((config.debug.dumpMode & DEBUG_DETAIL) != 0 ? BST_CHECKED : BST_UNCHECKED);
}

void CDebugTab::SaveSettings() {
	config.debug.dumpMode = 0;
	if (CButton(GetDlgItem(IDC_CHK_DUMP_LOW)).GetCheck() == BST_CHECKED)
		config.debug.dumpMode |= DEBUG_LOW;
	if (CButton(GetDlgItem(IDC_CHK_DUMP_NORMAL)).GetCheck() == BST_CHECKED)
		config.debug.dumpMode |= DEBUG_NORMAL;
	if (CButton(GetDlgItem(IDC_CHK_DUMP_DETAIL)).GetCheck() == BST_CHECKED)
		config.debug.dumpMode |= DEBUG_DETAIL;
}
