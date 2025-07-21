#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_texture.h"

/***********************************************
 * Texture functions
 ***********************************************/

/**
 * Creates a texture
 * \param filename The path to the texture
 * \param name The name of the texture
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_LoadTexture(char *filename, char *name) {
    _assert_engine_init();

    // Load texture
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][SSGE_LoadTexture] Failed to allocate memory for texture");
        exit(1);
    }
    texture->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (texture->name == NULL) {
        fprintf(stderr, "[SSGE][SSGE_LoadTexture] Failed to allcoate memory for texture name");
        exit(1);
    }
    strcpy(texture->name, name);
    texture->texture = IMG_LoadTexture(_engine->renderer, filename);
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][SSGE_LoadTexture] Failed to load image: %s\n", IMG_GetError());
        exit(1);
    }

    // Add texture to texture list
    return _add_texture_to_list(texture, name, "SSGE_LoadTexture");
}

/**
 * Gets a texture by id
 * \param id The id of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_GetTexture(uint32_t id) {
    _assert_engine_init();
    SSGE_Texture *ptr = SSGE_Array_Get(&_texture_list, id);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][SSGE_GetTexture] Texture not found: %u\n", id);
        exit(1);
    }
    return ptr;
}

static bool _find_texture_name(void *ptr, void *name) {
    return strcmp(((SSGE_Texture *)ptr)->name, (char *)name) == 0 ? 1 : 0;
}

/**
 * Gets a texture by name
 * \param name The name of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_GetTextureByName(char *name) {
    _assert_engine_init();
    SSGE_Texture *ptr = (SSGE_Texture *)SSGE_Array_Find(&_texture_list, _find_texture_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][SSGE_GetTextureByName] Texture not found: %s\n", name);
        exit(1);
    }
    return ptr;
}

/**
 * Draws a texture
 * \param texture The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_DrawTexture(SSGE_Texture *texture, int x, int y, int width, int height) {
    _assert_engine_init();
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, texture->texture, NULL, &rect);
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
SSGEDECL void SSGE_DrawTextureEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip) {
    _assert_engine_init();
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopyEx(_engine->renderer, texture->texture, NULL, &rect, angle, (SDL_Point *)&center, (SDL_RendererFlip)flip);
}

/**
 * Draws a texture from a file
 * \param filename The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_DrawTextureFromFile(char *filename, int x, int y, int width, int height) {
    _assert_engine_init();
    SDL_Texture *texture = IMG_LoadTexture(_engine->renderer, filename);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

/**
 * Destroys a texture
 * \param id The id of the texture
 */
SSGEDECL void SSGE_DestroyTexture(uint32_t id) {
    _assert_engine_init();
    SSGE_Texture *texture = SSGE_Array_Pop(&_texture_list, id);
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][SSGE_DestroyTexture] Texture not found: %u\n", id);
        exit(1);
    }
    SDL_DestroyTexture(texture->texture);
    free(texture->name);
    free(texture);
}

/**
 * Destroys a texture
 * \param name The name of the texture
 */
SSGEDECL void SSGE_DestroyTextureByName(char *name) {
    _assert_engine_init();
    SSGE_Texture *texture = SSGE_Array_FindPop(&_texture_list, _find_texture_name, name);
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][SSGE_DestroyTextureByName] Texture not found: %s\n", name);
        exit(1);
    }
    SDL_DestroyTexture(texture->texture);
    free(texture->name);
    free(texture);
}

/**
 * Destroys all texture
 */
SSGEDECL void SSGE_DestroyAllTextures() {
    _assert_engine_init();
    SSGE_Array_Destroy(&_texture_list, _destroy_texture);
    SSGE_Array_Create(&_texture_list);
}