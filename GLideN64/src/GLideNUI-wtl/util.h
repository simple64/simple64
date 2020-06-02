#pragma once
#include <string>

std::string FromUTF16(const wchar_t * UTF16Source);
std::wstring ToUTF16(const char * Source);
std::wstring FormatStrW(const wchar_t * Source, ...);

template <int min, int max> class range {
public:
	static bool contains(int i) { return min <= i && i < max; }
};