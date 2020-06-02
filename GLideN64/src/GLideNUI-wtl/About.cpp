#include "About.h"
#include "resource.h"
#include "Language.h"

extern HINSTANCE hInstance;

class CAboutContributersTab :
	public CAboutTab
{
public:
	BEGIN_MSG_MAP(CAboutContributersTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	CAboutContributersTab() :
		CAboutTab(IDD_TAB_CONTRIBUTORS)
	{
	}

	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
	{
		const TCHAR Contributors[] =
		{
			L"Logan McNaughton\r\n\r\n"
			L"Francisco Zurita\r\n\r\n"
			L"gizmo98\r\n\r\n"
			L"purplemarshmallow\r\n\r\n"
			L"zilmar\r\n\r\n"
			L"matthewharvey\r\n\r\n"
			L"lioncash\r\n\r\n"
			L"Predator82Germany\r\n\r\n"
			L"AmbientMalice\r\n\r\n"
			L"baptiste0602\r\n\r\n"
			L"Gilles Siberlin\r\n\r\n"
			L"Daniel Eck\r\n\r\n"
			L"Víctor \"IlDucci\"\r\n\r\n"
			L"orbea\r\n\r\n"
			L"BenjaminSiskoo\r\n\r\n"
			L"ptitSeb\r\n\r\n"
			L"Kimberly J.Ortega\r\n\r\n"
			L"Maxime Morel\r\n\r\n"
			L"tony971\r\n\r\n"
			L"SigmaVirus\r\n\r\n"
			L"Jools Wills\r\n\r\n"
			L"Nekokabu\r\n\r\n"
			L"nicklauslittle\r\n\r\n"
			L"Nebuleon\r\n\r\n"
			L"sergiobenrocha2\r\n\r\n"
			L"Michał Durak\r\n\r\n"
			L"Mushman"
		};
		GetDlgItem(IDC_CONTRIBUTORS).SetWindowText(Contributors);
		return true;
	}
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if ((HWND)lParam == GetDlgItem(IDC_CONTRIBUTORS))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (LRESULT)GetSysColorBrush(COLOR_3DFACE);
		}
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
};

