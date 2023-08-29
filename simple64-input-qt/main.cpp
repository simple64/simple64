/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   simple64-input-qt - main.cpp                                       *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2008-2011 Richard Goedeken                              *
 *   Copyright (C) 2008 Tillin9                                            *
 *   Copyright (C) 2002 Blight                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_common.h"
#include "m64p_types.h"
#include "m64p_config.h"
#include "m64p_plugin.h"
#include "main.h"
#include "configdialog.h"
#include "vosk/vosk_api.h"
#include "osal/osal_dynamiclib.h"
#include "vruwords.h"
#include "funcs.h"

#include <QDir>
#include <QCoreApplication>
#include <QLibrary>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringDecoder>

#define SETTINGS_VER 2
#define AXIS_PEAK 32768
#define MAX_AXIS_VALUE 85
#define DEADZONE_DEFAULT 5.0

#define QT_INPUT_PLUGIN_VERSION 0x020500
#define INPUT_PLUGIN_API_VERSION 0x020101
static int l_PluginInit = 0;
static int l_TalkingState = 0;
static unsigned char myKeyState[SDL_NUM_SCANCODES];
static QSettings* settings;
static QSettings* controllerSettings;
static QSettings* gameSettings;
static QSettings* gameControllerSettings;
static SController controller[4];   // 4 controllers
static m64p_dynlib_handle coreHandle;
static VoskModel *model;
static VoskRecognizer *recognizer;
static QStringList words;
static QList<int> word_indexes;
static QJsonObject vruwordsobject;
static int word_list_length;
static int word_list_count;
static QLibrary *voskLib;
static SDL_AudioDeviceID audio_dev;
static SDL_AudioSpec *hardware_spec;
static SDL_TimerID timer_id;

typedef VoskModel* (*ptr_vosk_model_new)(const char *model_path);
typedef void (*ptr_vosk_model_free)(VoskModel *model);
typedef VoskRecognizer* (*ptr_vosk_recognizer_new_grm)(VoskModel *model, float sample_rate, const char *grammar);
typedef void (*ptr_vosk_recognizer_free)(VoskRecognizer *recognizer);
typedef int (*ptr_vosk_recognizer_accept_waveform)(VoskRecognizer *recognizer, const char *data, int length);
typedef const char *(*ptr_vosk_recognizer_final_result)(VoskRecognizer *recognizer);
typedef void (*ptr_vosk_set_log_level)(int log_level);
typedef void (*ptr_vosk_recognizer_set_max_alternatives)(VoskRecognizer *recognizer, int max_alternatives);

ptr_vosk_recognizer_accept_waveform VoskAcceptWaveform;
ptr_vosk_recognizer_final_result VoskFinalResult;
ptr_vosk_model_new VoskNewModel;
ptr_vosk_recognizer_new_grm VoskNewRecognizer;
ptr_vosk_model_free VoskFreeModel;
ptr_vosk_recognizer_free VoskFreeRecognizer;
ptr_vosk_set_log_level VoskSetLogLevel;
ptr_vosk_recognizer_set_max_alternatives VoskSetAlternatives;

ptr_ConfigGetUserConfigPath ConfigGetUserConfigPath;
ptr_ConfigGetUserDataPath ConfigGetUserDataPath;

static void (*l_DebugCallback)(void *, int, const char *) = NULL;

void DebugMessage(int level, const char *message, ...)
{
    char msgbuf[1024];
    va_list args;

    if (l_DebugCallback == NULL)
        return;

    va_start(args, message);
    vsprintf(msgbuf, message, args);

    (*l_DebugCallback)((char*)"Input", level, msgbuf);

    va_end(args);
}

EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle CoreHandle, void * object, void (*DebugCallback)(void *, int, const char *))
{
    if (l_PluginInit)
        return M64ERR_ALREADY_INIT;

    coreHandle = CoreHandle;
    l_DebugCallback = DebugCallback;

    ConfigGetUserConfigPath = (ptr_ConfigGetUserConfigPath) osal_dynlib_getproc(CoreHandle, "ConfigGetUserConfigPath");
    ConfigGetUserDataPath = (ptr_ConfigGetUserDataPath) osal_dynlib_getproc(CoreHandle, "ConfigGetUserDataPath");

    QDir ini_path(ConfigGetUserConfigPath());
    settings = new QSettings(ini_path.absoluteFilePath("input-profiles.ini"), QSettings::IniFormat, (QObject*)object);
    controllerSettings = new QSettings(ini_path.absoluteFilePath("input-settings.ini"), QSettings::IniFormat, (QObject*)object);

    if (!settings->contains("version") || settings->value("version").toInt() != SETTINGS_VER)
    {
        settings->clear();
        settings->setValue("version", SETTINGS_VER);
    }
    if (!controllerSettings->contains("version") || controllerSettings->value("version").toInt() != SETTINGS_VER)
    {
        controllerSettings->clear();
        controllerSettings->setValue("version", SETTINGS_VER);
    }

    QString section;
    for (int i = 1; i < 5; ++i) {
        section = "Controller" + QString::number(i);
        if (!controllerSettings->childGroups().contains(section)) {
            controllerSettings->setValue(section + "/Profile", "Auto");
            controllerSettings->setValue(section + "/Gamepad", "Auto");
            controllerSettings->setValue(section + "/Pak", "Memory");
        }
    }

    QStringList values;
    section = "Auto-Keyboard";
    values.insert(0, "0"/*blank value*/);
    values.insert(1, "0"/*Keyboard*/);
    if (!settings->childGroups().contains(section)) {
        values.replace(0, QString::number(SDL_SCANCODE_LSHIFT));
        settings->setValue(section + "/A", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_LCTRL));
        settings->setValue(section + "/B", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_Z));
        settings->setValue(section + "/Z", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_X));
        settings->setValue(section + "/L", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_C));
        settings->setValue(section + "/R", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_RETURN));
        settings->setValue(section + "/Start", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_A));
        settings->setValue(section + "/DPadL", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_D));
        settings->setValue(section + "/DPadR", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_W));
        settings->setValue(section + "/DPadU", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_S));
        settings->setValue(section + "/DPadD", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_J));
        settings->setValue(section + "/CLeft", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_L));
        settings->setValue(section + "/CRight", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_I));
        settings->setValue(section + "/CUp", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_K));
        settings->setValue(section + "/CDown", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_LEFT));
        settings->setValue(section + "/AxisLeft", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_RIGHT));
        settings->setValue(section + "/AxisRight", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_UP));
        settings->setValue(section + "/AxisUp", values.join(","));
        values.replace(0, QString::number(SDL_SCANCODE_DOWN));
        settings->setValue(section + "/AxisDown", values.join(","));

        settings->setValue(section + "/Deadzone", DEADZONE_DEFAULT);
        settings->setValue(section + "/Sensitivity", 100.0);
    }

    section = "Auto-Gamepad";
    values.replace(1, "1"/*Button*/);
    if (!settings->childGroups().contains(section)) {
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_A));
        settings->setValue(section + "/A", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_X));
        settings->setValue(section + "/B", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_TRIGGERLEFT));
        values.replace(1, "2"/*Axis*/);
        values.insert(2, "1" /* positive axis value*/);
        settings->setValue(section + "/Z", values.join(","));
        values.removeAt(2);
        values.replace(1, "1"/*Button*/);
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
        settings->setValue(section + "/L", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
        settings->setValue(section + "/R", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_START));
        settings->setValue(section + "/Start", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
        settings->setValue(section + "/DPadL", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
        settings->setValue(section + "/DPadR", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_DPAD_UP));
        settings->setValue(section + "/DPadU", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
        settings->setValue(section + "/DPadD", values.join(","));
        values.replace(1, "2"/*Axis*/);
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_RIGHTX));
        values.insert(2, "-1" /* negative axis value*/);
        settings->setValue(section + "/CLeft", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_RIGHTX));
        values.replace(2, "1" /* positive axis value*/);
        settings->setValue(section + "/CRight", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_RIGHTY));
        values.replace(2, "-1" /* negative axis value*/);
        settings->setValue(section + "/CUp", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_RIGHTY));
        values.replace(2, "1" /* positive axis value*/);
        settings->setValue(section + "/CDown", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_LEFTX));
        values.replace(2, "-1" /* negative axis value*/);
        settings->setValue(section + "/AxisLeft", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_LEFTX));
        values.replace(2, "1" /* positive axis value*/);
        settings->setValue(section + "/AxisRight", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_LEFTY));
        values.replace(2, "-1" /* negative axis value*/);
        settings->setValue(section + "/AxisUp", values.join(","));
        values.replace(0, QString::number(SDL_CONTROLLER_AXIS_LEFTY));
        values.replace(2, "1" /* positive axis value*/);
        settings->setValue(section + "/AxisDown", values.join(","));

        settings->setValue(section + "/Deadzone", DEADZONE_DEFAULT);
        settings->setValue(section + "/Sensitivity", 100.0);
    }

    if (!SDL_WasInit(SDL_INIT_GAMECONTROLLER))
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    if (!SDL_WasInit(SDL_INIT_AUDIO))
        SDL_InitSubSystem(SDL_INIT_AUDIO);

    if (!SDL_WasInit(SDL_INIT_TIMER))
        SDL_InitSubSystem(SDL_INIT_TIMER);

    l_PluginInit = 1;

    return M64ERR_SUCCESS;
}

