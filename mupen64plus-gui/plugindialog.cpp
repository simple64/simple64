#include "plugindialog.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTabWidget>
#include <QGridLayout>
#include <QValidator>
#include <QMessageBox>
#include "settingclasses.h"

m64p_handle coreConfigHandle;
m64p_handle videoConfigHandle;
QGridLayout *coreLayout;
int coreLayoutRow;
QGridLayout *videoLayout;
int videoRow;

static void paramListCallback(void *context, const char *ParamName, m64p_type ParamType)
{
    if (strcmp((char*)context, "Core") == 0) {
        if (strcmp((char*)ParamName, "Version") == 0) return;
        else if (strcmp((char*)ParamName, "OnScreenDisplay") == 0) return;
        else if (strcmp((char*)ParamName, "NoCompiledJump") == 0) return;
        else if (strcmp((char*)ParamName, "EnableDebugger") == 0) return;
        else if (strcmp((char*)ParamName, "GbCameraVideoCaptureBackend1") == 0) return;
        else if (strcmp((char*)ParamName, "SaveDiskFormat") == 0) return;
    }

    QGridLayout *my_layout = nullptr;
    int * my_row = nullptr;
    m64p_handle current_handle = nullptr;
    if (strcmp((char*)context, "Core") == 0) {
        my_layout = coreLayout;
        my_row = &coreLayoutRow;
        current_handle = coreConfigHandle;
    } else if (strcmp((char*)context, "Video") == 0) {
        my_layout = videoLayout;
        my_row = &videoRow;
        current_handle = videoConfigHandle;
    }
    int l_ParamInt;
    bool l_ParamBool;
    float l_ParamFloat;
    QString l_ParamString;
    QString helper = (*ConfigGetParameterHelp)(current_handle, ParamName);
    QLabel *desc = new QLabel(ParamName);
    if (!helper.isEmpty()) {
       helper.prepend("<span style=\"color:black;\">");
       helper.append("</span>");
       desc->setToolTip(helper);
    }
    desc->setStyleSheet("border: 1px solid; padding: 10px");
    my_layout->addWidget(desc, *my_row, 0);
    void *my_Widget = nullptr;
    switch (ParamType) {
    case M64TYPE_INT:
        my_Widget = new CustomLineEdit();
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        l_ParamInt = (*ConfigGetParamInt)(current_handle, ParamName);
        ((CustomLineEdit*)my_Widget)->setText(QString::number(l_ParamInt));
        ((CustomLineEdit*)my_Widget)->setValidator(new QIntValidator());
        break;
    case M64TYPE_FLOAT:
        my_Widget = new CustomLineEdit();
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        l_ParamFloat = (*ConfigGetParamFloat)(current_handle, ParamName);
        ((CustomLineEdit*)my_Widget)->setText(QString::number(l_ParamFloat));
        ((CustomLineEdit*)my_Widget)->setValidator(new QDoubleValidator());
        break;
    case M64TYPE_BOOL:
        my_Widget = new CustomCheckBox();
        ((CustomCheckBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomCheckBox*)my_Widget)->setParamType(ParamType);
        ((CustomCheckBox*)my_Widget)->setParamName(ParamName);
        ((CustomCheckBox*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        l_ParamBool = (*ConfigGetParamBool)(current_handle, ParamName);
        ((CustomCheckBox*)my_Widget)->setCheckState(l_ParamBool ? Qt::Checked : Qt::Unchecked);
        break;
    case M64TYPE_STRING:
        my_Widget = new CustomLineEdit();
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        l_ParamString = (*ConfigGetParamString)(current_handle, ParamName);
        ((CustomLineEdit*)my_Widget)->setText(l_ParamString);
        break;
    }

    my_layout->addWidget((QWidget*)my_Widget, *my_row, 1);
    ++*my_row;
}

void PluginDialog::handleResetButton()
{
    int value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);
    if (value == M64EMU_STOPPED) {
        (*ConfigDeleteSection)("Core");
        (*ConfigDeleteSection)("Video-Parallel");
        (*ConfigSaveFile)();
        w->resetCore();
        this->close();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Emulator must be stopped.");
        msgBox.exec();
    }
}

PluginDialog::PluginDialog(QWidget *parent)
    : QDialog(parent)
{
    m64p_error res;
    int value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);

    coreLayoutRow = 0;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setUsesScrollButtons(false);

    QWidget *coreSettings = new QWidget(this);
    coreLayout = new QGridLayout(coreSettings);
    coreSettings->setLayout(coreLayout);
    res = (*ConfigOpenSection)("Core", &coreConfigHandle);
    if (res == M64ERR_SUCCESS)
        (*ConfigListParameters)(coreConfigHandle, (char*)"Core", paramListCallback);
    QScrollArea *coreScroll = new QScrollArea(this);
    coreScroll->setWidget(coreSettings);
    coreScroll->setMinimumWidth(coreSettings->sizeHint().width() + 20);
    coreScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(coreScroll, tr("Core"));

    QWidget *videoSettings = new QWidget(this);
    videoLayout = new QGridLayout(videoSettings);
    videoSettings->setLayout(videoLayout);
    res = (*ConfigOpenSection)("Video-Parallel", &videoConfigHandle);
    if (res == M64ERR_SUCCESS)
        (*ConfigListParameters)(videoConfigHandle, (char*)"Video", paramListCallback);
    QScrollArea *videoScroll = new QScrollArea(this);
    videoScroll->setWidget(videoSettings);
    videoScroll->setMinimumWidth(videoSettings->sizeHint().width() + 20);
    videoScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(videoScroll, tr("ParaLLEl Video"));

    QLabel *myLabel = new QLabel("Hover your mouse over the configuration item name for a description.\n", this);
    myLabel->setStyleSheet("font-weight: bold");
    mainLayout->addWidget(myLabel);
    mainLayout->addWidget(tabWidget);
    QPushButton *resetButton = new QPushButton("Reset All Settings", this);
    resetButton->setAutoDefault(false);
    connect(resetButton, SIGNAL (released()),this, SLOT (handleResetButton()));
    mainLayout->addWidget(resetButton);
    setLayout(mainLayout);
}