class CAboutFundersTab :
	public CAboutTab
{
public:
	BEGIN_MSG_MAP(CAboutFundersTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	CAboutFundersTab() :
		CAboutTab(IDD_TAB_FUNDERS)
	{
	}

	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
	{
		const TCHAR Funders1[] =
		{
			L"zolcos\r\n"
			L"Mush Man\r\n"
			L"nesplayer4life\r\n"
			L"neko9876\r\n"
			L"AnthonyHeathcoat\r\n"
			L"daman6009\r\n"
			L"Paul Lamb\r\n"
			L"zilmar\r\n"
		};
		std::wstring FundersList =
		{
			L"Ryan Rosser\r\n"
			L"Amadeus Sterl\r\n"
			L"Narann\r\n"
			L"camara_luiz\r\n"
			L"weinerschnitzel\r\n"
			L"microdev\r\n"
			L"Thomas Ginelli\r\n"
			L"ace977\r\n"
			L"patryk.szalanski\r\n"
			L"Detomine\r\n"
			L"itasovski\r\n"
			L"keithclark1985\r\n"
			L"josephrmoore\r\n"
			L"fckyourlies\r\n"
			L"dougforr\r\n"
			L"camdenfurse\r\n"
			L"grandslam810\r\n"
			L"rictic\r\n"
			L"Fred Lambes\r\n"
			L"David Vercruyssen\r\n"
			L"danielgormly\r\n"
			L"lukecool\r\n"
			L"rhilsky\r\n"
			L"phillipstuerzl\r\n"
			L"killjoy1337\r\n"
			L"ratop46\r\n"
			L"william.a.moore\r\n"
			L"RSP16\r\n"
			L"kzidek127\r\n"
			L"Dan Holberg\r\n"
			L"178amm\r\n"
			L"peterchrjoergensen\r\n"
			L"hill_jm\r\n"
			L"petercullenbryan\r\n"
			L"Christopher M Rock\r\n"
			L"Kenny.R.Mitchell\r\n"
			L"Kevin Grasso\r\n"
			L"mtgyure\r\n"
			L"Anthony Heathcoat\r\n"
			L"Liam Burns\r\n"
			L"Steven Impson\r\n"
			L"Gwyn.Whieldon\r\n"
			L"hipnotoad\r\n"
			L"shmuklidooha\r\n"
			L"bcanard123\r\n"
			L"Ben Slater\r\n"
			L"Mike Nagy\r\n"
			L"littlegreendude55\r\n"
			L"Jay Loring\r\n"
			L"Damion D\r\n"
			L"heranbago\r\n"
			L"baptiste.guilbert\r\n"
			L"shadowpower69\r\n"
			L"j.mcguirk72\r\n"
			L"Peter Greenwood\r\n"
			L"fla56\r\n"
			L"Sergio\r\n"
			L"theboy_181\r\n"
			L"Jindo Fox\r\n"
			L"s1n.pcc\r\n"
			L"rafaelvasco\r\n"
			L"copileo\r\n"
			L"hugues.fabien\r\n"
			L"seanmcm157\r\n"
			L"David Morris\r\n"
			L"Jason Lightner\r\n"
			L"olivier_crepin77\r\n"
			L"Paul Lamb\r\n"
			L"thegump2.0\r\n"
			L"Bates\r\n"
			L"cdoublejj\r\n"
			L"buddybenj\r\n"
			L"don.carmical\r\n"
			L"kyussgreen\r\n"
			L"info1092\r\n"
			L"YQ\r\n"
			L"Allan Nordhøy\r\n"
			L"christian010\r\n"
			L"creuseur2patateradio\r\n"
			L"chrisbevanlee\r\n"
			L"theschklingen\r\n"
			L"Thomas Lindstrøm\r\n"
			L"Djipi\r\n"
			L"Dartus\r\n"
			L"Oscar Abraham\r\n"
			L"nwstrathdee\r\n"
			L"will7046\r\n"
			L"Richard42\r\n"
			L"V1del\r\n"
			L"AnthonyBentley\r\n"
			L"buddybenj\r\n"
			L"nickshooter251\r\n"
			L"sicurella12\r\n"
			L"jcspringer\r\n"
			L"Gru So\r\n"
			L"Vinícius dos Santos Oliveira\r\n"
			L"Jimmy Haugh\r\n"
			L"Malcolm\r\n"
			L"Alex Strange\r\n"
			L"Espen Jensen\r\n"
			L"m.johnsondelta\r\n"
			L"alexzandar.toxic2\r\n"
			L"Ben Slater\r\n"
			L"WC-Predator\r\n"
			L"Mush Man\r\n"
			L"Ben Slater\r\n"
			L"aznlucidx\r\n"
			L"Nathan Dick\r\n"
			L"paulanocom\r\n"
			L"Ryan Rosser\r\n"
			L"nekow42\r\n"
			L"mgos1\r\n"
			L"ian.macdonald996\r\n"
			L"itasovski\r\n"
			L"vikingpower1\r\n"
			L"DukeX007X\r\n"
			L"palaciosgabriel\r\n"
			L"Franz-Josef Haider\r\n"
			L"e-male\r\n"
			L"aweath\r\n"
			L"famicom4\r\n"
			L"Keith_at_UMR\r\n"
			L"sweatypickle\r\n"
			L"jeremydmiller\r\n\r\n"
		};
		FundersList += wGS(ABOUT_AND_MORE);
		CWindow Funders = GetDlgItem(IDC_FUNDERS);
		Funders.SetWindowText(Funders1);
		m_SubtitleFont.Apply(m_hWnd, CWindowFont::typeBold | CWindowFont::typeSubheading, IDC_FUNDERS);
		GetDlgItem(IDC_FUNDERS_LIST).SetWindowText(FundersList.c_str());
		return true;
	};
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if ((HWND)lParam == GetDlgItem(IDC_FUNDERS_LIST))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (LRESULT)GetSysColorBrush(COLOR_3DFACE);
		}
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
};

class CAboutCreditsTab :
	public CAboutTab
{
public:
	BEGIN_MSG_MAP(CAboutCreditsTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
	END_MSG_MAP()

	CAboutCreditsTab() :
		CAboutTab(IDD_TAB_CREDITS)
	{
	}
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
	{
		SetDlgItemTextW(IDC_TXT_AUTHOR_GLN64, wGS(ABOUT_AUTHOR_GLN64).c_str());
		SetDlgItemTextW(IDC_TXT_AUTHOR_GLES2N64, wGS(ABOUT_AUTHOR_GLES2N64).c_str());
		SetDlgItemTextW(IDC_TXT_AUTHOR_GLIDEHQ, wGS(ABOUT_AUTHOR_GLIDEHQ).c_str());
		SetDlgItemTextW(IDC_TXT_AUTHOR_Z64, wGS(ABOUT_AUTHOR_Z64).c_str());

		const UINT Creditors[] =
		{
			IDC_ORKIN,
			IDC_YONGZH,
			IDC_HIROSHI,
			IDC_ZIGGY
		};
		for (const UINT &Creditor : Creditors) {
		  m_SubtitleFont.Apply(m_hWnd, CWindowFont::typeBold | CWindowFont::typeSubheading, Creditor);
		}
		return true;
	};
};

CAboutTab::CAboutTab(uint32_t _IDD) :
	IDD(_IDD)
{
}

CAboutTab::~CAboutTab()
{
}

BOOL CAboutTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	SetDlgItemTextW(IDC_TXT_NEW_GEN, wGS(ABOUT_NEXT_GENERATION).c_str());
	SetDlgItemTextW(IDC_TXT_AUTHORS, wGS(ABOUT_AUTHORS).c_str());
	SetDlgItemTextW(IDC_TXT_DEVELOPER, wGS(ABOUT_DEVELOPER).c_str());
	SetDlgItemTextW(IDC_TXT_BETA_TESTER, wGS(ABOUT_BETA_TESTER).c_str());
	SetDlgItemTextW(IDC_TXT_GUI_DESIGNER, wGS(ABOUT_GUI_DESIGNER).c_str());

	const UINT Authors[] =
	{
		IDC_SERGEY,
		IDC_OLIVIER,
		IDC_RYAN
	};
	for (const UINT &Author : Authors) {
	  m_SubtitleFont.Apply(m_hWnd, CWindowFont::typeBold | CWindowFont::typeSubheading, Author);
	}
	return true;
}

