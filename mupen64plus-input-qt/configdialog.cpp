#include "configdialog.h"
#include "qt2sdl2.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QKeyEvent>
#include <QNetworkReply>
#include <QProcess>
#include <QCoreApplication>
#include <QDir>

void ControllerTab::fileDownloaded(QNetworkReply* pReply) {
    if (pReply->error())
    {
        pReply->deleteLater();
        return;
    }
    QFile file("vosk-model.zip");
    if (!file.open(QIODevice::WriteOnly))
    {
        pReply->deleteLater();
        return;
    }
    file.write(pReply->readAll());
    file.flush();
    QProcess process;
    QString command = "7za x vosk-model.zip";
    process.start(command);
    process.waitForFinished(-1);
    file.remove();
    pReply->deleteLater();
}

ControllerTab::ControllerTab(unsigned int controller, QSettings* settings, QSettings* controllerSettings, QWidget *parent)
    : QWidget(parent)
{
    connect(&modelDownloader, SIGNAL (finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));
    QGridLayout *layout = new QGridLayout(this);
    QLabel *profileLabel = new QLabel("Profile", this);
    layout->addWidget(profileLabel, 0, 0);

    profileSelect = new QComboBox(this);
    profileSelect->addItems(settings->childGroups());
    profileSelect->removeItem(profileSelect->findText("Auto-Gamepad"));
    profileSelect->removeItem(profileSelect->findText("Auto-Keyboard"));
    profileSelect->insertItem(0, "Auto");
    profileSelect->setCurrentText(controllerSettings->value("Controller" + QString::number(controller) + "/Profile").toString());
    connect(profileSelect, &QComboBox::currentTextChanged, [=](QString text) {
        controllerSettings->setValue("Controller" + QString::number(controller) + "/Profile", text);
    });
    layout->addWidget(profileSelect, 0, 1);

    QLabel *gamepadLabel = new QLabel("Gamepad", this);
    layout->addWidget(gamepadLabel, 1, 0);

    gamepadSelect = new QComboBox(this);
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
        gamepadSelect->addItem(QString::number(i) + ":" + SDL_JoystickNameForIndex(i));

    gamepadSelect->insertItem(0, "Auto");
    gamepadSelect->addItem("Keyboard");
    gamepadSelect->addItem("Emulate VRU");
    gamepadSelect->addItem("None");
    gamepadSelect->setCurrentText(controllerSettings->value("Controller" + QString::number(controller) + "/Gamepad").toString());
    connect(gamepadSelect, &QComboBox::currentTextChanged, [=](QString text) {
        controllerSettings->setValue("Controller" + QString::number(controller) + "/Gamepad", text);
        if (text == "Emulate VRU")
        {
            if (!QFile::exists(QDir(QCoreApplication::applicationDirPath()).filePath("vosk-model-small-en-us-0.15/conf/mfcc.conf")))
            {
                QNetworkRequest request(QUrl("http://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip"));
                request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
                progress = new QProgressDialog("Downloading voice model...", "Abort", 0, 100, this);
                progress->show();
                QNetworkReply *reply = modelDownloader.get(request);
                connect(progress, &QProgressDialog::canceled, reply, &QNetworkReply::abort);
                connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
                    int total = ((double)bytesReceived / (double)bytesTotal) * 100.0;
                    progress->setValue(total);
                });
            }
        }
    });
    layout->addWidget(gamepadSelect, 1, 1);

    QLabel *pakLabel = new QLabel("Pak", this);
    layout->addWidget(pakLabel, 2, 0);

    pakSelect = new QComboBox(this);
    pakSelect->addItem("Memory");
    pakSelect->addItem("Rumble");
    pakSelect->addItem("Transfer");
    pakSelect->addItem("None");
    pakSelect->setCurrentText(controllerSettings->value("Controller" + QString::number(controller) + "/Pak").toString());
    connect(pakSelect, &QComboBox::currentTextChanged, [=](QString text) {
        controllerSettings->setValue("Controller" + QString::number(controller) + "/Pak", text);
    });
    layout->addWidget(pakSelect, 2, 1);

    setLayout(layout);
}

