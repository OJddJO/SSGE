#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_animation.h"

SSGEDECL SSGE_Animation *SSGE_Animation_CreateFrames(uint32_t *id, char *name, uint32_t frameCount, uint16_t width, uint16_t height) {
    _assert_engine_init

    SSGE_Animation *anim = (SSGE_Animation *)malloc(sizeof(SSGE_Animation));
    if (anim == NULL)
        SSGE_Error("Failed to allocate memory for animation")

    anim->type = SSGE_ANIM_FRAMES;
    anim->data.frames = (SDL_Texture **)calloc(frameCount, sizeof(SDL_Texture *));
    anim->data.frameCount = frameCount;
    anim->data.width = width;
    anim->data.height = height;

    _add_to_list(&_animation_list, anim, name, id, __func__);
    return anim;
}

SSGEDECL SSGE_Animation *SSGE_Animation_CreateFunc(uint32_t *id, char *name, void (*draw)(SSGE_AnimationState *)) {
    _assert_engine_init

    SSGE_Animation *anim = (SSGE_Animation *)malloc(sizeof(SSGE_Animation));
    if (anim == NULL) 
        SSGE_Error("Failed to allocate memory for animation")

    anim->type = SSGE_ANIM_FUNCTION;
    anim->draw = draw;

    _add_to_list(&_animation_list, anim, name, id, __func__);
    return anim;
}

SSGEDECL void SSGE_Animation_Anchor(SSGE_Animation *animation, int x, int y) {
    _assert_engine_init

    if (animation->type != SSGE_ANIM_FRAMES)
        SSGE_Error("Wrong animation type")

    animation->data.anchorX = x;
    animation->data.anchorY = y;
}

SSGEDECL void SSGE_Animation_AddFrame(SSGE_Animation *animation, uint8_t frametime, char *file) {
    _assert_engine_init

    if (animation->type != SSGE_ANIM_FRAMES)
        SSGE_Error("Wrong animation type")

    SDL_Texture *frame = IMG_LoadTexture(_engine.renderer, file);
    if (frame == NULL)
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError())

    if (animation->data.currentCount >= animation->data.frameCount)
        SSGE_Error("Animation already have max number of frames")

    animation->data.frames[animation->data.currentCount] = frame;
    animation->data.frametimes[animation->data.currentCount++] = frametime;
}

SSGEDECL void SSGE_Animation_AddFrameTilemap(SSGE_Animation *animation, uint8_t frametime, SSGE_Tilemap *tilemap, int row, int col) {
    _assert_engine_init

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

SSGEDECL uint32_t SSGE_Animation_Play(SSGE_Animation *animation, int x, int y, uint32_t loop, bool reversed, bool pingpong) {
    _assert_engine_init

    SSGE_AnimationState *state = (SSGE_AnimationState*)malloc(sizeof(SSGE_AnimationState));
    if (state == NULL)
        SSGE_Error("Failed to allocate memory for animation state")

    state->animation = animation;
    state->x = x;
    state->y = y;
    state->currentFrame = 0;
    state->currentFrameTime = 0;
    state->loop = loop;
    state->reversed = reversed;
    state->pingpong = pingpong;
    state->isPlaying = true;

    return SSGE_Array_Add(&_playingAnim, state);
}

SSGEDECL void SSGE_Animation_Pause(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    state->isPlaying = false;
}

SSGEDECL void SSGE_Animation_Resume(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    state->isPlaying = true;
}

SSGEDECL void SSGE_Animation_Stop(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Pop(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id)

    free(state);
}

SSGEDECL void SSGE_Animation_Move(uint32_t id, int x, int y) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);
    
    state->x = x;
    state->y = y;
}

SSGEDECL void SSGE_Animation_Update(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);

    SSGE_Animation *anim = state->animation;
    switch (anim->type) {
        case SSGE_ANIM_FRAMES:
            SDL_Rect dest = {
                state->x - anim->data.anchorX,
                state->y - anim->data.anchorY,
                anim->data.width,
                anim->data.height,
            };
            SDL_RenderCopy(_engine.renderer, anim->data.frames[state->currentFrame], NULL, &dest);

            if (state->currentFrameTime >= anim->data.frametimes[state->currentFrame]) {
                state->currentFrame += 1 - 2*state->reversed;
                state->currentFrameTime = 0;
            }

            if (state->currentFrame < anim->data.currentCount)
                break;

            if (!(state->loop || state->pingpong)) // If there is no play count modifier
                SSGE_Array_Remove(&_playingAnim, id, free);

            if (state->pingpong) { // If pingpong then we need to decrement the frame (according to the reversed state)
                state->currentFrame -= 2 * (1 - 2*state->reversed);
                state->reversed = !state->reversed;
            } else { // else just set the frame to 0
                state->currentFrame = 0;
            }

            // If not loop then we can set pingpong to false (whatever the state of pingpong)
            if (!state->loop)
                state->pingpong = false;

            if (state->loop && state->loop != -1) --state->loop; // Decrement loop count
            break;

        case SSGE_ANIM_FUNCTION:
            anim->draw(state);
            break;
    }
}

SSGEDECL void SSGE_Animation_UpdateAll() {
    _assert_engine_init

    uint32_t i = 0, count = _playingAnim.count;
    while (i < count) {
        SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, i);
        if (state == NULL) continue;
        if (!state->isPlaying) continue;

        SSGE_Animation_Update(i);

        ++i;
    }
}
