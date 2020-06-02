#pragma once
#include "IniFileClass.h"
#include <set>

class GlColor
{
public:
	GlColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha = 0);

	inline uint8_t Red(void) const { return m_Red; }
	inline uint8_t Green(void) const { return m_Green; }
	inline uint8_t Blue(void) const { return m_Blue; }
	inline uint8_t Alpha(void) const { return m_Alpha; }
private:
	GlColor();

	uint8_t m_Red, m_Green, m_Blue, m_Alpha;
};

class GlSettingValue
{
public:
	GlSettingValue(const char *);
	GlSettingValue(const GlSettingValue&);
	GlSettingValue& operator=(const GlSettingValue&);

	int32_t toInt() const;
	float toFloat() const;
	std::string toString() const;
	GlColor toGlColor() const;

private:
	GlSettingValue();

	static uint8_t ParseGlColor(const char *);

	std::string m_Value;
};

class GlSettings
{
public:
	typedef std::set<std::string> sections;
	
	GlSettings(const char * IniFile);

	GlSettingValue value(const char * Key);
	GlSettingValue value(const char * Key, int32_t DefaultValue);
	GlSettingValue value(const char * Key, uint32_t DefaultValue);
	GlSettingValue value(const char * Key, float DefaultValue);
	GlSettingValue value(const char * Key, const char * DefaultValue);
	GlSettingValue value(const char * Key, const GlColor & DefaultValue);

	void setValue(const char * Key, int32_t value);
	void setValue(const char * Key, uint32_t value);
	void setValue(const char * Key, float value);
	void setValue(const char * Key, const char * value);
	void setValue(const char * Key, const GlColor & value);

	sections childGroups();

	void clear(void);
	void remove(const char * section);
	void beginGroup(const char * section);
	void endGroup(void);

private:
	GlSettings();
	GlSettings(const GlSettings&);
	GlSettings& operator=(const GlSettings&);
  
	CIniFile m_IniFile;
	std::string m_CurrentSection;
	std::string m_SubSection;
};