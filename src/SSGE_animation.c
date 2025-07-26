#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_animation.h"

/******************************
 * Animation functions
 ******************************/

/**
 * Creates an animation
 * \param name The name of the animation
 * \param type The type of animation
 * \param frameCount The number of frame if `type` is `SSGE_ANIM_FRAME`, ignored if not
 * \param draw The draw function of the animation if `type` is `SSGE_ANIM_FUNCTION`, ignored if not
 * \return The id of the animation
 */
SSGEDECL uint32_t SSGE_Animation_Create(char *name, SSGE_AnimationType type, uint16_t frameCount, void (*draw)(SSGE_AnimationState *)) {
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
            break;
        case SSGE_ANIM_FUNCTION:
            anim->draw = draw;
            break;
    }

    return _add_to_list(&_animation_list, anim, name, __func__);
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