void closeControllers()
{
    for (int i = 0; i < 4; ++i) {
        if (controller[i].gamepad != NULL)
            SDL_GameControllerClose(controller[i].gamepad);
        else if (controller[i].joystick != NULL)
            SDL_JoystickClose(controller[i].joystick);
        controller[i].gamepad = NULL;
        controller[i].joystick = NULL;
    }
}

EXPORT m64p_error CALL PluginShutdown(void)
{
    if (!l_PluginInit)
        return M64ERR_NOT_INIT;

    closeControllers();

    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    SDL_QuitSubSystem(SDL_INIT_TIMER);

    settings->sync();
    controllerSettings->sync();

    l_PluginInit = 0;

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    /* set version info */
    if (PluginType != NULL)
        *PluginType = M64PLUGIN_INPUT;

    if (PluginVersion != NULL)
        *PluginVersion = QT_INPUT_PLUGIN_VERSION;

    if (APIVersion != NULL)
        *APIVersion = INPUT_PLUGIN_API_VERSION;

    if (PluginNamePtr != NULL)
        *PluginNamePtr = "simple64 Qt Input Plugin";

    if (Capabilities != NULL)
    {
        *Capabilities = 0;
    }

    return M64ERR_SUCCESS;
}

static unsigned char DataCRC( unsigned char *Data, int iLenght )
{
    unsigned char Remainder = Data[0];

    int iByte = 1;
    unsigned char bBit = 0;

    while( iByte <= iLenght )
    {
        int HighBit = ((Remainder & 0x80) != 0);
        Remainder = Remainder << 1;

        Remainder += ( iByte < iLenght && Data[iByte] & (0x80 >> bBit )) ? 1 : 0;

        Remainder ^= (HighBit) ? 0x85 : 0;

        bBit++;
        iByte += bBit/8;
        bBit %= 8;
    }

    return Remainder;
}

EXPORT void CALL SendVRUWord(uint16_t length, uint16_t *word, uint8_t lang)
{
    QByteArray word_array;
    uint8_t *bytes = (uint8_t*)word;
    for (int i = 0; i < (length*2); i+=2)
    {
        word_array.append(bytes[i+1]);
        word_array.append(bytes[i]);
    }

    QString hex = word_array.toHex();
    QJsonValue value = vruwordsobject.value(hex.toUpper());
    if (value == QJsonValue::Undefined)
    {
        auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
        QString encoded_string = toUtf8(word_array);
        if (toUtf8.hasError())
        {
            if (lang == 0 /* English */)
                DebugMessage(M64MSG_ERROR, "Unknown word: %s", hex.toUpper().toUtf8().constData());
            else /* Japanese or demo */
                DebugMessage(M64MSG_ERROR, "Unknown Japanese word: %s", hex.toUpper().toUtf8().constData());
        }
        else
        {
            words.append(encoded_string.toLower());
            word_indexes.append(word_list_count);
        }
    }
    else
    {
        QJsonArray value_list = value.toArray();
        for (int i = 0; i < value_list.size(); ++i)
        {
            words.append(value_list.at(i).toString().toLower());
            word_indexes.append(word_list_count);
            DebugMessage(M64MSG_VERBOSE, "word loaded: %s, index: %d", words.last().toUtf8().constData(), word_indexes.last());
        }
    }

    ++word_list_count;
    if (word_list_count == word_list_length)
    {
        words.append("[unk]");
        word_indexes.append(word_list_count);
        QJsonArray array = QJsonArray::fromStringList(words);
        QJsonDocument doc;
        doc.setArray(array);
        if (recognizer)
            VoskFreeRecognizer(recognizer);
        recognizer = VoskNewRecognizer(model, (float)hardware_spec->freq, doc.toJson().constData());
        VoskSetAlternatives(recognizer, 3);
    }
}

Uint32 StopTalking(Uint32, void *)
{
    l_TalkingState = 0;
    timer_id = 0;
    return(0);
}