LRESULT CAboutTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}

CAboutDlg::~CAboutDlg()
{
	for (size_t i = 0, n = m_TabWindows.size(); i < n; i++)
	{
		delete m_TabWindows[i];
	}
	m_TabWindows.clear();
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/ , LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(wGS(ABOUT_ABOUT_TITLE).c_str());

	HICON hIcon = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDI_APPICON, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	SIZEF dpiScale = DpiScale(m_hWnd);
	SIZE iconSz = { (LONG)(48 * dpiScale.cx), (LONG)(48 * dpiScale.cy) };

	m_TitleFont.Apply(m_hWnd, CWindowFont::typeBold | CWindowFont::typeHeading, IDC_ABOUT_TITLE);
	m_AboutIcon.SubclassWindow(GetDlgItem(IDC_ABOUT_ICON));
	if (dpiScale.cx > 1.0 || dpiScale.cy > 1.0) {
		m_AboutIcon.SetIcon(MAKEINTRESOURCE(IDI_APPICON), 256, 256); //load hi-def icon
	}
	else {
		m_AboutIcon.SetIcon(MAKEINTRESOURCE(IDI_APPICON), 48, 48);   //load regular icon
	}
	m_AboutIcon.SetWindowPos(HWND_TOP, 0, 0, iconSz.cx, iconSz.cy, SWP_NOMOVE | SWP_NOZORDER);

	m_Tabs.Attach(GetDlgItem(IDC_TABS));
	AddTab(wGS(ABOUT_TAB_ABOUT).c_str(), new CAboutTab(IDD_TAB_ABOUT));
	AddTab(wGS(ABOUT_TAB_CONTRIBUTORS).c_str(), new CAboutContributersTab);
	AddTab(wGS(ABOUT_TAB_FUNDERS).c_str(), new CAboutFundersTab);
	AddTab(wGS(ABOUT_TAB_CREDITS).c_str(), new CAboutCreditsTab);
	return 0;
}

CRect CAboutDlg::GetTabRect()
{
	CRect TabRect;
	m_Tabs.GetWindowRect(&TabRect);
	ScreenToClient(&TabRect);
	m_Tabs.AdjustRect(FALSE, &TabRect);
	return TabRect;
}

void CAboutDlg::AddTab(const wchar_t * caption, CAboutTab * tab)
{
	m_Tabs.AddItem(caption);
	tab->Create(m_hWnd, 0);
	tab->SetWindowPos(m_hWnd, 0, 0, 0, 0, SWP_HIDEWINDOW);
	m_TabWindows.push_back(tab);

	if (m_TabWindows.size() == 1)
	{
		ShowTab(0);
	}
}

void CAboutDlg::ShowTab(int nPage)
{
	for (size_t i = 0; i < m_TabWindows.size(); i++)
	{
		m_TabWindows[i]->ShowWindow(SW_HIDE);
	}

	CRect TabRect = GetTabRect();
	m_TabWindows[nPage]->SetWindowPos(HWND_TOP, TabRect.left, TabRect.top, TabRect.Width(), TabRect.Height(), SWP_SHOWWINDOW);
	
	CRect WinRect, ClientRect;
	m_TabWindows[nPage]->GetWindowRect(WinRect);
	m_TabWindows[nPage]->GetClientRect(ClientRect);

	m_Tabs.RedrawWindow();
}

LRESULT CAboutDlg::OnTabChange(NMHDR* /*pNMHDR*/)
{
	ShowTab(m_Tabs.GetCurSel());
	return FALSE;
}

LRESULT CAboutDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAboutDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}