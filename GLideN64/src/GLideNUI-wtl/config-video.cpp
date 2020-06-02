#include "config-video.h"
#include "FullscreenResolutions.h"
#include "util.h"
#include "Language.h"
#include "ConfigDlg.h"
#include "../Config.h"
#include "wtl-tooltip.h"

static struct {
	unsigned short width, height;
	LPCTSTR description;
}
WindowedModes[] = {
	{ 320, 240, _T("320 x 240") },
	{ 400, 300, _T("400 x 300") },
	{ 480, 360, _T("480 x 360") },
	{ 640, 480, _T("640 x 480") },
	{ 800, 600, _T("800 x 600") },
	{ 960, 720, _T("960 x 720") },
	{ 1024, 768, _T("1024 x 768") },
	{ 1152, 864, _T("1152 x 864") },
	{ 1280, 960, _T("1280 x 960") },
	{ 1280, 1024, _T("1280 x 1024") },
	{ 1440, 1080, _T("1440 x 1080") },
	{ 1600, 1024, _T("1600 x 1024") },
	{ 1600, 1200, _T("1600 x 1200") }
};
static const unsigned int numWindowedModes = sizeof(WindowedModes) / sizeof(WindowedModes[0]);
static const LPCTSTR englishLang = _T("English");

static u32 pow2(u32 dim)
{
	if (dim == 0) return 0;

	return (1 << dim);
}

static u32 powof(u32 dim)
{
	if (dim == 0) return 0;

	u32 num = 2; u32 i = 1;
	while (num < dim) {
		num <<= 1;
		i++;
	}
	return i;
}

CVideoTab::CVideoTab(CConfigDlg & Dlg, CFrameBufferTab & FrameBufferTab, const char * strIniPath) :
	CConfigTab(IDD_TAB_VIDEO),
	m_strIniPath(strIniPath),
	m_LangList(GetLanguageList(strIniPath)),
	m_Dlg(Dlg),
	m_FrameBufferTab(FrameBufferTab)
{
	m_AAInfoWarning = false;
}

CVideoTab::~CVideoTab() {
	for (size_t i = 0; i < m_OverscanTabs.size(); i++)
		delete m_OverscanTabs[i];
	m_OverscanTabs.clear();
}

BOOL CVideoTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/) {
	TTInit();
	TTSize(400);

	m_OverScanTab.Attach(GetDlgItem(IDC_TAB_OVERSCAN));
	AddOverScanTab(VIDEO_NTSC);
	AddOverScanTab(VIDEO_PAL);

	m_AliasingSlider.Attach(GetDlgItem(IDC_ALIASING_SLIDER));
	m_AliasingSlider.SetTicFreq(1);
	m_AliasingSlider.SetRangeMin(0);
	m_AliasingSlider.SetRangeMax(3);

	m_AnisotropicSlider.Attach(GetDlgItem(IDC_ANISOTROPIC_SLIDER));
	m_AnisotropicSlider.SetTicFreq(2);
	m_AnisotropicSlider.SetRangeMin(0);
	m_AnisotropicSlider.SetRangeMax(16);

	CComboBox aspectComboBox(GetDlgItem(IDC_CMB_ASPECT_RATIO));
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_4_3).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_16_19).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_STRETCH).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_ADJUST).c_str());

	CComboBox ditheringModeComboBox(GetDlgItem(IDC_CMB_PATTERN));
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_DISABLE).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_BAYER).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_MAGIC_SQUARE).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_BLUE_NOISE).c_str());

	SIZE iconSz = { ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON) };
	m_AAInfoIcon.SubclassWindow(GetDlgItem(IDC_AA_INFO_ICON));
	m_AAInfoIcon.SetIcon(MAKEINTRESOURCE(IDI_ICON_INFO), iconSz.cx, iconSz.cy);
	m_AAInfoIcon.SetWindowPos(HWND_TOP, 0, 0, iconSz.cx, iconSz.cy, SWP_NOMOVE | SWP_NOZORDER);
	m_AAInfoIcon.SetBackroundBrush((HBRUSH)GetStockObject(WHITE_BRUSH));

	CComboBox translationsComboBox(GetDlgItem(IDC_CMB_LANGUAGE));
	for (LanguageList::const_iterator itr = m_LangList.begin(); itr != m_LangList.end(); itr++) {
		int indx = translationsComboBox.AddString(ToUTF16(itr->LanguageName.c_str()).c_str());
		translationsComboBox.SetItemData(indx, (DWORD_PTR)itr->Filename.c_str());
	}
	if (translationsComboBox.FindString(-1, englishLang) == CB_ERR) {
		int indx = translationsComboBox.AddString(englishLang);
		translationsComboBox.SetItemData(indx, (DWORD_PTR)"");
	}
	return true;
}

