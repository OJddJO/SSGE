#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_text.h"

SSGEAPI void SSGE_Font_Create(char *filename, int size, char *name) {
    _assert_engine_init
    SSGE_Font *font = (SSGE_Font *)malloc(sizeof(SSGE_Font));
    if (font == NULL) 
        SSGE_Error("Failed to allocate memory for font")

    font->font = TTF_OpenFont(filename, size);
    if (font->font == NULL) 
        SSGE_ErrorEx("Failed to load font: %s", TTF_GetError())

    font->name = (char *)malloc(sizeof(char) * strlen(filename) + 1);
    if (font->name == NULL) 
        SSGE_Error("Failed to allocate memory for font name")
    strcpy(font->name, name);

    SSGE_Array_Add(&_fontList, font);
}

inline static bool _find_font_name(void *ptr, void *name) {
    return strcmp(((SSGE_Font *)ptr)->name, (char *)name) == 0;
}

static SSGE_Font *_get_font(char *name, char *funcname) {
    SSGE_Font *ptr = SSGE_Array_Find(&_fontList, _find_font_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][%s] Font not found: %s\n", funcname, name);
        exit(1);
    }
    return ptr;
}

SSGEAPI void SSGE_Font_Close(char *name) {
    _assert_engine_init
    SSGE_Font *font = SSGE_Array_FindPop(&_fontList, _find_font_name, name);
    if (font == NULL) 
        SSGE_ErrorEx("Font not found: %s", name)
    destroyFont(font);
}

SSGEAPI void SSGE_Font_CloseAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_fontList, (_SSGE_Destroy)destroyFont);
    SSGE_Array_Create(&_fontList);
}

SSGEAPI void SSGE_Text_Draw(char *fontName, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor) {
    _assert_engine_init

    if (color.a == 0) return;

    SSGE_Font *font = _get_font(fontName, "SSGE_Text_Draw");
    if (font == NULL)
        SSGE_ErrorEx("Font not found: %s", fontName)

    SDL_Surface *surface = TTF_RenderText_Solid(font->font, text, *(SDL_Color *)&color);
    if (surface == NULL) 
        SSGE_ErrorEx("Failed to draw text: %s", TTF_GetError())

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine.renderer, surface);
    if (texture == NULL)
        SSGE_ErrorEx("Failed to draw text: %s", SDL_GetError())

    SDL_Rect rect = {x, y, surface->w, surface->h};
    switch (anchor) {
        case SSGE_NW:
            break;
        case SSGE_N:
            rect.x -= surface->w / 2;
            break;
        case SSGE_NE:
            rect.x -= surface->w;
            break;
        case SSGE_W:
            rect.y -= surface->h / 2;
            break;
        case SSGE_CENTER:
            rect.x -= surface->w / 2;
            rect.y -= surface->h / 2;
            break;
        case SSGE_E:
            rect.x -= surface->w;
            rect.y -= surface->h / 2;
            break;
        case SSGE_SW:
            rect.y -= surface->h;
            break;
        case SSGE_S:
            rect.x -= surface->w / 2;
            rect.y -= surface->h;
            break;
        case SSGE_SE:
            rect.x -= surface->w;
            rect.y -= surface->h;
            break;
    }

    SDL_RenderCopy(_engine.renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

SSGEAPI SSGE_Texture *SSGE_Text_Create(uint32_t *id, char *fontName, char *text, SSGE_Color color, char *textureName) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    if (color.a != 0) {
        SSGE_Font *font = _get_font(fontName, "SSGE_Text_Create");
        if (font == NULL)
            SSGE_ErrorEx("Font not found: %s", fontName)

        SDL_Surface *surface = TTF_RenderText_Solid(font->font, text, *(SDL_Color *)&color);
        if (surface == NULL) 
            SSGE_ErrorEx("Failed to render text: %s", TTF_GetError())

        texture->texture = SDL_CreateTextureFromSurface(_engine.renderer, surface);
        if (texture->texture == NULL) 
            SSGE_ErrorEx("Failed to create texture from surface: %s", SDL_GetError())

        SDL_FreeSurface(surface);
    } else {
        texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    }

    texture->anchorX = 0;
    texture->anchorY = 0;

    atomic_init(&texture->refCount, 1);
    atomic_init(&texture->markedForDestroy, false);

    SSGE_Array_Create(&texture->queue);

    _addToList(&_textureList, texture, textureName, id, __func__);
    return texture;
}