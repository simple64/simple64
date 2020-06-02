#ifndef SETTINGCLASSES_H
#define SETTINGCLASSES_H

#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include "m64p_types.h"

class CustomLineEdit : public QLineEdit
{
public:
    explicit CustomLineEdit(QWidget *parent = 0);
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
};

class CustomCheckBox : public QCheckBox
{
public:
    explicit CustomCheckBox(QWidget *parent = 0);
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
};

class CheatCheckBox : public QCheckBox
{
public:
    explicit CheatCheckBox(QWidget *parent = 0);
    void setNumber(int num) {
        m_Number = num;
    }
    void setOption(int opt) {
        m_Option = opt;
    }
    void setButtonGroup (QCheckBox* bgroup) {
        m_ButtonGroup = bgroup;
    }

private:
    int m_Number;
    int m_Option;
    bool m_Checked;
    QCheckBox* m_ButtonGroup;
};

#endif // SETTINGCLASSES_H
