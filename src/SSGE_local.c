#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "SSGE/SSGE_local.h"

SSGE_Engine _engine             = {0};
SSGE_Array  _textureList        = {0};
SSGE_Array  _objectList         = {0};
SSGE_Array  _objectTemplateList = {0};
SSGE_Array  _fontList           = {0};
SSGE_Array  _audioList          = {0};
SSGE_Array  _animationList      = {0};
SSGE_Array  _playingAnim        = {0};
SSGE_Event  _event              = {0};
SSGE_Color  _color              = {0, 0, 0, 255};
SSGE_Color  _bgColor            = {0, 0, 0, 255};
bool        _manualUpdateFrame  = false;
bool        _updateFrame        = true; // set to true to draw the first frame

void destroyTexture(void *ptr) {
    atomic_store(&((SSGE_Texture *)ptr)->markedForDestroy, true);
    SSGE_Array_Destroy(&((SSGE_Texture *)ptr)->queue, free);
    SSGE_Array_Create(&((SSGE_Texture *)ptr)->queue);

    // Release ref from _textureList
    textureRelease(ptr);
}

void destroyObject(void *ptr) {
    free(((SSGE_Object *)ptr)->name);
    if (((SSGE_Object *)ptr)->destroyData != NULL)
        ((SSGE_Object *)ptr)->destroyData(((SSGE_Object *)ptr)->data);
    free(ptr);
}

void destroyTemplate(void *ptr) {
    free(((SSGE_ObjectTemplate *)ptr)->name);
    free(ptr);
}

void destroyFont(void *ptr) {
    TTF_CloseFont(((SSGE_Font *)ptr)->font);
    free(((SSGE_Font *)ptr)->name);
    free(ptr);
}

void destroyAudio(void *ptr) {
    Mix_FreeChunk(((SSGE_Audio *)ptr)->audio);
    free(((SSGE_Audio *)ptr)->name);
    free(ptr);
}

void destroyAnimation(void *ptr) {
    if (((SSGE_Animation *)ptr)->type == SSGE_ANIM_FRAMES) {
        for (uint32_t i = 0; i < ((SSGE_Animation *)ptr)->data.frameCount; i++) {
            if (((SSGE_Animation *)ptr)->data.frames[i] != NULL) {
                SDL_DestroyTexture(((SSGE_Animation *)ptr)->data.frames[i]);
            }
        }
        free(((SSGE_Animation *)ptr)->data.frames);
    }
    free(((SSGE_Animation *)ptr)->name);
    free(ptr);
}

void textureAcquire(SSGE_Texture *texture) {
    if (texture)
        atomic_fetch_add(&texture->refCount, 1);
}

void textureRelease(SSGE_Texture *texture) {
    if (!texture) return;
    int oldCount = atomic_fetch_sub(&texture->refCount, 1);
    if (oldCount == 1 && texture->markedForDestroy) { // No more reference to the texture
        SDL_DestroyTexture(texture->texture);
        SSGE_Array_Destroy(&texture->queue, free);
        free(texture->name);
        free(texture);
    }
}