void CVideoTab::ApplyLanguage(void) {
	SetDlgItemTextW(IDC_VIDEO_GROUP, wGS(VIDEO_GROUP).c_str());
	SetDlgItemTextW(IDC_TXT_FULL_SCREEN_RES, wGS(VIDEO_FULL_SCREEN_RES).c_str());
	SetDlgItemTextW(IDC_TXT_REFRESH_RATE, wGS(VIDEO_REFRESH_RATE).c_str());
	SetDlgItemTextW(IDC_TXT_WINDOWED_RESOLUTION, wGS(VIDEO_WINDOWED_RESOLUTION).c_str());
	SetDlgItemTextW(IDC_TXT_ASPECT_RATIO, wGS(VIDEO_ASPECT_RATIO).c_str());
	SetDlgItemTextW(IDC_CHK_VERTICAL_SYNC, wGS(VIDEO_VSYNC).c_str());
	SetDlgItemTextW(IDC_CHK_THREADED_VIDEO, wGS(VIDEO_THREADED_VIDEO).c_str());
	SetDlgItemTextW(IDC_CHK_OVERSCAN, wGS(VIDEO_OVERSCAN).c_str());
	SetDlgItemTextW(IDC_AA_GROUP, wGS(VIDEO_ANTI_ALIASING).c_str());
	SetDlgItemTextW(IDC_NOAA_RADIO, wGS(VIDEO_NO_ANTI_ALIASING).c_str());
	SetDlgItemTextW(IDC_FXAA_RADIO, wGS(VIDEO_FAST_ANTI_ALIASING).c_str());
	SetDlgItemTextW(IDC_MSAA_RADIO, wGS(VIDEO_MULTISAMPLE_ANTI_ALIASING).c_str());
	SetDlgItemTextW(IDC_AA_OFF, wGS(VIDEO_AA_OFF).c_str());
	SetDlgItemTextW(IDC_AA_HIGH, wGS(VIDEO_AA_HIGH).c_str());
	SetDlgItemTextW(IDC_AA_INFO, wGS(VIDEO_AA_INFO).c_str());
	SetDlgItemTextW(IDC_FILTERING_GROUP, wGS(VIDEO_FILTERING_GROUP).c_str());
	SetDlgItemTextW(IDC_ANISOTROPIC, wGS(VIDEO_ANISOTROPIC).c_str());
	SetDlgItemTextW(IDC_ANISOTROPIC_OFF, wGS(VIDEO_ANISOTROPIC_OFF).c_str());
	SetDlgItemTextW(IDC_ANISOTROPIC_HIGH, wGS(VIDEO_ANISOTROPIC_HIGH).c_str());
	SetDlgItemTextW(IDC_BILINEAR, wGS(VIDEO_BILINEAR).c_str());
	SetDlgItemTextW(IDC_BILINEAR_STANDARD, wGS(VIDEO_BILINEAR_STANDARD).c_str());
	SetDlgItemTextW(IDC_BILINEAR_3POINT, wGS(VIDEO_BILINEAR_3POINT).c_str());

	SetDlgItemTextW(IDC_DITHERING_GROUP, wGS(VIDEO_DITHERING_GROUP).c_str());
	SetDlgItemTextW(IDC_PATTERN, wGS(VIDEO_PATTERN).c_str());
	SetDlgItemTextW(IDC_CHK_APPLY_TO_OUTPUT, wGS(VIDEO_DITHERING_APPLY_TO_OUTPUT).c_str());
	SetDlgItemTextW(IDC_CHK_5BIT_QUANTIZATION, wGS(VIDEO_DITHERING_5BIT_QUANTIZATION).c_str());
	SetDlgItemTextW(IDC_CHK_HIRES_NOISE, wGS(VIDEO_DITHERING_HIRES_NOISE).c_str());
	SetDlgItemTextW(IDC_LANGUAGE, wGS(VIDEO_LANGUAGE).c_str());

	std::wstring tooltip = wGS(VIDEO_FULL_SCREEN_RES_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_FULL_SCREEN_RES), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_FULL_SCREEN_RES), tooltip.c_str());
	tooltip = wGS(VIDEO_REFRESH_RATE_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_REFRESH_RATE), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_REFRESH_RATE), tooltip.c_str());
	tooltip = wGS(VIDEO_WINDOWED_RESOLUTION_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_WINDOWED_RESOLUTION), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_WINDOWED_RESOLUTION), tooltip.c_str());
	tooltip = wGS(VIDEO_ASPECT_RATIO_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_TXT_ASPECT_RATIO), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_CMB_ASPECT_RATIO), tooltip.c_str());
	tooltip = wGS(VIDEO_VSYNC_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_VERTICAL_SYNC), tooltip.c_str());
	tooltip = wGS(VIDEO_THREADED_VIDEO_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_THREADED_VIDEO), tooltip.c_str());
	tooltip = wGS(VIDEO_OVERSCAN_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_OVERSCAN), tooltip.c_str());
	tooltip = wGS(VIDEO_AA_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_AA_GROUP), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_NOAA_RADIO), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_FXAA_RADIO), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_MSAA_RADIO), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_ALIASING_SLIDER), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_AA_OFF), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_AA_HIGH), tooltip.c_str());
	tooltip = wGS(VIDEO_BILINEAR_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_BILINEAR), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_BILINEAR_STANDARD), tooltip.c_str());
	TTSetTxt(GetDlgItem(IDC_BILINEAR_3POINT), tooltip.c_str());
	tooltip = wGS(VIDEO_DITHERING_APPLY_TO_OUTPUT_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_APPLY_TO_OUTPUT), tooltip.c_str());
	tooltip = wGS(VIDEO_DITHERING_5BIT_QUANTIZATION_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_5BIT_QUANTIZATION), tooltip.c_str());
	tooltip = wGS(VIDEO_DITHERING_HIRES_NOISE_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CHK_HIRES_NOISE), tooltip.c_str());
	tooltip = wGS(VIDEO_DITHERING_MODE_TOOLTIP);
	TTSetTxt(GetDlgItem(IDC_CMB_PATTERN), tooltip.c_str());

	CComboBox aspectComboBox(GetDlgItem(IDC_CMB_ASPECT_RATIO));
	int selectedIndx = aspectComboBox.GetCurSel();
	aspectComboBox.ResetContent();
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_4_3).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_16_19).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_STRETCH).c_str());
	aspectComboBox.AddString(wGS(VIDEO_ASPECT_ADJUST).c_str());
	if (selectedIndx >= 0)
		aspectComboBox.SetCurSel(selectedIndx);

	CComboBox ditheringModeComboBox(GetDlgItem(IDC_CMB_PATTERN));
	selectedIndx = ditheringModeComboBox.GetCurSel();
	ditheringModeComboBox.ResetContent();
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_DISABLE).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_BAYER).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_MAGIC_SQUARE).c_str());
	ditheringModeComboBox.AddString(wGS(VIDEO_DITHERING_BLUE_NOISE).c_str());
	if (selectedIndx >= 0)
		ditheringModeComboBox.SetCurSel(selectedIndx);

	for (int i = 0, n = m_OverScanTab.GetItemCount(); i < n; i++) {
		TCITEM tci = { 0 };
		tci.mask = TCIF_PARAM;
		m_OverScanTab.GetItem(i, &tci);
		if (tci.lParam != 0) {
			tci.mask = TCIF_TEXT;
			std::wstring caption = wGS((languageStringID)tci.lParam);
			tci.pszText = (LPWSTR)caption.c_str();
			m_OverScanTab.SetItem(i, &tci);
		}
	}

}

