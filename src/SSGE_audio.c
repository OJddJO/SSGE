#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_audio.h"

SSGEDECL SSGE_Audio *SSGE_Audio_Create(uint32_t *id, char *filename, char *name) {
    _assert_engine_init
    SSGE_Audio *audio = (SSGE_Audio *)malloc(sizeof(SSGE_Audio));
    if (audio == NULL) 
        SSGE_Error("Failed to allocate memory for audio")

    audio->audio = Mix_LoadWAV(filename);
    if (audio->audio == NULL) 
        SSGE_ErrorEx("Failed to load audio: %s", Mix_GetError());

    _add_to_list(&_audio_list, audio, name, id, __func__);
    return audio;
}

SSGEDECL SSGE_Audio *SSGE_Audio_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Audio *ptr = SSGE_Array_Get(&_audio_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Audio not found: %u", id)
    return ptr;
}

static bool _find_audio_name(void *ptr, void *name) {
    return strcmp(((SSGE_Audio *)ptr)->name, (char *)name) == 0;
}

SSGEDECL SSGE_Audio *SSGE_Audio_GetName(char *name) {
    _assert_engine_init
    SSGE_Audio *ptr = (SSGE_Audio *)SSGE_Array_Find(&_audio_list, _find_audio_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name)
    return ptr;
}

SSGEDECL int SSGE_Audio_Play(SSGE_Audio *audio, int channel) {
    _assert_engine_init

    if ((channel = Mix_PlayChannel(channel, audio->audio, 0)) == -1) 
        SSGE_ErrorEx("Audio could not be played: %s", Mix_GetError())
    
    return channel;
}

SSGEDECL void SSGE_Audio_Resume(int channel) {
    _assert_engine_init
    Mix_Resume(channel);
}

SSGEDECL void SSGE_Audio_Pause(int channel) {
    _assert_engine_init
    Mix_Pause(channel);
}

SSGEDECL void SSGE_Audio_Stop(int channel) {
    _assert_engine_init
    Mix_HaltChannel(channel);
}

SSGEDECL void SSGE_Audio_Close(uint32_t id) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_Pop(&_audio_list, id);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %u", id)
    _destroy_audio(audio);
}

SSGEDECL void SSGE_Audio_CloseName(char *name) {
    _assert_engine_init
    SSGE_Audio *audio = SSGE_Array_FindPop(&_audio_list, _find_audio_name, name);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name)
    _destroy_audio(audio);
}

SSGEDECL void SSGE_Audio_CloseAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_audio_list, _destroy_audio);
    SSGE_Array_Create(&_audio_list);
}
