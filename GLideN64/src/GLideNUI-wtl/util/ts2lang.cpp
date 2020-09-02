#include <windows.h>
#include "../Language.h"
#include "../ticpp/ticpp.h"
#include "path.h"
#include "md5.h"
#include "StdString.h"
#include <map>
#include <set>
#include <regex>

//#define DEBUG_LANG
typedef std::map<stdstr, stdstr> Translations;

std::string CleanString(std::string source)
{
	stdstr value = source;
	value.Replace("</p><p>", "\\n\\n");
	value.Replace("<br/>", "\\n");
	value.Replace("&quot;", "\\\"");
	const std::regex re("<[^>]*>");
	while (std::regex_search(value, re)) {
		value = std::regex_replace(value, re, "");
	}
	value.Replace("\\n\\n\"", "\"");
	value.Replace(stdstr().FromUTF16(L"–"), "-");
	return value;
}

bool GetLangInfo(const char * filename, std::string & version, std::string & langCode, Translations & translations)
{
	ticpp::Document doc(filename);
	doc.LoadFile();

	ticpp::Element * pElem = doc.FirstChildElement();
	if (pElem == NULL)
	{
		return false;
	}
	if (pElem->Value() != "TS")
	{
		return false;
	}
	std::string text = pElem->GetText(false);
	std::string elmValue = pElem->Value();
	for (ticpp::Attribute* child = pElem->FirstAttribute(false); child; child = child->Next(false))
	{
		if (child->Name() == "version")
		{
			version = child->Value();
		}
		else if (child->Name() == "language")
		{
			langCode = child->Value();
		}
	}

	if (version.empty() || langCode.empty())
	{
		return false;
	}
	for (ticpp::Element* childElement = pElem->FirstChildElement(false); childElement; childElement = childElement->NextSiblingElement(false))
	{
		if (childElement->Value() != "context")
		{
			continue;
		}
		for (ticpp::Element* contextElement = childElement->FirstChildElement(false); contextElement; contextElement = contextElement->NextSiblingElement(false))
		{
			if (contextElement->Value() != "message")
			{
				continue;
			}
			stdstr source, translation;
			for (ticpp::Element* messageElement = contextElement->FirstChildElement(false); messageElement; messageElement = messageElement->NextSiblingElement(false))
			{
				if (messageElement->Value() == "source")
				{
					source = CleanString(messageElement->GetText());
				}
				else if (messageElement->Value() == "translation")
				{
					translation = CleanString(messageElement->GetText(false));
				}
			}
			if (source.length() > 0 && translation.length() > 0)
			{
				translations.insert(Translations::value_type(source, translation));
			}
		}
	}
	return translations.size() > 0;
}

typedef std::map<int32_t, stdstr> ID_MAP;
typedef std::set<stdstr> ID_USED;
void WriteSection(FILE * file, const ID_MAP & defaultStrings, const Translations & hashedTranslations, ID_USED &usedTranslations, const char * sectionName, int32_t start, int32_t end)
{
	std::string section;
	for (int32_t id = start; id <= end; id++)
	{
		ID_MAP::const_iterator defaultStr_itr = defaultStrings.find(id);
		if (defaultStr_itr == defaultStrings.end())
		{
			continue;
		}
		stdstr defaultStr = defaultStr_itr->second;
		defaultStr.Replace("\n", "\\n"); //escape characters for comparison
		defaultStr.Replace("\"", "\\\"");
		stdstr matchingHash = MD5(defaultStr).hex_digest();
		Translations::const_iterator translation_itr = hashedTranslations.find(matchingHash);
		if (translation_itr == hashedTranslations.end())
		{
			continue;
		}
		usedTranslations.insert(matchingHash);
		stdstr_f line("#%04d# \"%s\"\n", id, translation_itr->second.c_str());
		section += line;
	}
	if (section.length() == 0)
	{
		return;
	}
	fprintf(file, "/*********************************************************************************\n");
	fprintf(file, "* %-78s *\n", sectionName);
	fprintf(file, "*********************************************************************************/\n");
	fprintf(file, "%s\n", section.c_str());
}