void ProfileTab::setComboBox(QComboBox* box, ControllerTab **_controllerTabs, QSettings* settings, QSettings* controllerSettings)
{
    for (int i = 1; i < 5; ++i) {
        QString current = controllerSettings->value("Controller" + QString::number(i) + "/Profile").toString();
        _controllerTabs[i-1]->profileSelect->clear();
        _controllerTabs[i-1]->profileSelect->addItems(settings->childGroups());
        _controllerTabs[i-1]->profileSelect->removeItem(_controllerTabs[i-1]->profileSelect->findText("Auto-Gamepad"));
        _controllerTabs[i-1]->profileSelect->removeItem(_controllerTabs[i-1]->profileSelect->findText("Auto-Keyboard"));
        _controllerTabs[i-1]->profileSelect->insertItem(0, "Auto");
        _controllerTabs[i-1]->profileSelect->setCurrentText(current);
    }
    box->clear();
    box->addItems(settings->childGroups());
    box->removeItem(box->findText("Auto-Gamepad"));
    box->removeItem(box->findText("Auto-Keyboard"));
}

int ProfileTab::checkNotRunning(m64p_dynlib_handle coreHandle)
{
    int value;
    ptr_CoreDoCommand CoreDoCommand = (ptr_CoreDoCommand) osal_dynlib_getproc(coreHandle, "CoreDoCommand");
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);
    if (value == M64EMU_RUNNING) {
        QMessageBox msgBox;
        msgBox.setText("Stop game before editing profiles.");
        msgBox.exec();
        return 0;
    }
    return 1;
}

ProfileTab::ProfileTab(m64p_dynlib_handle coreHandle, ControllerTab **_controllerTabs, QSettings* settings, QSettings* controllerSettings, QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    QComboBox *profileSelect = new QComboBox(this);
    setComboBox(profileSelect, _controllerTabs, settings, controllerSettings);
    QPushButton *buttonNewKeyboard = new QPushButton("New Profile (Keyboard)", this);
    connect(buttonNewKeyboard, &QPushButton::released, [=]() {
        if (checkNotRunning(coreHandle)) {
            ProfileEditor editor("Auto-Keyboard", settings, this);
            editor.exec();
            setComboBox(profileSelect, _controllerTabs, settings, controllerSettings);
        }
    });
    QPushButton *buttonNewGamepad = new QPushButton("New Profile (Gamepad)", this);
    connect(buttonNewGamepad, &QPushButton::released, [=]() {
        if (checkNotRunning(coreHandle)) {
            ProfileEditor editor("Auto-Gamepad", settings, this);
            editor.exec();
            setComboBox(profileSelect, _controllerTabs, settings, controllerSettings);
        }
    });
    QPushButton *buttonEdit = new QPushButton("Edit Profile", this);
    connect(buttonEdit, &QPushButton::released, [=]() {
        if (!profileSelect->currentText().isEmpty() && checkNotRunning(coreHandle)) {
            ProfileEditor editor(profileSelect->currentText(), settings, this);
            editor.exec();
        }
    });

    QPushButton *buttonDelete = new QPushButton("Delete Profile", this);
    connect(buttonDelete, &QPushButton::released, [=]() {
        if (!profileSelect->currentText().isEmpty() && checkNotRunning(coreHandle)) {
            settings->remove(profileSelect->currentText());
            setComboBox(profileSelect, _controllerTabs, settings, controllerSettings);
        }
    });

    layout->addWidget(profileSelect, 1, 0);
    layout->addWidget(buttonNewKeyboard, 0, 1);
    layout->addWidget(buttonNewGamepad, 0, 2);
    layout->addWidget(buttonEdit, 1, 1);
    layout->addWidget(buttonDelete, 1, 2);
    setLayout(layout);
}

CustomButton::CustomButton(QString section, QString setting, QSettings *settings, QWidget* parent)
{
    ProfileEditor* editor = (ProfileEditor*) parent;
    item = setting;
    QString direction;
    QStringList value = settings->value(section + "/" + item).toString().split(",");
    switch (value.at(1).toInt()) {
        case 0/*Keyboard*/:
            type = 0;
            key = (SDL_Scancode)value.at(0).toInt();
            this->setText(SDL_GetScancodeName(key));
            break;
        case 1/*Button*/:
            type = 1;
            button = (SDL_GameControllerButton)value.at(0).toInt();
            this->setText(SDL_GameControllerGetStringForButton((SDL_GameControllerButton)value.at(0).toInt()));
            break;
        case 2/*Axis*/:
            type = 2;
            axis = (SDL_GameControllerAxis)value.at(0).toInt();
            axisValue = value.at(2).toInt();
            direction = axisValue > 0 ? " +" : " -";
            this->setText(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)value.at(0).toInt()) + direction);
            break;
        case 3/*Joystick Hat*/:
            type = 3;
            joystick_hat = value.at(0).toInt();
            axisValue = value.at(2).toInt();
            this->setText("Hat " + QString::number(joystick_hat) + " " + QString::number(axisValue));
            break;
        case 4/*Joystick Button*/:
            type = 4;
            joystick_button = value.at(0).toInt();
            this->setText("Button " + QString::number(joystick_button));
            break;
        case 5/*Joystick Axis*/:
            type = 5;
            joystick_axis = value.at(0).toInt();
            axisValue = value.at(2).toInt();
            direction = axisValue > 0 ? " +" : " -";
            this->setText("Axis " + QString::number(joystick_axis) + direction);
           break;
    }
    connect(this, &QPushButton::released, [=]{
        editor->acceptInput(this);
    });
}

