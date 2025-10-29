#include "SSGE_local.h"
#include "SSGE/SSGE_texture.h"

SSGEAPI SSGE_Texture *SSGE_Texture_Create(uint32_t *id, const char *name, const char *filename) {
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = IMG_LoadTexture(_engine.renderer, filename);
    if (texture == NULL) 
        SSGE_ErrorEx("Failed to load image: %s", IMG_GetError())

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_Texture_Get(uint32_t id) {
    SSGE_Texture *ptr = SSGE_Array_Get(&_textureList, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id)
    return ptr;
}

static bool _find_texture_name(void *ptr, void *name) {
    return strcmp(((SSGE_Texture *)ptr)->name, (char *)name) == 0;
}

SSGEAPI SSGE_Texture *SSGE_Texture_GetName(const char *name) {
    SSGE_Texture *ptr = (SSGE_Texture *)SSGE_Array_Find(&_textureList, _find_texture_name, (void *)name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name)
    return ptr;
}

SSGEAPI void SSGE_Texture_Anchor(SSGE_Texture *texture, int x, int y) {
    texture->anchorX = x;
    texture->anchorY = y;
}

SSGEAPI void SSGE_Texture_Draw(SSGE_Texture *texture, int x, int y, int width, int height) {
    _SSGE_RenderData *renderData = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData));
    *renderData = (_SSGE_RenderData){
        .dest = {
            .x = x,
            .y = y,
            .w = width,
            .h = height
        },
        .once = true
    };
    SSGE_Array_Add(&texture->queue, renderData);
}

SSGEAPI void SSGE_Texture_DrawEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point center, SSGE_Flip flip) {
    _SSGE_RenderData *renderData = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData));
    *renderData = (_SSGE_RenderData){
        .dest = {
            .x = x,
            .y = y,
            .w = width,
            .h = height
        },
        .once = true,
        .angle = angle,
        .rotationCenter = center,
        .flip = flip
    };
    SSGE_Array_Add(&texture->queue, renderData);
}

SSGEAPI void SSGE_Texture_Destroy(uint32_t id) {
    SSGE_Texture *texture = SSGE_Array_Pop(&_textureList, id);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %u", id)
    destroyTexture(texture);
}

SSGEAPI void SSGE_Texture_DestroyName(const char *name) {
    SSGE_Texture *texture = SSGE_Array_FindPop(&_textureList, _find_texture_name, (void *)name);
    if (texture == NULL) 
        SSGE_ErrorEx("Texture not found: %s", name)
    destroyTexture(texture);
}

SSGEAPI void SSGE_Texture_DestroyAll() {
    SSGE_Array_Destroy(&_textureList, (SSGE_DestroyData)destroyTexture);
    SSGE_Array_Create(&_textureList);
}
