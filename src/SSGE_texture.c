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
SSGEDECL uint32_t SSGE_Texture_Create(char *filename, char *name) {
    _assert_engine_init

    // Load texture
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");
    texture->texture = IMG_LoadTexture(_engine.renderer, filename);
    if (texture == NULL) 
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError());

    // Add texture to texture list
    return _add_to_list(&_texture_list, texture, name, __func__);
}

/**
 * Gets a texture by id
 * \param id The id of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_Texture_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Texture *ptr = SSGE_Array_Get(&_texture_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id);
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
SSGEDECL SSGE_Texture *SSGE_Texture_GetName(char *name) {
    _assert_engine_init
    SSGE_Texture *ptr = (SSGE_Texture *)SSGE_Array_Find(&_texture_list, _find_texture_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name);
    return ptr;
}

/**
 * Draws a texture
 * \param texture The texture to draw
 * \param x The x coordinate at which the texture is drawn
 * \param y The y coordinate at which the texture is drawn
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_Texture_Draw(SSGE_Texture *texture, int x, int y, int width, int height) {
    _assert_engine_init
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
}

/**
 * Draws a texture with more options
 * \param texture The texture to draw
 * \param x The x coordinate at which the texture is drawn
 * \param y The y coordinate at which the texture is drawn
 * \param width The width of the texture
 * \param height The height of the texture
 * \param angle The angle to rotate the texture, can be NULL
 * \param center The center of the rotation, can be NULL
 * \param flip The flip of the texture
 */
SSGEDECL void SSGE_Texture_DrawEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip) {
    _assert_engine_init
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopyEx(_engine.renderer, texture->texture, NULL, &rect, angle, (SDL_Point *)&center, (SDL_RendererFlip)flip);
}

/**
 * Draws a texture from a file
 * \param filename The texture to draw
 * \param x The x coordinate at which the texture is drawn
 * \param y The y coordinate at which the texture is drawn
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_Texture_DrawFile(char *filename, int x, int y, int width, int height) {
    _assert_engine_init
    SDL_Texture *texture = IMG_LoadTexture(_engine.renderer, filename);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

/**
 * Destroys a texture
 * \param id The id of the texture
 */
SSGEDECL void SSGE_Texture_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Texture *texture = SSGE_Array_Pop(&_texture_list, id);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id);
    SDL_DestroyTexture(texture->texture);
    free(texture->name);
    free(texture);
}

/**
 * Destroys a texture
 * \param name The name of the texture
 */
SSGEDECL void SSGE_Texture_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Texture *texture = SSGE_Array_FindPop(&_texture_list, _find_texture_name, name);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name);
    SDL_DestroyTexture(texture->texture);
    free(texture->name);
    free(texture);
}

/**
 * Destroys all texture
 */
SSGEDECL void SSGE_Texture_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_texture_list, _destroy_texture);
    SSGE_Array_Create(&_texture_list);
}