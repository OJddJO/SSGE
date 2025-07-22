#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_animation.h"

/******************************
 * Animation functions
 ******************************/

/**
 * Creates an animation with frames
 * \param name The name of the animation
 * \param type The type of animation
 * \param draw The draw function of the animation, if `type` is `SSGE_ANIM_FUNCTION`. Ignored if not `SSGE_ANIM_FUNCTION`
 * \return The id of the animation
 */
SSGEDECL uint32_t SSGE_CreateAnimation(char *name, SSGE_AnimationType type, void (*draw)(SSGE_AnimationState *)) {
    switch (type) {
        case SSGE_ANIM_FRAMES:
            break;
        case SSGE_ANIM_FUNCTION:
            break;
    }
}