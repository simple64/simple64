#pragma once
#include "config-tab.h"
#include "wtl-tooltip.h"
#include "resource.h"

class CTextureEnhancementTab :
	public CConfigTab,
	public CToolTipDialog<CTextureEnhancementTab>
{
public:
	BEGIN_MSG_MAP(CTextureEnhancementTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColorStatic)
		COMMAND_HANDLER_EX(IDC_CHK_ENHANCED_TEX_FILE_STORAGE, BN_CLICKED, OnFileStorage)
		COMMAND_HANDLER_EX(IDC_CHK_TEXTURE_PACK, BN_CLICKED, OnTexturePack)
		COMMAND_ID_HANDLER_EX(IDC_TEX_PACK_PATH_BTN, OnSelectTexPackPath)
		COMMAND_ID_HANDLER_EX(IDC_TEX_CACHE_PATH_BTN, OnSelectTexCachePath)
		COMMAND_ID_HANDLER_EX(IDC_TEX_DUMP_PATH_BTN, OnSelectTexDumpPath)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		CHAIN_MSG_MAP(CToolTipDialog<CTextureEnhancementTab>)
	END_MSG_MAP()

	CTextureEnhancementTab();
	BOOL OnInitDialog(CWindow /*wndFocus*/, LPARAM /*lInitParam*/);
	LRESULT OnColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	void OnFileStorage(UINT /*Code*/, int id, HWND /*ctl*/);
	void OnTexturePack(UINT /*Code*/, int id, HWND /*ctl*/);
	void ApplyLanguage(void);
	void LoadSettings(bool blockCustomSettings);
	void SaveSettings();

private:
	static int CALLBACK SelectDirCallBack(HWND hwnd, uint32_t uMsg, uint32_t lp, uint32_t lpData);

	void SaveDirectory(int EditCtrl, wchar_t * txPath);
	void SelectDir(wchar_t * Title, int EditCtrl);
	void OnSelectTexPackPath(UINT Code, int id, HWND ctl);
	void OnSelectTexCachePath(UINT Code, int id, HWND ctl);
	void OnSelectTexDumpPath(UINT Code, int id, HWND ctl);

	CEdit m_TextureFilterCacheTxt;
	CUpDownCtrl m_TextureFilterCacheSpin;
};

