#pragma once
#include <string>
#include <set>
typedef std::set<std::string> ProfileList;

void loadSettings(const char * _strIniFolder);
void writeSettings(const char * _strIniFolder);
void loadCustomRomSettings(const char * _strIniFolder, const char * _strRomName);
void saveCustomRomSettings(const char * _strIniFolder, const char * _strRomName);
std::string getTranslationFile();
ProfileList getProfiles(const char * _strIniFolder);
std::string getCurrentProfile(const char * _strIniFolder);
void changeProfile(const char * _strIniFolder, const char * _strProfile);
void addProfile(const char * _strIniFolder, const char * _strProfile);
void removeProfile(const char * _strIniFolder, const char * _strProfile);
