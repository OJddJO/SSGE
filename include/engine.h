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
    SDL_Texture *texture;
    int x;
    int y;
    int width;
    int height;
    void *data;
} Object;

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
    SDL_Texture *texture;
    struct _TextureList *next;
} TextureList;

// Engine functions

void engine_init(const char *title, int width, int height, int fps);
void engine_quit();
void engine_run(void (*update)(void *), void (*draw)(void *), void (*event_handler)(SDL_Event, void *), void *game);

// Window functions

void window_resizable(bool resizable);
void window_fullscreen(bool fullscreen);

// Drawing functions

SDL_Texture *load_texture(char *filename, char *name);
SDL_Texture *get_texture_by_name(char *name);
void draw_texture(SDL_Texture *texture, int x, int y, int width, int height);
void draw_texture_from_path(char *filename, int x, int y, int width, int height);
void destroy_all_textures();

// Tilemap functions



// Object functions

Object *create_object(char *name, char *texture, int x, int y, int width, int height, void *data);
Object *instantiate_object(ObjectTemplate *object_template, char *name, int x, int y, void *data);
void draw_object(Object *object);
Object *get_object_by_id(int id);
void destroy_object_by_id(int id);
void destroy_object_by_name(char *name);
void destroy_all_objects();

// Object template functions

ObjectTemplate *create_object_template(char *name, char *texture, int width, int height);
ObjectTemplate *get_template_by_name(char *name);
void destroy_object_template(char *name);
void destroy_all_templates();

// Event handling functions

bool object_is_hovered(Object *object);
bool object_is_hovered_by_id(int id);

#endif // __ENGINE_H__