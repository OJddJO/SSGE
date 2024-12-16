#include "engine.h"

Engine *engine;
int object_id = 0;
ObjectList *object_list;
ObjectTemplateList *object_template_list;
TextureList *texture_list;

/***********************************************
 * Engine functions
 ***********************************************/

/**
 * Initializes the engine
 * \param title The title of the window
 * \param width The width of the window
 * \param height The height of the window
 * \param fps The frames per second
 */
void engine_init(const char *title, int width, int height, int fps) {
    if (engine != NULL) {
        fprintf(stderr, "Engine already initialized\n");
        exit(1);
    }

    engine = (Engine *)malloc(sizeof(Engine));
    if (engine == NULL) {
        fprintf(stderr, "Failed to allocate memory for engine\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (engine->window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
    if (engine->renderer == NULL) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    engine->isRunning = true;
    engine->width = width;
    engine->height = height;
    engine->fps = fps;
}

/**
 * Quits the engine
 */
void engine_quit() {
    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    SDL_Quit();
    free(engine);
}

/**
 * Runs the engine
 * \param update The update function
 * \param draw The draw function
 * \param event_handler The event handler function
 * \param game The game data to pass to the functions (update, draw, event_handler)
 */
void engine_run(void (*update)(void *), void (*draw)(void *), void (*event_handler)(SDL_Event, void *), void *game) {
    Uint32 frameStart;
    int frameTime;

    while (engine->isRunning) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                engine->isRunning = 0;
            } else {
                event_handler(event, game);
            }
        }

        update(game);
        SDL_RenderClear(engine->renderer);
        draw(game);

        SDL_RenderPresent(engine->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000 / engine->fps) {
            SDL_Delay((1000 / engine->fps) - frameTime);
        }
    }
}

/***********************************************
 * Window functions
 ************************************************/

/**
 * Sets the window as resizable
 * \param resizable True if the window should be resizable, false otherwise
 */
void window_resizable(bool resizable) {
    SDL_SetWindowResizable(engine->window, resizable ? SDL_TRUE : SDL_FALSE);
}

/***********************************************
 * Texture functions
 ***********************************************/

/**
 * Loads a texture
 * \param texture_path The path to the texture
 * \return The texture
 * \note The texture must be destroyed after use
 */
SDL_Texture *load_texture(char *texture_path) {
    SDL_Texture *texture = IMG_LoadTexture(engine->renderer, texture_path);
    if (texture == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        exit(1);
    }

    TextureList *texture_list_item = (TextureList *)malloc(sizeof(TextureList));
    if (texture_list_item == NULL) {
        fprintf(stderr, "Failed to allocate memory for texture list item\n");
        exit(1);
    }
    texture_list_item->texture = texture;

    if (texture_list == NULL) {
        texture_list = texture_list_item;
    } else {
        TextureList *current = texture_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = texture_list_item;
    }

    return texture;
}

/**
 * Draws a texture
 * \param texture The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
void draw_texture(SDL_Texture *texture, int x, int y, int width, int height) {
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(engine->renderer, texture, NULL, &rect);
}

/**
 * Draws a texture
 * \param texture_path The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
void draw_texture_from_path(char *texture_path, int x, int y, int width, int height) {
    SDL_Texture *texture = load_texture(texture_path);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(engine->renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

/***********************************************
 * Object functions
 ***********************************************/

/**
 * Adds an object to the object list
 * \param object The object to add
 */
static void _add_object_to_list(Object *object, char *name) {
    ObjectList *object_list_item = (ObjectList *)malloc(sizeof(ObjectList));
    if (object_list_item == NULL) {
        fprintf(stderr, "Failed to allocate memory for object list item\n");
        exit(1);
    }

    object_list_item->object = object;
    object_list_item->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (object_list_item->name == NULL) {
        fprintf(stderr, "Failed to allocate memory for object name\n");
        exit(1);
    }
    strcpy(object_list_item->name, name);

    if (object_list == NULL) {
        object_list = object_list_item;
    } else {
        ObjectList *current = object_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = object_list_item;
    }
}

/**
 * Creates an object
 * \param name The name of the object
 * \param texture The texture of the object
 * \param x The x position of the object
 * \param y The y position of the object
 * \param width The width of the object
 * \param height The height of the object
 * \return The object
 */
