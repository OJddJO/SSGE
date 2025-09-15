#include "SSGE_local.h"

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

void destroyTexture(SSGE_Texture *ptr) {
    SDL_DestroyTexture(ptr->texture);
    SSGE_Array_Destroy(&ptr->queue, free);
    if (ptr->name) free(ptr->name);
    free(ptr);
}

void destroyObject(SSGE_Object *ptr) {
    if (ptr->name) free(ptr->name);
    if (ptr->destroyData != NULL)
        ptr->destroyData(ptr->data);
    if (ptr->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&ptr->texture.texture->queue, ptr->texture.renderDataIdx));
    free(ptr);
}

void destroyTemplate(SSGE_ObjectTemplate *ptr) {
    if (ptr->name) free(ptr->name);
    free(ptr);
}

void destroyFont(SSGE_Font *ptr) {
    TTF_CloseFont(ptr->font);
    if (ptr->name) free(ptr->name);
    free(ptr);
}

void destroyAudio(SSGE_Audio *ptr) {
    Mix_FreeChunk(ptr->audio);
    if (ptr->name) free(ptr->name);
    free(ptr);
}

void destroyAnimation(SSGE_Animation *ptr) {
    if (ptr->type == SSGE_ANIM_FRAMES) {
        for (uint32_t i = 0; i < ptr->data.frameCount; i++) {
            if (ptr->data.frames[i] != NULL) {
                SDL_DestroyTexture(ptr->data.frames[i]);
            }
        }
        free(ptr->data.frames);
    }
    if (ptr->name) free(ptr->name);
    free(ptr);
}
