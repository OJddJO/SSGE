#ifndef __SSGE_ANIMATION_H__
#define __SSGE_ANIMATION_H__

#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Animation frame struct
 * \param texture The texture of the frame, if the type is SSGE_ANIM_IMAGE
 * \param duration The frametime of the frame
 * \param offset_x
 */
typedef struct _SSGE_AnimationFrame {
    struct _SSGE_Texture *texture;
    uint32_t duration;
    int xOffset;
    int yOffset;
} SSGE_AnimationFrame;

/**
 * Animation structure
 * \param type The animation type
 * \param data The data of the animation, if `type` is `SSGE_ANIM_FRAMES`
 * \param data.frames An array of the animation frames
 * \param data.frameCount The number of animation frames
 * \param draw The animation function, if `type` is `SSGE_ANIM_FUNCTION`
 */
typedef struct _SSGE_Animation {
    char *name;
    enum _SSGE_AnimationType {
        SSGE_ANIM_FRAMES,
        SSGE_ANIM_FUNCTION
    } type;
    union {
        struct _SSGE_AnimationData {
            SSGE_AnimationFrame *frames;
            uint32_t frameCount;
        } data;
        void (*draw)(void *);
    };
    bool loop;
    bool pingpong;
} SSGE_Animation;

#ifdef __cplusplus
}
#endif

#endif //__SSGE_ANIMATION_H__