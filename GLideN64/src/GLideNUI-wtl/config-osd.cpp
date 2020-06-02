#include "config-osd.h"
#include "resource.h"
#include "util.h"
#include "../Config.h"
#include "FontInfo.h"
#include "Language.h"

COsdTab::COsdTab() :
	CConfigTab(IDD_TAB_OSD),
	m_PosTopLeft(IDI_OSD_TOP_LEFT),
	m_PosTop(IDI_OSD_TOP),
	m_PosTopRight(IDI_OSD_TOP_RIGHT),
	m_PosCenterLeft(IDI_OSD_LEFT),
	m_PosCenter(0),
	m_PosCenterRight(IDI_OSD_RIGHT),
	m_PosBottomLeft(IDI_OSD_BOTTOM_LEFT),
	m_PosBottom(IDI_OSD_BOTTOM),
	m_PosBottomRight(IDI_OSD_BOTTOM_RIGHT)
{
	m_FontsLoaded = false;
}

BOOL COsdTab::OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/)
{
	m_PosTopLeft.SubclassWindow(GetDlgItem(IDC_OSD_TOP_LEFT));
	m_PosTop.SubclassWindow(GetDlgItem(IDC_OSD_TOP));
	m_PosTopRight.SubclassWindow(GetDlgItem(IDC_OSD_TOP_RIGHT));
	m_PosCenterLeft.SubclassWindow(GetDlgItem(IDC_OSD_CENTER_LEFT));
	m_PosCenter.SubclassWindow(GetDlgItem(IDC_OSD_CENTER));
	m_PosCenterRight.SubclassWindow(GetDlgItem(IDC_OSD_CENTER_RIGHT));
	m_PosBottomLeft.SubclassWindow(GetDlgItem(IDC_OSD_BOTTOM_LEFT));
	m_PosBottom.SubclassWindow(GetDlgItem(IDC_OSD_BOTTOM));
	m_PosBottomRight.SubclassWindow(GetDlgItem(IDC_OSD_BOTTOM_RIGHT));
	m_Fonts.Attach(GetDlgItem(IDC_FONTS));
	m_OsdColor.SubclassWindow(GetDlgItem(IDC_OSD_COLOR));
	m_OsdPreview.SubclassWindow(GetDlgItem(IDC_OSD_PREVIEW));

	m_FontSizeTxt.Attach(GetDlgItem(IDC_FONT_SIZE_TXT));
	m_FontSizeSpin.Attach(GetDlgItem(IDC_FONT_SIZE_SPIN));
	m_FontSizeSpin.SetBase(10);
	m_FontSizeSpin.SetRange(6, 99);
	m_FontSizeSpin.SetBuddy(m_FontSizeTxt);

	m_PosCenterLeft.EnableWindow(false);
	m_PosCenter.EnableWindow(false);
	m_PosCenterRight.EnableWindow(false);

	return true;
}

void COsdTab::LoadFonts(void)
{
	FontList fonts = GetFontFiles();
	HTREEITEM hCurrentItem = TVI_ROOT;
	for (FontList::const_iterator itr = fonts.begin(); itr != fonts.end(); itr++) {
		std::wstring FontFile = ToUTF16(itr->first.c_str());
		std::wstring FontName = ToUTF16(itr->second.c_str());

		TVINSERTSTRUCT tv = { 0 };
		wchar_t Item[500];
		tv.item.mask = TVIF_TEXT;
		tv.item.pszText = Item;
		tv.item.cchTextMax = sizeof(Item) / sizeof(Item[0]);
		tv.item.hItem = m_Fonts.GetChildItem(TVI_ROOT);
		HTREEITEM hParent = TVI_ROOT;
		while (tv.item.hItem) {
			m_Fonts.GetItem(&tv.item);
			if (wcscmp(FontName.c_str(), Item) == 0) {
				hParent = tv.item.hItem;
				break;
			}
			tv.item.hItem = m_Fonts.GetNextSiblingItem(tv.item.hItem);
		}

		if (hParent == TVI_ROOT) {
			tv.item.mask = TVIF_TEXT;
			tv.item.pszText = (LPWSTR)FontName.c_str();
			tv.item.cchTextMax = static_cast<int>(FontName.length());
			tv.hInsertAfter = TVI_SORT;
			tv.hParent = TVI_ROOT;
			hParent = m_Fonts.InsertItem(&tv);
		}
		tv.item.mask = TVIF_TEXT;
		tv.item.pszText = (LPWSTR)FontFile.c_str();
		tv.item.cchTextMax = static_cast<int>(FontFile.length());
		tv.hInsertAfter = TVI_SORT;
		tv.hParent = hParent;
		m_Fonts.InsertItem(&tv);
	}
	if (hCurrentItem != TVI_ROOT) {
		m_Fonts.SelectItem(hCurrentItem);
		m_Fonts.SetItemState(hCurrentItem, TVIF_STATE | TVIS_SELECTED, TVIF_STATE | TVIS_SELECTED);
		m_Fonts.SetFocus();
	}
	m_FontsLoaded = true;
	SelCurrentFont();
}