Object *object_create(char *name, char *texture, int x, int y, int width, int height) {
    Object *object = (Object *)malloc(sizeof(Object));
    if (object == NULL) {
        fprintf(stderr, "Failed to allocate memory for object\n");
        exit(1);
    }

    object->id = object_id++;
    object->texture = IMG_LoadTexture(engine->renderer, texture);
    if (object->texture == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
        exit(1);
    }
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;

    // Add object to object list
    _add_object_to_list(object, name);

    return object;
}

/**
 * Draws an object
 * \param object The object to draw
 */
void object_draw(Object *object) {
    SDL_Rect rect = {object->x, object->y, object->width, object->height};
    SDL_RenderCopy(engine->renderer, object->texture, NULL, &rect);
}

/**
 * Destroys an object
 * \param object The object to destroy
 */
void object_destroy(Object *object) {
    SDL_DestroyTexture(object->texture);
    free(object);
}

/**
 * Adds an object template to the object template list
 * \param template The object template to add
 * \param name The name of the object template
 */
static void _add_object_template_to_list(ObjectTemplate *template, char *name) {
    ObjectTemplateList *object_template_list_item = (ObjectTemplateList *)malloc(sizeof(ObjectTemplateList));
    if (object_template_list_item == NULL) {
        fprintf(stderr, "Failed to allocate memory for object template list item\n");
        exit(1);
    }

    object_template_list_item->object_template = template;
    object_template_list_item->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (object_template_list_item->name == NULL) {
        fprintf(stderr, "Failed to allocate memory for object template name\n");
        exit(1);
    }
    strcpy(object_template_list_item->name, name);

    if (object_template_list == NULL) {
        object_template_list = object_template_list_item;
    } else {
        ObjectTemplateList *current = object_template_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = object_template_list_item;
    }
}

/**
 * Creates an object template
 * \param name The name of the object template
 * \param texture The texture of the object template
 * \param width The width of the object template
 * \param height The height of the object template
 * \return The object template
 */
ObjectTemplate *object_template_create(char *name, char *texture, int width, int height) {
    ObjectTemplate *object_template = (ObjectTemplate *)malloc(sizeof(ObjectTemplate));
    if (object_template == NULL) {
        fprintf(stderr, "Failed to allocate memory for object template\n");
        exit(1);
    }
    object_template->texture = texture;
    object_template->width = width;
    object_template->height = height;

    // Add object template to object template list
    _add_object_template_to_list(object_template, name);

    return object_template;
}

/**
 * Gets an object template by name
 * \param name The name of the object template
 * \return The object template
 */
ObjectTemplate *get_template_by_name(char *name) {
    ObjectTemplateList *current = object_template_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->object_template;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Destroys an object template
 * \param object_template The object template to destroy
 */
void object_template_destroy(ObjectTemplate *object_template) {
    if (object_template->texture) free(object_template->texture);
    free(object_template);
}

/**
 * Instantiates an object from an object template
 * \param object_template The object template to instantiate
 * \param x The x position of the object
 * \param y The y position of the object
 * \return The object
 */
Object *object_instantiate(ObjectTemplate *object_template, char *name, int x, int y) {
    return object_create(name, object_template->texture, x, y, object_template->width, object_template->height);
}

/**
 * Destroys all objects
 */
void destroy_all_objects() {
    ObjectList *current = object_list;
    while (current != NULL) {
        ObjectList *next = current->next;
        object_destroy(current->object);
        free(current);
        current = next;
    }
    object_list = NULL;
}

/***********************************************
 * Utility functions
 ***********************************************/

/**
 * Checks if an object is hovered
 * \param object The object to check
 * \return True if the object is hovered, false otherwise
 */
bool object_is_hovered(Object *object) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
}

/**
 * Checks if an object is hovered by id
 * \param id The id of the object to check
 * \return True if the object is hovered, false otherwise (or if the object does not exist)
 */
bool object_is_hovered_by_id(int id) {
    ObjectList *current = object_list;
    while (current != NULL) {
        if (current->object->id == id) {
            return object_is_hovered(current->object);
        }
        current = current->next;
    }
    return false;
}