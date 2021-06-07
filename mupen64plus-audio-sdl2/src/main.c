#include <SDL.h>
#include <SDL_audio.h>
#include <samplerate.h>
#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_common.h"
#include "m64p_types.h"
#include "m64p_plugin.h"

#define SDL2_AUDIO_PLUGIN_VERSION 0x020500
#define AUDIO_PLUGIN_API_VERSION 0x020000

static SDL_AudioDeviceID dev;
static SDL_AudioSpec *hardware_spec;

static int l_PluginInit = 0;
static int GameFreq = 0;
static AUDIO_INFO AudioInfo;
static unsigned char primaryBuffer[0x40000];
static float output_buffer[0x20000];
static float mix_buffer[0x20000];
static float convert_buffer[0x20000];
static int VolIsMuted = 0;
static unsigned int paused = 0;
static int ff = 0;
static SRC_STATE *src_state;
static int VolSDL = SDL_MIX_MAXVOLUME;

EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context, void (*DebugCallback)(void *, int, const char *))
{
    if (l_PluginInit)
        return M64ERR_ALREADY_INIT;

    SDL_Init(SDL_INIT_AUDIO);
    l_PluginInit = 1;
    VolIsMuted = 0;
    ff = 0;

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
    if (!l_PluginInit)
        return M64ERR_NOT_INIT;

    if(hardware_spec != NULL) free(hardware_spec);
    hardware_spec = NULL;

    if (src_state) src_state = src_delete(src_state);
    src_state = NULL;

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    l_PluginInit = 0;

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    /* set version info */
    if (PluginType != NULL)
        *PluginType = M64PLUGIN_AUDIO;

    if (PluginVersion != NULL)
        *PluginVersion = SDL2_AUDIO_PLUGIN_VERSION;

    if (APIVersion != NULL)
        *APIVersion = AUDIO_PLUGIN_API_VERSION;

    if (PluginNamePtr != NULL)
        *PluginNamePtr = "Mupen64Plus SDL2 Audio Plugin";

    if (Capabilities != NULL)
    {
        *Capabilities = 0;
    }

    return M64ERR_SUCCESS;
}

