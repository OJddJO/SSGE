#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
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
    char *name;
    char *texture;
    int width;
    int height;
} ObjectTemplate;

typedef struct _ObjectTemplateList {
    ObjectTemplate *object_template;
    struct _ObjectTemplateList *next;
} ObjectTemplateList;

typedef struct _Object {
    int id;
    char *name;
    SDL_Texture *texture;
    int x;
    int y;
    int width;
    int height;
} Object;

typedef struct _ObjectList {
    Object *object;
    struct _ObjectList *next;
} ObjectList;

extern Engine *engine;

// Engine functions

void engine_init(const char *title, int width, int height, int fps);
void engine_quit();
void engine_run(void (*update)(void *), void (*draw)(void *), void (*event_handler)(SDL_Event, void *), void *game);

// Drawing functions

SDL_Texture *load_texture(char *texture_path);
void draw_texture(SDL_Texture *texture, int x, int y, int width, int height);
void draw_texture_from_path(char *texture_path, int x, int y, int width, int height);

// Object functions

Object *object_create(char *name, char *texture, int x, int y, int width, int height);
void object_draw(Object *object);
void object_destroy(Object *object);
ObjectTemplate *object_template_create(char *name, char *texture, int width, int height);
void object_template_destroy(ObjectTemplate *object_template);
ObjectTemplate *get_template_by_name(char *name);
Object *object_instantiate(ObjectTemplate *object_template, int x, int y);
void destroy_all_objects();

// Event handling functions

bool object_is_hovered(Object *object);
bool object_is_hovered_by_id(int id);

#endif // __ENGINE_H__