#include "core_commands.h"

ptr_CoreStartup CoreStartup = nullptr;
ptr_CoreShutdown CoreShutdown = nullptr;
ptr_CoreDoCommand CoreDoCommand = nullptr;
ptr_CoreAttachPlugin CoreAttachPlugin = nullptr;
ptr_CoreDetachPlugin CoreDetachPlugin = nullptr;
ptr_CoreOverrideVidExt CoreOverrideVidExt = nullptr;

ptr_ConfigGetUserConfigPath ConfigGetUserConfigPath = nullptr;
ptr_ConfigSaveFile ConfigSaveFile = nullptr;
ptr_ConfigGetParameterHelp ConfigGetParameterHelp = nullptr;
ptr_ConfigGetParamInt ConfigGetParamInt = nullptr;
ptr_ConfigGetParamFloat ConfigGetParamFloat = nullptr;
ptr_ConfigGetParamBool ConfigGetParamBool = nullptr;
ptr_ConfigGetParamString ConfigGetParamString = nullptr;
ptr_ConfigSetParameter ConfigSetParameter = nullptr;
ptr_ConfigDeleteSection ConfigDeleteSection = nullptr;
ptr_ConfigOpenSection ConfigOpenSection = nullptr;
ptr_ConfigSaveSection ConfigSaveSection = nullptr;
ptr_ConfigListParameters ConfigListParameters = nullptr;
ptr_ConfigGetSharedDataFilepath ConfigGetSharedDataFilepath = nullptr;

ptr_CoreAddCheat CoreAddCheat = nullptr;