LRESULT CVideoTab::OnScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LONG SliderId = CWindow((HWND)lParam).GetWindowLong(GWL_ID);
	if (SliderId == IDC_ALIASING_SLIDER) {
		int32_t multisampling = m_AliasingSlider.GetPos();
		std::wstring AliasingText = FormatStrW(L"%dx", pow2(multisampling));
		CWindow(GetDlgItem(IDC_ALIASING_LABEL)).SetWindowTextW(AliasingText.c_str());
		CButton(GetDlgItem(multisampling != 0 ? IDC_MSAA_RADIO : IDC_NOAA_RADIO)).SetCheck(BST_CHECKED);
		CButton(GetDlgItem(multisampling != 0 ? IDC_NOAA_RADIO : IDC_MSAA_RADIO)).SetCheck(BST_UNCHECKED);
		CButton(GetDlgItem(IDC_FXAA_RADIO)).SetCheck(BST_UNCHECKED);
	} else if (SliderId == IDC_ANISOTROPIC_SLIDER) {
		CWindow(GetDlgItem(IDC_ANISOTROPIC_LABEL)).SetWindowTextW(FormatStrW(L"%dx", m_AnisotropicSlider.GetPos()).c_str());
	}
	return 0;
}

void CVideoTab::OnOverscan(UINT /*Code*/, int /*id*/, HWND /*ctl*/) {
	CButton OverScan(GetDlgItem(IDC_CHK_OVERSCAN));
	if (OverScan.GetCheck() == BST_CHECKED) {
		GetDlgItem(IDC_TAB_OVERSCAN).ShowWindow(SW_SHOW);
		ShowOverScanTab(m_OverScanTab.GetCurSel());
	} else {
		GetDlgItem(IDC_TAB_OVERSCAN).ShowWindow(SW_HIDE);
		m_OverscanTabs[m_OverScanTab.GetCurSel()]->ShowWindow(SW_HIDE);
	}
}

