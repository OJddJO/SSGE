#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "SDL2_gfxPrimitives.h"

// Texture structure (SDL_Texture)
#define Texture SDL_Texture
// Color structure {r, g, b, a} (SDL_Color)
#define Color SDL_Color
// Point structure {x, y} (SDL_Point)
#define Point SDL_Point
/**
 * Flip enum (SDL_RendererFlip)
 * \param SDL_FLIP_NONE No flip
 * \param SDL_FLIP_HORIZONTAL Horizontal flip
 * \param SDL_FLIP_VERTICAL Vertical flip
 * \param SDL_FLIP_HORIZONTAL_AND_VERTICAL Horizontal and vertical flip
 */ 
#define Flip SDL_RendererFlip
// Audio structure (Mix_Chunk)
#define Audio Mix_Chunk

// Structure prototypes

// Game structure prototype to contain all datas
typedef struct _Game Game;

// Engine structures

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
    int isRunning;
    int width;
    int height;
    int fps;
} SSGE_Engine;

/**
 * Object template structure
 * \param texture The filename of the texture
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox If objects created from this template have a hitbox
 */
typedef struct _SSGE_ObjectTemplate {
    Texture *texture;
    int width;
    int height;
    bool hitbox;
} SSGE_ObjectTemplate;

/**
 * Object structure
 * \param texture The texture of the object
 * \param x The x position of the object
 * \param y The y position of the object
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox If the object has a hitbox
 * \param data The data of the object
 */
typedef struct _SSGE_Object {
    Texture *texture;
    int x;
    int y;
    int width;
    int height;
    bool hitbox;
    void *data;
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
    Texture *texture;
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

/**
 * Object template list structure
 * \param id The id of the object template list item
 * \param name The name of the object template
 * \param object_template The object template
 * \param next The next object template list item
 */
typedef struct _SSGE_ObjectTemplateList {
    Uint32 id;
    char *name;
    SSGE_ObjectTemplate *object_template;
    struct _SSGE_ObjectTemplateList *next;
} SSGE_ObjectTemplateList;

/**
 * Object list structure
 * \param id The id of the object list item
 * \param name The name of the object
 * \param object The object
 * \param next The next object list item
 */
typedef struct _SSGE_ObjectList {
    Uint32 id;
    char *name;
    SSGE_Object *object;
    struct _SSGE_ObjectList *next;
} SSGE_ObjectList;

/**
 * Texture list structure
 * \param id The id of the texture list item
 * \param name The name of the texture
 * \param texture The texture
 * \param next The next texture list item
 */
typedef struct _SSGE_TextureList {
    Uint32 id;
    char *name;
    Texture *texture;
    struct _SSGE_TextureList *next;
} SSGE_TextureList;

/**
 * Audio list structure
 * \param id The id of the audio list item
 * \param name The name of the audio
 * \param audio The audio
 * \param next The next audio list item
 */
typedef struct _SSGE_Audiolist {
    Uint32 id;
    char *name;
    Mix_Chunk *audio;
    struct _SSGE_Audiolist *next;
} SSGE_Audiolist;

/**
 * Font structure
 * \param name The name of the font
 * \param font The font
 * \param next The next font
 */
typedef struct _SSGE_Font {
    char *name;
    TTF_Font *font;
    struct _SSGE_Font *next;
} SSGE_Font;

typedef enum _SSGE_Anchor {
    NW,
    N,
    NE,
    W,
    CENTER,
    E,
    SW,
    S,
    SE
} SSGE_Anchor;

// SSGE_Engine functions

void SSGE_engine_init(const char *title, int width, int height, int fps);
void SSGE_engine_quit();
void SSGE_engine_run(void (*update)(Game *), void (*draw)(Game *), void (*event_handler)(SDL_Event, Game *), Game *game);

// Window functions

void SSGE_set_window_title(char *title);
void SSGE_set_window_icon(char *filename);
void SSGE_window_resizable(bool resizable);
void SSGE_window_fullscreen(bool fullscreen);
void SSGE_set_manual_update(bool manual_update);
void SSGE_manual_update();

// Texture functions

Uint32 SSGE_load_texture(char *filename, char *name);
Texture *SSGE_get_texture(Uint32 id);
Texture *SSGE_get_texture_by_name(char *name);
void SSGE_draw_texture(Texture *texture, int x, int y, int width, int height);
void SSGE_draw_texture_ex(Texture *texture, int x, int y, int width, int height, double angle, Point *center, Flip flip);
void SSGE_draw_texture_from_path(char *filename, int x, int y, int width, int height);
void SSGE_destroy_texture(Uint32 id);
void SSGE_destroy_texture_by_name(char *name);
void SSGE_destroy_all_textures();

// Tilemap functions

SSGE_Tilemap *SSGE_load_tilemap(char *filename, int tile_width, int tile_height, int spacing, int nb_rows, int nb_cols);
SSGE_Tile *SSGE_get_tile(SSGE_Tilemap *tilemap, int tile_row, int tile_col);
Uint32 SSGE_get_tile_as_texture(char *name, SSGE_Tilemap *tilemap, int tile_row, int tile_col);
void SSGE_draw_tile(SSGE_Tile *tile, int x, int y);
void SSGE_draw_tile_with_size(SSGE_Tile *tile, int x, int y, int width, int height);
void SSGE_draw_tile_from_tilemap(SSGE_Tilemap *tilemap, int tile_row, int tile_col, int x, int y);
void SSGE_destroy_tile(SSGE_Tile *tile);
void SSGE_destroy_tilemap(SSGE_Tilemap *tilemap);

// Object functions

Uint32 SSGE_create_object(char *name, Texture *texture, int x, int y, int width, int height, bool hitbox, void *data);
Uint32 SSGE_instantiate_object(SSGE_ObjectTemplate *object_template, char *name, int x, int y, void *data);
bool SSGE_object_exists(Uint32 id);
bool SSGE_object_exists_by_name(char *name);
void SSGE_draw_object(SSGE_Object *object);
void SSGE_change_object_texture(SSGE_Object *object, Texture *texture);
SSGE_Object *SSGE_get_object(Uint32 id);
SSGE_Object *SSGE_get_object_by_name(char *name);
void SSGE_destroy_object(Uint32 id);
void SSGE_destroy_object_by_name(char *name);
void SSGE_destroy_all_objects();

// Object template functions

Uint32 SSGE_create_object_template(char *name, Texture *texture, int width, int height, bool hitbox);
SSGE_ObjectTemplate *SSGE_get_template(Uint32 id);
SSGE_ObjectTemplate *SSGE_get_template_by_name(char *name);
void SSGE_destroy_object_template(Uint32 id);
void SSGE_destroy_object_template_by_name(char *name);
void SSGE_destroy_all_templates();

// Hitbox functions

SSGE_Object *SSGE_create_hitbox(char *name, int x, int y, int width, int height);
bool SSGE_hitbox_is_colliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2);

