#pragma once
#include "wtl.h"
#include "ConfigDlg.h"
#include "../Config.h"
#include "Settings.h"
#include "config-video.h"
#include "config-emulation.h"
#include "config-framebuffer.h"
#include "config-texture.h"
#include "config-osd.h"
#include "config-debug.h"
#include "util.h"
#include "InputDialog.h"
#include "Language.h"

CConfigDlg::CConfigDlg() :
	m_blockReInit(false),
	m_EmulationTab(NULL),
	m_Saved(false),
	m_TabLeft(0),
	m_ProfileLeft(0)
{
}

CConfigDlg::~CConfigDlg() {
	m_EmulationTab = NULL;
	for (size_t i = 0; i < m_TabWindows.size(); i++)
		delete m_TabWindows[i];
	m_TabWindows.clear();
}

void CConfigDlg::setIniPath(const std::string & IniPath) {
	m_strIniPath = IniPath;
}

void CConfigDlg::setRomName(const char * RomName) {
	m_romName = RomName == NULL || strlen(RomName) == 0 ? NULL : RomName;
}

LRESULT CConfigDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HICON hIcon = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_FrameBufferTab = new CFrameBufferTab();
	m_VideoTab = new CVideoTab(*this, *m_FrameBufferTab, m_strIniPath.c_str());
	m_EmulationTab = new CEmulationTab(*this);
	m_OsdTab = new COsdTab();

	m_Tabs.Attach(GetDlgItem(IDC_TABS));
	AddTab(TAB_VIDEO, m_VideoTab);
	AddTab(TAB_EMULATION, m_EmulationTab);
	AddTab(TAB_FRAME_BUFFER, m_FrameBufferTab);
	AddTab(TAB_TEXTURE_ENHANCEMENT, new CTextureEnhancementTab);
	AddTab(TAB_OSD, m_OsdTab);
#ifdef DEBUG_DUMP
	AddTab(TAB_DEBUG, new CDebugTab);
#endif

	RECT Rect;
	GetDlgItem(IDC_TABS).GetWindowRect(&Rect);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
	m_TabLeft = Rect.left;

	if (m_romName != NULL) {
		std::wstring RomName(ToUTF16(m_romName));
		CWindow dlgItem = GetDlgItem(IDC_GAME_PROFILE_NAME);
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		dc.SelectFont(dlgItem.GetFont());
		SIZE size;
		dc.GetTextExtent(RomName.c_str(), static_cast<int>(RomName.length()), &size);

		RECT Rect;
		dlgItem.GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
		Rect.right = Rect.left + size.cx;
		dlgItem.MoveWindow(&Rect);
		dlgItem.SetWindowText(RomName.c_str());

		m_ProfileLeft = Rect.right + 10;

		CButton(GetDlgItem(IDC_GAME_PROFILE)).SetCheck(BST_CHECKED);
		CButton(GetDlgItem(IDC_USE_PROFILE)).SetCheck(BST_UNCHECKED);
	} else {
		CButton(GetDlgItem(IDC_GAME_PROFILE)).SetCheck(BST_UNCHECKED);
		CButton(GetDlgItem(IDC_USE_PROFILE)).SetCheck(BST_CHECKED);
		GetDlgItem(IDC_SETTINGS_PROFILE_STATIC).GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
		m_ProfileLeft = Rect.left;
	}

	ProfileList Profiles = getProfiles(m_strIniPath.c_str());
	std::string CurrentProfile = getCurrentProfile(m_strIniPath.c_str());
	CComboBox profilesComboBox(GetDlgItem(IDC_PROFILE));
	profilesComboBox.ResetContent();
	for (ProfileList::const_iterator itr = Profiles.begin(); itr != Profiles.end(); itr++) {
		int Index = profilesComboBox.AddString(ToUTF16(itr->c_str()).c_str());
		if (CurrentProfile == *itr)
			profilesComboBox.SetCurSel(Index);
	}
	profilesComboBox.AddString(L"New...");
	GetDlgItem(IDC_REMOVE_PROFILE).EnableWindow(profilesComboBox.GetCount() > 2);
	Init();
	return 0;
}

