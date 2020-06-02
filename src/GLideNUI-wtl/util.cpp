#include "util.h"
#include <windows.h>
#include <memory>

std::string FromUTF16(const wchar_t * UTF16Source)
{
	std::string Result;

	uint32_t utf8size = WideCharToMultiByte(CP_UTF8, 0, UTF16Source, -1, NULL, 0, NULL, NULL);
	if (utf8size > 0)
	{
		std::unique_ptr<char> pUTF8(new char[utf8size]);
		WideCharToMultiByte(CP_UTF8, 0, UTF16Source, -1, pUTF8.get(), utf8size, NULL, NULL);
		Result = pUTF8.get();
	}
	return Result;
}

std::wstring ToUTF16(const char * Source)
{
	std::wstring res;

	DWORD nNeeded = MultiByteToWideChar(CP_UTF8, 0, Source, -1, NULL, 0);
	if (nNeeded > 0)
	{
		std::unique_ptr<wchar_t> pUTF8(new wchar_t[nNeeded]);
		if (pUTF8.get() != NULL)
		{
			nNeeded = MultiByteToWideChar(CP_UTF8, 0, Source, -1, pUTF8.get(), nNeeded);
			if (nNeeded)
			{
				res = pUTF8.get();
			}
		}
	}
	return res;
}

std::wstring FormatStrW(const wchar_t * strFormat, ...)
{
	va_list args;
	va_start(args, strFormat);

	size_t nlen = _vscwprintf(strFormat, args) + 1;
	wchar_t * buffer = (wchar_t *)alloca(nlen * sizeof(wchar_t));
	buffer[nlen - 1] = 0;

	std::wstring res;
	if (buffer != NULL)
	{
		_vsnwprintf(buffer, nlen - 1, strFormat, args);
		res = buffer;
	}
	va_end(args);
	return res;

}
