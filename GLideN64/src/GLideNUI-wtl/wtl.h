#pragma once

#undef OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4091) // warning C4091: 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used: nameless struct/union
#pragma warning(disable : 4302) // warning C4302: 'type cast': truncation from 'LPCTSTR' to 'WORD'
#pragma warning(disable : 4458) // warning C4458: declaration of 'dwCommonButtons' hides class member
#pragma warning(disable : 4838) // warning C4838: conversion from 'int' to 'UINT' requires a narrowing conversion
#pragma warning(disable : 4996) // warning C4996: 'GetVersionExA': was declared deprecated
#define _ATL_DISABLE_NOTHROW_NEW
#include <atlbase.h>
#include "wtl\atlapp.h"
#include "wtl\atlcrack.h"
#include "wtl\atlctrls.h"
#include "WTL\atlctrlx.h"
#include "WTL\atlgdi.h"
#include "WTL\atlmisc.h"
#include "WTL\atlwinx.h"
#include <atlwin.h>
#pragma warning(pop)
