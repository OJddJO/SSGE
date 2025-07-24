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
            break;
        case SSGE_ANIM_FUNCTION:
            anim->draw = draw;
            break;
    }

    return _add_to_list(&_animation_list, anim, name, __func__);
}

/**
 * Add a frame in an animation
 */
SSGEDECL void SSGE_Animation_AddFrame(uint32_t id, char *file) {
    _assert_engine_init

    SDL_Texture *frame = IMG_LoadTexture(_engine.renderer, file);
    if (frame == NULL)
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError());

    SSGE_Animation *anim = SSGE_Array_Get(&_animation_list, id);
    if (anim == NULL) 
        SSGE_ErrorEx("Animation not found: %s", id);

    uint32_t i = 0;
    while (anim->data.frames[i] != NULL && i < anim->data.frameCount)
        i++;

    if (i >= anim->data.frameCount)
        SSGE_Error("Animation already have max number of frames");

    anim->data.frames[i] = frame;
}