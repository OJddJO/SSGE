#include "engine.h"

Engine *_engine;
int _object_id = 0;
ObjectList *_object_list;
ObjectTemplateList *_object_template_list;
TextureList *_texture_list;

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
    if (_engine != NULL) {
        fprintf(stderr, "Engine already initialized\n");
        exit(1);
    }

    _engine = (Engine *)malloc(sizeof(Engine));
    if (_engine == NULL) {
        fprintf(stderr, "Failed to allocate memory for engine\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    _engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (_engine->window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    _engine->renderer = SDL_CreateRenderer(_engine->window, -1, SDL_RENDERER_ACCELERATED);
    if (_engine->renderer == NULL) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    _engine->isRunning = true;
    _engine->width = width;
    _engine->height = height;
    _engine->fps = fps;
}

/**
 * Quits the engine
 * \note This function must be called at the end of the program
 * \warning This function DOES NOT free the memory allocated for objects, object templates, and textures
 */
void engine_quit() {
    SDL_DestroyRenderer(_engine->renderer);
    SDL_DestroyWindow(_engine->window);
    SDL_Quit();
    free(_engine);
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

    while (_engine->isRunning) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                _engine->isRunning = 0;
            } else {
                event_handler(event, game);
            }
        }

        update(game);
        SDL_RenderClear(_engine->renderer);
        draw(game);

        SDL_RenderPresent(_engine->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000 / _engine->fps) {
            SDL_Delay((1000 / _engine->fps) - frameTime);
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
    SDL_SetWindowResizable(_engine->window, resizable ? SDL_TRUE : SDL_FALSE);
}

/**
 * Sets the window as fullscreen
 * \param fullscreen True if the window should be fullscreen, false otherwise
 */
void window_fullscreen(bool fullscreen) {
    SDL_SetWindowFullscreen(_engine->window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

/***********************************************
 * Texture functions
 ***********************************************/

/**
 * Loads a texture
 * \param filename The path to the texture
 * \param name The name of the texture
 * \return The texture
 * \note The texture must be destroyed after use
 */
SDL_Texture *load_texture(char *filename, char *name) {
    SDL_Texture *texture = IMG_LoadTexture(_engine->renderer, filename);
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
    texture_list_item->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (texture_list_item->name == NULL) {
        fprintf(stderr, "Failed to allocate memory for texture name\n");
        exit(1);
    }
    strcpy(texture_list_item->name, name);

    if (_texture_list == NULL) {
        _texture_list = texture_list_item;
    } else {
        TextureList *current = _texture_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = texture_list_item;
    }

    return texture;
}

/**
 * Gets a texture by name
 * \param name The name of the texture
 * \return The texture
 */
SDL_Texture *get_texture_by_name(char *name) {
    TextureList *current = _texture_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->texture;
        }
        current = current->next;
    }
    fprintf(stderr, "Texture not found: %s\n", name);
    exit(1);
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
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);
}

/**
 * Draws a texture
 * \param filename The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
void draw_texture_from_path(char *filename, int x, int y, int width, int height) {
    SDL_Texture *texture = IMG_LoadTexture(_engine->renderer, filename);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

/**
 * Destroys a texture
 * \param texture The texture to destroy
 */
void destroy_all_textures() {
    TextureList *current = _texture_list;
    while (current != NULL) {
        TextureList *next = current->next;
        SDL_DestroyTexture(current->texture);
        free(current->name);
        free(current);
        current = next;
    }
    _texture_list = NULL;
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

    if (_object_list == NULL) {
        _object_list = object_list_item;
    } else {
        ObjectList *current = _object_list;
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
 * \param data The data of the object
 * \return The object
 */
Object *create_object(char *name, char *texture, int x, int y, int width, int height, void *data) {
    Object *object = (Object *)malloc(sizeof(Object));
    if (object == NULL) {
        fprintf(stderr, "Failed to allocate memory for object\n");
        exit(1);
    }

    object->id = _object_id++;
    object->texture = IMG_LoadTexture(_engine->renderer, texture);
    if (object->texture == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
        exit(1);
    }
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->data = data;

    // Add object to object list
    _add_object_to_list(object, name);

    return object;
}

/**
 * Instantiates an object from an object template
 * \param object_template The object template to instantiate
 * \param x The x position of the object
 * \param y The y position of the object
 * \param data The data of the object
 * \return The object
 */
Object *instantiate_object(ObjectTemplate *object_template, char *name, int x, int y, void *data) {
    return create_object(name, object_template->texture, x, y, object_template->width, object_template->height, data);
}

/**
 * Draws an object
 * \param object The object to draw
 */
void draw_object(Object *object) {
    SDL_Rect rect = {object->x, object->y, object->width, object->height};
    SDL_RenderCopy(_engine->renderer, object->texture, NULL, &rect);
}

/**
 * Gets an object by id
 * \param id The id of the object
 * \return The object
 */
Object *get_object_by_id(int id) {
    ObjectList *current = _object_list;
    while (current != NULL) {
        if (current->object->id == id) {
            return current->object;
        }
        current = current->next;
    }
    fprintf(stderr, "Object with id %d not found\n", id);
    exit(1);
}

/**
 * Destroys an object by name
 * \param id The id of the object
 */
void destroy_object_by_id(int id) {
    ObjectList *current = _object_list;
    ObjectList *prev = NULL;
    while (current != NULL) {
        if (current->object->id == id) {
            if (prev == NULL) {
                _object_list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->object);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Destroys all objects with a given name
 * \param name The name of the object
 */
void destroy_object_by_name(char *name) {
    ObjectList *current = _object_list;
    ObjectList *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _object_list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->object);
            free(current->name);
            free(current);
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Destroys all objects
 */
void destroy_all_objects() {
    ObjectList *current = _object_list;
    while (current != NULL) {
        ObjectList *next = current->next;
        free(current->object);
        free(current->name);
        free(current);
        current = next;
    }
    _object_list = NULL;
}

/***********************************************
 * Object template functions
 ***********************************************/

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

    if (_object_template_list == NULL) {
        _object_template_list = object_template_list_item;
    } else {
        ObjectTemplateList *current = _object_template_list;
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
ObjectTemplate *create_object_template(char *name, char *texture, int width, int height) {
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
    ObjectTemplateList *current = _object_template_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->object_template;
        }
        current = current->next;
    }
    fprintf(stderr, "Object template not found: %s\n", name);
    exit(1);
}

/**
 * Destroys an object template by name
 * \param name The name of the object template
 */
void destroy_object_template(char *name) {
    ObjectTemplateList *current = _object_template_list;
    ObjectTemplateList *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _object_template_list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->object_template);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Destroys all object templates
 */
void destroy_all_templates() {
    ObjectTemplateList *current = _object_template_list;
    while (current != NULL) {
        ObjectTemplateList *next = current->next;
        free(current->object_template);
        free(current->name);
        free(current);
        current = next;
    }
    _object_template_list = NULL;
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
    ObjectList *current = _object_list;
    while (current != NULL) {
        if (current->object->id == id) {
            return object_is_hovered(current->object);
        }
        current = current->next;
    }
    return false;
}