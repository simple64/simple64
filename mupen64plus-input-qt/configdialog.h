#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QSettings>
#include <QPushButton>
#include <QComboBox>
#include <SDL2/SDL.h>

extern QSettings* settings;
extern QSettings* controllerSettings;
extern int emu_running;

class ControllerTab : public QWidget
{
    Q_OBJECT

public:
    ControllerTab(unsigned int controller);
    QComboBox *profileSelect;
private:
    QComboBox *gamepadSelect;
    QComboBox *pakSelect;
};

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog();

private:
    QTabWidget *tabWidget;
    ControllerTab *controllerTabs[4];
};

class ProfileTab : public QWidget
{
    Q_OBJECT

public:
    ProfileTab(ControllerTab **_controllerTabs);
private:
    int checkNotRunning();
    void setComboBox(QComboBox* box, ControllerTab **_controllerTabs);
};

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(QString section, QString setting, QWidget* parent);
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
    ProfileEditor(QString profile);
    ~ProfileEditor();
    void acceptInput(CustomButton* button);
protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    SDL_GameController* gamepad[4];
    SDL_Joystick* joystick[4];
    QList<CustomButton*> buttonList;
    CustomButton* activeButton;
    int buttonTimer;
    int timer;
};

#endif
