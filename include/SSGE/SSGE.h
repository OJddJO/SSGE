#ifndef __SSGE_H__
#define __SSGE_H__

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "SSGE/SSGE_decl.h"

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
typedef struct SDL_Color SSGE_Color;

/**
 * Point structure (SDL_Point)
 * \param x `int` The x coordinate of the point
 * \param y `int` The y coordinate of the point
 */
typedef struct SDL_Point SSGE_Point;

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

// Event structure (SDL_Event)
typedef union SDL_Event SSGE_Event;

// Font structure (TTF_Font)
typedef struct _TTF_Font SSGE_Font;

// Audio structure (Mix_Chunk)
typedef struct Mix_Chunk SSGE_Audio;


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
 * \param isRunning The running state of the engine
 * \param width The width of the window
 * \param height The height of the window
 * \param fps The frames per second
 */
typedef struct _SSGE_Engine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int width;
    int height;
    int fps;
} SSGE_Engine;

/**
 * Texture structure
 * \param name The name of the texture
 * \param texture The SDL_Texture
 */
typedef struct _SSGE_Texture {
    char *name;
    SDL_Texture *texture;
} SSGE_Texture;

/**
 * Object template structure
 * \param name The name of the template
 * \param texture The texture for the template
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox If objects created from this template have a hitbox
 */
typedef struct _SSGE_ObjectTemplate {
    char *name;
    SDL_Texture *texture;
    int width;
    int height;
    bool hitbox;
    void (*destroy_data)(void *);
} SSGE_ObjectTemplate;

/**
 * Object structure
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
    SSGE_Texture *texture;
    int x;
    int y;
    int width;
    int height;
    bool hitbox;
    void *data;
    void (*destroy_data)(void *);
} SSGE_Object;

/**
 * Tilemap structure
 * \param texture The texture of the tilemap
 * \param tile_width The width of the tiles
 * \param tile_height The height of the tiles
 * \param spacing The spacing between the tiles
 * \param nb_rows The number of rows in the tilemap
 * \param nb_cols The number of columns in the tilemap
 */
typedef struct _SSGE_Tilemap {
    SSGE_Texture *texture;
    int tile_width;
    int tile_height;
    int spacing;
    int nb_rows;
    int nb_cols;
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

/************************************************
 * SSGE Functions
 ************************************************/

SSGEDECL void SSGE_Init(const char *title, int width, int height, int fps);
SSGEDECL void SSGE_Quit();
SSGEDECL void SSGE_Run(void (*update)(Game *), void (*draw)(Game *), void (*event_handler)(SSGE_Event, Game *), Game *data);

// Window functions

SSGEDECL void SSGE_SetWindowTitle(char *title);
SSGEDECL void SSGE_SetWindowIcon(char *filename);
SSGEDECL void SSGE_WindowResizable(bool resizable);
SSGEDECL void SSGE_WindowFullscreen(bool fullscreen);
SSGEDECL void SSGE_SetManualUpdate(bool manual_update);
SSGEDECL void SSGE_ManualUpdate();

// Texture functions

SSGEDECL uint32_t SSGE_LoadTexture(char *filename, char *name);
SSGEDECL SSGE_Texture *SSGE_GetTexture(uint32_t id);
SSGEDECL SSGE_Texture *SSGE_GetTextureByName(char *name);
SSGEDECL void SSGE_DrawTexture(SSGE_Texture *texture, int x, int y, int width, int height);
SSGEDECL void SSGE_DrawTextureEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip);
SSGEDECL void SSGE_DrawTextureFromPath(char *filename, int x, int y, int width, int height);
SSGEDECL void SSGE_DestroyTexture(uint32_t id);
SSGEDECL void SSGE_DestroyTextureByName(char *name);
SSGEDECL void SSGE_DestroyAllTextures();

// Tilemap functions

SSGEDECL SSGE_Tilemap *SSGE_LoadTilemap(char *filename, int tile_width, int tile_height, int spacing, int nb_rows, int nb_cols);
SSGEDECL SSGE_Tile *SSGE_GetTile(SSGE_Tilemap *tilemap, int tile_row, int tile_col);
SSGEDECL uint32_t SSGE_GetTileAsTexture(char *name, SSGE_Tilemap *tilemap, int tile_row, int tile_col);
SSGEDECL void SSGE_DrawTile(SSGE_Tile *tile, int x, int y);
SSGEDECL void SSGE_DrawTileWithSize(SSGE_Tile *tile, int x, int y, int width, int height);
SSGEDECL void SSGE_DrawTileFromTilemap(SSGE_Tilemap *tilemap, int tile_row, int tile_col, int x, int y);
SSGEDECL void SSGE_DestroyTile(SSGE_Tile *tile);
SSGEDECL void SSGE_DestroyTilemap(SSGE_Tilemap *tilemap);

// Object functions

SSGEDECL uint32_t SSGE_CreateObject(char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroy_data)(void *));
SSGEDECL uint32_t SSGE_InstantiateObject(SSGE_ObjectTemplate *object_template, char *name, int x, int y, void *data);
SSGEDECL bool SSGE_ObjectExists(uint32_t id);
SSGEDECL bool SSGE_ObjectExistsByName(char *name);
SSGEDECL void SSGE_DrawObject(SSGE_Object *object);
SSGEDECL void SSGE_ChangeObjectTexture(SSGE_Object *object, SSGE_Texture *texture);
SSGEDECL SSGE_Object *SSGE_GetObject(uint32_t id);
SSGEDECL SSGE_Object *SSGE_GetObjectByName(char *name);
SSGEDECL void SSGE_DestroyObject(uint32_t id);
SSGEDECL void SSGE_DestroyObjectByName(char *name);
SSGEDECL void SSGE_DestroyAllObjects();