void ProfileEditor::keyReleaseEvent(QKeyEvent *event)
{
    int modValue = QT2SDL2MOD(event->modifiers());
    int keyValue = QT2SDL2(event->key());
    SDL_Scancode value = (SDL_Scancode)((modValue << 16) + keyValue);
    if (activeButton != nullptr) {
        killTimer(timer);
        activeButton->type = 0;
        activeButton->key = value;
        activeButton->setText(SDL_GetScancodeName(activeButton->key));
        activeButton = nullptr;
        for (int i = 0; i < buttonList.size(); ++i)
            buttonList.at(i)->setDisabled(0);
    }
}

void ProfileEditor::timerEvent(QTimerEvent *)
{
    int i;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_JOYHATMOTION:
                killTimer(timer);
                activeButton->type = 3;
                activeButton->joystick_hat = e.jhat.hat;
                activeButton->axisValue = e.jhat.value;
                activeButton->setText("Hat " + QString::number(activeButton->joystick_hat) + " " + QString::number(activeButton->axisValue));
                activeButton = nullptr;
                for (i = 0; i < buttonList.size(); ++i)
                    buttonList.at(i)->setDisabled(0);
                return;
            case SDL_JOYBUTTONDOWN:
                killTimer(timer);
                activeButton->type = 4;
                activeButton->joystick_button = e.jbutton.button;
                activeButton->setText("Button " + QString::number(activeButton->joystick_button));
                activeButton = nullptr;
                for (i = 0; i < buttonList.size(); ++i)
                    buttonList.at(i)->setDisabled(0);
                return;
            case SDL_JOYAXISMOTION:
                if (abs(e.jaxis.value) > 16384) {
                    killTimer(timer);
                    activeButton->type = 5;
                    activeButton->joystick_axis = e.jaxis.axis;
                    activeButton->axisValue = e.jaxis.value > 0 ? 1 : -1;
                    QString direction = activeButton->axisValue > 0 ? " +" : " -";
                    activeButton->setText("Axis " + QString::number(activeButton->joystick_axis) + direction);
                    activeButton = nullptr;
                    for (i = 0; i < buttonList.size(); ++i)
                        buttonList.at(i)->setDisabled(0);
                    return;
                }
                break;
        }
    }

    if (buttonTimer == 0) {
        killTimer(timer);
        activeButton->setText(activeButton->origText);
        activeButton = nullptr;
        for (i = 0; i < buttonList.size(); ++i) {
            buttonList.at(i)->setDisabled(0);
        }
        return;
    }
    --buttonTimer;
    activeButton->setText(QString::number(ceil(buttonTimer/10.0)));
}

void ProfileEditor::acceptInput(CustomButton* button)
{
    activeButton = button;
    for (int i = 0; i < buttonList.size(); ++i) {
        buttonList.at(i)->setDisabled(1);
    }
    buttonTimer = 50;
    activeButton->origText = activeButton->text();
    activeButton->setText(QString::number(buttonTimer/10));
    SDL_Event e;
    while (SDL_PollEvent(&e)) {}
    timer = startTimer(100);
}

ProfileEditor::~ProfileEditor()
{
    for (int i = 0; i < 4; ++i) {
        if (joystick[i]) {
            SDL_JoystickClose(joystick[i]);
            joystick[i] = NULL;
        }
    }
}

