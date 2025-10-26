#ifndef __SSGE_TYPES_H__
#define __SSGE_TYPES_H__

#include <stdbool.h>
#include <stdint.h>

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_events.h"
#include "SSGE/SSGE_keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 * SDL Structs
 ************************************************/

// Color struct (SDL_Color)
typedef struct _SSGE_Color {
    uint8_t r; // The red component of the color
    uint8_t g; // The green component of the color
    uint8_t b; // The blue component of the color
    uint8_t a; // The alpha component of the color
} SSGE_Color;

// Point struct (SDL_Point)
typedef struct _SSGE_Point {
    int x; // The x coordinate of the point
    int y; // The y coordinate of the point
} SSGE_Point;

// Flip enum (SDL_RendererFlip)
typedef enum {
    SSGE_FLIP_NONE       = 0x00000000, // No flip
    SSGE_FLIP_HORIZONTAL = 0x00000001, // Flip horizontally
    SSGE_FLIP_VERTICAL   = 0x00000002, // Flip vertically
    SSGE_FLIP_HOR_VER    = 0x00000003  // Flip horizontally and vertically
} SSGE_Flip;

typedef enum {
    SSGE_WINDOWMODE_WINDOWED = 0,
    SSGE_WINDOWMODE_FULLSCREEN = 1,
    SSGE_WINDOWMODE_FULLSCREEN_DESKTOP = 4097,
} SSGE_WindowMode;

/*************************************************
 * SSGE Types
 * For events and keyboard structs,
 *  see the related headers
 *************************************************/

typedef void (*SSGE_UpdateFunc)(void *);
typedef void (*SSGE_DrawFunc)(void *);
typedef void (*SSGE_EventHandler)(SSGE_Event, void *);

typedef void (*SSGE_DestroyData)(void *);

typedef enum _SSGE_AnimationType {
    SSGE_ANIM_FRAMES = 0,   // Animation using frames
    SSGE_ANIM_FUNCTION      // Animation using a function
} SSGE_AnimationType;

typedef enum _SSGE_SpriteType {
    SSGE_SPRITE_NONE = -1U, // No sprite
    SSGE_SPRITE_STATIC = 0, // Single frame sprite
    SSGE_SPRITE_ANIM        // Animated sprite
} SSGE_SpriteType;

typedef enum _SSGE_Anchor {
    SSGE_NW,
    SSGE_N,
    SSGE_NE,
    SSGE_W,
    SSGE_CENTER,
    SSGE_E,
    SSGE_SW,
    SSGE_S,
    SSGE_SE
} SSGE_Anchor;

typedef struct _SSGE_Engine SSGE_Engine;
typedef struct _SSGE_Array SSGE_Array;
typedef struct _SSGE_Texture SSGE_Texture;

typedef struct _SSGE_AnimationState SSGE_AnimationState;
typedef struct _SSGE_Animation SSGE_Animation;
typedef struct _SSGE_AnimationState SSGE_AnimationState;

typedef struct _SSGE_Object SSGE_Object;
typedef struct _SSGE_ObjectTemplate SSGE_ObjectTemplate;
typedef struct _SSGE_Tilemap SSGE_Tilemap;
typedef struct _SSGE_Tile SSGE_Tile;

typedef struct _SSGE_Font SSGE_Font;
typedef struct _SSGE_Audio SSGE_Audio;

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TYPES_H__