void CConfigDlg::OnCustomSettingsToggled(bool checked) {
	if (m_hWnd == NULL)
		return;

	checked = m_romName != NULL ? checked : false;
	GetDlgItem(IDC_GAME_PROFILE).ShowWindow(checked ? SW_SHOWNORMAL : SW_HIDE);
	GetDlgItem(IDC_SAVE_SETTINGS_STATIC).ShowWindow(checked ? SW_SHOWNORMAL : SW_HIDE);
	GetDlgItem(IDC_GAME_PROFILE_NAME).ShowWindow(checked ? SW_SHOWNORMAL : SW_HIDE);
	GetDlgItem(IDC_USE_PROFILE).ShowWindow(checked ? SW_SHOWNORMAL : SW_HIDE);

	int32_t Move = 0;
	if (checked) {
		RECT Rect;
		CWindow UseProfile = GetDlgItem(IDC_USE_PROFILE);
		UseProfile.GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
		Move = Rect.left - m_ProfileLeft;
		if (Move != 0) {
			Rect.left -= Move;
			Rect.right -= Move;
			UseProfile.MoveWindow(&Rect);
		}
		uint32_t Left = Rect.right + 2;

		CWindow ProfileStatic = GetDlgItem(IDC_SETTINGS_PROFILE_STATIC);
		ProfileStatic.GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);
		Move = Rect.left - Left;
	} else {
		RECT Rect;
		GetDlgItem(IDC_SETTINGS_PROFILE_STATIC).GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);

		Move = Rect.left - m_TabLeft;
	}
	if (Move != 0) {
		int nID[] = {
			IDC_SETTINGS_PROFILE_STATIC,
			IDC_PROFILE,
			IDC_REMOVE_PROFILE,
		};

		RECT Rect;
		GetDlgItem(nID[0]).GetWindowRect(&Rect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);

		for (size_t i = 0, n = sizeof(nID) / sizeof(nID[0]); i < n; i++) {
			CWindow window = GetDlgItem(nID[i]);
			window.GetWindowRect(&Rect);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&Rect, 2);

			Rect.left -= Move;
			Rect.right -= Move;
			window.MoveWindow(&Rect);
		}
	}
}

LRESULT CConfigDlg::OnProfileChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hwnd*/, BOOL& /*bHandled*/) {
	CComboBox profilesComboBox(GetDlgItem(IDC_PROFILE));
	int nIndex = profilesComboBox.GetCurSel();
	if (nIndex < 0) { return 0; }

	int nLen = profilesComboBox.GetLBTextLen(nIndex);
	if (nLen == CB_ERR) { return FALSE; }

	std::wstring Profile;
	Profile.resize(nLen);
	profilesComboBox.GetLBText(nIndex, (wchar_t *)Profile.data());

	CButton(GetDlgItem(IDC_USE_PROFILE)).SetCheck(BST_CHECKED);
	CButton(GetDlgItem(IDC_GAME_PROFILE)).SetCheck(BST_UNCHECKED);
	if (Profile == L"New...") {
		bool ok;
		std::string switchToProfile = getCurrentProfile(m_strIniPath.c_str());
		std::string newProfile = FromUTF16(CInputDialog::getText(L"New Profile", L"New profile name:", ok).c_str());
		CComboBox profilesComboBox(GetDlgItem(IDC_PROFILE));
		if (ok) {
			ProfileList Profiles = getProfiles(m_strIniPath.c_str());
			if (strcmp(newProfile.c_str(), "New...") == 0) {
				MessageBox(L"New settings profiles cannot be called \"New...\".", L"New Profile", MB_OK | MB_ICONWARNING);
			}
			else if (newProfile.empty()) {
				MessageBox(L"Please type a name for your new settings profile.", L"New Profile", MB_OK | MB_ICONWARNING);
			}
			else if (Profiles.find(newProfile.c_str()) != Profiles.end()) {
				MessageBox(L"This settings profile already exists.", L"New Profile", MB_OK | MB_ICONWARNING);
			} else {
				profilesComboBox.AddString(ToUTF16(newProfile.c_str()).c_str());
				addProfile(m_strIniPath.c_str(), newProfile.c_str());
				GetDlgItem(IDC_REMOVE_PROFILE).EnableWindow(profilesComboBox.GetCount() > 2);
				switchToProfile = newProfile;
			}
		}
		for (int i = 0, n = profilesComboBox.GetCount(); i < n; ++i) {
			std::wstring Profile;
			Profile.resize(profilesComboBox.GetLBTextLen(i) + 1);
			profilesComboBox.GetLBText(i, (wchar_t *)Profile.data());

			if (strcmp(FromUTF16(Profile.c_str()).c_str(),switchToProfile.c_str()) == 0) {
				profilesComboBox.SetCurSel(i);
				break;
			}
		}
		return 0;
	}
	changeProfile(m_strIniPath.c_str(), FromUTF16(Profile.c_str()).c_str());
	Init(true);
	return 0;
}