ProfileEditor::ProfileEditor(QString profile, QSettings *settings, QWidget *parent)
    : QDialog(parent)
{
    memset(joystick, 0, sizeof(SDL_Joystick*) * 4);
    for (int i = 0; i < 4; ++i)
        joystick[i] = SDL_JoystickOpen(i);

    activeButton = nullptr;
    QString section = profile;
    QLineEdit *profileName = new QLineEdit(this);
    if (profile == "Auto-Keyboard" || profile == "Auto-Gamepad") {
        profileName->setDisabled(0);
        profile = "";
    }
    else
        profileName->setDisabled(1);

    QGridLayout *layout = new QGridLayout(this);
    QLabel *profileNameLabel = new QLabel("Profile Name", this);
    profileName->setText(profile);
    layout->addWidget(profileNameLabel, 0, 3);
    layout->addWidget(profileName, 0, 4);

    QFrame* lineH = new QFrame(this);
    lineH->setFrameShape(QFrame::HLine);
    lineH->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineH, 1, 0, 1, 8);

    QLabel *buttonLabelA = new QLabel("A", this);
    buttonLabelA->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushA = new CustomButton(section, "A", settings, this);
    buttonList.append(buttonPushA);
    layout->addWidget(buttonLabelA, 2, 0);
    layout->addWidget(buttonPushA, 2, 1);

    QLabel *buttonLabelB = new QLabel("B", this);
    buttonLabelB->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushB = new CustomButton(section, "B", settings, this);
    buttonList.append(buttonPushB);
    layout->addWidget(buttonLabelB, 3, 0);
    layout->addWidget(buttonPushB, 3, 1);

    QLabel *buttonLabelZ = new QLabel("Z", this);
    buttonLabelZ->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushZ = new CustomButton(section, "Z", settings, this);
    buttonList.append(buttonPushZ);
    layout->addWidget(buttonLabelZ, 4, 0);
    layout->addWidget(buttonPushZ, 4, 1);

    QLabel *buttonLabelStart = new QLabel("Start", this);
    buttonLabelStart->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushStart = new CustomButton(section, "Start", settings, this);
    buttonList.append(buttonPushStart);
    layout->addWidget(buttonLabelStart, 5, 0);
    layout->addWidget(buttonPushStart, 5, 1);

    QLabel *buttonLabelLTrigger = new QLabel("Left Trigger", this);
    buttonLabelLTrigger->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushLTrigger = new CustomButton(section, "L", settings, this);
    buttonList.append(buttonPushLTrigger);
    layout->addWidget(buttonLabelLTrigger, 6, 0);
    layout->addWidget(buttonPushLTrigger, 6, 1);

    QLabel *buttonLabelRTrigger = new QLabel("Right Trigger", this);
    buttonLabelRTrigger->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushRTrigger = new CustomButton(section, "R", settings, this);
    buttonList.append(buttonPushRTrigger);
    layout->addWidget(buttonLabelRTrigger, 7, 0);
    layout->addWidget(buttonPushRTrigger, 7, 1);

    QFrame* lineV = new QFrame(this);
    lineV->setFrameShape(QFrame::VLine);
    lineV->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineV, 2, 2, 6, 1);

    QLabel *buttonLabelDPadL = new QLabel("DPad Left", this);
    buttonLabelDPadL->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushDPadL = new CustomButton(section, "DPadL", settings, this);
    buttonList.append(buttonPushDPadL);
    layout->addWidget(buttonLabelDPadL, 2, 3);
    layout->addWidget(buttonPushDPadL, 2, 4);

    QLabel *buttonLabelDPadR = new QLabel("DPad Right", this);
    buttonLabelDPadR->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushDPadR = new CustomButton(section, "DPadR", settings, this);
    buttonList.append(buttonPushDPadR);
    layout->addWidget(buttonLabelDPadR, 3, 3);
    layout->addWidget(buttonPushDPadR, 3, 4);

    QLabel *buttonLabelDPadU = new QLabel("DPad Up", this);
    buttonLabelDPadU->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushDPadU = new CustomButton(section, "DPadU", settings, this);
    buttonList.append(buttonPushDPadU);
    layout->addWidget(buttonLabelDPadU, 4, 3);
    layout->addWidget(buttonPushDPadU, 4, 4);

    QLabel *buttonLabelDPadD = new QLabel("DPad Down", this);
    buttonLabelDPadD->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushDPadD = new CustomButton(section, "DPadD", settings, this);
    buttonList.append(buttonPushDPadD);
    layout->addWidget(buttonLabelDPadD, 5, 3);
    layout->addWidget(buttonPushDPadD, 5, 4);

    QFrame* lineV2 = new QFrame(this);
    lineV2->setFrameShape(QFrame::VLine);
    lineV2->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineV2, 2, 5, 6, 1);

    QLabel *buttonLabelCL = new QLabel("C Left", this);
    buttonLabelCL->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushCL = new CustomButton(section, "CLeft", settings, this);
    buttonList.append(buttonPushCL);
    layout->addWidget(buttonLabelCL, 2, 6);
    layout->addWidget(buttonPushCL, 2, 7);

    QLabel *buttonLabelCR = new QLabel("C Right", this);
    buttonLabelCR->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushCR = new CustomButton(section, "CRight", settings, this);
    buttonList.append(buttonPushCR);
    layout->addWidget(buttonLabelCR, 3, 6);
    layout->addWidget(buttonPushCR, 3, 7);

    QLabel *buttonLabelCU = new QLabel("C Up", this);
    buttonLabelCU->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushCU = new CustomButton(section, "CUp", settings, this);
    buttonList.append(buttonPushCU);
    layout->addWidget(buttonLabelCU, 4, 6);
    layout->addWidget(buttonPushCU, 4, 7);

    QLabel *buttonLabelCD = new QLabel("C Down", this);
    buttonLabelCD->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushCD = new CustomButton(section, "CDown", settings, this);
    buttonList.append(buttonPushCD);
    layout->addWidget(buttonLabelCD, 5, 6);
    layout->addWidget(buttonPushCD, 5, 7);


    QLabel *buttonLabelStickL = new QLabel("Control Stick Left", this);
    buttonLabelStickL->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushStickL = new CustomButton(section, "AxisLeft", settings, this);
    buttonList.append(buttonPushStickL);
    layout->addWidget(buttonLabelStickL, 6, 3);
    layout->addWidget(buttonPushStickL, 6, 4);

    QLabel *buttonLabelStickR = new QLabel("Control Stick Right", this);
    buttonLabelStickR->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushStickR = new CustomButton(section, "AxisRight", settings, this);
    buttonList.append(buttonPushStickR);
    layout->addWidget(buttonLabelStickR, 7, 3);
    layout->addWidget(buttonPushStickR, 7, 4);

    QLabel *buttonLabelStickU = new QLabel("Control Stick Up", this);
    buttonLabelStickU->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushStickU = new CustomButton(section, "AxisUp", settings, this);
    buttonList.append(buttonPushStickU);
    layout->addWidget(buttonLabelStickU, 6, 6);
    layout->addWidget(buttonPushStickU, 6, 7);

    QLabel *buttonLabelStickD = new QLabel("Control Stick Down", this);
    buttonLabelStickD->setAlignment(Qt::AlignCenter);
    CustomButton *buttonPushStickD = new CustomButton(section, "AxisDown", settings, this);
    buttonList.append(buttonPushStickD);
    layout->addWidget(buttonLabelStickD, 7, 6);
    layout->addWidget(buttonPushStickD, 7, 7);

    QFrame* lineH2 = new QFrame(this);
    lineH2->setFrameShape(QFrame::HLine);
    lineH2->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineH2, 8, 0, 1, 8);

    QLabel *buttonLabelDeadzone = new QLabel("Deadzone", this);
    buttonLabelDeadzone->setAlignment(Qt::AlignCenter);
    QLabel *buttonLabelDeadzoneValue = new QLabel(this);
    if (!settings->contains(section + "/Deadzone"))
        settings->setValue(section + "/Deadzone", 12.5);
    float deadzoneValue = settings->value(section + "/Deadzone").toFloat();
    buttonLabelDeadzoneValue->setText(QString::number(deadzoneValue) + "%");
    buttonLabelDeadzoneValue->setAlignment(Qt::AlignCenter);
    QSlider *sliderDeadzone = new QSlider(Qt::Horizontal, this);
    sliderDeadzone->setMinimum(0);
    sliderDeadzone->setMaximum(250);
    sliderDeadzone->setTickPosition(QSlider::TicksBothSides);
    sliderDeadzone->setTickInterval(5);
    sliderDeadzone->setSliderPosition((int)(deadzoneValue * 10.0));
    connect(sliderDeadzone, &QSlider::valueChanged, [=](int value) {
        float percent = value / 10.0;
        buttonLabelDeadzoneValue->setText(QString::number(percent, 'f', 1) + "%");
    });

    layout->addWidget(buttonLabelDeadzone, 9, 0);
    layout->addWidget(buttonLabelDeadzoneValue, 9, 1);
    layout->addWidget(sliderDeadzone, 9, 2, 1, 6);

    QLabel *buttonLabelSensitivity = new QLabel("Analog Sensitivity", this);
    buttonLabelSensitivity->setAlignment(Qt::AlignCenter);
    QLabel *buttonLabelSensitivityValue = new QLabel(this);
    if (!settings->contains(section + "/Sensitivity"))
        settings->setValue(section + "/Sensitivity", 100.0);
    float sensitivityValue = settings->value(section + "/Sensitivity").toFloat();
    buttonLabelSensitivityValue->setText(QString::number(sensitivityValue) + "%");
    buttonLabelSensitivityValue->setAlignment(Qt::AlignCenter);
    QSlider *sliderSensitivity = new QSlider(Qt::Horizontal, this);
    sliderSensitivity->setMinimum(800);
    sliderSensitivity->setMaximum(1200);
    sliderSensitivity->setTickPosition(QSlider::TicksBothSides);
    sliderSensitivity->setTickInterval(5);
    sliderSensitivity->setSliderPosition((int)(sensitivityValue * 10.0));
    connect(sliderSensitivity, &QSlider::valueChanged, [=](int value) {
        float percent = value / 10.0;
        buttonLabelSensitivityValue->setText(QString::number(percent, 'f', 1) + "%");
    });

    layout->addWidget(buttonLabelSensitivity, 10, 0);
    layout->addWidget(buttonLabelSensitivityValue, 10, 1);
    layout->addWidget(sliderSensitivity, 10, 2, 1, 6);

    QFrame* lineH3 = new QFrame(this);
    lineH3->setFrameShape(QFrame::HLine);
    lineH3->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineH3, 11, 0, 1, 8);

    QPushButton *buttonPushSave = new QPushButton("Save and Close", this);
    connect(buttonPushSave, &QPushButton::released, [=]() {
        const QString saveSection = profileName->text();
        if (!saveSection.startsWith("Auto-") && !saveSection.isEmpty() && (!settings->contains(saveSection + "/A") || !profileName->isEnabled())) {
            QStringList value;
            for (int i = 0; i < buttonList.size(); ++i) {
                value.clear();
                switch (buttonList.at(i)->type) {
                    case 0:
                        value.insert(0, QString::number(buttonList.at(i)->key));
                        value.insert(1, QString::number(0));
                        break;
                    case 1:
                        value.insert(0, QString::number(buttonList.at(i)->button));
                        value.insert(1, QString::number(1));
                        break;
                    case 2:
                        value.insert(0, QString::number(buttonList.at(i)->axis));
                        value.insert(1, QString::number(2));
                        value.insert(2, QString::number(buttonList.at(i)->axisValue));
                        break;
                    case 3:
                        value.insert(0, QString::number(buttonList.at(i)->joystick_hat));
                        value.insert(1, QString::number(3));
                        value.insert(2, QString::number(buttonList.at(i)->axisValue));
                        break;
                    case 4:
                        value.insert(0, QString::number(buttonList.at(i)->joystick_button));
                        value.insert(1, QString::number(4));
                        break;
                    case 5:
                        value.insert(0, QString::number(buttonList.at(i)->joystick_axis));
                        value.insert(1, QString::number(5));
                        value.insert(2, QString::number(buttonList.at(i)->axisValue));
                        break;
                }
                settings->setValue(saveSection + "/" + buttonList.at(i)->item, value.join(","));
            }
            float percent = sliderDeadzone->value() / 10.0;
            settings->setValue(saveSection + "/Deadzone", percent);
            percent = sliderSensitivity->value() / 10.0;
            settings->setValue(saveSection + "/Sensitivity", percent);
            this->done(1);
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Invalid profile name.");
            msgBox.exec();
        }
    });
    layout->addWidget(buttonPushSave, 12, 0, 1, 2);
    QPushButton *buttonPushClose = new QPushButton("Close Without Saving", this);
    connect(buttonPushClose, &QPushButton::released, [=]() {
        this->done(1);
    });
    layout->addWidget(buttonPushClose, 12, 6, 1, 2);

    setLayout(layout);
    setWindowTitle(tr("Profile Editor"));
}

ConfigDialog::ConfigDialog(m64p_dynlib_handle CoreHandle, QSettings* settings, QSettings* controllerSettings)
{
    unsigned int i;

    tabWidget = new QTabWidget(this);
    tabWidget->setUsesScrollButtons(0);
    for (i = 1; i < 5; ++i) {
        controllerTabs[i-1] = new ControllerTab(i, settings, controllerSettings, this);
        tabWidget->addTab(controllerTabs[i-1], "Controller " + QString::number(i));
    }

    tabWidget->addTab(new ProfileTab(CoreHandle, controllerTabs, settings, controllerSettings, this), tr("Manage Profiles"));
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setWindowTitle(tr("Controller Configuration"));
}