bool WriteLangFile(const char * dstFile, const char * language, const Translations & translations, bool debugEnglish)
{
	ID_MAP defaultStrings = (ID_MAP)GetDefaultStrings();
	Translations hashedTranslations;
	ID_USED usedTranslations;
	for (Translations::const_iterator translation_itr = translations.begin(); translation_itr != translations.end(); translation_itr++)
	{
		std::string translation = debugEnglish ? translation_itr->first : translation_itr->second;
		std::string key = MD5(translation_itr->first).hex_digest();
		hashedTranslations.insert(Translations::value_type(key, translation));
	}
	
	FILE * file = NULL;
	fopen_s(&file, dstFile, "wb");
	if (file == NULL)
	{
		return false;
	}
	uint8_t utf_bom[3] = { 0xEF, 0xBB, 0xBF };
	fwrite(utf_bom, sizeof(utf_bom), 1, file);

	fprintf(file, "/*********************************************************************************\n");
	fprintf(file, "* Meta Information                                                               *\n");
	fprintf(file, "*********************************************************************************/\n");
	fprintf(file, "#0001# \"%s\"\n\n", language);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Config Dialog", 1000, 1999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Video Tab", 2000, 2999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Emulation Tab", 3000, 3999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Frame Buffer Tab", 4000, 4999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Texture enhancement", 5000, 5999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "On-screen display", 6000, 6999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "Debug", 7000, 7999);
	WriteSection(file, defaultStrings, hashedTranslations, usedTranslations, "About", 9000, 9999);

	for (ID_USED::const_iterator usedTranslation_itr = usedTranslations.begin(); usedTranslation_itr != usedTranslations.end(); usedTranslation_itr++)
	{
		Translations::iterator translation_itr = hashedTranslations.find(usedTranslation_itr->c_str());
		if (translation_itr != hashedTranslations.end())
		{
			hashedTranslations.erase(translation_itr);
		}
	}
#ifdef DEBUG_LANG
	if (hashedTranslations.size() > 0)
	{
		fprintf(file, "/*********************************************************************************\n");
		fprintf(file, "* Untranslated Strings                                                           *\n");
		fprintf(file, "*********************************************************************************/\n");
		for (Translations::const_iterator translation_itr = hashedTranslations.begin(); translation_itr != hashedTranslations.end(); translation_itr++)
		{
			fprintf(file, "%s=\"%s\"\n", translation_itr->first.c_str(), translation_itr->second.c_str());
		}
	}
#endif
	fclose(file);
	return true;
}

int main()
{
	if (__argc < 2)
	{
		return 0;
	}

	CPath src(__argv[1], "*.ts");
	CPath dst(__argv[2], "");

	if (!src.Exists())
	{
		return 0;
	}

	std::map<std::string, std::wstring> languages = {
		{ "de_DE", L"Deutsch" },
		{ "es_ES", L"Español" },
		{ "fr_FR", L"Français" },
		{ "it_IT", L"Italiano" },
		{ "ja_JP", L"日本語" },
		{ "pl_PL", L"Polski" },
		{ "pt_BR", L"Português do Brasil" }
	};

	if (src.FindFirst())
	{
		do
		{
			Translations translations;
			std::string version, langCode, language;
			if (GetLangInfo(src, version, langCode, translations))
			{
				language = stdstr().FromUTF16(languages[langCode].c_str());
				dst.SetName(src.GetName().c_str());
				dst.SetExtension("Lang");
				if (!WriteLangFile(dst, language.c_str(), translations, 0))
				{
					return false;
				}
#ifdef DEBUG_LANG
				if (langCode == "es_ES")
				{
					dst.SetName("gliden64_en");
					dst.SetExtension("Lang");
					WriteLangFile(dst, "English", translations, 1);
				}
#endif
			}
		} while (src.FindNext());
	}
	return 0;
}
