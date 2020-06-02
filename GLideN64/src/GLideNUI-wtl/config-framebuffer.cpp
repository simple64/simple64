#include "config-framebuffer.h"
#include "resource.h"
#include "../Config.h"
#include "Language.h"

CFrameBufferTab::CFrameBufferTab() :
	CConfigTab(IDD_TAB_FRAME_BUFFER)
{
}

BOOL CFrameBufferTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	TTInit();
	TTSize(400);

	SIZE iconSz = { ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON) };
	m_EmulateFBIcon.SubclassWindow(GetDlgItem(IDC_EMULATE_FB_ICON));
	m_EmulateFBIcon.SetIcon(MAKEINTRESOURCE(IDI_ICON_WARNING), iconSz.cx, iconSz.cy);
	m_EmulateFBIcon.SetWindowPos(HWND_TOP, 0, 0, iconSz.cx, iconSz.cy, SWP_NOMOVE | SWP_NOZORDER);
	m_EmulateFBIcon.SetBackroundBrush((HBRUSH)GetStockObject(WHITE_BRUSH));

	CComboBox frameBufferSwapComboBox(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP));
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_VERTICAL_INTERRUPT).c_str());
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_VI_ORIGIN_CHANGE).c_str());
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_COLOR_BUFFER_CHANGE).c_str());

	CComboBox copyColorBufferComboBox(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER));
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_NEVER).c_str());
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_SYNCHRONOUS).c_str());
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_ASYNCHRONOUS).c_str());

	CComboBox copyDepthBufferComboBox(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER));
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_NEVER).c_str());
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_VRAM).c_str());
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_SOFTWARE).c_str());

	CComboBox n64DepthCompareComboBox(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE));
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_DISABLE).c_str());
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_FAST).c_str());
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_COMPATIBLE).c_str());
	return true;
}