// Geometry functions

void SSGE_draw_line(int x1, int y1, int x2, int y2, Color color);
void SSGE_draw_rect(int x1, int y1, int x2, int y2, Color color);
void SSGE_draw_circle(int x, int y, int radius, Color color);
void SSGE_draw_ellipse(int x, int y, int rx, int ry, Color color);
void SSGE_draw_line_thick(int x1, int y1, int x2, int y2, Color color, int thickness);
void SSGE_draw_rect_thick(int x1, int y1, int x2, int y2, Color color, int thickness);
void SSGE_draw_circle_thick(int x, int y, int radius, Color color, int thickness);
void SSGE_draw_ellipse_thick(int x, int y, int rx, int ry, Color color, int thickness);

void SSGE_fill_rect(int x1, int y1, int x2, int y2, Color color);
void SSGE_fill_circle(int x, int y, int radius, Color color);
void SSGE_fill_ellipse(int x, int y, int rx, int ry, Color color);

void SSGE_draw_geometry(Texture *texture, int x, int y);
Uint32 SSGE_create_line(char *name, int x1, int y1, int x2, int y2, Color color);
Uint32 SSGE_create_rect(char *name, int x1, int y1, int x2, int y2, Color color);
Uint32 SSGE_create_circle(char *name, int x, int y, int radius, Color color);
Uint32 SSGE_create_ellipse(char *name, int x, int y, int rx, int ry, Color color);
Uint32 SSGE_create_line_thick(char *name, int x1, int y1, int x2, int y2, Color color, int thickness);
Uint32 SSGE_create_rect_thick(char *name, int x1, int y1, int x2, int y2, Color color, int thickness);
Uint32 SSGE_create_circle_thick(char *name, int x, int y, int radius, Color color, int thickness);
Uint32 SSGE_create_ellipse_thick(char *name, int x, int y, int rx, int ry, Color color, int thickness);

// Utility functions

void SSGE_set_color(Color color);
void SSGE_set_background_color(Color color);

// Event functions

void SSGE_get_mouse_position(int *x, int *y);
bool SSGE_any_key_pressed();
bool SSGE_object_is_hovered(SSGE_Object *object);
bool SSGE_object_is_hovered_by_name(char *name);
void SSGE_get_hovered_objects(SSGE_Object *objects[], int size);
void SSGE_get_hovered_objects_ids(Uint32 ids[], int size);

// Text functions

void SSGE_load_font(char *filename, int size, char *name);
void SSGE_draw_text(char *font_name, char *text, int x, int y, Color color, SSGE_Anchor anchor);
Uint32 SSGE_create_text_as_texture(char *font_name, char *text, Color color, char *texture_name);
void SSGE_close_font(char *name);
void SSGE_close_all_fonts();

// Audio functions

Uint32 SSGE_load_audio(char *filename, char *name);
Audio *SSGE_get_audio(Uint32 id);
Audio *SSGE_get_audio_by_name(char *name);
void SSGE_play_audio(Audio *audio, int channel);
void SSGE_play_audio_by_name(char *name, int channel);
void SSGE_pause_audio(int channel);
void SSGE_stop_audio(int channel);
void SSGE_close_audio(Uint32 id);
void SSGE_close_audio_by_name(char *name);
void SSGE_close_all_audios();

#endif // __ENGINE_H__