void CConfigDlg::SaveSettings() {
	m_Saved = true;
	for (size_t i = 0; i < m_TabWindows.size(); i++)
		m_TabWindows[i]->SaveSettings();
    
	if (config.generalEmulation.enableCustomSettings && CButton(GetDlgItem(IDC_GAME_PROFILE)).GetCheck() == BST_CHECKED && m_romName != nullptr)
		saveCustomRomSettings(m_strIniPath.c_str(), m_romName);
    else
        writeSettings(m_strIniPath.c_str());
    
}

LRESULT CConfigDlg::OnRestoreDefaults(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	int Res = MessageBox(L"Are you sure you want to reset all settings to default?", L"Restore Defaults?", MB_YESNO | MB_ICONWARNING);
	if (Res == IDYES) {
		const u32 enableCustomSettings = config.generalEmulation.enableCustomSettings;
		config.resetToDefaults();
		config.generalEmulation.enableCustomSettings = enableCustomSettings;
		setRomName(m_romName);
		Init();
	}
	return 0;
}

LRESULT CConfigDlg::OnGameProfile(UINT /*Code*/, int /*id*/, HWND /*ctl*/) {
	CButton(GetDlgItem(IDC_USE_PROFILE)).SetCheck(BST_UNCHECKED);
	Init(true, true);
	return 0;
}

LRESULT CConfigDlg::OnUseProfile(UINT /*Code*/, int /*id*/, HWND /*ctl*/) {
	CButton(GetDlgItem(IDC_GAME_PROFILE)).SetCheck(BST_UNCHECKED);
	Init(true, true);
	return 0;
}

LRESULT CConfigDlg::OnRemoveProfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CComboBox profilesComboBox(GetDlgItem(IDC_PROFILE));
	if (profilesComboBox.GetCount() <= 2)
		return 0;

	int nIndex = profilesComboBox.GetCurSel();
	std::wstring profile;
	profile.resize(profilesComboBox.GetLBTextLen(nIndex) + 1);
	profilesComboBox.GetLBText(nIndex, (wchar_t *)profile.data());

	ProfileList Profiles = getProfiles(m_strIniPath.c_str());
	if (Profiles.find(FromUTF16(profile.c_str()).c_str()) == Profiles.end())
		return 0;

	std::wstring msg = L"Are you sure you want to remove the settings profile \"";
	msg += profile.c_str();
	msg += L"\"?";
	if (MessageBox(msg.c_str(), L"Remove Profile", MB_YESNO | MB_ICONWARNING) == IDYES) {
		removeProfile(m_strIniPath.c_str(), FromUTF16(profile.c_str()).c_str());
		for (int i = 0, n = profilesComboBox.GetCount(); i < n; ++i) {
			std::wstring ProfileItem;
			ProfileItem.resize(profilesComboBox.GetLBTextLen(i) + 1);
			profilesComboBox.GetLBText(i, (wchar_t *)ProfileItem.data());

			if (wcscmp(ProfileItem.c_str(),profile.c_str()) == 0) {
				profilesComboBox.DeleteString(i);
				break;
			}
		}
		for (int i = 0, n = profilesComboBox.GetCount(); i < n; ++i) {
			std::wstring ProfileItem;
			ProfileItem.resize(profilesComboBox.GetLBTextLen(i) + 1);
			profilesComboBox.GetLBText(i, (wchar_t *)ProfileItem.data());
			if (wcscmp(ProfileItem.c_str(),L"New...") != 0) {
				profilesComboBox.SetCurSel(i);
				changeProfile(m_strIniPath.c_str(), FromUTF16(ProfileItem.c_str()).c_str());
				Init(true);
				break;
			}
		}
		GetDlgItem(IDC_REMOVE_PROFILE).EnableWindow(profilesComboBox.GetCount() > 2);
	}
	return 0;
}

LRESULT CConfigDlg::OnSaveClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	SaveSettings();
	EndDialog(wID);
	return 0;
}

LRESULT CConfigDlg::OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	SaveSettings();
	return 0;
}

LRESULT CConfigDlg::OnTabChange(NMHDR* /*pNMHDR*/) {
	ShowTab(m_Tabs.GetCurSel());
	return FALSE;
}

LRESULT CConfigDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(wID);
	return 0;
}