LRESULT CVideoTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}

LRESULT CVideoTab::OnOverscanTabChange(NMHDR* /*pNMHDR*/) {
	ShowOverScanTab(m_OverScanTab.GetCurSel());
	return FALSE;
}

void CVideoTab::OnFullScreenChanged(UINT /*Code*/, int /*id*/, HWND /*ctl*/) {
	CComboBox fullScreenResolutionComboBox(GetDlgItem(IDC_CMB_FULL_SCREEN_RES));
	int32_t index = fullScreenResolutionComboBox.GetCurSel();
	StringList fullscreenRatesList;
	int fullscreenRate;
	fillFullscreenRefreshRateList(index, fullscreenRatesList, fullscreenRate);

	CComboBox RefreshRateComboBox(GetDlgItem(IDC_CMB_REFRESH_RATE));
	RefreshRateComboBox.ResetContent();
	for (size_t i = 0, n = fullscreenRatesList.size(); i < n; i++) {
		std::wstring fullscreenRateStr(fullscreenRatesList[i].begin(), fullscreenRatesList[i].end());
		int index = RefreshRateComboBox.AddString(fullscreenRateStr.c_str());
		if (fullscreenRate == i)
			RefreshRateComboBox.SetCurSel(index);
	}
}

LRESULT CVideoTab::OnLanguageChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/) {
	CComboBox translationsComboBox(GetDlgItem(IDC_CMB_LANGUAGE));
	std::string currentLang = (const char *)translationsComboBox.GetItemDataPtr(translationsComboBox.GetCurSel());
	m_Dlg.SetLanguage(currentLang);
	return 0;
}