void InitAudio()
{
    SDL_AudioSpec *desired, *obtained;
    if(hardware_spec != NULL) free(hardware_spec);
    desired = malloc(sizeof(SDL_AudioSpec));
    obtained = malloc(sizeof(SDL_AudioSpec));
    desired->freq = 48000;
    desired->format=AUDIO_F32;
    desired->channels=2;
    desired->samples = 256;
    desired->callback = NULL;
    desired->userdata = NULL;

    const char *dev_name = SDL_GetAudioDeviceName(-1, 0);
    dev = SDL_OpenAudioDevice(dev_name, 0, desired, obtained, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    free(desired);
    hardware_spec=obtained;
    SDL_PauseAudioDevice(dev, 0);
    paused = 0;

    if (src_state) src_state = src_delete(src_state);
    int error;
    src_state = src_new (SRC_SINC_MEDIUM_QUALITY, 2, &error);
}

void CloseAudio()
{
    SDL_ClearQueuedAudio(dev);
    SDL_CloseAudioDevice(dev);

    if(hardware_spec != NULL) free(hardware_spec);
    hardware_spec = NULL;

    if (src_state) src_state = src_delete(src_state);
    src_state = NULL;
}

EXPORT void CALL AiDacrateChanged( int SystemType )
{
    if (!l_PluginInit)
        return;

    switch (SystemType)
    {
        case SYSTEM_NTSC:
            GameFreq = 48681812 / (*AudioInfo.AI_DACRATE_REG + 1);
            break;
        case SYSTEM_PAL:
            GameFreq = 49656530 / (*AudioInfo.AI_DACRATE_REG + 1);
            break;
        case SYSTEM_MPAL:
            GameFreq = 48628316 / (*AudioInfo.AI_DACRATE_REG + 1);
            break;
    }
    if (src_state) src_state = src_delete(src_state);
    int error;
    src_state = src_new (SRC_SINC_MEDIUM_QUALITY, 2, &error);
}

EXPORT void CALL AiLenChanged( void )
{
    if (!l_PluginInit)
        return;

    unsigned int LenReg = *AudioInfo.AI_LEN_REG;
    unsigned char *p = AudioInfo.RDRAM + (*AudioInfo.AI_DRAM_ADDR_REG & 0xFFFFFF);

    unsigned int i;

    for ( i = 0 ; i < LenReg ; i += 4 )
    {
        // Left channel
        primaryBuffer[ i ] = p[ i + 2 ];
        primaryBuffer[ i + 1 ] = p[ i + 3 ];

        // Right channel
        primaryBuffer[ i + 2 ] = p[ i ];
        primaryBuffer[ i + 3 ] = p[ i + 1 ];
    }

    if (!VolIsMuted && !ff)
    {
        src_short_to_float_array ((short*)primaryBuffer, convert_buffer, LenReg / 2) ;
        SRC_DATA data;
        data.data_in = convert_buffer;
        data.input_frames = LenReg / 4;
        data.data_out = output_buffer;
        data.output_frames = sizeof(output_buffer) / 8;
        data.src_ratio = (float)hardware_spec->freq / GameFreq;
        data.end_of_input = 0;

        src_process(src_state, &data);
        
        unsigned int audio_queued = SDL_GetQueuedAudioSize(dev);
        unsigned int acceptable_latency = (hardware_spec->freq * 0.2) * 8;
        unsigned int min_latency = (hardware_spec->freq * 0.05) * 8;

        if (!paused && audio_queued < min_latency)
        {
            SDL_PauseAudioDevice(dev, 1);
            paused = 1;
        }
        else if (paused && audio_queued >= min_latency)
        {
            SDL_PauseAudioDevice(dev, 0);
            paused = 0;
        }

        unsigned int output_length = data.output_frames_gen * 8;
        if (audio_queued < acceptable_latency)
        {
            SDL_memset(mix_buffer, 0, output_length);
            SDL_MixAudioFormat((Uint8*)mix_buffer, (Uint8*)output_buffer, AUDIO_F32, output_length, VolSDL);
            SDL_QueueAudio(dev, mix_buffer, output_length);
        }
    }
}

EXPORT int CALL InitiateAudio( AUDIO_INFO Audio_Info )
{
    if (!l_PluginInit)
        return 0;

    GameFreq = 33600;
    AudioInfo = Audio_Info;

    return 1;
}

EXPORT int CALL RomOpen(void)
{
    if (!l_PluginInit)
        return 0;

    InitAudio();

    return 1;
}

EXPORT void CALL RomClosed( void )
{
    if (!l_PluginInit)
        return;

    CloseAudio();
}

EXPORT void CALL ProcessAList(void)
{
}

EXPORT void CALL SetSpeedFactor(int percentage)
{
    if (percentage > 100)
        ff = 1;
    else
        ff = 0;
}

EXPORT void CALL VolumeMute(void)
{
    if (!l_PluginInit)
        return;

    VolIsMuted = !VolIsMuted;
}

EXPORT void CALL VolumeUp(void)
{
}

EXPORT void CALL VolumeDown(void)
{
}

EXPORT int CALL VolumeGetLevel(void)
{
    return VolIsMuted ? 0 : 100;
}

EXPORT void CALL VolumeSetLevel(int level)
{
    if (level < 0)
        level = 0;
    else if (level > 100)
        level = 100;

    VolSDL = SDL_MIX_MAXVOLUME * level / 100;
}

EXPORT const char * CALL VolumeGetString(void)
{
    static char VolumeString[32];

    if (VolIsMuted)
    {
        strcpy(VolumeString, "Mute");
    }
    else
    {
        sprintf(VolumeString, "%i%%", 100);
    }

    return VolumeString;
}
