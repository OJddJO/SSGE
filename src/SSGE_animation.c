#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_animation.h"

/******************************
 * Animation functions
 ******************************/

/**
 * Create an animation
 * \param name The name of the animation
 * \param type The type of animation
 * \param frameCount The number of frame if `type` is `SSGE_ANIM_FRAME`, ignored if not
 * \param draw The draw function of the animation if `type` is `SSGE_ANIM_FUNCTION`, ignored if not
 * \return The id of the animation
 */
SSGEDECL uint32_t SSGE_Animation_Create(char *name, SSGE_AnimationType type, uint32_t frameCount, void (*draw)(SSGE_AnimationState *)) {
    _assert_engine_init

    SSGE_Animation *anim = (SSGE_Animation *)malloc(sizeof(SSGE_Animation));
    if (anim == NULL)
        SSGE_Error("Failed to allocate memory for animation");

    anim->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (anim->name == NULL)
        SSGE_Error("Failed to allocate memory for name");

    anim->type = type;

    switch (type) {
        case SSGE_ANIM_FRAMES:
            anim->data.frames = (SDL_Texture **)calloc(frameCount, sizeof(SDL_Texture *));
            anim->data.frameCount = frameCount;
            anim->data.currentCount = 0;
            anim->data.anchorX = 0;
            anim->data.anchorY = 0;
            break;
        case SSGE_ANIM_FUNCTION:
            anim->draw = draw;
            break;
    }

    return _add_to_list(&_animation_list, anim, name, __func__);
}

/**
 * Set the anchor of an animation
 * \param id The id of the animation to move
 * \param x The anchor's x coordinate relative to the frame
 * \param y The anchor's y coordinate relative to the frame
 */
SSGEDECL void SSGE_Animation_Anchor(uint32_t id, int x, int y) {
    _assert_engine_init

    SSGE_Animation *anim = SSGE_Array_Get(&_animation_list, id);
    if (anim == NULL)
        SSGE_ErrorEx("Animation not found: %u", id);

    anim->data.anchorX = x;
    anim->data.anchorY = y;
}

/**
 * Add a frame in an animation
 * \param id The id of the animation to add a frame
 * \param texture The path of the image
 */
SSGEDECL void SSGE_Animation_AddFrame(uint32_t id, char *file) {
    _assert_engine_init

    SSGE_Animation *anim = SSGE_Array_Get(&_animation_list, id);
    if (anim == NULL)
        SSGE_ErrorEx("Animation not found: %u", id);

    SDL_Texture *frame = IMG_LoadTexture(_engine.renderer, file);
    if (frame == NULL)
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError());

    if (anim->data.currentCount >= anim->data.frameCount)
        SSGE_Error("Animation already have max number of frames");

    anim->data.frames[anim->data.currentCount++] = frame;
}

/**
 * Add a frame in an animation from a tilemap
 * \param id The id of the animation to add a frame
 * \param tilemap The tilemap to get the frame from
 * \param row The row of the tile
 * \param col The col of the tile
 */
SSGEDECL void SSGE_Animation_AddFrameTilemap(uint32_t id, SSGE_Tilemap *tilemap, int row, int col) {
    _assert_engine_init

    SSGE_Animation *anim = SSGE_Array_Get(&_animation_list, id);
    if (anim == NULL)
        SSGE_ErrorEx("Animation not found: %u", id);

    SDL_Texture *frame = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilemap->tileWidth, tilemap->tileHeight);
    if (frame == NULL)
        SSGE_Error("Failed to allocate memory for texture");

    SDL_Rect src = {col * (tilemap->tileWidth + tilemap->spacing), row * (tilemap->tileHeight + tilemap->spacing), tilemap->tileWidth, tilemap->tileHeight};
    SDL_Rect dest = {0, 0, tilemap->tileWidth,tilemap->tileHeight};
    SDL_SetRenderTarget(_engine.renderer, frame);
    SDL_RenderCopy(_engine.renderer, tilemap->texture, &src, &dest);
    SDL_SetRenderTarget(_engine.renderer, NULL);

    if (anim->data.currentCount >= anim->data.frameCount)
        SSGE_Error("Animation already have max number of frames");

    anim->data.frames[anim->data.currentCount++] = frame;
}

/**
 * Play an animation
 * \param id The id of the animation to play
 * \param loop If the animation should loop
 * \param reversed If the animation should be reversed
 * \param pingpong If the animation should pingpong
 * \param callback The function to call at the end of the animation
 * \param callbackData The data to pass to the callback
 * \param destroyData The function to destroy the data
 * \return The id of the animation state
 */
SSGEDECL uint32_t SSGE_Animation_Play(uint32_t id, int x, int y, bool loop, bool reversed, bool pingpong, void (*callback)(void *), void *callbackData, void (*destroyData)(void *)) {
    _assert_engine_init

    SSGE_Animation *anim = SSGE_Array_Get(&_animation_list, id);
    if (anim == NULL)
        SSGE_ErrorEx("Animation not found: %u", id);

    SSGE_AnimationState *state = (SSGE_AnimationState*)malloc(sizeof(SSGE_AnimationState));
    if (state == NULL)
        SSGE_Error("Failed to allocate memory for animation state");

    state->animation = anim;
    state->x = x;
    state->y = y;
    state->currentFrame = 0;
    state->startFrame = 0;
    state->elpasedFrame = 0;
    state->loop = loop;
    state->reversed = reversed;
    state->pingpong = pingpong;
    state->isPlaying = true;
    state->callback = callback;
    state->callbackData = callbackData;
    state->destroyData = destroyData;

    return SSGE_Array_Add(&_playingAnim, state);
}

/**
 * Pause an animation
 * \param id The id of the animation state
 */
SSGEDECL void SSGE_Animation_Pause(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);

    state->isPlaying = false;
}

/**
 * Resume an animation
 * \param id The id of the animation state
 */
SSGEDECL void SSGE_Animation_Resume(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);
    
    state->isPlaying = true;
}

/**
 * Stop an animation
 */
SSGEDECL void SSGE_Animation_Stop(uint32_t id) {
    _assert_engine_init

    SSGE_AnimationState *state = SSGE_Array_Get(&_playingAnim, id);
    if (state == NULL)
        SSGE_ErrorEx("Animation state not found: %u", id);

    SSGE_Array_Remove(&_playingAnim, id, _destroy_animation_state);
}