void CFrameBufferTab::ApplyLanguage(void)
{
	SetDlgItemTextW(IDC_CHK_ENABLE_FRAMEBUFFER, wGS(FRAMEBUFFER_ENABLE).c_str());
	SetDlgItemTextW(IDC_FRAME_BUFFER_INFO, wGS(FRAMEBUFFER_ENABLE_INFO).c_str());
	SetDlgItemTextW(IDC_CHK_COPY_AUX_BUFFERS, wGS(FRAMEBUFFER_COPY_AUX_BUFFERS).c_str());
	SetDlgItemTextW(IDC_TXT_FRAMEBUFFER_SWAP, wGS(FRAMEBUFFER_SWAP).c_str());
	SetDlgItemTextW(IDC_CHK_FB_INFO_ENABLE, wGS(FRAMEBUFFER_INFO_ENABLE).c_str());
	SetDlgItemTextW(IDC_CHK_READ_COLOR_CHUNK, wGS(FRAMEBUFFER_READ_COLOR_CHUNK).c_str());
	SetDlgItemTextW(IDC_CHK_READ_DEPTH_CHUNK, wGS(FRAMEBUFFER_READ_DEPTH_CHUNK).c_str());
	SetDlgItemTextW(IDC_TXT_COPY_COLOR_BUFFER, wGS(FRAMEBUFFER_COPY_COLOR_BUFFER).c_str());
	SetDlgItemTextW(IDC_TXT_COPY_DEPTH_BUFFER, wGS(FRAMEBUFFER_COPY_DEPTH_BUFFER).c_str());
	SetDlgItemTextW(IDC_TXT_N64_DEPTH_COMPARE, wGS(FRAMEBUFFER_N64_DEPTH_COMPARE).c_str());
	SetDlgItemTextW(IDC_CHK_FORCE_DEPTH_BUFFER_CLEAR, wGS(FRAMEBUFFER_FORCE_DEPTH_BUFFER_CLEAR).c_str());
	SetDlgItemTextW(IDC_CHK_RENDER_FRAMEBUFFER, wGS(FRAMEBUFFER_RENDER_FRAMEBUFFER).c_str());
	SetDlgItemTextW(IDC_CHK_COPY_DEPTH_TO_MAIN_DEPTH_BUFFER, wGS(FRAMEBUFFER_COPY_DEPTH_TO_MAIN).c_str());

	std::wstring tooltip = wGS(FRAMEBUFFER_COPY_AUX_BUFFERS_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_COPY_AUX_BUFFERS), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_SWAP_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_FRAMEBUFFER_SWAP), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_INFO_ENABLE_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_FB_INFO_ENABLE), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_READ_COLOR_CHUNK_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_READ_COLOR_CHUNK), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_READ_DEPTH_CHUNK_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_READ_DEPTH_CHUNK), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_COPY_COLOR_BUFFER_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_COPY_COLOR_BUFFER), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_COPY_DEPTH_BUFFER_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_COPY_COLOR_BUFFER), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_N64_DEPTH_COMPARE_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_N64_DEPTH_COMPARE), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_FORCE_DEPTH_BUFFER_CLEAR_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_FORCE_DEPTH_BUFFER_CLEAR), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_RENDER_FRAMEBUFFER_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_RENDER_FRAMEBUFFER), tooltip.c_str());
	tooltip = wGS(FRAMEBUFFER_COPY_DEPTH_TO_MAIN_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_COPY_DEPTH_TO_MAIN_DEPTH_BUFFER), tooltip.c_str());

	CComboBox frameBufferSwapComboBox(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP));
	int selectedIndx = frameBufferSwapComboBox.GetCurSel();
	frameBufferSwapComboBox.ResetContent();
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_VERTICAL_INTERRUPT).c_str());
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_VI_ORIGIN_CHANGE).c_str());
	frameBufferSwapComboBox.AddString(wGS(FRAMEBUFFER_COLOR_BUFFER_CHANGE).c_str());
	if (selectedIndx >= 0)
		frameBufferSwapComboBox.SetCurSel(selectedIndx);

	CComboBox copyColorBufferComboBox(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER));
	selectedIndx = copyColorBufferComboBox.GetCurSel();
	copyColorBufferComboBox.ResetContent();
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_NEVER).c_str());
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_SYNCHRONOUS).c_str());
	copyColorBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_ASYNCHRONOUS).c_str());
	if (selectedIndx >= 0)
		copyColorBufferComboBox.SetCurSel(selectedIndx);

	CComboBox copyDepthBufferComboBox(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER));
	selectedIndx = copyDepthBufferComboBox.GetCurSel();
	copyDepthBufferComboBox.ResetContent();
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_NEVER).c_str());
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_VRAM).c_str());
	copyDepthBufferComboBox.AddString(wGS(FRAMEBUFFER_COPY_DEPTH_SOFTWARE).c_str());
	if (selectedIndx >= 0)
		copyDepthBufferComboBox.SetCurSel(selectedIndx);

	CComboBox n64DepthCompareComboBox(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE));
	selectedIndx = n64DepthCompareComboBox.GetCurSel();
	n64DepthCompareComboBox.ResetContent();
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_DISABLE).c_str());
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_FAST).c_str());
	n64DepthCompareComboBox.AddString(wGS(FRAMEBUFFER_N64_DEPTH_COMPATIBLE).c_str());
	if (selectedIndx >= 0)
		n64DepthCompareComboBox.SetCurSel(selectedIndx);
}

LRESULT CFrameBufferTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}

LRESULT CFrameBufferTab::OnEnableFramebuffer(UINT /*Code*/, int /*id*/, HWND /*ctl*/)
{
	bool fbEmulationEnabled = CButton(GetDlgItem(IDC_CHK_ENABLE_FRAMEBUFFER)).GetCheck() == BST_CHECKED;
	bool fbInfoEnabled = CButton(GetDlgItem(IDC_CHK_FB_INFO_ENABLE)).GetCheck() == BST_CHECKED;
	CButton(GetDlgItem(IDC_CHK_COPY_AUX_BUFFERS)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CHK_FB_INFO_ENABLE)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CHK_FORCE_DEPTH_BUFFER_CLEAR)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CHK_RENDER_FRAMEBUFFER)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CHK_COPY_DEPTH_TO_MAIN_DEPTH_BUFFER)).EnableWindow(fbEmulationEnabled);
	CButton(GetDlgItem(IDC_CHK_READ_COLOR_CHUNK)).EnableWindow(fbEmulationEnabled && fbInfoEnabled);
	CButton(GetDlgItem(IDC_CHK_READ_DEPTH_CHUNK)).EnableWindow(fbEmulationEnabled && fbInfoEnabled);
	return 0;
}


