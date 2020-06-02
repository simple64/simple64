#include "settingclasses.h"
#include "core_interface.h"
#include "plugin.h"
#include "cheat.h"
#include "common.h"
#include <SDL.h>
#include <QLabel>
#include <QVBoxLayout>

CustomLineEdit::CustomLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this, &QLineEdit::editingFinished, [=](){
        int i_value = this->text().toInt();
        float f_value = this->text().toFloat();
        switch (m_ParamType) {
        case M64TYPE_INT:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &i_value);
            break;
        case M64TYPE_FLOAT:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &f_value);
            break;
        case M64TYPE_STRING:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, this->text().toLatin1().data());
            break;
        default:
            break;
        }
        (*ConfigSaveFile)();
    });
}

CustomCheckBox::CustomCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    connect(this, &QCheckBox::stateChanged, [=](int state){
        int value = state == Qt::Checked ? 1 : 0;
        (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &value);
        (*ConfigSaveFile)();
    });
}

CheatCheckBox::CheatCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    m_Checked = false;
    connect(this, &QAbstractButton::clicked, [=](bool checked){
        sCheatInfo *pCheat;
        pCheat = CheatFindCode(m_Number);
        if (checked && !m_Checked) {
            if (pCheat == NULL)
               DebugMessage(M64MSG_WARNING, "invalid cheat code number %i", m_Number);
            else
            {
                if (pCheat->VariableLine != -1 && pCheat->Count > pCheat->VariableLine && m_Option < pCheat->Codes[pCheat->VariableLine].var_count)
                    pCheat->Codes[pCheat->VariableLine].var_to_use = m_Option;
                CheatActivate(pCheat);
                pCheat->active = true;
            }
            m_Checked = true;
        }
        else {
            (*CoreCheatEnabled)(pCheat->Name, 0);
            pCheat->active = false;
            if (m_ButtonGroup != nullptr)
                m_ButtonGroup->setChecked(true);
        }
    });
    connect(this, &QCheckBox::stateChanged, [=](int state){
        int value = state == Qt::Checked ? 1 : 0;
        if (!value)
            m_Checked = false;
    });
}