// Object template functions

SSGEDECL uint32_t SSGE_CreateObjectTemplate(char *name, SSGE_Texture *texture, int width, int height, bool hitbox, void (*destroy_data)(void *));
SSGEDECL SSGE_ObjectTemplate *SSGE_GetTemplate(uint32_t id);
SSGEDECL SSGE_ObjectTemplate *SSGE_GetTemplateByName(char *name);
SSGEDECL void SSGE_DestroyObjectTemplate(uint32_t id);
SSGEDECL void SSGE_DestroyObjectTemplateByName(char *name);
SSGEDECL void SSGE_DestroyAllTemplates();

// Hitbox functions

SSGEDECL uint32_t SSGE_CreateHitbox(char *name, int x, int y, int width, int height);
SSGEDECL bool SSGE_HitboxIsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2);

// Geometry functions

SSGEDECL void SSGE_DrawLine(int x1, int y1, int x2, int y2, SSGE_Color color);
SSGEDECL void SSGE_DrawRect(int x1, int y1, int x2, int y2, SSGE_Color color);
SSGEDECL void SSGE_DrawCircle(int x, int y, int radius, SSGE_Color color);
SSGEDECL void SSGE_DrawEllipse(int x, int y, int rx, int ry, SSGE_Color color);

SSGEDECL void SSGE_DrawLineThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness);
SSGEDECL void SSGE_DrawRectThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness);
SSGEDECL void SSGE_DrawCircleThick(int x, int y, int radius, SSGE_Color color, int thickness);
SSGEDECL void SSGE_DrawEllipseThick(int x, int y, int rx, int ry, SSGE_Color color, int thickness);

SSGEDECL void SSGE_FillRect(int x1, int y1, int x2, int y2, SSGE_Color color);
SSGEDECL void SSGE_FillCircle(int x, int y, int radius, SSGE_Color color);
SSGEDECL void SSGE_FillEllipse(int x, int y, int rx, int ry, SSGE_Color color);

SSGEDECL void SSGE_DrawGeometry(SSGE_Texture *texture, int x, int y);

SSGEDECL uint32_t SSGE_CreateLine(char *name, int x1, int y1, int x2, int y2, SSGE_Color color);
SSGEDECL uint32_t SSGE_CreateRect(char *name, int x1, int y1, int x2, int y2, SSGE_Color color);
SSGEDECL uint32_t SSGE_CreateCircle(char *name, int x, int y, int radius, SSGE_Color color);
SSGEDECL uint32_t SSGE_CreateEllipse(char *name, int x, int y, int rx, int ry, SSGE_Color color);

SSGEDECL uint32_t SSGE_CreateLineThick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness);
SSGEDECL uint32_t SSGE_CreateRectThick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness);
SSGEDECL uint32_t SSGE_CreateCircleThick(char *name, int x, int y, int radius, SSGE_Color color, int thickness);
SSGEDECL uint32_t SSGE_CreateEllipseThick(char *name, int x, int y, int rx, int ry, SSGE_Color color, int thickness);

// Utility functions

SSGEDECL void SSGE_SetColor(SSGE_Color color);
SSGEDECL void SSGE_SetBackgroundColor(SSGE_Color color);

// Event functions

SSGEDECL void SSGE_GetMousePosition(int *x, int *y);
SSGEDECL bool SSGE_AnyKeyPressed();
SSGEDECL bool SSGE_ObjectIsHovered(SSGE_Object *object);
SSGEDECL bool SSGE_ObjectIsHoveredByName(char *name);
SSGEDECL void SSGE_GetHoveredObjects(SSGE_Object *objects[], int size);
SSGEDECL void SSGE_GetHoveredObjectsIds(uint32_t ids[], int size);

// Text functions

SSGEDECL void SSGE_LoadFont(char *filename, int size, char *name);
SSGEDECL void SSGE_DrawText(char *font_name, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor);
SSGEDECL uint32_t SSGE_CreateText(char *font_name, char *text, SSGE_Color color, char *texture_name);
SSGEDECL void SSGE_CloseFont(char *name);
SSGEDECL void SSGE_CloseAllFonts();

// Audio functions

SSGEDECL uint32_t SSGE_LoadAudio(char *filename, char *name);
SSGEDECL SSGE_Audio *SSGE_GetAudio(uint32_t id);
SSGEDECL SSGE_Audio *SSGE_GetAudioByName(char *name);
SSGEDECL void SSGE_PlayAudio(SSGE_Audio *audio, int channel);
SSGEDECL void SSGE_PlayAudioByName(char *name, int channel);
SSGEDECL void SSGE_PauseAudio(int channel);
SSGEDECL void SSGE_StopAudio(int channel);
SSGEDECL void SSGE_CloseAudio(uint32_t id);
SSGEDECL void SSGE_CloseAudioByName(char *name);
SSGEDECL void SSGE_CloseAllAudios();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_H__