void COsdTab::SelCurrentFont(void)
{
	std::wstring CurrentFile = ToUTF16(config.font.name.c_str());
	TVINSERTSTRUCT tv = { 0 };
	wchar_t Item[500];
	tv.item.mask = TVIF_TEXT;
	tv.item.pszText = Item;
	tv.item.cchTextMax = sizeof(Item) / sizeof(Item[0]);
	tv.item.hItem = m_Fonts.GetChildItem(TVI_ROOT);
	HTREEITEM hCurrentItem = NULL;
	while (tv.item.hItem) {
		m_Fonts.GetItem(&tv.item);
		m_Fonts.SetItemState(tv.item.hItem, 0, TVIF_STATE | TVIS_SELECTED);
		HTREEITEM hChild = m_Fonts.GetChildItem(tv.item.hItem);
		HTREEITEM NextItem = m_Fonts.GetNextSiblingItem(tv.item.hItem);
		if (hChild != NULL) {
			tv.item.hItem = hChild;
			while (tv.item.hItem) {
				m_Fonts.GetItem(&tv.item);
				m_Fonts.SetItemState(tv.item.hItem, 0, TVIF_STATE | TVIS_SELECTED);
				if (Item == CurrentFile)
					hCurrentItem = tv.item.hItem;
				tv.item.hItem = m_Fonts.GetNextSiblingItem(tv.item.hItem);
			}
		}
		tv.item.hItem = NextItem;
	}
	if (hCurrentItem != TVI_ROOT) {
		m_Fonts.SelectItem(hCurrentItem);
		m_Fonts.SetItemState(hCurrentItem, TVIF_STATE | TVIS_SELECTED, TVIF_STATE | TVIS_SELECTED);
		m_Fonts.SetFocus();
	}
}

void COsdTab::ApplyLanguage(void)
{
	SetDlgItemTextW(IDC_POSITION, wGS(OSD_POSITION).c_str());
	SetDlgItemTextW(IDC_CHK_FPS, wGS(OSD_DISPLAY_FPS).c_str());
	SetDlgItemTextW(IDC_CHK_VIS, wGS(OSD_DISPLAY_VI).c_str());
	SetDlgItemTextW(IDC_CHK_PERCENT, wGS(OSD_DISPLAY_PERCENTAGE).c_str());
	SetDlgItemTextW(IDC_INTERNAL_RESOLUTION, wGS(OSD_DISPLAY_INTERNAL_RESOLUTION).c_str());
	SetDlgItemTextW(IDC_RENDERING_RESOLUTION, wGS(OSD_DISPLAY_RENDERING_RESOLUTION).c_str());
	SetDlgItemTextW(IDC_FONT, wGS(OSD_FONT).c_str());
	SetDlgItemTextW(IDC_TXT_SIZE, wGS(OSD_SIZE).c_str());
	SetDlgItemTextW(IDC_PX, wGS(OSD_PX).c_str());
	SetDlgItemTextW(IDC_COLOR, wGS(OSD_COLOR).c_str());
}

LRESULT COsdTab::OnScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LONG CtrlId = CWindow((HWND)lParam).GetWindowLong(GWL_ID);
	if (CtrlId == IDC_FONT_SIZE_SPIN)
		m_OsdPreview.SetFontSize(m_FontSizeSpin.GetPos());
	return 0;
}

void COsdTab::ClearOsdChecked()
{
	m_PosTopLeft.SetChecked(false);
	m_PosTopLeft.Invalidate();
	m_PosTop.SetChecked(false);
	m_PosTop.Invalidate();
	m_PosTopRight.SetChecked(false);
	m_PosTopRight.Invalidate();
	m_PosBottomLeft.SetChecked(false);
	m_PosBottomLeft.Invalidate();
	m_PosBottom.SetChecked(false);
	m_PosBottom.Invalidate();
	m_PosBottomRight.SetChecked(false);
	m_PosBottomRight.Invalidate();
}

LRESULT COsdTab::OnOsdTopLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosTopLeft.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnOsdTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosTop.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnOsdTopRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosTopRight.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnOsdBottomLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosBottomLeft.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnOsdBottom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosBottom.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnOsdBottomRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ClearOsdChecked();
	m_PosBottomRight.SetChecked(true);
	return 0;
}

LRESULT COsdTab::OnFontItemChanged(NMHDR* /*phdr*/)
{
	m_OsdPreview.SetFont(GetSelectedFont());
	return 0;
}

LRESULT COsdTab::OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)GetStockObject(WHITE_BRUSH);
}

LRESULT COsdTab::OnNotifyOsdColor(LPNMHDR /*pnmh*/)
{
	m_OsdPreview.SetColor(m_OsdColor.Red(), m_OsdColor.Green(), m_OsdColor.Blue());
	m_OsdPreview.Invalidate();
	return 0;
}