void CConfigDlg::Init(bool reInit, bool blockCustomSettings) {
	if (m_blockReInit)
		return;

	m_blockReInit = true;
	bool CustomSettings = m_EmulationTab != NULL && CButton(m_EmulationTab->GetDlgItem(IDC_CHK_USE_PER_GAME)).GetCheck() == BST_CHECKED;

	if (reInit && m_romName != NULL && CustomSettings && CButton(GetDlgItem(IDC_GAME_PROFILE)).GetCheck() == BST_CHECKED) {
		loadCustomRomSettings(m_strIniPath.c_str(), m_romName);
	} else if (reInit) {
		loadSettings(m_strIniPath.c_str());
	}

	for (size_t i = 0; i < m_TabWindows.size(); i++)
		m_TabWindows[i]->LoadSettings(blockCustomSettings);

	for (size_t i = 0; i < m_TabWindows.size(); i++)
		m_TabWindows[i]->ApplyLanguage();
	ApplyLanguage();
	m_blockReInit = false;
}

CRect CConfigDlg::GetTabRect() {
	CRect TabRect;
	m_Tabs.GetWindowRect(&TabRect);
	ScreenToClient(&TabRect);
	m_Tabs.AdjustRect(FALSE, &TabRect);
	return TabRect;
}

void CConfigDlg::AddTab(languageStringID caption, CConfigTab * tab) {
	m_Tabs.AddItem(TCIF_TEXT | TCIF_PARAM, wGS(caption).c_str(), 0, caption);
	tab->Create(m_hWnd, 0);
	tab->SetWindowPos(m_hWnd, 0, 0, 0, 0, SWP_HIDEWINDOW);
	m_TabWindows.push_back(tab);

	if (m_TabWindows.size() == 1)
		ShowTab(0);
}

void CConfigDlg::ShowTab(int nPage) {
	if (nPage == 4 && !m_OsdTab->m_FontsLoaded) { // OSD tab
		m_OsdTab->LoadFonts();
	}

	for (size_t i = 0; i < m_TabWindows.size(); i++)
		m_TabWindows[i]->ShowWindow(SW_HIDE);

	CRect TabRect = GetTabRect();
	m_TabWindows[nPage]->SetWindowPos(HWND_TOP, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_SHOWWINDOW);

	CRect WinRect, ClientRect;
	m_TabWindows[nPage]->GetWindowRect(WinRect);
	m_TabWindows[nPage]->GetClientRect(ClientRect);

	m_Tabs.RedrawWindow();
}

void CConfigDlg::SetLanguage(const std::string & language) {
	LoadCurrentStrings(m_strIniPath.c_str(), language);
	for (int i = 0, n = m_Tabs.GetItemCount(); i < n; i++) {
		TCITEM tci = { 0 };
		tci.mask = TCIF_PARAM;
		m_Tabs.GetItem(i, &tci);
		if (tci.lParam != 0) {
			tci.mask = TCIF_TEXT;
			std::wstring caption = wGS((languageStringID)tci.lParam);
			tci.pszText = (LPWSTR)caption.c_str();
			m_Tabs.SetItem(i, &tci);
		}
	}
	for (size_t i = 0; i < m_TabWindows.size(); i++) 
		m_TabWindows[i]->ApplyLanguage();
	ApplyLanguage();
}

void CConfigDlg::ApplyLanguage(void)
{
	SetDlgItemTextW(IDC_SAVE_SETTINGS_STATIC, wGS(CFG_SAVE_SETTINGS_FOR).c_str());
	SetDlgItemTextW(IDC_SETTINGS_PROFILE_STATIC, wGS(CFG_SETTINGS_PROFILE).c_str());
	SetDlgItemTextW(IDC_REMOVE_PROFILE, wGS(CFG_REMOVE).c_str());
	SetDlgItemTextW(ID_RESTORE_DEFAULTS, wGS(CFG_RESTORE_DEFAULTS).c_str());
	SetDlgItemTextW(ID_SAVECLOSE, wGS(CFG_SAVE_AND_CLOSE).c_str());
	SetDlgItemTextW(ID_SAVE, wGS(CFG_SAVE).c_str());
	SetDlgItemTextW(IDCANCEL, wGS(CFG_CLOSE).c_str());
}

class GlideN64WtlModule :
	public CAppModule
{
public:
	GlideN64WtlModule(HINSTANCE hinst) {
		Init(NULL, hinst);
	}
	virtual ~GlideN64WtlModule(void) {
		Term();
	}
};

GlideN64WtlModule * WtlModule = NULL;

void ConfigInit(void * hinst) {
	WtlModule = new GlideN64WtlModule((HINSTANCE)hinst);
}

void ConfigCleanup(void) {
	if (WtlModule) {
		delete WtlModule;
		WtlModule = NULL;
	}
}