EXPORT void CALL SetMicState(int state)
{
    if (timer_id)
    {
        SDL_RemoveTimer(timer_id);
        timer_id = 0;
    }
    l_TalkingState = state;

    if (state)
    {
        timer_id = SDL_AddTimer(2000, StopTalking, NULL);
        SDL_ClearQueuedAudio(audio_dev);
        SDL_PauseAudioDevice(audio_dev, 0);
    }
    else
        SDL_PauseAudioDevice(audio_dev, 1);
}

EXPORT void CALL ClearVRUWords(uint8_t length)
{
    word_list_count = 0;
    word_list_length = length;
    words.clear();
    word_indexes.clear();
    if (recognizer)
        VoskFreeRecognizer(recognizer);
    DebugMessage(M64MSG_VERBOSE, "word list cleared");
    recognizer = nullptr;
}

EXPORT void CALL SetVRUWordMask(uint8_t, uint8_t *)
{
}

static bool compare(QString &s1, QString &s2)
{
    return s1.size() > s2.size();
}

EXPORT void CALL ReadVRUResults(uint16_t *error_flags, uint16_t *num_results, uint16_t *mic_level, uint16_t *voice_level, uint16_t *voice_length, uint16_t *matches)
{
    SDL_PauseAudioDevice(audio_dev, 1);
    uint16_t match[5] = {0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF};
    *error_flags = 0;
    QByteArray data(SDL_GetQueuedAudioSize(audio_dev), 0);
    uint32_t audio_length = SDL_DequeueAudio(audio_dev, data.data(), SDL_GetQueuedAudioSize(audio_dev));
    VoskAcceptWaveform(recognizer, data.constData(), audio_length);
    QJsonDocument doc = QJsonDocument::fromJson(VoskFinalResult(recognizer));
    QJsonObject object = doc.object();
    QStringList found;
    QStringList alternatives;
    QString textResult;
    QJsonArray alternatives_array = object.value("alternatives").toArray();
    for (int i = 0; i < alternatives_array.size(); ++i)
    {
        textResult = alternatives_array.at(i).toObject().value("text").toString();
        if (!textResult.isEmpty())
            alternatives.append(textResult);
    }
    for (int i = 0; i < words.size() - 1 /* -1 to skip the [unk] entry */; ++i)
    {
        if (!found.contains(words.at(i)) && alternatives.filter(words.at(i)).size() > 0)
        {
            /* found a match */
            found.append(words.at(i));
            if (found.size() == 5)
                break;
        }
    }
    /* Use the longest match */
    std::sort(found.begin(), found.end(), compare);
    for (int i = 0; i < found.size(); ++i)
    {
        match[i] = word_indexes.at(words.indexOf(found.at(i)));
        DebugMessage(M64MSG_INFO, "match %d: %s (%u)", i, found.at(i).toUtf8().constData(), match[i]);
    }
    if (found.size() == 0 && alternatives.size() > 0)
    {
        /* heard something but it didn't match anything */
        *error_flags = 0x4000;
        match[0] = 0; // we match index 0, but mark the error flag saying we are really not sure
        found.append("0");
        DebugMessage(M64MSG_INFO, "heard a noise, but no word match");
    }

    *num_results = found.size();
    /* The N64 programming manual states what range these values should be within */
    *mic_level = 0xBB8;
    *voice_level = 0xBB8;
    *voice_length = 0x8004;
    matches[0] = match[0];
    matches[1] = *error_flags ? 0x700 : 0;
    matches[2] = match[1];
    matches[3] = (match[1] == 0x7FFF) ? 0 : 0x100;
    matches[4] = match[2];
    matches[5] = (match[2] == 0x7FFF) ? 0 : 0x200;
    matches[6] = match[3];
    matches[7] = (match[3] == 0x7FFF) ? 0 : 0x300;
    matches[8] = match[4];
    matches[9] = (match[4] == 0x7FFF) ? 0 : 0x400;
}

