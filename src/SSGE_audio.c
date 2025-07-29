#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_audio.h"

/***********************************************
 * Audio functions
 ***********************************************/

/**
 * Loads an audio
 * \param filename The path to the audio
 * \param name The name of the audio
 * \return The audio
 */
SSGEDECL uint32_t SSGE_Audio_Create(char *filename, char *name) {
    _assert_engine_init
    SSGE_Audio *audio = (SSGE_Audio *)malloc(sizeof(SSGE_Audio));
    if (audio == NULL) 
        SSGE_Error("Failed to allocate memory for audio")

    audio->audio = Mix_LoadWAV(filename);
    if (audio->audio == NULL) 
        SSGE_ErrorEx("Failed to load audio: %s", Mix_GetError());

    return _add_to_list(&_audio_list, audio, name, __func__);
}

/**
 * Plays an audio by id
 * \param id The id of the audio
 * \param channel The channel to play the audio on, -1 for first free channel.
 * \return The channel used to play the audio
 */
SSGEDECL int SSGE_Audio_Play(uint32_t id, int channel) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_Get(&_audio_list, id);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %d", id);

    if ((channel = Mix_PlayChannel(channel, audio->audio, 0)) == -1) 
        SSGE_ErrorEx("Audio could not be played: %s", Mix_GetError());
    
    return channel;
}

static bool _find_audio_name(void *audio, void *name) {
    return strcmp(((SSGE_Audio *)audio)->name, (char *)name) == 0 ? 1 : 0;
}

/**
 * Plays an audio by name
 * \param name The name of the audio
 * \param channel The channel to play the audio on, -1 for first free channel.
 * \return The channel used to play the audio
 */
SSGEDECL int SSGE_Audio_PlayName(char *name, int channel) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_Find(&_audio_list, _find_audio_name, name);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name);

    if ((channel = Mix_PlayChannel(channel, audio->audio, 0)) == -1) 
        SSGE_ErrorEx("Audio could not be played: %s", Mix_GetError());

    return channel;
}

/**
 * Resume an audio
 * \param channel The channel to resume the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Resume(int channel) {
    _assert_engine_init
    Mix_Resume(channel);
}

/**
 * Pauses an audio
 * \param channel The channel to pause the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Pause(int channel) {
    _assert_engine_init
    Mix_Pause(channel);
}

/**
 * Stops an audio
 * \param channel The channel to stop the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Stop(int channel) {
    _assert_engine_init
    Mix_HaltChannel(channel);
}

/**
 * Closes an audio by id
 * \param id The id of the audio
 */
SSGEDECL void SSGE_Audio_Close(uint32_t id) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_Pop(&_audio_list, id);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %u", id);
    Mix_FreeChunk(audio->audio);
    free(audio->name);
    free(audio);
}

/**
 * Closes an audio by name
 * \param name The name of the audio
 */
SSGEDECL void SSGE_Audio_CloseName(char *name) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_FindPop(&_audio_list, _find_audio_name, name);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name);
    Mix_FreeChunk(audio->audio);
    free(audio->name);
    free(audio);
}

/**
 * Closes all audios
 */
SSGEDECL void SSGE_Audio_CloseAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_audio_list, _destroy_audio);
    SSGE_Array_Create(&_audio_list);
}
