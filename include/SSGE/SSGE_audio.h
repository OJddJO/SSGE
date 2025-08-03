#ifndef __SSGE_AUDIO_H__
#define __SSGE_AUDIO_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Loads an audio
 * \param id Where to store the id of the audio
 * \param filename The path to the audio
 * \param name The name of the audio
 * \return The audio
 */
SSGEDECL SSGE_Audio *SSGE_Audio_Create(uint32_t *id, char *filename, char *name);

/**
 * Get an audio
 * \param id The id of the audio
 * \return The audio
 */
SSGEDECL SSGE_Audio *SSGE_Audio_Get(uint32_t id);

/**
 * Get an audio by name
 * \param name The name of the audio
 * \return The audio
 */
SSGEDECL SSGE_Audio *SSGE_Audio_GetName(char *name);

/**
 * Plays an audio by id
 * \param audio The audio to play
 * \param channel The channel to play the audio on, -1 for first free channel.
 * \return The channel used to play the audio
 */
SSGEDECL int SSGE_Audio_Play(SSGE_Audio *audio, int channel);

/**
 * Resume an audio
 * \param channel The channel to resume the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Resume(int channel);

/**
 * Pauses an audio
 * \param channel The channel to pause the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Pause(int channel);

/**
 * Stops an audio
 * \param channel The channel to stop the audio on, -1 for all
 */
SSGEDECL void SSGE_Audio_Stop(int channel);

/**
 * Closes an audio by id
 * \param id The id of the audio
 */
SSGEDECL void SSGE_Audio_Close(uint32_t id);

/**
 * Closes an audio by name
 * \param name The name of the audio
 */
SSGEDECL void SSGE_Audio_CloseName(char *name);

/**
 * Closes all audios
 */
SSGEDECL void SSGE_Audio_CloseAll();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_AUDIO_H__