EXPORT void CALL ControllerCommand(int Control, unsigned char *Command)
{
    unsigned char *Data = &Command[5];

    if (Control == -1)
        return;

    switch (Command[2])
    {
        case RD_GETSTATUS:
        case RD_READKEYS:
            break;
        case RD_READPAK:
            if (controller[Control].control->Plugin == PLUGIN_RAW)
            {
                unsigned int dwAddress = (Command[3] << 8) + (Command[4] & 0xE0);

                if(( dwAddress >= 0x8000 ) && ( dwAddress < 0x9000 ) )
                    memset( Data, 0x80, 32 );
                else
                    memset( Data, 0x00, 32 );

                Data[32] = DataCRC( Data, 32 );
            }
            break;
        case RD_WRITEPAK:
            if (controller[Control].control->Plugin == PLUGIN_RAW)
            {
                unsigned int dwAddress = (Command[3] << 8) + (Command[4] & 0xE0);
                if(dwAddress == PAK_IO_RUMBLE) {
                    if (*Data) {
                        if (controller[Control].gamepad)
                            SDL_GameControllerRumble(controller[Control].gamepad, 0xFFFF, 0xFFFF, SDL_HAPTIC_INFINITY);
                        else
                            SDL_JoystickRumble(controller[Control].joystick, 0xFFFF, 0xFFFF, SDL_HAPTIC_INFINITY);
                    } else {
                        if (controller[Control].gamepad)
                            SDL_GameControllerRumble(controller[Control].gamepad, 0, 0, 0);
                        else
                            SDL_JoystickRumble(controller[Control].joystick, 0, 0, 0);
                    }
                }
                Data[32] = DataCRC( Data, 32 );
            }
            break;
        case RD_RESETCONTROLLER:
        case RD_READEEPROM:
        case RD_WRITEEPROM:
            break;
        }
}

void boundAxis(double* x, double* y) {
    double radius = 95.0; // this is roughly the maxium diagonal distance of the controller

    // Calculate the distance from the origin (0, 0)
    double distance = sqrt((*x) * (*x) + (*y) * (*y));

    // If the distance is greater than the radius, scale the coordinates
    if (distance > radius) {
        double scaleFactor = radius / distance;
        *x *= scaleFactor;
        *y *= scaleFactor;
    }
}

int modifyAxisValue(int axis_value, int Control, int direction)
{
    axis_value = ((abs(axis_value) - controller[Control].deadzone) * MAX_AXIS_VALUE) / controller[Control].range;
    axis_value *= direction;
    axis_value = (float)axis_value * controller[Control].sensitivity;
    axis_value = std::max(-MAX_AXIS_VALUE, std::min(axis_value, MAX_AXIS_VALUE));

    return axis_value;
}

void setAxis(int Control, int axis, BUTTONS *Keys, QString axis_dir, int direction)
{
    int axis_value;
    QStringList value = gameSettings->value(controller[Control].profile + "/" + axis_dir).toString().split(",");

    switch (value.at(1).toInt()) {
        case 0 /*Keyboard*/:
            if (myKeyState[value.at(0).toInt()]) {
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
                else
                    Keys->Y_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
            }
            break;
        case 1 /*Button*/:
            if (SDL_GameControllerGetButton(controller[Control].gamepad, (SDL_GameControllerButton)value.at(0).toInt())) {
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
                else
                    Keys->Y_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
            }
            break;
        case 2 /*Axis*/:
            axis_value = SDL_GameControllerGetAxis(controller[Control].gamepad, (SDL_GameControllerAxis)value.at(0).toInt());
            if (abs(axis_value) > controller[Control].deadzone && axis_value * value.at(2).toInt() > 0) {
                axis_value = modifyAxisValue(axis_value, Control, direction);
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)axis_value;
                else
                    Keys->Y_AXIS = (int8_t)axis_value;
            }
            break;
        case 3 /*Joystick Hat*/:
            if (SDL_JoystickGetHat(controller[Control].joystick, value.at(0).toInt()) & value.at(2).toInt()) {
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
                else
                    Keys->Y_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
            }
            break;
        case 4 /*Joystick Button*/:
            if (SDL_JoystickGetButton(controller[Control].joystick, value.at(0).toInt())) {
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
                else
                    Keys->Y_AXIS = (int8_t)(MAX_AXIS_VALUE * direction);
            }
            break;
        case 5 /*Joystick Axis*/:
            axis_value = SDL_JoystickGetAxis(controller[Control].joystick, value.at(0).toInt());
            if (abs(axis_value) > controller[Control].deadzone && axis_value * value.at(2).toInt() > 0) {
                axis_value = modifyAxisValue(axis_value, Control, direction);
                if (axis == 0)
                    Keys->X_AXIS = (int8_t)axis_value;
                else
                    Keys->Y_AXIS = (int8_t)axis_value;
            }
            break;
    }
}

