#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_animation.h"

/******************************
 * Animation functions
 ******************************/

/**
 * Creates an animation
 * \param id Where to store the id, can be usefull to get the animation
 * \param name The name of the animation
 * \param type The type of animation
 * \param frameCount The number of frame if `type` is `SSGE_ANIM_FRAME`, ignored if not
 * \param draw The draw function of the animation if `type` is `SSGE_ANIM_FUNCTION`, ignored if not
 * \return The animation
 */
SSGEDECL SSGE_Animation *SSGE_CreateAnimation(uint32_t *id, char *name, SSGE_AnimationType type, uint32_t frameCount, void (*draw)(SSGE_AnimationState *)) {
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
            anim->data.frames = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * frameCount);
            anim->data.frameCount = frameCount;
            break;
        case SSGE_ANIM_FUNCTION:
            anim->draw = draw;
            break;
    }

    *id = _add_to_list(&_animation_list, anim, name, "SSGE_CreateAnimation");
    return anim;
}

/**
 * Add a frame in an animation
 */