void CVideoTab::HideMSAADepthWarning(bool hide)
{
	int nID[] = {
	IDC_AA_INFO,
	IDC_AA_INFO_ICON,
	IDC_FILTERING_GROUP,
	IDC_ANISOTROPIC,
	IDC_ANISOTROPIC_OFF,
	IDC_ANISOTROPIC_HIGH,
	IDC_ANISOTROPIC_SLIDER,
	IDC_ANISOTROPIC_LABEL,
	IDC_BILINEAR,
	IDC_BILINEAR_STANDARD,
	IDC_BILINEAR_3POINT,
	IDC_DITHERING_GROUP,
	IDC_PATTERN,
	IDC_CMB_PATTERN,
	IDC_CHK_APPLY_TO_OUTPUT,
	IDC_CHK_5BIT_QUANTIZATION,
	IDC_CHK_HIRES_NOISE
	};

	RECT Rect; RECT groupRect;
	GetDlgItem(nID[0]).GetWindowRect(&Rect);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
	int32_t Move = Rect.bottom - Rect.top + 2;

	GetDlgItem(nID[0]).ShowWindow(hide ? SW_HIDE : SW_SHOWNORMAL);
	GetDlgItem(nID[1]).ShowWindow(hide ? SW_HIDE : SW_SHOWNORMAL);

	CWindow AAgroup = GetDlgItem(IDC_AA_GROUP);
	AAgroup.GetWindowRect(&groupRect);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&groupRect, 2);
	SIZE AAgroupSz = { groupRect.right - groupRect.left, hide ? groupRect.bottom - groupRect.top - Move - 1 : groupRect.bottom - groupRect.top + Move + 1 };
	AAgroup.SetWindowPos(HWND_TOP, 0, 0, AAgroupSz.cx, AAgroupSz.cy, SWP_NOMOVE | SWP_NOZORDER);

	for (size_t i = 2, n = sizeof(nID) / sizeof(nID[0]); i < n; i++) {
		CWindow window = GetDlgItem(nID[i]);
		window.GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);

		Rect.top -= hide ? Move : -Move;
		Rect.bottom -= hide ? Move : -Move;
		window.MoveWindow(&Rect);
	}

	m_AAInfoWarning = hide;
}

void CVideoTab::DisallowMSAA(bool disallow)
{
	GetDlgItem(IDC_MSAA_RADIO).EnableWindow(!disallow);
	GetDlgItem(IDC_ALIASING_SLIDER).EnableWindow(!disallow);
	GetDlgItem(IDC_ALIASING_LABEL).EnableWindow(!disallow);
	GetDlgItem(IDC_AA_HIGH).EnableWindow(!disallow);
	GetDlgItem(IDC_AA_OFF).EnableWindow(!disallow);
}


void CVideoTab::AddOverScanTab(languageStringID caption) {
	m_OverScanTab.AddItem(TCIF_TEXT | TCIF_PARAM, wGS(caption).c_str(), 0, caption);

	COverScanTab * tab = new COverScanTab;
	tab->Create(m_hWnd, 0);
	tab->SetWindowPos(m_hWnd, 0, 0, 0, 0, SWP_HIDEWINDOW);
	m_OverscanTabs.push_back(tab);

	if (m_OverscanTabs.size() == 1)
		ShowOverScanTab(0);
}

void CVideoTab::ShowOverScanTab(int nTab) {
	for (size_t i = 0; i < m_OverscanTabs.size(); i++)
		m_OverscanTabs[i]->ShowWindow(SW_HIDE);

	CRect TabRect;
	m_OverScanTab.GetWindowRect(&TabRect);
	ScreenToClient(&TabRect);
	m_OverScanTab.AdjustRect(FALSE, &TabRect);

	m_OverscanTabs[nTab]->SetWindowPos(HWND_TOP, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_SHOWWINDOW);

	CRect WinRect, ClientRect;
	m_OverscanTabs[nTab]->GetWindowRect(WinRect);
	m_OverscanTabs[nTab]->GetClientRect(ClientRect);

	m_OverScanTab.RedrawWindow();
}

