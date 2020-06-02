#pragma once
#include <stdint.h>
#include "wtl.h"

class CConfigTab :
	public CDialogImpl<CConfigTab>
{
public:
	CConfigTab(uint32_t _IDD);
	virtual ~CConfigTab();

	BEGIN_MSG_MAP(CConfigTab)
	END_MSG_MAP()

	virtual void ApplyLanguage() = 0;
	virtual void LoadSettings(bool blockCustomSettings) = 0;
	virtual void SaveSettings() = 0;

	uint32_t IDD;
};
