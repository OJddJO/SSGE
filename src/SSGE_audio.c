#include <stdio.h>

#define SSGE_WANT_SDL2

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
SSGEDECL uint32_t SSGE_LoadAudio(char *filename, char *name) {
    _assert_engine_init();
    SSGE_Audio *audio = (SSGE_Audio *)malloc(sizeof(SSGE_Audio));
    if (audio == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for audio\n");
        exit(1);
    }

    audio->audio = Mix_LoadWAV(filename);
    if (audio->audio == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Failed to load audio: %s\n", Mix_GetError());
        exit(1);
    }

    return _add_audio_to_list(audio, name);
}

/**
 * Plays an audio by id
 * \param id The id of the audio
 * \param channel The channel to play the audio on, -1 for first free channel. Channels must be a number between 0 and 3
 */
SSGEDECL void SSGE_PlayAudio(uint32_t id, int channel) {
    _assert_engine_init();
    SSGE_Audio *audio = SSGE_Array_Get(_audio_list, id);
    if (audio == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Audio not found: %u\n", id);
        exit(1);
    }

    if (Mix_PlayChannel(channel, audio->audio, 0) == -1) {
        fprintf(stderr, "[SSGE][ENGINE] Audio could not be played: %s", Mix_GetError());
    }
}

static bool _find_audio_name(void *audio, void *name) {
    return strcmp(((SSGE_Audio *)audio)->name, (char *)name) == 0 ? 1 : 0;
}

/**
 * Plays an audio by name
 * \param name The name of the audio
 * \param channel The channel to play the audio on, -1 for first free channel. Channels must be a number between 0 and 3
 */
SSGEDECL void SSGE_PlayAudioByName(char *name, int channel) {
    _assert_engine_init();
    SSGE_Audio *audio = SSGE_Array_Find(_audio_list, _find_audio_name, name);
    if (audio == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Audio not found: %s\n", name);
        exit(1);
    }

    if (Mix_PlayChannel(channel, audio->audio, 0) == -1) {
        fprintf(stderr, "[SSGE][ENGINE] Audio could not be played: %s", Mix_GetError());
    }
}

/**
 * Resume an audio
 * \param channel The channel to resume the audio on, -1 for all
 */
SSGEDECL void SSGE_ResumeAudio(int channel) {
    _assert_engine_init();
    Mix_Resume(channel);
}

/**
 * Pauses an audio
 * \param channel The channel to pause the audio on, -1 for all
 */
SSGEDECL void SSGE_PauseAudio(int channel) {
    _assert_engine_init();
    Mix_Pause(channel);
}

/**
 * Stops an audio
 * \param channel The channel to stop the audio on, -1 for all
 */
SSGEDECL void SSGE_StopAudio(int channel) {
    _assert_engine_init();
    Mix_HaltChannel(channel);
}

/**
 * Closes an audio by id
 * \param id The id of the audio
 */
SSGEDECL void SSGE_CloseAudio(uint32_t id) {
    _assert_engine_init();
    SSGE_Audio *audio = SSGE_Array_Pop(_audio_list, id);
    if (audio == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Audio not found: %d", id);
        exit(1);
    }
    Mix_FreeChunk(audio->audio);
    free(audio->name);
    free(audio);
}

/**
 * Closes an audio by name
 * \param name The name of the audio
 */
SSGEDECL void SSGE_CloseAudioByName(char *name) {
    _assert_engine_init();
    SSGE_Audio *audio = SSGE_Array_FindPop(_audio_list, _find_audio_name, name);
    if (audio == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Audio not found: %s", name);
        exit(1);
    }
    Mix_FreeChunk(audio->audio);
    free(audio->name);
    free(audio);
}

/**
 * Closes all audios
 */
SSGEDECL void SSGE_CloseAllAudios() {
    _assert_engine_init();
    SSGE_Array_Destroy(_audio_list, _destroy_audio);
    _audio_list = SSGE_Array_Create();
}