void CVideoTab::LoadSettings(bool /*blockCustomSettings*/) {
	CComboBox WindowedResolutionComboBox(GetDlgItem(IDC_CMB_WINDOWED_RESOLUTION));
	if (WindowedResolutionComboBox.GetCount() == 0) {
		for (unsigned int i = 0; i < numWindowedModes; ++i) {
			int index = WindowedResolutionComboBox.AddString(WindowedModes[i].description);
			WindowedResolutionComboBox.SetItemData(index, i);
			if (WindowedModes[i].width == config.video.windowedWidth && WindowedModes[i].height == config.video.windowedHeight)
				WindowedResolutionComboBox.SetCurSel(index);
		}
		if (WindowedResolutionComboBox.GetCount() > 0 && WindowedResolutionComboBox.GetCurSel() < 0) {
			WindowedResolutionComboBox.AddString(FormatStrW(L"%d x %d", config.video.windowedWidth, config.video.windowedHeight).c_str());
			WindowedResolutionComboBox.SetCurSel(numWindowedModes);
		}
	}

	CButton overscanCheckBox(GetDlgItem(IDC_CHK_OVERSCAN));
	overscanCheckBox.SetCheck(config.frameBufferEmulation.enableOverscan != 0 ? BST_CHECKED : BST_UNCHECKED);
	OnOverscan(0, 0, NULL);

	m_OverscanTabs[0]->SetValue(
		config.frameBufferEmulation.overscanNTSC.left,
		config.frameBufferEmulation.overscanNTSC.right,
		config.frameBufferEmulation.overscanNTSC.top,
		config.frameBufferEmulation.overscanNTSC.bottom
	);

	m_OverscanTabs[1]->SetValue(
		config.frameBufferEmulation.overscanPAL.left,
		config.frameBufferEmulation.overscanPAL.right,
		config.frameBufferEmulation.overscanPAL.top,
		config.frameBufferEmulation.overscanPAL.bottom
	);

	StringList fullscreenModesList, fullscreenRatesList;
	int fullscreenMode, fullscreenRate;
	fillFullscreenResolutionsList(fullscreenModesList, fullscreenMode, fullscreenRatesList, fullscreenRate);

	CComboBox fullScreenResolutionComboBox(GetDlgItem(IDC_CMB_FULL_SCREEN_RES));
	fullScreenResolutionComboBox.ResetContent();
	for (size_t i = 0, n = fullscreenModesList.size(); i < n; i++) {
		std::wstring fullscreenModeStr(fullscreenModesList[i].begin(), fullscreenModesList[i].end());
		int index = fullScreenResolutionComboBox.AddString(fullscreenModeStr.c_str());
		if (fullscreenMode == i)
			fullScreenResolutionComboBox.SetCurSel(index);
	}
	CComboBox RefreshRateComboBox(GetDlgItem(IDC_CMB_REFRESH_RATE));
	RefreshRateComboBox.ResetContent();
	for (size_t i = 0, n = fullscreenRatesList.size(); i < n; i++) {
		std::wstring fullscreenRateStr(fullscreenRatesList[i].begin(), fullscreenRatesList[i].end());
		int index = RefreshRateComboBox.AddString(fullscreenRateStr.c_str());
		if (fullscreenRate == i)
			RefreshRateComboBox.SetCurSel(index);
	}
	const unsigned int multisampling = config.video.fxaa == 0 && config.video.multisampling > 0
		? config.video.multisampling
		: 8;
	m_AliasingSlider.SetPos(powof(multisampling));
	std::wstring AliasingText = FormatStrW(L"%dx", multisampling);
	CWindow(GetDlgItem(IDC_ALIASING_LABEL)).SetWindowTextW(AliasingText.c_str());

	if (config.frameBufferEmulation.N64DepthCompare == 0) {
		if (!m_AAInfoWarning) {
			HideMSAADepthWarning(true);
			DisallowMSAA(false);
			RedrawWindow();
		}
	} else {
		if (m_AAInfoWarning) {
			HideMSAADepthWarning(false);
			DisallowMSAA(true);
			RedrawWindow();
		}
	}

	CButton(GetDlgItem(IDC_NOAA_RADIO)).SetCheck(config.video.multisampling == 0 && config.video.fxaa == 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_FXAA_RADIO)).SetCheck(config.video.fxaa != 0 && config.video.multisampling == 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_MSAA_RADIO)).SetCheck(config.video.fxaa == 0 && config.video.multisampling != 0 ? BST_CHECKED : BST_UNCHECKED);
	m_AnisotropicSlider.SetPos(config.texture.maxAnisotropy);
	CWindow(GetDlgItem(IDC_ANISOTROPIC_LABEL)).SetWindowTextW(FormatStrW(L"%dx", m_AnisotropicSlider.GetPos()).c_str());

	CButton(GetDlgItem(IDC_CHK_VERTICAL_SYNC)).SetCheck(config.video.verticalSync != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_THREADED_VIDEO)).SetCheck(config.video.threadedVideo != 0 ? BST_CHECKED : BST_UNCHECKED);

	CButton(GetDlgItem(IDC_BILINEAR_3POINT)).SetCheck(config.texture.bilinearMode == BILINEAR_3POINT ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_BILINEAR_STANDARD)).SetCheck(config.texture.bilinearMode == BILINEAR_STANDARD ? BST_CHECKED : BST_UNCHECKED);

	CComboBox aspectComboBox(GetDlgItem(IDC_CMB_ASPECT_RATIO));
	switch (config.frameBufferEmulation.aspect) {
	case Config::aStretch: aspectComboBox.SetCurSel(2); break;
	case Config::a43: aspectComboBox.SetCurSel(0); break;
	case Config::a169: aspectComboBox.SetCurSel(1); break;
	case Config::aAdjust: aspectComboBox.SetCurSel(3); break;
	}

	CComboBox(GetDlgItem(IDC_CMB_PATTERN)).SetCurSel(config.generalEmulation.rdramImageDitheringMode);
	CButton(GetDlgItem(IDC_CHK_APPLY_TO_OUTPUT)).SetCheck(config.generalEmulation.enableDitheringPattern != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_5BIT_QUANTIZATION)).SetCheck(config.generalEmulation.enableDitheringQuantization != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_HIRES_NOISE)).SetCheck(config.generalEmulation.enableHiresNoiseDithering != 0 ? BST_CHECKED : BST_UNCHECKED);

	CComboBox translationsComboBox(GetDlgItem(IDC_CMB_LANGUAGE));
	translationsComboBox.SetCurSel(-1);
	int englishIndx = -1;
	for (int i = 0, n = translationsComboBox.GetCount(); i < n; i++) {
		const char * translations = (const char *)translationsComboBox.GetItemDataPtr(i);
		if (_stricmp(translations, "gliden64_en.Lang") == 0)
			englishIndx = i;

		if (config.translationFile == translations) {
			translationsComboBox.SetCurSel(i);
			break;
		}
	} // default: attempt to use gliden64_en.Lang
	if (englishIndx >= 0 && translationsComboBox.GetCurSel() < 0)
		translationsComboBox.SetCurSel(englishIndx);
	else if (translationsComboBox.GetCurSel() < 0) // gliden64_en.Lang not found; select hardcoded english
		translationsComboBox.SetCurSel(translationsComboBox.FindString(-1, englishLang));
}

