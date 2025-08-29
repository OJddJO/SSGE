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
    SDL_DestroyTexture(((SSGE_Texture *)ptr)->texture);
    SSGE_Array_Destroy(&((SSGE_Texture *)ptr)->queue, free);
    free(((SSGE_Texture *)ptr)->name);
    free(ptr);
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
