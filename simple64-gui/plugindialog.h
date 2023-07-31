#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include "settingclasses.h"

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    PluginDialog(QWidget *parent = nullptr);
    QString getSection();
    m64p_handle getCoreHandle();
    m64p_handle getVideoHandle();
    QGridLayout * getCoreLayout();
    QGridLayout * getVideoLayout();
    int * getCoreLayoutRow();
    int * getVideoLayoutRow();
private slots:
    void handleResetButton();
private:
    m64p_handle m_coreConfigHandle;
    m64p_handle m_videoConfigHandle;
    QGridLayout *m_coreLayout;
    int m_coreLayoutRow;
    QGridLayout *m_videoLayout;
    int m_videoLayoutRow;
    QString m_section;
};

#endif // PLUGINDIALOG_H
