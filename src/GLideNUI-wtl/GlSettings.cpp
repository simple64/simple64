#include "GlSettings.h"

GlColor::GlColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha) :
	m_Red(Red),
	m_Green(Green),
	m_Blue(Blue),
	m_Alpha(Alpha)
{
}

GlSettings::GlSettings(const char * IniFile) :
	m_IniFile(IniFile)
{
	m_IniFile.SetAutoFlush(false);
	m_CurrentSection = "General";
}

GlSettingValue GlSettings::value(const char * Key)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection+"\\"+ Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), "");
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

GlSettingValue GlSettings::value(const char * Key, int32_t DefaultValue)
{
	char szDefaultValue[400];
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "%d", DefaultValue);
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), szDefaultValue);
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

GlSettingValue GlSettings::value(const char * Key, uint32_t DefaultValue)
{
	char szDefaultValue[400];
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "%d", DefaultValue);
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), szDefaultValue);
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

GlSettingValue GlSettings::value(const char * Key, float DefaultValue)
{
	char szDefaultValue[400];
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "%f", DefaultValue);
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), szDefaultValue);
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

GlSettingValue GlSettings::value(const char * Key, const char * DefaultValue)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), DefaultValue);
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

GlSettingValue GlSettings::value(const char * Key, const GlColor & DefaultValue)
{
	char szDefaultValue[400];
#pragma warning(disable: 4996)
	sprintf(szDefaultValue, "@Variant(\\0\\0\\x43\\x1\\xff\\xff\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x)", DefaultValue.Red(), DefaultValue.Red(), DefaultValue.Green(), DefaultValue.Green(), DefaultValue.Blue(), DefaultValue.Blue(), DefaultValue.Alpha(), DefaultValue.Alpha());
#pragma warning(default: 4996)
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	std::string value = m_IniFile.GetString(m_CurrentSection.c_str(), SectionKey.c_str(), szDefaultValue);
	GlSettingValue SettingsValue(value.c_str());
	return SettingsValue;
}

void GlSettings::setValue(const char * Key, float value)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	char text[400];
#pragma warning(disable: 4996)
	sprintf(text, "%f", value);
#pragma warning(default: 4996)
	m_IniFile.SaveString(m_CurrentSection.c_str(), SectionKey.c_str(), text);
}

void GlSettings::setValue(const char * Key, int32_t value)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	m_IniFile.SaveNumber(m_CurrentSection.c_str(), SectionKey.c_str(), value);
}

void GlSettings::setValue(const char * Key, uint32_t value)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	m_IniFile.SaveNumber(m_CurrentSection.c_str(), SectionKey.c_str(), value);
}

void GlSettings::setValue(const char * Key, const char * value)
{
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	m_IniFile.SaveString(m_CurrentSection.c_str(), SectionKey.c_str(), value);
}

void GlSettings::setValue(const char * Key, const GlColor & value)
{
	char txValue[400];
#pragma warning(disable: 4996)
	sprintf(txValue, "@Variant(\\0\\0\\x43\\x1\\xff\\xff\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x\\x%x)", value.Red(), value.Red(), value.Green(), value.Green(), value.Blue(), value.Blue(), value.Alpha(), value.Alpha());
#pragma warning(default: 4996)
	std::string SectionKey = !m_SubSection.empty() ? m_SubSection + "\\" + Key : Key;
	m_IniFile.SaveString(m_CurrentSection.c_str(), SectionKey.c_str(), txValue);
}

void GlSettings::clear(void)
{
	CIniFile::SectionList sections;
	m_IniFile.GetVectorOfSections(sections);
	for (CIniFile::SectionList::const_iterator itr = sections.begin(); itr != sections.end(); itr++)
	{
		m_IniFile.DeleteSection(itr->c_str());
	}
}

void GlSettings::remove(const char * section)
{
	m_IniFile.DeleteSection(section);
}

void GlSettings::beginGroup(const char * section)
{
	if (strcmp(m_CurrentSection.c_str(), "General") == 0)
	{
		m_CurrentSection = section;
	}
	else
	{
		m_SubSection = section;
	}
}

void GlSettings::endGroup(void)
{
	if (!m_SubSection.empty())
	{
		m_SubSection = "";
	}
	else
	{
		m_CurrentSection = "General";
		m_IniFile.FlushChanges();
	}
}

GlSettings::sections GlSettings::childGroups()
{
	CIniFile::SectionList Children;
	m_IniFile.GetVectorOfSections(Children);

	sections Sections;
	for (CIniFile::SectionList::iterator itr = Children.begin(); itr != Children.end(); itr++)
	{
#pragma warning(disable: 4996)
		if (stricmp("General", itr->c_str()) == 0)
		{
			continue;
		}
#pragma warning(default: 4996)
		Sections.insert(itr->c_str());
	}
	return Sections;
}


GlSettingValue::GlSettingValue(const char * value) :
	m_Value(value != NULL ? value : "")
{
}

GlSettingValue::GlSettingValue(const GlSettingValue& rhs) :
	m_Value(rhs.m_Value)
{
}

GlSettingValue& GlSettingValue::operator=(const GlSettingValue&rhs)
{
	m_Value = rhs.m_Value;
	return *this;
}

int32_t GlSettingValue::toInt() const
{
	uint32_t Value = 0;
#pragma warning(disable: 4996)
	sscanf(m_Value.c_str(), "%d", &Value);
#pragma warning(default: 4996)
	return Value;
}

float GlSettingValue::toFloat() const
{
	float Value = 0.0;
#pragma warning(disable: 4996)
	sscanf(m_Value.c_str(), "%f", &Value);
#pragma warning(default: 4996)
	return Value;
}

std::string GlSettingValue::toString() const
{
	return m_Value;
}

GlColor GlSettingValue::toGlColor() const
{
	uint8_t Red = 0, Green = 0, Blue = 0, Alpha = 0;

	if (strncmp("@Variant(", m_Value.c_str(), 9) == 0)
	{
		std::vector<std::string> tokens;

		std::string value = m_Value.substr(9);
		char delimiter = '\\';
		std::string::size_type lastPos = value.find_first_not_of(delimiter, 0);
		std::string::size_type pos = value.find_first_of(delimiter, lastPos);
		while (std::string::npos != pos)
		{
			tokens.push_back(value.substr(lastPos, pos - lastPos));
			lastPos = pos + 1;
			pos = value.find_first_of(delimiter, lastPos);
		}
		if (std::string::npos != lastPos)
		{
			tokens.push_back(value.substr(lastPos));
		}

		if (tokens.size() >= 14)
		{
			Red = ParseGlColor(tokens[7].c_str());
			Green = ParseGlColor(tokens[9].c_str());
			Blue = ParseGlColor(tokens[11].c_str());
			Alpha = ParseGlColor(tokens[13].c_str());
		}
	}

	return GlColor(Red, Green, Blue, Alpha);
}

uint8_t GlSettingValue::ParseGlColor(const char * color)
{
	if (color == NULL) { return 0; }
	if (color[0] == '0') { return 0; }
	if (color[0] == 'x')
	{
		uint32_t Value = 0;
#pragma warning(disable: 4996)
		sscanf(&color[1], "%X", &Value);
#pragma warning(default: 4996)
		return (uint8_t)Value;
	}
	return 0;
}
