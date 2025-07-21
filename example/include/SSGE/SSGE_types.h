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

/******************************
 * SDL Structures
 ******************************/

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

/******************************
 * Prototypes
 ******************************/

// Game structure prototype to contain all datas
typedef struct _Game Game;

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TYPES_H__