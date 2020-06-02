#pragma once
#include <string>
#include <map>

typedef std::map<std::string, std::string> FontList;

FontList GetFontFiles();
std::string GetFontFolder();
std::string GetFontNameFromFile(const char * FontFilePath);

