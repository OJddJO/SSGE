#include <stdio.h>

#include "SSGE_local.h"
#include "SSGE/SSGE_animation.h"

SSGEAPI SSGE_Animation *SSGE_Animation_CreateFrames(uint32_t *id, const char *name, uint32_t frameCount, uint16_t width, uint16_t height) {
    SSGE_Animation *anim = (SSGE_Animation *)malloc(sizeof(SSGE_Animation));
    if (anim == NULL)
        SSGE_Error("Failed to allocate memory for animation")

    anim->type = SSGE_ANIM_FRAMES;
    anim->data.frames = (SDL_Texture **)calloc(frameCount, sizeof(SDL_Texture *));
    anim->data.frametimes = (uint8_t *)calloc(frameCount, sizeof(uint8_t));
    anim->data.frameCount = frameCount;
    anim->data.currentCount = 0;
    anim->data.width = width;
    anim->data.height = height;
    anim->data.anchorX = 0;
    anim->data.anchorY = 0;

    _addToList(&_animationList, anim, name, id, __func__);
    return anim;
}

SSGEAPI SSGE_Animation *SSGE_Animation_CreateFunc(uint32_t *id, const char *name, void (*draw)(SSGE_AnimationState *)) {
    SSGE_Animation *anim = (SSGE_Animation *)malloc(sizeof(SSGE_Animation));
    if (anim == NULL) 
        SSGE_Error("Failed to allocate memory for animation")

    anim->type = SSGE_ANIM_FUNCTION;
    anim->draw = draw;

    _addToList(&_animationList, anim, name, id, __func__);
    return anim;
}

SSGEAPI void SSGE_Animation_Anchor(SSGE_Animation *animation, int x, int y) {
    if (animation->type != SSGE_ANIM_FRAMES)
        SSGE_Error("Wrong animation type")

    animation->data.anchorX = x;
    animation->data.anchorY = y;
}

SSGEAPI void SSGE_Animation_AddFrame(SSGE_Animation *animation, uint8_t frametime, const char *filename) {
    if (animation->type != SSGE_ANIM_FRAMES)
        SSGE_Error("Wrong animation type")

    SDL_Texture *frame = IMG_LoadTexture(_engine.renderer, filename);
    if (frame == NULL)
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError())

    if (animation->data.currentCount >= animation->data.frameCount)
        SSGE_Error("Animation already have max number of frames")

    animation->data.frames[animation->data.currentCount] = frame;
    animation->data.frametimes[animation->data.currentCount++] = frametime;
}

SSGEAPI void SSGE_Animation_AddFrameTilemap(SSGE_Animation *animation, uint8_t frametime, SSGE_Tilemap *tilemap, int row, int col) {
    if (animation->type != SSGE_ANIM_FRAMES)
        SSGE_Error("Wrong animation type")

    SDL_Texture *frame = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilemap->tileWidth, tilemap->tileHeight);
    if (frame == NULL)
        SSGE_Error("Failed to allocate memory for texture")

    SDL_Rect src = {col * (tilemap->tileWidth + tilemap->spacing), row * (tilemap->tileHeight + tilemap->spacing), tilemap->tileWidth, tilemap->tileHeight};
    SDL_Rect dest = {0, 0, tilemap->tileWidth,tilemap->tileHeight};
    SDL_SetRenderTarget(_engine.renderer, frame);
    SDL_RenderCopy(_engine.renderer, tilemap->texture, &src, &dest);
    SDL_SetRenderTarget(_engine.renderer, NULL);

    if (animation->data.currentCount >= animation->data.frameCount)
        SSGE_Error("Animation already have max number of frames")

    animation->data.frames[animation->data.currentCount] = frame;
    animation->data.frametimes[animation->data.currentCount++] = frametime;
}

SSGEAPI SSGE_Animation *SSGE_Animation_Get(uint32_t id) {
    SSGE_Animation *ptr = SSGE_Array_Get(&_animationList, id);
    if (ptr == NULL)
        SSGE_ErrorEx("Animation not found: %u", id)
    return ptr;
}

inline static bool _find_animation_name(void *ptr, void *name) {
    return strcmp(((SSGE_Animation *)ptr)->name, (char *)name) == 0;
}

SSGEAPI SSGE_Animation *SSGE_Animation_GetName(const char *name) {
    SSGE_Animation *ptr = SSGE_Array_Find(&_animationList, _find_animation_name, (void *)name);
    if (ptr == NULL)
        SSGE_ErrorEx("Animation not found: %s", name)
    return ptr;
}

SSGEAPI uint32_t SSGE_Animation_Play(SSGE_Animation *animation, int x, int y, uint32_t loop, bool reversed, bool pingpong) {
    SSGE_AnimationState *state = (SSGE_AnimationState*)malloc(sizeof(SSGE_AnimationState));
    if (state == NULL)
        SSGE_Error("Failed to allocate memory for animation state")

    state->animation = animation;
    state->x = x;
    state->y = y;
    state->loop = loop;
    state->currentFrame = 0;
    state->currentFrameTime = 0;
    state->reversed = reversed;
    state->pingpong = pingpong;
    state->isPlaying = true;

    return SSGE_Array_Add(&_playingAnim, state);
}

SSGEAPI void SSGE_Animation_Pause(uint32_t id) {
    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    state->isPlaying = false;
}

SSGEAPI void SSGE_Animation_Resume(uint32_t id) {
    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    state->isPlaying = true;
}

SSGEAPI void SSGE_Animation_Stop(uint32_t id) {
    SSGE_AnimationState *state = SSGE_Array_Pop(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    free(state);
}

SSGEAPI void SSGE_Animation_Move(uint32_t id, int x, int y) {
    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);
    
    state->x = x;
    state->y = y;
}
