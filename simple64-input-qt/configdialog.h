#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "osal/osal_dynamiclib.h"
#include "m64p_frontend.h"

#include <QDialog>
#include <QTabWidget>
#include <QSettings>
#include <QPushButton>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QProgressDialog>
#include <SDL2/SDL.h>

class ControllerTab : public QWidget
{
    Q_OBJECT

public:
    ControllerTab(unsigned int controller, QSettings* settings, QSettings* controllerSettings, QWidget *parent);
    QComboBox *profileSelect;
private slots:
    void fileDownloaded(QNetworkReply* pReply);
    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
private:
    QComboBox *gamepadSelect;
    QComboBox *pakSelect;
    QNetworkAccessManager modelDownloader;
    QProgressDialog *progress;
};

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(m64p_dynlib_handle CoreHandle, QSettings* settings, QSettings* controllerSettings);

private:
    QTabWidget *tabWidget;
    ControllerTab *controllerTabs[4];
};

class ProfileTab : public QWidget
{
    Q_OBJECT

public:
    ProfileTab(m64p_dynlib_handle coreHandle, ControllerTab **_controllerTabs, QSettings* settings, QSettings* controllerSettings, QWidget *parent);
private:
    int checkNotRunning(m64p_dynlib_handle coreHandle);
    void setComboBox(QComboBox* box, ControllerTab **_controllerTabs, QSettings* settings, QSettings* controllerSettings);
};

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(QString section, QString setting, QSettings* settings, QWidget* parent);
    int type; //0 = Keyboard, 1 = Button, 2 = Axis
    int axisValue;
    SDL_GameControllerButton button;
    SDL_GameControllerAxis axis;
    SDL_Scancode key;
    int joystick_axis;
    int joystick_hat;
    int joystick_button;
    QString item;
    QString origText;
};

class ProfileEditor : public QDialog
{
    Q_OBJECT

public:
    ProfileEditor(QString profile, QSettings* settings, QWidget* parent);
    ~ProfileEditor();
    void acceptInput(CustomButton* button);
protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    SDL_Joystick* joystick[4];
    QList<CustomButton*> buttonList;
    CustomButton* activeButton;
    int buttonTimer;
    int timer;
};

#endif
