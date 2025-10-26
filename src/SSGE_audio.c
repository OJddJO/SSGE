#include "SSGE_local.h"
#include "SSGE/SSGE_audio.h"

SSGEAPI SSGE_Audio *SSGE_Audio_Create(uint32_t *id, const char *name, const char *filename) {
    SSGE_Audio *audio = (SSGE_Audio *)malloc(sizeof(SSGE_Audio));
    if (audio == NULL) 
        SSGE_Error("Failed to allocate memory for audio")

    audio->audio = Mix_LoadWAV(filename);
    if (audio->audio == NULL) 
        SSGE_ErrorEx("Failed to load audio: %s", Mix_GetError());

    _addToList(&_audioList, audio, name, id, __func__);
    return audio;
}

SSGEAPI SSGE_Audio *SSGE_Audio_Get(uint32_t id) {
    SSGE_Audio *ptr = SSGE_Array_Get(&_audioList, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Audio not found: %u", id)
    return ptr;
}

inline static bool _find_audio_name(void *ptr, void *name) {
    return strcmp(((SSGE_Audio *)ptr)->name, (char *)name) == 0;
}

SSGEAPI SSGE_Audio *SSGE_Audio_GetName(const char *name) {
    SSGE_Audio *ptr = (SSGE_Audio *)SSGE_Array_Find(&_audioList, _find_audio_name, (void *)name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name)
    return ptr;
}

SSGEAPI int SSGE_Audio_Play(SSGE_Audio *audio, int channel) {

    if ((channel = Mix_PlayChannel(channel, audio->audio, 0)) == -1) 
        SSGE_ErrorEx("Audio could not be played: %s", Mix_GetError())
    
    return channel;
}

SSGEAPI void SSGE_Audio_Resume(int channel) {
    Mix_Resume(channel);
}

SSGEAPI void SSGE_Audio_Pause(int channel) {
    Mix_Pause(channel);
}

SSGEAPI void SSGE_Audio_Stop(int channel) {
    Mix_HaltChannel(channel);
}

SSGEAPI void SSGE_Audio_Close(uint32_t id) {
    SSGE_Audio *audio = SSGE_Array_Pop(&_audioList, id);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %u", id)
    destroyAudio(audio);
}

SSGEAPI void SSGE_Audio_CloseName(const char *name) {
    SSGE_Audio *audio = SSGE_Array_FindPop(&_audioList, _find_audio_name, (void *)name);
    if (audio == NULL) 
        SSGE_ErrorEx("Audio not found: %s", name)
    destroyAudio(audio);
}

SSGEAPI void SSGE_Audio_CloseAll() {
    SSGE_Array_Destroy(&_audioList, (SSGE_DestroyData)destroyAudio);
    SSGE_Array_Create(&_audioList);
}
