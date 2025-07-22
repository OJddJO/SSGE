#ifndef __SSGE_TYPES_H__
#define __SSGE_TYPES_H__

#include <stdbool.h>
#include <stdint.h>

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_array.h"
#include "SSGE/SSGE_events.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 * SDL Structures
 ************************************************/

/**
 * Color structure (SDL_Color)
 * \param r `Uint8` The red component of the color
 * \param g `Uint8` The green component of the color
 * \param b `Uint8` The blue component of the color
 * \param a `Uint8` The alpha component of the color
 */
typedef struct _SSGE_Color {
    uint8_t r; // red component of the color
    uint8_t g; // green component of the color
    uint8_t b; // blue component of the color
    uint8_t a; // alpha component of the color
} SSGE_Color;

/**
 * Point structure (SDL_Point)
 * \param x `int` The x coordinate of the point
 * \param y `int` The y coordinate of the point
 */
typedef struct _SSGE_Point {
    int x; // x coordinate of the point
    int y; // y coordinate of the point
} SSGE_Point;

/**
 * Flip enum (SDL_RendererFlip)
 * \param SSGE_FLIP_NONE No flip
 * \param SSGE_FLIP_HORIZONTAL Horizontal flip
 * \param SSGE_FLIP_VERTICAL Vertical flip
 * \param SSGE_FLIP_HOR_VER Horizontal and vertical flip
 */
typedef enum {
    SSGE_FLIP_NONE       = 0x00000000,   // no flip
    SSGE_FLIP_HORIZONTAL = 0x00000001,   // flip horizontally
    SSGE_FLIP_VERTICAL   = 0x00000002,   // flip vertically
    SSGE_FLIP_HOR_VER    = 0x00000003    // flip horizontally and vertically
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
 * \param window The window
 * \param renderer The renderer
 * \param width The width of the window
 * \param height The height of the window
 * \param fps The frames per second
 * \param isRunning The running state of the engine
 * \param initialized If the `SSGE_Engine` has been initialized
 */
typedef struct _SSGE_Engine {
    struct SDL_Window *window;
    struct SDL_Renderer *renderer;
    int width;
    int height;
    int fps;
    bool isRunning;
    bool initialized;
} SSGE_Engine;

/**
 * Texture structure
 * \param id The id of the texture
 * \param name The name of the texture
 * \param texture The SDL_Texture
 */
typedef struct _SSGE_Texture {
    char *name;
    uint32_t id;
    struct SDL_Texture *texture;
} SSGE_Texture;

/**
 * Object template structure
 * \param id The id of the template
 * \param name The name of the template
 * \param texture The texture for the template
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox If objects created from this template have a hitbox
 */
typedef struct _SSGE_ObjectTemplate {
    char *name;
    uint32_t id;
    SSGE_Texture *texture;
    int width;
    int height;
    bool hitbox;
    void (*destroyData)(void *);
} SSGE_ObjectTemplate;

/**
 * Object structure
 * \param id The id of the object
 * \param name The name of the object
 * \param texture The texture of the object
 * \param x The x position of the object
 * \param y The y position of the object
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox If the object has a hitbox
 * \param data The data of the object
 */
typedef struct _SSGE_Object {
    char *name;
    uint32_t id;
    struct SDL_Texture *texture;
    int x;
    int y;
    int width;
    int height;
    bool hitbox;
    void *data;
    void (*destroyData)(void *);
} SSGE_Object;

/**
 * Tilemap structure
 * \param texture The texture of the tilemap
 * \param tileWidth The width of the tiles
 * \param tileHeight The height of the tiles
 * \param spacing The spacing between the tiles
 * \param nbRows The number of rows in the tilemap
 * \param nbCols The number of columns in the tilemap
 */
typedef struct _SSGE_Tilemap {
    struct SDL_Texture *texture;
    int tileWidth;
    int tileHeight;
    int spacing;
    int nbRows;
    int nbCols;
} SSGE_Tilemap;

/**
 * Tile structure
 * \param tilemap The tilemap of the tile
 * \param row The row of the tile
 * \param col The column of the tile
 */
typedef struct _SSGE_Tile {
    SSGE_Tilemap *tilemap;
    int row;
    int col;
} SSGE_Tile;

typedef struct _SSGE_AnimationState SSGE_AnimationState;

typedef enum _SSGE_AnimationType {
    SSGE_ANIM_FRAMES = 0,
    SSGE_ANIM_FUNCTION
} SSGE_AnimationType;

/**
 * Animation structure
 * \param id The id of the animation
 * \param name The name of the animation
 * \param type The animation type
 * \param data The data of the animation if `type` is `SSGE_ANIM_FRAMES`
 * \param data.frames An array of the animation frames
 * \param data.frameCount The number of animation frames
 * \param draw The animation function if `type` is `SSGE_ANIM_FUNCTION`. Must take in an `SSGE_AnimationState` pointer
 */
typedef struct _SSGE_Animation {
    char *name;
    uint32_t id;
    SSGE_AnimationType type;
    union {
        struct _SSGE_AnimationData {
            struct SDL_Texture **frames;
            uint32_t frameCount;
        } data;
        void (*draw)(SSGE_AnimationState *);
    };
} SSGE_Animation;

/**
 * Animation state structure
 * \param animation The animation to track the animation state
 * \param currentFrame The index of the current frame
 * \param startFrame The index of the start frame
 * \param elapsedFrame The number of frame elapsed since the animation start
 * \param loop If the animation should loop
 * \param pingpong If the animation should pingpong (normal -> reversed)
 * \param isPlaying If the animation is playing or not
 * \param reverse If the animation is reversed or not
 * \param callback The function to call at the end of the animation
 * \param callbackData The data passed to the callback function
 */
typedef struct _SSGE_AnimationState {
    SSGE_Animation *animation;
    uint32_t currentFrame;
    uint32_t startFrame;
    uint32_t elpasedFrame;
    bool loop;
    bool pingpong;
    bool isPlaying;
    bool reverse;
    void (*callback)(void *);
    void *callbackData;
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
 * \param name The name of the font
 * \param font The TTF_Font 
 */
typedef struct _SSGE_Font {
    char *name;
    struct _TTF_Font *font;
} SSGE_Font;

/**
 * Audio structure
 * \param name The name of the audio
 * \param audio The Mix_Chunk
 */
typedef struct _SSGE_Audio {
    char *name;
    uint32_t id;
    struct Mix_Chunk *audio;
} SSGE_Audio;

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TYPES_H__