LRESULT CFrameBufferTab::OnFbInfoEnable(UINT /*Code*/, int /*id*/, HWND /*ctl*/)
{
	bool fbInfoEnabled = CButton(GetDlgItem(IDC_CHK_FB_INFO_ENABLE)).GetCheck() == BST_CHECKED;
	CButton(GetDlgItem(IDC_CHK_READ_COLOR_CHUNK)).EnableWindow(fbInfoEnabled);
	CButton(GetDlgItem(IDC_CHK_READ_DEPTH_CHUNK)).EnableWindow(fbInfoEnabled);
	return 0;
}

void CFrameBufferTab::LoadSettings(bool /*blockCustomSettings*/)
{
	const bool fbEmulationEnabled = config.frameBufferEmulation.enable != 0;
	CButton(GetDlgItem(IDC_CHK_ENABLE_FRAMEBUFFER)).SetCheck(fbEmulationEnabled != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_COPY_AUX_BUFFERS)).SetCheck(config.frameBufferEmulation.copyAuxToRDRAM != 0 ? BST_CHECKED : BST_UNCHECKED);
	CComboBox(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP)).SetCurSel(config.frameBufferEmulation.bufferSwapMode);
	CButton(GetDlgItem(IDC_CHK_FB_INFO_ENABLE)).SetCheck(config.frameBufferEmulation.fbInfoDisabled == 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_READ_COLOR_CHUNK)).SetCheck(config.frameBufferEmulation.fbInfoReadColorChunk != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_READ_DEPTH_CHUNK)).SetCheck(config.frameBufferEmulation.fbInfoReadDepthChunk != 0 ? BST_CHECKED : BST_UNCHECKED);
	CComboBox(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER)).SetCurSel(config.frameBufferEmulation.copyToRDRAM);
	CComboBox(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER)).SetCurSel(config.frameBufferEmulation.copyDepthToRDRAM);
	CComboBox(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE)).SetCurSel(config.frameBufferEmulation.N64DepthCompare);
	CButton(GetDlgItem(IDC_CHK_FORCE_DEPTH_BUFFER_CLEAR)).SetCheck(config.frameBufferEmulation.forceDepthBufferClear != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_RENDER_FRAMEBUFFER)).SetCheck(config.frameBufferEmulation.copyFromRDRAM != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_COPY_DEPTH_TO_MAIN_DEPTH_BUFFER)).SetCheck(config.frameBufferEmulation.copyDepthToMainDepthBuffer != 0 ? BST_CHECKED : BST_UNCHECKED);

	OnFbInfoEnable(0, 0, NULL);
	OnEnableFramebuffer(0, 0, NULL);
}

void CFrameBufferTab::SaveSettings()
{
	config.frameBufferEmulation.enable = CButton(GetDlgItem(IDC_CHK_ENABLE_FRAMEBUFFER)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.copyAuxToRDRAM = CButton(GetDlgItem(IDC_CHK_COPY_AUX_BUFFERS)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.bufferSwapMode = CComboBox(GetDlgItem(IDC_CMB_FRAMEBUFFER_SWAP)).GetCurSel();
	config.frameBufferEmulation.fbInfoDisabled = CButton(GetDlgItem(IDC_CHK_FB_INFO_ENABLE)).GetCheck() == BST_CHECKED ? 0 : 1;
	config.frameBufferEmulation.fbInfoReadColorChunk = CButton(GetDlgItem(IDC_CHK_READ_COLOR_CHUNK)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.fbInfoReadDepthChunk = CButton(GetDlgItem(IDC_CHK_READ_DEPTH_CHUNK)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.copyToRDRAM = CComboBox(GetDlgItem(IDC_CMB_COPY_COLOR_BUFFER)).GetCurSel();
	config.frameBufferEmulation.copyDepthToRDRAM = CComboBox(GetDlgItem(IDC_CMB_COPY_DEPTH_BUFFER)).GetCurSel();
	config.frameBufferEmulation.N64DepthCompare = CComboBox(GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE)).GetCurSel();
	config.frameBufferEmulation.forceDepthBufferClear = CButton(GetDlgItem(IDC_CHK_FORCE_DEPTH_BUFFER_CLEAR)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.copyFromRDRAM = CButton(GetDlgItem(IDC_CHK_RENDER_FRAMEBUFFER)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.frameBufferEmulation.copyDepthToMainDepthBuffer = CButton(GetDlgItem(IDC_CHK_COPY_DEPTH_TO_MAIN_DEPTH_BUFFER)).GetCheck() == BST_CHECKED ? 1 : 0;
}
