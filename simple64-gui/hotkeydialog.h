#ifndef HOTKEYDIALOG_H
#define HOTKEYDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include "m64p_types.h"

class CustomButton : public QPushButton
{
public:
    explicit CustomButton(QWidget *parent = 0);
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }
    void setDialog(void* Dialog) {
        m_dialog = Dialog;
    }
    QString getOrigText() {
        return origText;
    }
    const char * getParamName() {
        return m_ParamName.toUtf8().constData();
    }
    m64p_type getParamType() {
        return m_ParamType;
    }
private:
    m64p_type m_ParamType;
    QString m_ParamName;
    m64p_handle m_CurrentHandle;
    void* m_dialog;
    QString origText;
};

class ClearButton : public QPushButton
{
public:
    explicit ClearButton(QWidget *parent = 0);
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }
    void setMainButton(CustomButton* MainButton) {
        m_MainButton = MainButton;
    }
    void setDialog(void* Dialog) {
        m_dialog = Dialog;
    }

private:
    m64p_type m_ParamType;
    QString m_ParamName;
    m64p_handle m_CurrentHandle;
    CustomButton* m_MainButton;
    void* m_dialog;
};

class HotkeyDialog : public QDialog
{
    Q_OBJECT
public:
    HotkeyDialog(QWidget *parent = nullptr);
    QList<CustomButton*>* getButtonList(){
        return &m_coreEventsButtonList;
    }
    m64p_handle getHandle(){
        return m_configHandle;
    }
    QGridLayout* getLayout(){
        return m_layout;
    }
    int* getLayoutRow(){
        return &m_layoutRow;
    }
    void setTimer(int timer){
        m_timer = startTimer(timer);
    }
    void setButtonTimer(int timer){
        m_buttonTimer = timer;
    }
    void setActiveButton(CustomButton* button){
        m_activeButton = button;
    }
    CustomButton* getActiveButton()
    {
        return m_activeButton;
    }
protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private slots:
    void handleResetButton();
private:
    QList<CustomButton*> m_coreEventsButtonList;
    m64p_handle m_configHandle;
    QGridLayout *m_layout;
    int m_layoutRow;
    CustomButton* m_activeButton;
    int m_timer;
    int m_buttonTimer;
};

#endif // HOTKEYDIALOG_H
