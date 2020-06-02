#include "Log.h"
#include <android/log.h>
#include <vector>
#include <sstream>

void LogDebug(const char* _fileName, int _line, u16 _type, const char* _format, ...) {

	if (_type > LOG_LEVEL)
		return;

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, _format);

	// reliably acquire the size from a copy of
	// the variable argument array
	// and a functionally reliable call
	// to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf(NULL, 0, _format, vaCopy);
	va_end(vaCopy);

	// return a formatted string without
	// risking memory mismanagement
	// and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), _format, vaArgs);
	va_end(vaArgs);

	static const char* logLevelText[] = {
			"NONE",
			"ERROR",
			"MINIMAL",
			"WARNING",
			"VERBOSE",
			"APIFUNC",
	};

	std::stringstream lcFormatString;
	lcFormatString << _fileName << ":" << _line << "," << logLevelText[_type] << ", \"" << zc.data() << "\"" << std::endl;

	static android_LogPriority androidLogTranslate[] = {
			ANDROID_LOG_SILENT,
			ANDROID_LOG_ERROR,
			ANDROID_LOG_INFO,
			ANDROID_LOG_WARN,
			ANDROID_LOG_DEBUG,
			ANDROID_LOG_VERBOSE,
	};

	__android_log_write(androidLogTranslate[_type], "GLideN64", lcFormatString.str().c_str());
}