#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define Texture SDL_Texture
#define Color SDL_Color

typedef struct _Engine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int isRunning;
    int width;
    int height;
    int fps;
} Engine;

typedef struct _ObjectTemplate {
    char *texture;
    int width;
    int height;
} ObjectTemplate;

typedef struct _Object {
    int id;
    Texture *texture;
    int x;
    int y;
    int width;
    int height;
    void *data;
} Object;

typedef struct _Tilemap {
    Texture *texture;
    int tile_width;
    int tile_height;
    int spacing;
    int nb_rows;
    int nb_cols;
} Tilemap;

typedef struct _Tile {
    Tilemap *tilemap;
    int row;
    int col;
} Tile;

typedef struct _ObjectTemplateList {
    char *name;
    ObjectTemplate *object_template;
    struct _ObjectTemplateList *next;
} ObjectTemplateList;

typedef struct _ObjectList {
    char *name;
    Object *object;
    struct _ObjectList *next;
} ObjectList;

typedef struct _TextureList {
    char *name;
    Texture *texture;
    struct _TextureList *next;
} TextureList;

typedef struct _Font {
    char *name;
    TTF_Font *font;
    struct _Font *next;
} Font;

typedef enum _Anchor {
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT
} Anchor;


// Engine functions

void engine_init(const char *title, int width, int height, int fps);
void engine_quit();
void engine_run(void (*update)(void *), void (*draw)(void *), void (*event_handler)(SDL_Event, void *), void *game);

// Window functions

void set_window_icon(char *filename);
void window_resizable(bool resizable);
void window_fullscreen(bool fullscreen);

// Texture functions

Texture *load_texture(char *filename, char *name);
Texture *get_texture_by_name(char *name);
void draw_texture(Texture *texture, int x, int y, int width, int height);
void draw_texture_from_path(char *filename, int x, int y, int width, int height);
void destroy_all_textures();
void change_draw_color(Color color);
void rotate_texture(char *name, double angle); //need to be tested

// Tilemap functions

Tilemap *create_tilemap(char *filename, int tile_width, int tile_height, int spacing, int nb_rows, int nb_cols);
Tile *get_tile(Tilemap *tilemap, int tile_row, int tile_col);
void draw_tile(Tile *tile, int x, int y);
void draw_tile_with_size(Tile *tile, int x, int y, int width, int height);
void draw_tile_from_tilemap(Tilemap *tilemap, int tile_row, int tile_col, int x, int y);
void destroy_tile(Tile *tile);
void destroy_tilemap(Tilemap *tilemap);

// Object functions

Object *create_object(char *name, char *texture, int x, int y, int width, int height, void *data);
Object *instantiate_object(ObjectTemplate *object_template, char *name, int x, int y, void *data);
bool object_exists(char *name);
void draw_object(Object *object);
Object *get_object_by_id(int id);
Object *get_object_by_name(char *name);
void destroy_object_by_id(int id);
void destroy_object_by_name(char *name);
void destroy_all_objects();

// Object template functions

ObjectTemplate *create_object_template(char *name, char *texture, int width, int height);
ObjectTemplate *get_template_by_name(char *name);
void destroy_object_template(char *name);
void destroy_all_templates();

// Hitbox functions

Object *create_hitbox(char *name, int x, int y, int width, int height);
bool hitbox_is_colliding(Object *hitbox1, Object *hitbox2);

// Geometry functions

void draw_line(int x1, int y1, int x2, int y2, Color color);
void draw_line_thick(int x1, int y1, int x2, int y2, int thickness, Color color);
void draw_rect(int x, int y, int width, int height, Color color);
void draw_rect_thick(int x, int y, int width, int height, int thickness, Color color);
void draw_circle(int x, int y, int radius, Color color);
void draw_circle_thick(int x, int y, int radius, int thickness, Color color);
void fill_rect(int x, int y, int width, int height, Color color);
void fill_circle(int x, int y, int radius, Color color);

// Utility functions

bool object_is_hovered(Object *object);
bool object_is_hovered_by_id(int id);

// Text functions

void load_font(char *filename, int size, char *name);
void draw_text(char *font_name, char *text, int x, int y, Color color, Anchor anchor);
void close_font(char *name);
void close_all_fonts();

#endif // __ENGINE_H__