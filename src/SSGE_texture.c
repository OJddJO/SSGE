#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_texture.h"

SSGEAPI SSGE_Texture *SSGE_Texture_Create(uint32_t *id, char *filename, char *name) {
    _assert_engine_init

    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")
    texture->texture = IMG_LoadTexture(_engine.renderer, filename);
    if (texture == NULL) 
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError())

    texture->anchorX = 0;
    texture->anchorY = 0;

    _add_to_list(&_texture_list, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_Texture_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Texture *ptr = SSGE_Array_Get(&_texture_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id)
    return ptr;
}

static bool _find_texture_name(void *ptr, void *name) {
    return strcmp(((SSGE_Texture *)ptr)->name, (char *)name) == 0;
}

SSGEAPI SSGE_Texture *SSGE_Texture_GetName(char *name) {
    _assert_engine_init
    SSGE_Texture *ptr = (SSGE_Texture *)SSGE_Array_Find(&_texture_list, _find_texture_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name)
    return ptr;
}

SSGEAPI void SSGE_Texture_Anchor(SSGE_Texture *texture, int x, int y) {
    _assert_engine_init
    texture->anchorX = x;
    texture->anchorY = y;
}

SSGEAPI void SSGE_Texture_Draw(SSGE_Texture *texture, int x, int y, int width, int height) {
    _assert_engine_init
    SDL_Rect rect = {x + texture->anchorX, y + texture->anchorY, width, height};
    SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
}

SSGEAPI void SSGE_Texture_DrawEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip) {
    _assert_engine_init
    SDL_Rect rect = {x + texture->anchorX, y + texture->anchorY, width, height};
    SDL_RenderCopyEx(_engine.renderer, texture->texture, NULL, &rect, angle, (SDL_Point *)&center, (SDL_RendererFlip)flip);
}

SSGEAPI void SSGE_Texture_DrawFile(char *filename, int x, int y, int width, int height) {
    _assert_engine_init
    SDL_Texture *texture = IMG_LoadTexture(_engine.renderer, filename);

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

SSGEAPI void SSGE_Texture_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Texture *texture = SSGE_Array_Pop(&_texture_list, id);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id)
    _destroy_texture(texture);
}

SSGEAPI void SSGE_Texture_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Texture *texture = SSGE_Array_FindPop(&_texture_list, _find_texture_name, name);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name)
    _destroy_texture(texture);
}

SSGEAPI void SSGE_Texture_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_texture_list, _destroy_texture);
    SSGE_Array_Create(&_texture_list);
}