void setKey(int Control, uint32_t key, BUTTONS *Keys, QString button)
{
    int axis_value;
    QStringList value = gameSettings->value(controller[Control].profile + "/" + button).toString().split(",");
    switch (value.at(1).toInt()) {
        case 0 /*Keyboard*/:
            if (myKeyState[value.at(0).toInt()])
                Keys->Value |= key;
            break;
        case 1 /*Button*/:
            if (SDL_GameControllerGetButton(controller[Control].gamepad, (SDL_GameControllerButton)value.at(0).toInt()))
                Keys->Value |= key;
            break;
        case 2 /*Axis*/:
            axis_value = SDL_GameControllerGetAxis(controller[Control].gamepad, (SDL_GameControllerAxis)value.at(0).toInt());
            if (abs(axis_value) >= (AXIS_PEAK / 2) && axis_value * value.at(2).toInt() > 0)
                Keys->Value |= key;
            break;
        case 3 /*Joystick Hat*/:
            if (SDL_JoystickGetHat(controller[Control].joystick, value.at(0).toInt()) & value.at(2).toInt())
                Keys->Value |= key;
            break;
        case 4 /*Joystick Button*/:
            if (SDL_JoystickGetButton(controller[Control].joystick, value.at(0).toInt()))
                Keys->Value |= key;
            break;
        case 5 /*Joystick Axis*/:
            axis_value = SDL_JoystickGetAxis(controller[Control].joystick, value.at(0).toInt());
            if (key == 0x0020/*Z*/ || key == 0x1000/*R_TRIG*/ || key == 0x2000/*L_TRIG*/)
            {
                if (axis_value > 0)
                    Keys->Value |= key;
                break;
            }

            if (abs(axis_value) >= (AXIS_PEAK / 2) && axis_value * value.at(2).toInt() > 0)
                Keys->Value |= key;
            break;
    }
}

void setPak(int Control)
{
    QString pak = gameControllerSettings->value("Controller" + QString::number(Control + 1) + "/Pak").toString();
    if (controller[Control].control->Type == CONT_TYPE_VRU)
        controller[Control].control->Plugin = PLUGIN_NONE;
    else if (pak == "Transfer")
        controller[Control].control->Plugin = PLUGIN_TRANSFER_PAK;
    else if (pak == "Rumble")
        controller[Control].control->Plugin = PLUGIN_RAW;
    else if (pak == "None")
        controller[Control].control->Plugin = PLUGIN_NONE;
    else
        controller[Control].control->Plugin = PLUGIN_MEMPAK;
}

EXPORT void CALL GetKeys( int Control, BUTTONS *Keys )
{
    if (controller[Control].control->Present == 0)
        return;

    if (controller[Control].joystick && SDL_JoystickGetAttached(controller[Control].joystick) == SDL_FALSE)
    {
        for (int j = 0; j < SDL_NumJoysticks(); ++j)
        {
            if (controller[Control].device_path == SDL_JoystickPathForIndex(j))
            {
                if (SDL_IsGameController(j))
                {
                    SDL_GameControllerClose(controller[Control].gamepad);
                    controller[Control].gamepad = SDL_GameControllerOpen(j);
                    controller[Control].joystick = SDL_GameControllerGetJoystick(controller[Control].gamepad);
                }
                else
                {
                    SDL_JoystickClose(controller[Control].joystick);
                    controller[Control].joystick = SDL_JoystickOpen(j);
                }
                break;
            }
        }
    }
    setPak(Control);
    if (controller[Control].control->Type == CONT_TYPE_VRU)
    {
        if (l_TalkingState == 0)
        {
            Keys->Value = 0;
            return;
        }
        else
        {
            // In the future it would be better to only set this when the player is talking
            Keys->Value = 0x0020;
            return;
        }
    }

    Keys->Value = 0;
    setKey(Control, 0x0001/*R_DPAD*/, Keys, "DPadR");
    setKey(Control, 0x0002/*L_DPAD*/, Keys, "DPadL");
    setKey(Control, 0x0004/*D_DPAD*/, Keys, "DPadD");
    setKey(Control, 0x0008/*U_DPAD*/, Keys, "DPadU");
    setKey(Control, 0x0010/*START_BUTTON*/, Keys, "Start");
    setKey(Control, 0x0020/*Z_TRIG*/, Keys, "Z");
    setKey(Control, 0x0040/*B_BUTTON*/, Keys, "B");
    setKey(Control, 0x0080/*A_BUTTON*/, Keys, "A");
    setKey(Control, 0x0100/*R_CBUTTON*/, Keys, "CRight");
    setKey(Control, 0x0200/*L_CBUTTON*/, Keys, "CLeft");
    setKey(Control, 0x0400/*D_CBUTTON*/, Keys, "CDown");
    setKey(Control, 0x0800/*U_CBUTTON*/, Keys, "CUp");
    setKey(Control, 0x1000/*R_TRIG*/, Keys, "R");
    setKey(Control, 0x2000/*L_TRIG*/, Keys, "L");

    setAxis(Control, 0/*X_AXIS*/, Keys, "AxisLeft", -1);
    setAxis(Control, 0/*X_AXIS*/, Keys, "AxisRight", 1);
    setAxis(Control, 1/*Y_AXIS*/, Keys, "AxisUp", 1);
    setAxis(Control, 1/*Y_AXIS*/, Keys, "AxisDown", -1);

    double boundX = Keys->X_AXIS;
    double boundY = Keys->Y_AXIS;
    boundAxis(&boundX, &boundY);
    Keys->X_AXIS = round(boundX);
    Keys->Y_AXIS = round(boundY);
}