void COsdTab::LoadSettings(bool /*blockCustomSettings*/)
{
	m_FontSizeTxt.SetWindowText(FormatStrW(L"%d", config.font.size).c_str());
	if (config.font.size > 0) {
		m_FontSizeSpin.SetPos(config.font.size);
		m_OsdPreview.SetFontSize(config.font.size);
	}
	if (!(config.font.color[0] == NULL && config.font.color[1] == NULL && config.font.color[2] == NULL)) {
		m_OsdColor.SetColor(config.font.color[0], config.font.color[1], config.font.color[2]);
		m_OsdPreview.SetColor(config.font.color[0], config.font.color[1], config.font.color[2]);
	}

	m_PosTopLeft.SetChecked(config.onScreenDisplay.pos == Config::posTopLeft);
	m_PosTop.SetChecked(config.onScreenDisplay.pos == Config::posTopCenter);
	m_PosTopRight.SetChecked(config.onScreenDisplay.pos == Config::posTopRight);
	m_PosBottomLeft.SetChecked(config.onScreenDisplay.pos == Config::posBottomLeft);
	m_PosBottom.SetChecked(config.onScreenDisplay.pos == Config::posBottomCenter);
	m_PosBottomRight.SetChecked(config.onScreenDisplay.pos == Config::posBottomRight);

	CButton(GetDlgItem(IDC_CHK_FPS)).SetCheck(config.onScreenDisplay.fps != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_VIS)).SetCheck(config.onScreenDisplay.vis != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_CHK_PERCENT)).SetCheck(config.onScreenDisplay.percent != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_INTERNAL_RESOLUTION)).SetCheck(config.onScreenDisplay.internalResolution != 0 ? BST_CHECKED : BST_UNCHECKED);
	CButton(GetDlgItem(IDC_RENDERING_RESOLUTION)).SetCheck(config.onScreenDisplay.renderingResolution != 0 ? BST_CHECKED : BST_UNCHECKED);

	if (m_FontsLoaded)
		SelCurrentFont();
}

void COsdTab::SaveSettings()
{
	if (m_PosTopLeft.isChecked()) { config.onScreenDisplay.pos = Config::posTopLeft; }
	else if (m_PosTop.isChecked()) { config.onScreenDisplay.pos = Config::posTopCenter; }
	else if (m_PosTopRight.isChecked()) { config.onScreenDisplay.pos = Config::posTopRight; }
	else if (m_PosBottomLeft.isChecked()) { config.onScreenDisplay.pos = Config::posBottomLeft; }
	else if (m_PosBottom.isChecked()) { config.onScreenDisplay.pos = Config::posBottomCenter; }
	else if (m_PosBottomRight.isChecked()) { config.onScreenDisplay.pos = Config::posBottomRight; }

	config.onScreenDisplay.fps = CButton(GetDlgItem(IDC_CHK_FPS)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.onScreenDisplay.vis = CButton(GetDlgItem(IDC_CHK_VIS)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.onScreenDisplay.percent = CButton(GetDlgItem(IDC_CHK_PERCENT)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.onScreenDisplay.internalResolution = CButton(GetDlgItem(IDC_INTERNAL_RESOLUTION)).GetCheck() == BST_CHECKED ? 1 : 0;
	config.onScreenDisplay.renderingResolution = CButton(GetDlgItem(IDC_RENDERING_RESOLUTION)).GetCheck() == BST_CHECKED ? 1 : 0;

	std::string SelectedFont = FromUTF16(GetSelectedFont().c_str());
	if (!SelectedFont.empty())
		config.font.name = SelectedFont;
	if (config.font.name.empty())
		config.font.name = "arial.ttf";
	config.font.size = m_FontSizeSpin.GetPos();

	config.font.color[0] = m_OsdColor.Red();
	config.font.color[1] = m_OsdColor.Green();
	config.font.color[2] = m_OsdColor.Blue();
	config.font.color[3] = 0;
	config.font.colorf[0] = (float)m_OsdColor.Red();
	config.font.colorf[1] = (float)m_OsdColor.Green();
	config.font.colorf[2] = (float)m_OsdColor.Blue();
	config.font.colorf[3] = 0.0f;
}

std::wstring COsdTab::GetSelectedFont()
{
	if (!m_FontsLoaded)
		return ToUTF16(config.font.name.c_str());
	HTREEITEM hItem = m_Fonts.GetSelectedItem();
	if (hItem == NULL)
		return L"";
	HTREEITEM hChild = m_Fonts.GetChildItem(hItem);
	if (hChild != NULL)
		hItem = hChild;

	wchar_t ItemText[MAX_PATH];
	if (!m_Fonts.GetItemText(hItem, ItemText, sizeof(ItemText) / sizeof(ItemText[0])))
		return L"";
	return std::wstring(ItemText);
}