void CVideoTab::SaveSettings()
{
	getFullscreenResolutions(CComboBox(GetDlgItem(IDC_CMB_FULL_SCREEN_RES)).GetCurSel(), config.video.fullscreenWidth, config.video.fullscreenHeight);
	getFullscreenRefreshRate(CComboBox(GetDlgItem(IDC_CMB_REFRESH_RATE)).GetCurSel(), config.video.fullscreenRefresh);

	CComboBox WindowResCB(GetDlgItem(IDC_CMB_WINDOWED_RESOLUTION));
	CString WindowResStr;
	WindowResCB.GetWindowText(WindowResStr);
	std::string resolution(CW2A(WindowResStr.GetString()));
	std::regex parseRes("(\\d+) ?x ?(\\d+)");
	std::smatch tokens;
	if (std::regex_search(resolution, tokens, parseRes) && tokens.size() > 1) {
		// matches w x h where w is 300-7999 and h is 200-3999
		if (range<300, 8000>::contains(std::stoi(tokens[1])) && range<200, 4000>::contains(std::stoi(tokens[2]))) {
			config.video.windowedWidth = std::stoi(tokens[1]);
			config.video.windowedHeight = std::stoi(tokens[2]);
		}
	}

	int AspectIndx = CComboBox(GetDlgItem(IDC_CMB_ASPECT_RATIO)).GetCurSel();
	if (AspectIndx == 2) { config.frameBufferEmulation.aspect = Config::aStretch; }
	else if (AspectIndx == 0) { config.frameBufferEmulation.aspect = Config::a43; }
	else if (AspectIndx == 1) { config.frameBufferEmulation.aspect = Config::a169; }
	else if (AspectIndx == 3) { config.frameBufferEmulation.aspect = Config::aAdjust; }

	config.video.verticalSync = CButton(GetDlgItem(IDC_CHK_VERTICAL_SYNC)).GetCheck() == BST_CHECKED;
	config.video.threadedVideo = CButton(GetDlgItem(IDC_CHK_THREADED_VIDEO)).GetCheck() == BST_CHECKED;
	config.frameBufferEmulation.enableOverscan = CButton(GetDlgItem(IDC_CHK_OVERSCAN)).GetCheck() == BST_CHECKED;

	m_OverscanTabs[0]->GetValue(
		config.frameBufferEmulation.overscanNTSC.left,
		config.frameBufferEmulation.overscanNTSC.right,
		config.frameBufferEmulation.overscanNTSC.top,
		config.frameBufferEmulation.overscanNTSC.bottom
	);

	m_OverscanTabs[1]->GetValue(
		config.frameBufferEmulation.overscanPAL.left,
		config.frameBufferEmulation.overscanPAL.right,
		config.frameBufferEmulation.overscanPAL.top,
		config.frameBufferEmulation.overscanPAL.bottom
	);

	config.video.fxaa = CButton(GetDlgItem(IDC_FXAA_RADIO)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.video.multisampling =
		(CButton(GetDlgItem(IDC_FXAA_RADIO)).GetCheck() == BST_CHECKED
			|| CComboBox(m_FrameBufferTab.GetDlgItem(IDC_CMB_N64_DEPTH_COMPARE)).GetCurSel() != 0
			|| CButton(GetDlgItem(IDC_NOAA_RADIO)).GetCheck() == BST_CHECKED
			) ? 0
		: pow2(m_AliasingSlider.GetPos());
	config.texture.maxAnisotropy = m_AnisotropicSlider.GetPos();

	if (CButton(GetDlgItem(IDC_BILINEAR_3POINT)).GetCheck() == BST_CHECKED)
		config.texture.bilinearMode = BILINEAR_3POINT;
	if (CButton(GetDlgItem(IDC_BILINEAR_STANDARD)).GetCheck() == BST_CHECKED)
		config.texture.bilinearMode = BILINEAR_STANDARD;

	config.generalEmulation.rdramImageDitheringMode = CComboBox(GetDlgItem(IDC_CMB_PATTERN)).GetCurSel();
	config.generalEmulation.enableDitheringPattern = CButton(GetDlgItem(IDC_CHK_APPLY_TO_OUTPUT)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.generalEmulation.enableDitheringQuantization = CButton(GetDlgItem(IDC_CHK_5BIT_QUANTIZATION)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.generalEmulation.enableHiresNoiseDithering = CButton(GetDlgItem(IDC_CHK_HIRES_NOISE)).GetCheck() == BST_CHECKED ? 1 : 0;

	CComboBox translationsComboBox(GetDlgItem(IDC_CMB_LANGUAGE));
	config.translationFile = (const char *)translationsComboBox.GetItemDataPtr(translationsComboBox.GetCurSel());
}