static int setupVosk()
{
    if (voskLib)
        return 0;
    voskLib = new QLibrary((QDir(QCoreApplication::applicationDirPath()).filePath("vosk")));
    VoskAcceptWaveform = (ptr_vosk_recognizer_accept_waveform) voskLib->resolve("vosk_recognizer_accept_waveform");
    VoskFinalResult = (ptr_vosk_recognizer_final_result) voskLib->resolve("vosk_recognizer_final_result");
    VoskNewModel = (ptr_vosk_model_new) voskLib->resolve("vosk_model_new");
    VoskNewRecognizer = (ptr_vosk_recognizer_new_grm) voskLib->resolve("vosk_recognizer_new_grm");
    VoskFreeModel = (ptr_vosk_model_free) voskLib->resolve("vosk_model_free");
    VoskFreeRecognizer = (ptr_vosk_recognizer_free) voskLib->resolve("vosk_recognizer_free");
    VoskSetLogLevel = (ptr_vosk_set_log_level) voskLib->resolve("vosk_set_log_level");
    VoskSetAlternatives = (ptr_vosk_recognizer_set_max_alternatives) voskLib->resolve("vosk_recognizer_set_max_alternatives");

    VoskSetLogLevel(-1);

    QJsonDocument vruwordjson = QJsonDocument::fromJson(vruwords.toUtf8());
    vruwordsobject = vruwordjson.object();

    l_TalkingState = 0;
    SDL_AudioSpec *desired, *obtained;
    if(hardware_spec != NULL) free(hardware_spec);
    desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    obtained = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    desired->freq = 44100;
    desired->format = AUDIO_S16SYS;
    desired->channels = 1;
    desired->samples = 1024;
    desired->callback = NULL;
    desired->userdata = NULL;
    audio_dev = SDL_OpenAudioDevice(NULL, 1, desired, obtained, SDL_AUDIO_ALLOW_SAMPLES_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    free(desired);
    hardware_spec = obtained;
    recognizer = nullptr;
    model = nullptr;
    timer_id = 0;
    if (QFile::exists(QDir(ConfigGetUserDataPath()).filePath("vosk-model-small-en-us-0.15/conf/mfcc.conf")))
    {
        model = VoskNewModel(QDir(ConfigGetUserDataPath()).filePath("vosk-model-small-en-us-0.15").toUtf8().constData());
        return 1;
    }
    delete voskLib;
    voskLib = NULL;
    return 0;
}

EXPORT void CALL InitiateControllers(CONTROL_INFO ControlInfo)
{
    model = nullptr;
    recognizer = nullptr;
    gameSettings = new QSettings(settings->fileName(), QSettings::IniFormat);
    gameControllerSettings = new QSettings(controllerSettings->fileName(), QSettings::IniFormat);

    int i, j;

    for (i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        myKeyState[i] = 0;
    }
    // set our CONTROL struct pointers to the array that was passed in to this function from the core
    // this small struct tells the core whether each controller is plugged in, and what type of pak is connected
    QString gamepad;
    int controller_index;
    QString gamepad_name;
    QList<int> used_controllers;
    for (i = 0; i < 4; i++) {
        controller[i].control = ControlInfo.Controls + i;
        controller[i].control->RawData = 0;
        controller[i].control->Present = 0;
        controller[i].control->Type = CONT_TYPE_STANDARD;
        controller[i].gamepad = NULL;
        controller[i].joystick = NULL;
        gamepad = gameControllerSettings->value("Controller" + QString::number(i + 1) + "/Gamepad").toString();
        if (gamepad == "Keyboard")
            controller[i].control->Present = 1;
        else if (gamepad == "None")
            controller[i].control->Present = 0;
        else if (gamepad == "Auto") {
            for (j = 0; j < SDL_NumJoysticks(); ++j)
            {
                if (used_controllers.contains(j))
                    continue;

                if (SDL_IsGameController(j)) {
                    controller[i].gamepad = SDL_GameControllerOpen(j);
                    if (controller[i].gamepad)
                    {
                        controller[i].control->Present = 1;
                        used_controllers.append(j);
                        break;
                    }
                }
                else {
                    controller[i].joystick = SDL_JoystickOpen(j);
                    if (controller[i].joystick)
                    {
                        controller[i].control->Present = 1;
                        used_controllers.append(j);
                        break;
                    }
                }
            }
            if (i == 0) controller[i].control->Present = 1; //Player 1
        }
        else if (gamepad == "Emulate VRU") {
            controller[i].control->Type = CONT_TYPE_VRU;
            controller[i].control->Plugin = PLUGIN_NONE;
            if (!setupVosk())
            {
                controller[i].control->Present = 0;
                gameControllerSettings->setValue("Controller" + QString::number(i + 1) + "/Pak", "None");
                DebugMessage(M64MSG_ERROR, "Could not set up Vosk library, disabling VRU");
            }
            else
                controller[i].control->Present = 1;
        }
        else /*specific gamepad selected*/ {
            controller_index = gamepad.split(":")[0].toInt();
            gamepad_name = gamepad.split(":")[1];
            if (SDL_IsGameController(controller_index) && gamepad_name == QString(SDL_JoystickNameForIndex(controller_index))) {
                controller[i].gamepad = SDL_GameControllerOpen(controller_index);
                if (controller[i].gamepad)
                {
                    controller[i].control->Present = 1;
                    used_controllers.append(controller_index);
                }
            }
            else if (gamepad_name == QString(SDL_JoystickNameForIndex(controller_index))) {
                controller[i].joystick = SDL_JoystickOpen(controller_index);
                if (controller[i].joystick)
                {
                    controller[i].control->Present = 1;
                    used_controllers.append(controller_index);
                }
            }
            if (controller[i].control->Present == 0) {
                gameControllerSettings->setValue("Controller" + QString::number(i + 1) + "/Gamepad", "Auto");
                --i; //Try again using Auto
                continue;
            }
        }

        if (controller[i].gamepad)
            controller[i].joystick = SDL_GameControllerGetJoystick(controller[i].gamepad);
        if (controller[i].joystick)
            controller[i].device_path = SDL_JoystickPath(controller[i].joystick);

        controller[i].profile = gameControllerSettings->value("Controller" + QString::number(i + 1) + "/Profile").toString();
        if (!gameSettings->childGroups().contains(controller[i].profile))
            controller[i].profile = "Auto";

        if (controller[i].profile == "Auto") {
            if (controller[i].gamepad)
                controller[i].profile = "Auto-Gamepad";
            else
                controller[i].profile = "Auto-Keyboard";
        }

        if (!gameSettings->contains(controller[i].profile + "/Deadzone"))
            gameSettings->setValue(controller[i].profile + "/Deadzone", DEADZONE_DEFAULT);
        if (!gameSettings->contains(controller[i].profile + "/Sensitivity"))
            gameSettings->setValue(controller[i].profile + "/Sensitivity", 100.0);

        controller[i].deadzone = AXIS_PEAK * (gameSettings->value(controller[i].profile + "/Deadzone").toFloat() / 100.0);
        controller[i].range = AXIS_PEAK - controller[i].deadzone;
        controller[i].sensitivity = gameSettings->value(controller[i].profile + "/Sensitivity").toFloat() / 100.0;

        setPak(i);
    }
}

EXPORT void CALL ReadController(int, unsigned char *)
{
}

EXPORT int CALL RomOpen(void)
{
    return 1;
}

EXPORT void CALL RomClosed(void)
{
    closeControllers();

    gameSettings->sync();
    gameControllerSettings->sync();
    if (model)
        VoskFreeModel(model);
    if (recognizer)
        VoskFreeRecognizer(recognizer);
    if (voskLib)
        delete voskLib;
    voskLib = NULL;
    SDL_CloseAudioDevice(audio_dev);
    if(hardware_spec != NULL) free(hardware_spec);
    hardware_spec = NULL;
    delete gameSettings;
    delete gameControllerSettings;
}

EXPORT void CALL SDL_KeyDown(int, int keysym)
{
    myKeyState[keysym] = 1;
}

EXPORT void CALL SDL_KeyUp(int, int keysym)
{
    myKeyState[keysym] = 0;
}

EXPORT void CALL PluginConfig()
{
    ConfigDialog config(coreHandle, settings, controllerSettings);
    config.exec();
}
