#ifndef __SSGE_ANIMATION_H__
#define __SSGE_ANIMATION_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/**
 * Creates an animation with frames
 * \param name The name of the animation
 * \param type The type of animation
 * \param frameCount The number of frame if `type` is `SSGE_ANIM_FRAME`, ignored if not
 * \param draw The draw function of the animation if `type` is `SSGE_ANIM_FUNCTION`, ignored if not
 */
SSGEDECL uint32_t SSGE_CreateAnimation(char *name, SSGE_AnimationType type, uint32_t frameCount, void (*draw)(SSGE_AnimationState *));

#endif //__SSGE_ANIMATION_H__