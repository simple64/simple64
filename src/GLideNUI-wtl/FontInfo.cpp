#include "FontInfo.h"
#include "FileClass.h"
#include <stdint.h>
#include <memory>
#include <vector>
#include <Windows.h>
#include <Shlobj.h>

#pragma warning(disable:4996)

typedef struct _tagTT_OFFSET_TABLE 
{
	uint16_t uMajorVersion;
	uint16_t uMinorVersion;
	uint16_t uNumOfTables;
	uint16_t uSearchRange;
	uint16_t uEntrySelector;
	uint16_t uRangeShift;
} TT_OFFSET_TABLE;

typedef struct _tagTT_TABLE_DIRECTORY 
{
	char szTag[4];	   //table name
	uint32_t uCheckSum;  //Check sum
	uint32_t uOffset;	//Offset from beginning of file
	uint32_t uLength;	//length of the table in bytes
} TT_TABLE_DIRECTORY;

typedef struct _tagTT_NAME_TABLE_HEADER 
{
	uint16_t uFSelector;  //format selector. Always 0
	uint16_t uNRCount;	//Name Records count
	uint16_t uStorageOffset; //Offset for strings storage, from start of the table
} TT_NAME_TABLE_HEADER;

typedef struct _tagTT_NAME_RECORD 
{
	uint16_t uPlatformID;
	uint16_t uEncodingID;
	uint16_t uLanguageID;
	uint16_t uNameID;
	uint16_t uStringLength;
	uint16_t uStringOffset;	//from start of storage area
} TT_NAME_RECORD;

#define SWAPWORD(x)		MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x)		MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

std::string GetFontNameFromFile(const char * FontFilePath)
{
	CFile f;

	if (!f.Open(FontFilePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		return std::string("");
	}
	TT_OFFSET_TABLE ttOffsetTable;
	f.Read(&ttOffsetTable, sizeof(TT_OFFSET_TABLE));
	ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
	ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
	ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

	//check is this is a true type font and the version is 1.0
	if (ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0)
	{
		return std::string("");
	}

	TT_TABLE_DIRECTORY tblDir;
	bool bFound = false;
	for (int i = 0; i< ttOffsetTable.uNumOfTables; i++) 
	{
		f.Read(&tblDir, sizeof(TT_TABLE_DIRECTORY));
		if (_strnicmp(tblDir.szTag, "name", 4) == 0)
		{
			bFound = true;
			tblDir.uLength = SWAPLONG(tblDir.uLength);
			tblDir.uOffset = SWAPLONG(tblDir.uOffset);
			break;
		}
	}

	std::string res;
	if (bFound) 
	{
		f.Seek(tblDir.uOffset, CFile::begin);
		TT_NAME_TABLE_HEADER ttNTHeader;
		f.Read(&ttNTHeader, sizeof(TT_NAME_TABLE_HEADER));
		ttNTHeader.uNRCount = SWAPWORD(ttNTHeader.uNRCount);
		ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);
		TT_NAME_RECORD ttRecord;
		bFound = false;

		for (int i = 0; i<ttNTHeader.uNRCount; i++) 
		{
			f.Read(&ttRecord, sizeof(TT_NAME_RECORD));
			ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);
			if (ttRecord.uNameID == 1) 
			{
				ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
				ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);
				int nPos = f.GetPosition();
				f.Seek(tblDir.uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset, CFile::begin);

				std::vector<char> NameBuf;
				NameBuf.resize(ttRecord.uStringLength + 1);
				memset(NameBuf.data(), 0, ttRecord.uStringLength + 1);
				f.Read(NameBuf.data(), ttRecord.uStringLength);
				if (NameBuf[0] != '\0')
				{
					res = NameBuf.data();
					break;
				}
				f.Seek(nPos, CFile::begin);
			}
		}
	}
	f.Close();
	return res;
}

std::string GetFontFolder()
{
	char szFontPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, szFontPath)))
	{
		return szFontPath;
	}
	return "";
}

FontList GetFontFiles()
{
	std::string FontFolder = GetFontFolder();
	FontList fonts;

	if (FontFolder.length() > 0)
	{
		std::string SearchQuery = FontFolder;
		SearchQuery += "\\*.ttf";

		WIN32_FIND_DATAA FindData;
		HANDLE hFind = FindFirstFileA(SearchQuery.c_str(), &FindData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do 
			{
				std::string FontFile = FontFolder;
				FontFile += "\\";
				FontFile += FindData.cFileName;
				std::string FontName = GetFontNameFromFile(FontFile.c_str());
				if (FontName.length() > 0)
				{
					fonts.insert(FontList::value_type(FindData.cFileName, FontName));
				}
			} while (FindNextFileA(hFind, &FindData));
			FindClose(hFind);
		}
	}

	if (fonts.size() == 0)
	{
		fonts.insert(FontList::value_type("arial.ttf", "Arial"));
	}
	return fonts;
}
