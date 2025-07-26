#ifndef __SSGE_TYPES_H__
#define __SSGE_TYPES_H__

#include <stdbool.h>
#include <stdint.h>

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_events.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 * SDL Structures
 ************************************************/

/**
 * Color structure (SDL_Color)
 */
typedef struct _SSGE_Color {
    uint8_t r; // The red component of the color
    uint8_t g; // The green component of the color
    uint8_t b; // The blue component of the color
    uint8_t a; // The alpha component of the color
} SSGE_Color;

/**
 * Point structure (SDL_Point)
 */
typedef struct _SSGE_Point {
    int x; // The x coordinate of the point
    int y; // The y coordinate of the point
} SSGE_Point, SSGE_Coords;

/**
 * Flip enum (SDL_RendererFlip)
 */
typedef enum {
    SSGE_FLIP_NONE       = 0x00000000, // No flip
    SSGE_FLIP_HORIZONTAL = 0x00000001, // Flip horizontally
    SSGE_FLIP_VERTICAL   = 0x00000002, // Flip vertically
    SSGE_FLIP_HOR_VER    = 0x00000003  // Flip horizontally and vertically
} SSGE_Flip;

/************************************************
 * Prototypes
 ************************************************/

// Game structure prototype to contain all datas
typedef struct _Game Game;

/*************************************************
 * SSGE Structures
 *************************************************/

/**
 * Engine structure
 */
typedef struct _SSGE_Engine {
    struct SDL_Window   *window;    // The window
    struct SDL_Renderer *renderer;  // The renderer
    uint16_t            width;      // The width of the window
    uint16_t            height;     // The height of the window
    uint16_t            fps;        // The frames per second
    bool                isRunning;  // The running state of the engine
    bool                initialized; // If the `SSGE_Engine` has been initialized
} SSGE_Engine;

/**
 * Texture structure
 */
typedef struct _SSGE_Texture {
    char                *name;      // The name of the texture
    uint32_t            id;         // The id of the texture
    struct SDL_Texture  *texture;   // The SDL_Texture
} SSGE_Texture;

/**
 * Object structure
 */
typedef struct _SSGE_Object {
    char                *name;      // The name of the object
    uint32_t            id;         // The id of the object
    struct SDL_Texture  *texture;   // The texture of the object
    int                 x;          // The x position of the object
    int                 y;          // The y position of the object
    uint16_t            width;      // The width of the object
    uint16_t            height;     // The height of the object
    bool                hitbox;     // If the object has a hitbox
    void                *data;      // The data of the object
    void                (*destroyData)(void *); // The function to be called to destroy the data
} SSGE_Object;

/**
 * Object template structure
 */
typedef struct _SSGE_ObjectTemplate {
    char            *name;      // The name of the template
    uint32_t        id;         // The id of the template
    SSGE_Texture    *texture;   // The texture for the template
    int             width;      // The width of the object
    int             height;     // The height of the object
    bool            hitbox;     // If objects created from this template have a hitbox
    void            (*destroyData)(void *); // The function to be called to destroy the object data
} SSGE_ObjectTemplate;

/**
 * Tilemap structure
 */
typedef struct _SSGE_Tilemap {
    struct SDL_Texture  *texture;   // The texture of the tilemap
    int                 tileWidth;  // The width of the tiles
    int                 tileHeight; // The height of the tiles
    int                 spacing;    // The spacing between the tiles
    int                 nbRows;     // The number of rows in the tilemap
    int                 nbCols;     // The number of columns in the tilemap
} SSGE_Tilemap;

/**
 * Tile structure
 */
typedef struct _SSGE_Tile {
    SSGE_Tilemap    *tilemap;   // The tilemap of the tile
    int             row;        // The row of the tile
    int             col;        // The column of the tile
} SSGE_Tile;

typedef struct _SSGE_AnimationState SSGE_AnimationState;

typedef enum _SSGE_AnimationType {
    SSGE_ANIM_FRAMES = 0,
    SSGE_ANIM_FUNCTION
} SSGE_AnimationType;

/**
 * Animation structure
 */
typedef struct _SSGE_Animation {
    char                *name;  // The name of the animation
    uint32_t            id;     // The id of the animation
    SSGE_AnimationType  type;   // The animation type
    union {
        struct _SSGE_AnimationData {
            struct SDL_Texture  **frames;       // An array of the animation frames
            uint16_t            frameCount;     // The number of animation frames
            uint16_t            currentCount;   // The number of frames the animation currently have
        } data;

        /**
         * The animation function if `type` is `SSGE_ANIM_FUNCTION`.
         * Must take in an `SSGE_AnimationState` pointer
         */
        void (*draw)(SSGE_AnimationState *);
    };
} SSGE_Animation;

/**
 * Animation state structure
 */
typedef struct _SSGE_AnimationState {
    SSGE_Animation  *animation;     // The animation to track the animation state
    uint32_t        currentFrame;   // The index of the current frame
    uint32_t        startFrame;     // The index of the start frame
    uint32_t        elpasedFrame;   // The number of frame elapsed since the animation start
    bool            loop;           // If the animation should loop
    bool            pingpong;       // If the animation should pingpong (normal -> reversed)
    bool            isPlaying;      // If the animation is playing or not
    bool            reverse;        // If the animation is reversed or not
    void            (*callback)(void *); // The function to call at the end of the animation
    void            *callbackData;  // The data passed to the callback function
} SSGE_AnimationState;

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

/**
 * Font structure
 */
typedef struct _SSGE_Font {
    char                *name; // The name of the font
    struct _TTF_Font    *font; // The TTF_Font
} SSGE_Font;

/**
 * Audio structure
 */
typedef struct _SSGE_Audio {
    char                *name;  // The name of the audio
    uint32_t            id;     // The id of the audio
    struct Mix_Chunk    *audio; // The Mix_Chunk
} SSGE_Audio;

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TYPES_H__