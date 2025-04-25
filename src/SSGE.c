#include "SSGE.h"

static SSGE_Engine *_engine = NULL;
static Uint32 _object_count = 0;
static SSGE_ObjectList *_object_list = NULL;
static Uint32 _object_template_count = 0;
static SSGE_ObjectTemplateList *_object_template_list = NULL;
static Uint32 _texture_count = 0;
static SSGE_TextureList *_texture_list = NULL;
static Uint32 _audio_count = 0;
static SSGE_Audiolist *_audio_list = NULL;
static SSGE_Font *_font = NULL;
static SSGE_Event _event;
static SSGE_Color _color = {0, 0, 0, 255};
static SSGE_Color _clear_color = {0, 0, 0, 255};
static bool _manual_update_frame = false;
static bool _update_frame = true; // set to true to draw the first frame

static void _assert_engine_init() {
    if (_engine == NULL) {
        fprintf(stderr, "[ENGINE] Engine not initialized\n");
        exit(1);
    }
}

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
SSGEDECL void SSGE_engine_init(const char *title, int width, int height, int fps) {
    if (_engine != NULL) {
        fprintf(stderr, "[ENGINE] Engine already initialized\n");
        exit(1);
    }

    _engine = (SSGE_Engine *)malloc(sizeof(SSGE_Engine));
    if (_engine == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for engine\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[ENGINE] Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "[ENGINE] Failed to initialize TTF: %s\n", TTF_GetError());
        exit(1);
    }

    _engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (_engine->window == NULL) {
        fprintf(stderr, "[ENGINE] Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    _engine->renderer = SDL_CreateRenderer(_engine->window, -1, SDL_RENDERER_ACCELERATED);
    if (_engine->renderer == NULL) {
        fprintf(stderr, "[ENGINE] Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_SetRenderDrawBlendMode(_engine->renderer, SDL_BLENDMODE_BLEND) != 0) {
        fprintf(stderr, "[ENGINE] Failed to set render draw blend mode: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_Init(MIX_INIT_MP3 || MIX_INIT_OGG || MIX_INIT_WAVPACK) == 0) {
        fprintf(stderr, "[ENGINE] Failed to initialize mixer: %s\n", Mix_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        fprintf(stderr, "[ENGINE] Failed to open audio device for playback: %s\n", Mix_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(_engine->renderer, 0, 0, 0, 255);

    _engine->isRunning = true;
    _engine->width = width;
    _engine->height = height;
    _engine->fps = fps;
}

/**
 * Quits the engine
 * \warning This function DOES NOT free the memory allocated for objects, object templates, and textures
 * \warning You must free them manually, using the destroy functions
 * \note This function must be called at the end of the program
 */
SSGEDECL void SSGE_engine_quit() {
    _assert_engine_init();
    SDL_DestroyRenderer(_engine->renderer);
    SDL_DestroyWindow(_engine->window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    free(_engine);
}

/**
 * Runs the engine
 * \param update The update function. Should take a `Game *` as argument and return `void`
 * \param draw The draw function. Should takes a `Game *` as argument and returns `void`
 * \param event_handler The event handler function. Should takes a `SSGE_Event` and a `Game *` as arguments and returns `void`
 * \param data The `Game *` to pass to the functions (update, draw, event_handler)
 * \warning The engine runs in an infinite loop until the window is closed
 * \note The order of execution is as follows: Event handling, Update, (Clear screen), Draw
 */
SSGEDECL void SSGE_engine_run(void (*update)(Game *), void (*draw)(Game *), void (*event_handler)(SSGE_Event, Game *), Game *data) {
    _assert_engine_init();

    Uint32 frameStart;
    int frameTime;

    while (_engine->isRunning) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent((SDL_Event *)&_event)) {
            if (_event.type == SDL_QUIT) {
                _engine->isRunning = false;
            }
            if (event_handler) event_handler(_event, data);
        }

        if (update) update(data);
        if (_update_frame || !_manual_update_frame) {
            SDL_SetRenderDrawColor(_engine->renderer, _clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
            SDL_RenderClear(_engine->renderer);
            SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
            if (draw) draw(data);
            _update_frame = false;
        }

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
 * Sets the window title
 * \param title The title of the window
 */
SSGEDECL void SSGE_set_window_title(char *title) {
    _assert_engine_init();
    SDL_SetWindowTitle(_engine->window, title);
}

/**
 * Sets the window icon
 * \param filename The path to the icon
 */
SSGEDECL void SSGE_set_window_icon(char *filename) {
    _assert_engine_init();
    SDL_Surface *icon = IMG_Load(filename);
    if (icon == NULL) {
        fprintf(stderr, "[ENGINE] Failed to load icon: %s\n", IMG_GetError());
        exit(1);
    }
    SDL_SetWindowIcon(_engine->window, icon);
    SDL_FreeSurface(icon);
}

/**
 * Sets the window as resizable
 * \param resizable True if the window should be resizable, false otherwise
 */
SSGEDECL void SSGE_window_resizable(bool resizable) {
    _assert_engine_init();
    SDL_SetWindowResizable(_engine->window, resizable ? SDL_TRUE : SDL_FALSE);
}

/**
 * Sets the window as fullscreen
 * \param fullscreen True if the window should be fullscreen, false otherwise
 */
SSGEDECL void SSGE_window_fullscreen(bool fullscreen) {
    _assert_engine_init();
    SDL_SetWindowFullscreen(_engine->window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

/**
 * Sets the manual update mode
 * \param manual_update True if the manual update mode should be enabled, false otherwise
 * \note This function should be called before the `SSGE_engine_run` function
 * \note When the manual update mode is enabled, the screen will only be cleared and updated when the `SSGE_manual_update` function is called.
 * \note Setting the manual update mode may be more efficient when the screen does not need to be updated every frame
 */
SSGEDECL void SSGE_set_manual_update(bool manual_update) {
    _assert_engine_init();
    _manual_update_frame = manual_update;
}

/**
 * Manually updates the screen
 * \note This function should be called when the manual update mode is enabled
 * \note It does nothing if the manual update mode is disabled
 */
SSGEDECL void SSGE_manual_update() {
    if (_manual_update_frame) {
        _update_frame = true;
    }
}

/***********************************************
 * Texture functions
 ***********************************************/

/**
 * Adds a texture to the texture list
 * \param texture The texture to add
 * \param name The name of the texture
 */
static void _add_texture_to_list(SSGE_Texture *texture, char *name) {
    char *texture_name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (texture_name == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for texture name\n");
        exit(1);
    }
    strcpy(texture_name, name);

    SSGE_TextureList *texture_list_item = (SSGE_TextureList *)malloc(sizeof(SSGE_TextureList));
    if (texture_list_item == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for texture list item\n");
        exit(1);
    }

    texture_list_item->id = ++_texture_count;
    texture_list_item->texture = texture;
    texture_list_item->name = texture_name;
    texture_list_item->next = NULL;

    if (_texture_list == NULL) {
        _texture_list = texture_list_item;
    } else {
        SSGE_TextureList *current = _texture_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = texture_list_item;
    }
}

/**
 * Creates a texture
 * \param filename The path to the texture
 * \param name The name of the texture
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL Uint32 SSGE_load_texture(char *filename, char *name) {
    _assert_engine_init();

    // Load texture
    SSGE_Texture *texture = IMG_LoadTexture(_engine->renderer, filename);
    if (texture == NULL) {
        fprintf(stderr, "[ENGINE] Failed to load image: %s\n", IMG_GetError());
        exit(1);
    }

    // Add texture to texture list
    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Gets a texture by id
 * \param id The id of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_get_texture(Uint32 id) {
    _assert_engine_init();
    SSGE_TextureList *current = _texture_list;
    while (current != NULL) {
        if (current->id == id) {
            return current->texture;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Texture not found: %d\n", id);
    exit(1);
}

/**
 * Gets a texture by name
 * \param name The name of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_get_texture_by_name(char *name) {
    _assert_engine_init();
    SSGE_TextureList *current = _texture_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->texture;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Texture not found: %s\n", name);
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
SSGEDECL void SSGE_draw_texture(SSGE_Texture *texture, int x, int y, int width, int height) {
    _assert_engine_init();
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);
}

/**
 * Draws a texture with more options
 * \param texture The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 * \param angle The angle to rotate the texture, can be NULL
 * \param center The center of the rotation, can be NULL
 * \param flip The flip of the texture
 */
SSGEDECL void SSGE_draw_texture_ex(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip) {
    _assert_engine_init();
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopyEx(_engine->renderer, texture, NULL, &rect, angle, center, (SDL_RendererFlip)flip);
}

/**
 * Draws a texture
 * \param filename The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_draw_texture_from_path(char *filename, int x, int y, int width, int height) {
    _assert_engine_init();
    SSGE_Texture *texture = IMG_LoadTexture(_engine->renderer, filename);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

/**
 * Destroys a texture
 * \param id The id of the texture
 */
SSGEDECL void SSGE_destroy_texture(Uint32 id) {
    _assert_engine_init();
    SSGE_TextureList *current = _texture_list;
    SSGE_TextureList *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                _texture_list = current->next;
            } else {
                prev->next = current->next;
            }
            SDL_DestroyTexture(current->texture);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Texture not found: %d\n", id);
    exit(1);
}

/**
 * Destroys a texture
 * \param name The name of the texture
 */
SSGEDECL void SSGE_destroy_texture_by_name(char *name) {
    _assert_engine_init();
    SSGE_TextureList *current = _texture_list;
    SSGE_TextureList *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _texture_list = current->next;
            } else {
                prev->next = current->next;
            }
            SDL_DestroyTexture(current->texture);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Texture not found: %s\n", name);
    exit(1);
}

/**
 * Destroys all texture
 */
SSGEDECL void SSGE_destroy_all_textures() {
    _assert_engine_init();
    SSGE_TextureList *current = _texture_list;
    while (current != NULL) {
        SSGE_TextureList *next = current->next;
        SDL_DestroyTexture(current->texture);
        free(current->name);
        free(current);
        current = next;
    }
    _texture_list = NULL;
}

/***********************************************
 * Tilemap functions
 ***********************************************/

/**
 * Creates a tilemap
 * \param filename The path to the tilemap
 * \param tile_width The width of the tile
 * \param tile_height The height of the tile
 * \param spacing The spacing between tiles
 * \param nb_rows The number of rows in the tilemap
 * \param nb_cols The number of columns in the tilemap
 * \return The tilemap
 */
SSGEDECL SSGE_Tilemap *SSGE_load_tilemap(char *filename, int tile_width, int tile_height, int spacing, int nb_rows, int nb_cols) {
    _assert_engine_init();
    SSGE_Tilemap *tilemap = (SSGE_Tilemap *)malloc(sizeof(SSGE_Tilemap));
    if (tilemap == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for tilemap\n");
        exit(1);
    }

    tilemap->texture = IMG_LoadTexture(_engine->renderer, filename);
    if (tilemap->texture == NULL) {
        fprintf(stderr, "[ENGINE] Failed to load tilemap: %s\n", IMG_GetError());
        exit(1);
    }

    tilemap->tile_width = tile_width;
    tilemap->tile_height = tile_height;
    tilemap->spacing = spacing;
    tilemap->nb_rows = nb_rows;
    tilemap->nb_cols = nb_cols;

    return tilemap;
}

/**
 * Gets a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param tile_row The row of the tile
 * \param tile_col The column of the tile
 * \return The tile
 * \note The tile must be destroyed after use
 */
SSGEDECL SSGE_Tile *SSGE_get_tile(SSGE_Tilemap *tilemap, int tile_row, int tile_col) {
    _assert_engine_init();
    if (tile_row >= tilemap->nb_rows || tile_col >= tilemap->nb_cols) {
        fprintf(stderr, "[ENGINE] Tile out of bounds\n");
        exit(1);
    }

    SSGE_Tile *tile = (SSGE_Tile *)malloc(sizeof(SSGE_Tile));
    if (tile == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for tile\n");
        exit(1);
    }

    tile->tilemap = tilemap;
    tile->row = tile_row;
    tile->col = tile_col;

    return tile;
}

/**
 * Gets a texture from a tilemap
 * \param name The name of the texture
 * \param tilemap The tilemap to use
 * \param tile_row The row of the tile
 * \param tile_col The column of the tile
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_get_tile_as_texture(char *name, SSGE_Tilemap *tilemap, int tile_row, int tile_col) {
    _assert_engine_init();
    SSGE_Tile *tile = SSGE_get_tile(tilemap, tile_row, tile_col);
    SSGE_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilemap->tile_width, tilemap->tile_height);
    SDL_SetRenderTarget(_engine->renderer, texture);
    SSGE_draw_tile(tile, 0, 0);
    SDL_SetRenderTarget(_engine->renderer, NULL);
    SSGE_destroy_tile(tile);

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Draws a tile
 * \param tile The tile to draw
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 */
SSGEDECL void SSGE_draw_tile(SSGE_Tile *tile, int x, int y) {
    _assert_engine_init();
    SDL_Rect src = {tile->col * (tile->tilemap->tile_width + tile->tilemap->spacing), tile->row * (tile->tilemap->tile_height + tile->tilemap->spacing), tile->tilemap->tile_width, tile->tilemap->tile_height};
    SDL_Rect dest = {x, y, tile->tilemap->tile_width, tile->tilemap->tile_height};
    SDL_RenderCopy(_engine->renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Draws a tile with the specified width and height
 * \param tile The tile to draw
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 * \param width The width of the tile
 * \param height The height of the tile
 */
SSGEDECL void SSGE_draw_tile_with_size(SSGE_Tile *tile, int x, int y, int width, int height) {
    _assert_engine_init();
    SDL_Rect src = {tile->col * (tile->tilemap->tile_width + tile->tilemap->spacing), tile->row * (tile->tilemap->tile_height + tile->tilemap->spacing), tile->tilemap->tile_width, tile->tilemap->tile_height};
    SDL_Rect dest = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Draws a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param tile_row The row of the tile
 * \param tile_col The column of the tile
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 */
SSGEDECL void SSGE_draw_tile_from_tilemap(SSGE_Tilemap *tilemap, int tile_row, int tile_col, int x, int y) {
    _assert_engine_init();
    if (tile_row >= tilemap->nb_rows || tile_col >= tilemap->nb_cols) {
        fprintf(stderr, "[ENGINE] Tile out of bounds\n");
        exit(1);
    }

    SDL_Rect src = {tile_col * (tilemap->tile_width + tilemap->spacing), tile_row * (tilemap->tile_height + tilemap->spacing), tilemap->tile_width, tilemap->tile_height};
    SDL_Rect dest = {x, y, tilemap->tile_width, tilemap->tile_height};
    SDL_RenderCopy(_engine->renderer, tilemap->texture, &src, &dest);
}

/**
 * Destroys a tile
 * \param tile The tile to destroy
 * \note This function does not destroy the tilemap
 */
SSGEDECL void SSGE_destroy_tile(SSGE_Tile *tile) {
    free(tile);
}

/**
 * Destroys a tilemap
 * \param tilemap The tilemap to destroy
 */
SSGEDECL void SSGE_destroy_tilemap(SSGE_Tilemap *tilemap) {
    _assert_engine_init();
    SDL_DestroyTexture(tilemap->texture);
    free(tilemap);
}

/***********************************************
 * Object functions
 ***********************************************/

/**
 * Adds an object to the object list
 * \param object The object to add
 * \param name The name of the object
 */
static void _add_object_to_list(SSGE_Object *object, char *name) {
    char *obj_name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (obj_name == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object name\n");
        exit(1);
    }
    strcpy(obj_name, name);

    SSGE_ObjectList *object_list_item = (SSGE_ObjectList *)malloc(sizeof(SSGE_ObjectList));
    if (object_list_item == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object list item\n");
        exit(1);
    }

    object_list_item->id = ++_object_count;
    object_list_item->object = object;
    object_list_item->name = obj_name;
    object_list_item->next = NULL;

    if (_object_list == NULL) {
        _object_list = object_list_item;
    } else {
        SSGE_ObjectList *current = _object_list;
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
 * \param hitbox True if the object has a hitbox, false otherwise
 * \param data The data of the object
 * \param destroy_data The function to destroy the data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL Uint32 SSGE_create_object(char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroy_data)(void *)) {
    _assert_engine_init();
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object\n");
        exit(1);
    }

    object->texture = texture;
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->hitbox = hitbox;
    object->data = data;
    object->destroy_data = destroy_data;

    _add_object_to_list(object, name);

    return _object_count;
}

/**
 * Instantiates an object from an object template
 * \param object_template The object template to instantiate
 * \param x The x position of the object
 * \param y The y position of the object
 * \param data The data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL Uint32 SSGE_instantiate_object(SSGE_ObjectTemplate *object_template, char *name, int x, int y, void *data) {
    _assert_engine_init();
    return SSGE_create_object(name, object_template->texture, x, y, object_template->width, object_template->height, object_template->hitbox, data, object_template->destroy_data);
}

/**
 * Checks if an object exists
 * \param id The id of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_object_exists(Uint32 id) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        if (current->id == id) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * Checks if an object exists
 * \param name The name of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_object_exists_by_name(char *name) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * Draws an object
 * \param object The object to draw
 */
SSGEDECL void SSGE_draw_object(SSGE_Object *object) {
    _assert_engine_init();
    SDL_Rect rect = {object->x, object->y, object->width, object->height};
    SDL_RenderCopy(_engine->renderer, object->texture, NULL, &rect);
}

/**
 * Changes the texture of an object
 * \param object The object to change the texture of
 * \param texture The new texture of the object
 */
SSGEDECL void SSGE_change_object_texture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init();
    object->texture = texture;
}

/**
 * Gets an object by id
 * \param id The id of the object
 * \return The object
 */
SSGEDECL SSGE_Object *SSGE_get_object(Uint32 id) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        if (current->id == id) {
            return current->object;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Object not found: %d\n", id);
    exit(1);
}

/**
 * Gets an object by name
 * \param name The name of the object
 * \return The object with the given name
 */
SSGEDECL SSGE_Object *SSGE_get_object_by_name(char *name) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->object;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Object not found: %s\n", name);
    exit(1);
}

/**
 * Destroys an object by id
 * \param id The id of the object
 */
SSGEDECL void SSGE_destroy_object(Uint32 id) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    SSGE_ObjectList *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                _object_list = current->next;
            } else {
                prev->next = current->next;
            }
            // Destroy the data bind to the object
            if (current->object->destroy_data != NULL)
                current->object->destroy_data(current->object->data);
            free(current->object);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Object not found: %d\n", id);
    exit(1);
}

/**
 * Destroys all objects with a given name
 * \param name The name of the object
 */
SSGEDECL void SSGE_destroy_object_by_name(char *name) {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    SSGE_ObjectList *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _object_list = current->next;
            } else {
                prev->next = current->next;
            }
            // Destroy the data bind to the object
            if (current->object->destroy_data != NULL)
                current->object->destroy_data(current->object->data);
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
SSGEDECL void SSGE_destroy_all_objects() {
    _assert_engine_init();
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        SSGE_ObjectList *next = current->next;
        // Destroy the data bind to the object
        if (current->object->destroy_data != NULL)
            current->object->destroy_data(current->object->data);
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
static void _add_object_template_to_list(SSGE_ObjectTemplate *template, char *name) {
    char *objt_name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (objt_name == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object template name\n");
        exit(1);
    }
    strcpy(objt_name, name);

    SSGE_ObjectTemplateList *object_template_list_item = (SSGE_ObjectTemplateList *)malloc(sizeof(SSGE_ObjectTemplateList));
    if (object_template_list_item == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object template list item\n");
        exit(1);
    }

    object_template_list_item->id = ++_object_template_count;
    object_template_list_item->object_template = template;
    object_template_list_item->name = objt_name;
    object_template_list_item->next = NULL;

    if (_object_template_list == NULL) {
        _object_template_list = object_template_list_item;
    } else {
        SSGE_ObjectTemplateList *current = _object_template_list;
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
 * \param hitbox True if objects created from this template have a hitbox, false otherwise
 * \param destroy_data The function to destroy the data of the object
 * \return The object template id
 * \note The object template is stored internally and can be accessed by its name or its id
 */
SSGEDECL Uint32 SSGE_create_object_template(char *name, SSGE_Texture *texture, int width, int height, bool hitbox, void (*destroy_data)(void *)) {
    _assert_engine_init();
    SSGE_ObjectTemplate *object_template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (object_template == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for object template\n");
        exit(1);
    }
    object_template->texture = texture;
    object_template->width = width;
    object_template->height = height;
    object_template->hitbox = hitbox;
    object_template->destroy_data = destroy_data;

    // Add object template to object template list
    _add_object_template_to_list(object_template, name);

    return _object_template_count;
}

/**
 * Gets an object template by id
 * \param id The id of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_get_template(Uint32 id) {
    _assert_engine_init();
    SSGE_ObjectTemplateList *current = _object_template_list;
    while (current != NULL) {
        if (current->id == id) {
            return current->object_template;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Object template not found: %d\n", id);
    exit(1);
}

/**
 * Gets an object template by name
 * \param name The name of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_get_template_by_name(char *name) {
    _assert_engine_init();
    SSGE_ObjectTemplateList *current = _object_template_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->object_template;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Object template not found: %s\n", name);
    exit(1);
}

/**
 * Destroys an object template by id
 * \param id The id of the object template
 */
SSGEDECL void SSGE_destroy_object_template(Uint32 id) {
    _assert_engine_init();
    SSGE_ObjectTemplateList *current = _object_template_list;
    SSGE_ObjectTemplateList *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
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
    fprintf(stderr, "[ENGINE] Object template not found: %d\n", id);
    exit(1);
}

/**
 * Destroys an object template by name
 * \param name The name of the object template
 */
SSGEDECL void SSGE_destroy_object_template_by_name(char *name) {
    _assert_engine_init();
    SSGE_ObjectTemplateList *current = _object_template_list;
    SSGE_ObjectTemplateList *prev = NULL;
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
SSGEDECL void SSGE_destroy_all_templates() {
    _assert_engine_init();
    SSGE_ObjectTemplateList *current = _object_template_list;
    while (current != NULL) {
        SSGE_ObjectTemplateList *next = current->next;
        free(current->object_template);
        free(current->name);
        free(current);
        current = next;
    }
    _object_template_list = NULL;
}

/***********************************************
 * Hitbox functions
 ***********************************************/

/**
 * Creates a hitbox
 * \param name The name of the hitbox
 * \param x The x position of the hitbox
 * \param y The y position of the hitbox
 * \param width The width of the hitbox
 * \param height The height of the hitbox
 * \return The hitbox id
 * \note The hitbox is an object with a hitbox property set to true, it does not have texture
 * \note The hitbox is stored internally as an object and can be accessed by its name or its id
 * \warning The hitbox must be destroyed at the exit of the program, as texture
 */
SSGEDECL Uint32 SSGE_create_hitbox(char *name, int x, int y, int width, int height) {
    _assert_engine_init();
    SSGE_Object *new_hitbox = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (new_hitbox == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for hitbox\n");
        exit(1);
    }
    new_hitbox->x = x;
    new_hitbox->y = y;
    new_hitbox->width = width;
    new_hitbox->height = height;
    new_hitbox->hitbox = true;

    _add_object_to_list(new_hitbox, name);

    return _object_count;
}

/**
 * Checks if a hitbox is colliding with another hitbox
 * \param hitbox1 The first hitbox
 * \param hitbox2 The second hitbox
 * \return True if the hitboxes are colliding, false otherwise
 */
SSGEDECL bool SSGE_hitbox_is_colliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}

/***********************************************
 * Geometry functions
 ***********************************************/

/**
 * Draws a line
 * \param x1 The x position of the first point
 * \param y1 The y position of the first point
 * \param x2 The x position of the second point
 * \param y2 The y position of the second point
 * \param color The color of the line
 */
SSGEDECL void SSGE_draw_line(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    lineRGBA(_engine->renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a rectangle
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 * \param color The color of the rectangle
 */
SSGEDECL void SSGE_draw_rect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    rectangleRGBA(_engine->renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws an ellipse
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 */
SSGEDECL void SSGE_draw_ellipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    ellipseRGBA(_engine->renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a circle
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 */
SSGEDECL void SSGE_draw_circle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    circleRGBA(_engine->renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a line with a specified thickness
 * \param x1 The x position of the first point
 * \param y1 The y position of the first point
 * \param x2 The x position of the second point
 * \param y2 The y position of the second point
 * \param color The color of the line
 * \param thickness The thickness of the line
 */
SSGEDECL void SSGE_draw_line_thick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init();
    if (color.a == 0) return;
    thickLineRGBA(_engine->renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a rectangle with a specified thickness
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 * \param color The color of the rectangle
 * \param thickness The thickness of the rectangle
 */
SSGEDECL void SSGE_draw_rect_thick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init();
    if (color.a == 0) return;
    for (int i = 0; i < thickness; i++) {
        rectangleRGBA(_engine->renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
    }
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a circle with a specified thickness
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 * \param thickness The thickness of the circle
 */
SSGEDECL void SSGE_draw_circle_thick(int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init();
    if (color.a == 0) return;
    thickCircleRGBA(_engine->renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws an ellipse with a specified thickness
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 * \param thickness The thickness of the ellipse
 */
SSGEDECL void SSGE_draw_ellipse_thick(int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init();
    if (color.a == 0) return;
    thickEllipseRGBA(_engine->renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills a rectangle
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 */
SSGEDECL void SSGE_fill_rect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    boxRGBA(_engine->renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills a circle
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 */
SSGEDECL void SSGE_fill_circle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    filledCircleRGBA(_engine->renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills an ellipse
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 */
SSGEDECL void SSGE_fill_ellipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init();
    if (color.a == 0) return;
    filledEllipseRGBA(_engine->renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws geometry from a texture
 * \param texture The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 */
SSGEDECL void SSGE_draw_geometry(SSGE_Texture *texture, int x, int y) {
    _assert_engine_init();
    SDL_Rect rect = {x, y, _engine->width, _engine->height};
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);
    SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Create a line as a texture
 * \param name The name of the texture
 * \param x1 The x position of the first point
 * \param y1 The y position of the first point
 * \param x2 The x position of the second point
 * \param y2 The y position of the second point
 * \param color The color of the line
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_line(char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);

        lineRGBA(_engine->renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create a rectangle as a texture
 * \param name The name of the texture
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 * \param color The color of the rectangle
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_rect(char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);

        rectangleRGBA(_engine->renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create a circle as a texture
 * \param name The name of the texture
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_circle(char *name, int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);

        filledCircleRGBA(_engine->renderer, x, y, radius, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create an ellipse as a texture
 * \param name The name of the texture
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_ellipse(char *name, int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);
    
        ellipseRGBA(_engine->renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create a line with a specified thickness as a texture
 * \param name The name of the texture
 * \param x1 The x position of the first point
 * \param y1 The y position of the first point
 * \param x2 The x position of the second point
 * \param y2 The y position of the second point
 * \param color The color of the line
 * \param thickness The thickness of the line
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_line_thick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);
    
        thickLineRGBA(_engine->renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create a rectangle with a specified thickness as a texture
 * \param name The name of the texture
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 * \param color The color of the rectangle
 * \param thickness The thickness of the rectangle
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_rect_thick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);    
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);
    
        for (int i = 0; i < thickness; i++) {
            rectangleRGBA(_engine->renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
        }
    
        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create a circle with a specified thickness as a texture
 * \param name The name of the texture
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 * \param thickness The thickness of the circle
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_circle_thick(char *name, int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);
    
        thickCircleRGBA(_engine->renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    
        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/**
 * Create an ellipse with a specified thickness as a texture
 * \param name The name of the texture
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 * \param thickness The thickness of the ellipse
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name
 */
SSGEDECL Uint32 SSGE_create_ellipse_thick(char *name, int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init();
    SDL_Texture *texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine->renderer, texture);

        thickEllipseRGBA(_engine->renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);

        SDL_SetRenderTarget(_engine->renderer, NULL);
        SDL_SetRenderDrawColor(_engine->renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _add_texture_to_list(texture, name);

    return _texture_count;
}

/***********************************************
 * Utility functions
 ************************************************/

/**
 * Sets the color of the renderer
 * \param color The color to set
 */
SSGEDECL void SSGE_set_color(SSGE_Color color) {
    _assert_engine_init();
    _color = color;
    SDL_SetRenderDrawColor(_engine->renderer, color.r, color.g, color.b, color.a);
}

/**
 * Change the background color
 * \param color The color to set
 */
SSGEDECL void SSGE_set_background_color(SSGE_Color color) {
    _assert_engine_init();
    _clear_color = color;
}

/***********************************************
 * Event functions
 ***********************************************/

/**
 * Get the mouse position
 * \param x The variable to store the x position of the mouse
 * \param y The variable to store the y position of the mouse
 */
SSGEDECL void SSGE_get_mouse_position(int *x, int *y) {
    _assert_engine_init();
    SDL_GetMouseState(x, y);
}

/**
 * Checks if any key is pressed
 * \return True if any key is pressed, false otherwise
 */
SSGEDECL bool SSGE_any_key_pressed() {
    _assert_engine_init();
    return _event.type == SDL_KEYDOWN;
}

/**
 * Checks if an object is hovered
 * \param object The object to check
 * \return True if the object is hovered, false otherwise
 */
SSGEDECL bool SSGE_object_is_hovered(SSGE_Object *object) {
    _assert_engine_init();
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
}

/**
 * Checks if an object is hovered by name
 * \param name The name of the object to check
 * \return True if the object is hovered, false otherwise (or if the object does not exist)
 */
SSGEDECL bool SSGE_object_is_hovered_by_name(char *name) {
    _assert_engine_init();
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SSGE_ObjectList *current = _object_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            SSGE_Object *object = current->object;
            return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
        }
        current = current->next;
    }
    return false;
}

/**
 * Get the list of the objects that are hovered
 * \param objects The array to store the hovered objects
 * \param size The size of the array
 */
SSGEDECL void SSGE_get_hovered_objects(SSGE_Object *objects[], int size) {
    _assert_engine_init();
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SSGE_ObjectList *current = _object_list;
    int i = 0;
    while (current != NULL && i < size) {
        SSGE_Object *object = current->object;
        if (mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height) {
            objects[i++] = current->object;
        }
        current = current->next;
    }
}

/**
 * Get the list of the objects that are hovered
 * \param ids The array to store the hovered objects ids
 * \param size The size of the array
 */
SSGEDECL void SSGE_get_hovered_objects_ids(Uint32 ids[], int size) {
    _assert_engine_init();
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SSGE_ObjectList *current = _object_list;
    int i = 0;
    while (current != NULL && i < size) {
        SSGE_Object *object = current->object;
        if (mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height) {
            ids[i++] = current->id;
        }
        current = current->next;
    }
}

/***********************************************
 * Text functions
 ***********************************************/

/**
 * Loads a font
 * \param filename The path to the font
 * \param size The size of the font
 * \param name The name of the font
 */
SSGEDECL void SSGE_load_font(char *filename, int size, char *name) {
    _assert_engine_init();
    TTF_Font *font = TTF_OpenFont(filename, size);
    if (font == NULL) {
        fprintf(stderr, "[ENGINE] Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }
    char *name_alloc = (char *)malloc(sizeof(char) * strlen(filename) + 1);
    if (name_alloc == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for font filename\n");
        exit(1);
    }
    strcpy(name_alloc, name);

    SSGE_Font *font_struct = (SSGE_Font *)malloc(sizeof(SSGE_Font));
    if (font_struct == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for font\n");
        exit(1);
    }

    font_struct->name = name_alloc;
    font_struct->font = font;
    font_struct->next = NULL;

    if (_font == NULL) {
        _font = font_struct;
    } else {
        SSGE_Font *current = _font;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = font_struct;
    }
}

static SSGE_Font *_get_font(char *font_name) {
    SSGE_Font *current = _font;
    while (current != NULL) {
        if (strcmp(current->name, font_name) == 0) {
            return current;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Font not found: %s\n", font_name);
    exit(1);
}

/**
 * Draws text
 * \param font_name The name of the font
 * \param text The text to draw
 * \param x The x position to draw the text
 * \param y The y position to draw the text
 * \param color The color of the text
 * \param anchor The anchor of the text
 */
SSGEDECL void SSGE_draw_text(char *font_name, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor) {
    _assert_engine_init();

    if (color.a == 0) return;

    if (_font == NULL) {
        fprintf(stderr, "[ENGINE] Font not loaded\n");
        exit(1);
    }

    SSGE_Font *font_struct = _get_font(font_name);
    SDL_Surface *surface = TTF_RenderText_Solid(font_struct->font, text, color);
    if (surface == NULL) {
        fprintf(stderr, "[ENGINE] Failed to render text: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine->renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "[ENGINE] Failed to create texture from surface: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    switch (anchor) {
        case NW:
            break;
        case N:
            rect.x -= surface->w / 2;
            break;
        case NE:
            rect.x -= surface->w;
            break;
        case W:
            rect.y -= surface->h / 2;
            break;
        case CENTER:
            rect.x -= surface->w / 2;
            rect.y -= surface->h / 2;
            break;
        case E:
            rect.x -= surface->w;
            rect.y -= surface->h / 2;
            break;
        case SW:
            rect.y -= surface->h;
            break;
        case S:
            rect.x -= surface->w / 2;
            rect.y -= surface->h;
            break;
        case SE:
            rect.x -= surface->w;
            rect.y -= surface->h;
            break;
    }

    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

/**
 * Creates text as a texture
 * \param font_name The name of the font
 * \param text The text to draw
 * \param color The color of the text
 * \param texture_name The name of the texture
 */
SSGEDECL Uint32 SSGE_create_text_as_texture(char *font_name, char *text, SSGE_Color color, char *texture_name) {
    _assert_engine_init();
    if (_font == NULL) {
        fprintf(stderr, "[ENGINE] Font not loaded\n");
        exit(1);
    }

    SDL_Texture *texture;

    if (color.a != 0) {
        SSGE_Font *font_struct = _get_font(font_name);
        SDL_Surface *surface = TTF_RenderText_Solid(font_struct->font, text, color);
        if (surface == NULL) {
            fprintf(stderr, "[ENGINE] Failed to render text: %s\n", TTF_GetError());
            exit(1);
        }

        texture = SDL_CreateTextureFromSurface(_engine->renderer, surface);
        if (texture == NULL) {
            fprintf(stderr, "[ENGINE] Failed to create texture from surface: %s\n", SDL_GetError());
            exit(1);
        }

        SDL_FreeSurface(surface);
    } else {
        texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    }
    _add_texture_to_list(texture, texture_name);

    return _texture_count;
}

/**
 * Closes a font by name
 * \param font_name The name of the font
 */
SSGEDECL void SSGE_close_font(char *name) {
    _assert_engine_init();
    SSGE_Font *current = _font;
    SSGE_Font *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _font = current->next;
            } else {
                prev->next = current->next;
            }
            TTF_CloseFont(current->font);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Closes all fonts
 */
SSGEDECL void SSGE_close_all_fonts() {
    _assert_engine_init();
    if (_font == NULL) {
        return;
    }
    
    SSGE_Font *current = _font;
    while (current != NULL) {
        SSGE_Font *next = current->next;
        TTF_CloseFont(current->font);
        free(current->name);
        free(current);
        current = next;
    }
    _font = NULL;
}

/***********************************************
 * Audio functions
 ***********************************************/

/**
 * Adds an audio to the audio list
 * \param audio The audio to add
 * \param name The name of the audio
 */
static void _add_audio_to_list(Mix_Chunk *audio, char *name) {
    char *sound_name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (sound_name == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for audio name\n");
        exit(1);
    }
    strcpy(sound_name, name);

    SSGE_Audiolist *sound_list_item = (SSGE_Audiolist *)malloc(sizeof(SSGE_Audiolist));
    if (sound_list_item == NULL) {
        fprintf(stderr, "[ENGINE] Failed to allocate memory for audio list item\n");
        exit(1);
    }

    sound_list_item->id = ++_audio_count;
    sound_list_item->audio = audio;
    sound_list_item->name = sound_name;
    sound_list_item->next = NULL;

    if (_audio_list == NULL) {
        _audio_list = sound_list_item;
    } else {
        SSGE_Audiolist *current = _audio_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = sound_list_item;
    }
}

/**
 * Loads an audio
 * \param filename The path to the audio
 * \param name The name of the audio
 * \return The audio
 */
SSGEDECL Uint32 SSGE_load_audio(char *filename, char *name) {
    _assert_engine_init();
    SSGE_Audio *audio = Mix_LoadWAV(filename);
    if (audio == NULL) {
        fprintf(stderr, "[ENGINE] Failed to load audio: %s\n", Mix_GetError());
        exit(1);
    }

    _add_audio_to_list(audio, name);

    return _audio_count;
}

/**
 * Gets an audio by id
 * \param id The id of the audio
 * \return The audio
 */
SSGEDECL SSGE_Audio *SSGE_get_audio(Uint32 id) {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    while (current != NULL) {
        if (current->id == id) {
            return current->audio;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Audio not found: %d\n", id);
    exit(1);
}

/**
 * Gets an audio by name
 * \param name The name of the audio
 * \return The audio
 */
SSGEDECL SSGE_Audio *SSGE_get_audio_by_name(char *name) {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->audio;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Audio not found: %s\n", name);
    exit(1);
}

/**
 * Plays an audio
 * \param audio The audio to play
 * \param channel The channel to play the audio on, -1 for first free channel. Channels must be a number between 0 and 3
 */
SSGEDECL void SSGE_play_audio(SSGE_Audio *audio, int channel) {
    _assert_engine_init();
    Mix_PlayChannel(channel, audio, 0);
}

/**
 * Plays an audio byname
 * \param channel The channel to play the audio on, -1 for first free channel
 */
SSGEDECL void SSGE_play_audio_by_name(char *name, int channel) {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            Mix_PlayChannel(channel, current->audio, 0);
            return;
        }
        current = current->next;
    }
    fprintf(stderr, "[ENGINE] Audio not found: %s\n", name);
    exit(1);
}

/**
 * Pauses an audio
 * \param channel The channel to pause the audio on
 */
SSGEDECL void SSGE_pause_audio(int channel) {
    _assert_engine_init();
    Mix_Pause(channel);
}

/**
 * Stops an audio
 * \param channel The channel to stop the audio on
 */
SSGEDECL void SSGE_stop_audio(int channel) {
    _assert_engine_init();
    Mix_HaltChannel(channel);
}

/**
 * Closes an audio by id
 * \param id The id of the audio
 */
SSGEDECL void SSGE_close_audio(Uint32 id) {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    SSGE_Audiolist *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                _audio_list = current->next;
            } else {
                prev->next = current->next;
            }
            Mix_FreeChunk(current->audio);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Closes an audio by name
 * \param name The name of the audio
 */
SSGEDECL void SSGE_close_audio_by_name(char *name) {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    SSGE_Audiolist *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                _audio_list = current->next;
            } else {
                prev->next = current->next;
            }
            Mix_FreeChunk(current->audio);
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Closes all audios
 */
SSGEDECL void SSGE_close_all_audios() {
    _assert_engine_init();
    SSGE_Audiolist *current = _audio_list;
    while (current != NULL) {
        SSGE_Audiolist *next = current->next;
        Mix_FreeChunk(current->audio);
        free(current->name);
        free(current);
        current = next;
    }
    _audio_list = NULL;
}
