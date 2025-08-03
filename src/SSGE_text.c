#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_text.h"

/***********************************************
 * Text functions
 ***********************************************/

/**
 * Loads a font
 * \param filename The path to the font
 * \param size The size of the font
 * \param name The name of the font
 */
SSGEDECL void SSGE_Text_CreateFont(char *filename, int size, char *name) {
    _assert_engine_init
    SSGE_Font *font = (SSGE_Font *)malloc(sizeof(SSGE_Font));
    if (font == NULL) 
        SSGE_Error("Failed to allocate memory for font");

    font->font = TTF_OpenFont(filename, size);
    if (font->font == NULL) 
        SSGE_ErrorEx("Failed to load font: %s", TTF_GetError())

    font->name = (char *)malloc(sizeof(char) * strlen(filename) + 1);
    if (font->name == NULL) 
        SSGE_Error("Failed to allocate memory for font name");
    strcpy(font->name, name);

    SSGE_Array_Add(&_font_list, font);
}

static bool _find_font_name(void *ptr, void *name) {
    return strcmp(((SSGE_Font *)ptr)->name, (char *)name) == 0 ? 1 : 0;
}

static SSGE_Font *_get_font(char *name, char *funcname) {
    SSGE_Font *ptr = SSGE_Array_Find(&_font_list, _find_font_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][%s] Font not found: %s\n", funcname, name);
        exit(1);
    }
    return ptr;
}

/**
 * Draws text
 * \param fontName The name of the font
 * \param text The text to draw
 * \param x The x coordinate to draw the text
 * \param y The y coordinate to draw the text
 * \param color The color of the text
 * \param anchor The anchor of the text
 */
SSGEDECL void SSGE_Text_Draw(char *fontName, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor) {
    _assert_engine_init

    if (color.a == 0) return;

    SSGE_Font *font = _get_font(fontName, "SSGE_Text_Draw");
    if (font == NULL)
        SSGE_ErrorEx("Font not found: %s", fontName);

    SDL_Surface *surface = TTF_RenderText_Solid(font->font, text, *(SDL_Color *)&color);
    if (surface == NULL) 
        SSGE_ErrorEx("Failed to draw text: %s", TTF_GetError());

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine.renderer, surface);
    if (texture == NULL)
        SSGE_ErrorEx("Failed to draw text: %s", SDL_GetError());

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

/**
 * Creates text as a texture
 * \param id Where to store the texture
 * \param fontName The name of the font
 * \param text The text to draw
 * \param color The color of the text
 * \param textureName The name of the texture
 * \return The poitner of the texture
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL SSGE_Texture *SSGE_Text_Create(uint32_t *id, char *fontName, char *text, SSGE_Color color, char *textureName) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    if (color.a != 0) {
        SSGE_Font *font = _get_font(fontName, "SSGE_Text_Create");
        if (font == NULL)
            SSGE_ErrorEx("Font not found: %s", fontName);

        SDL_Surface *surface = TTF_RenderText_Solid(font->font, text, *(SDL_Color *)&color);
        if (surface == NULL) 
            SSGE_ErrorEx("Failed to render text: %s", TTF_GetError());

        texture->texture = SDL_CreateTextureFromSurface(_engine.renderer, surface);
        if (texture->texture == NULL) 
            SSGE_ErrorEx("Failed to create texture from surface: %s", SDL_GetError());

        SDL_FreeSurface(surface);
    } else {
        texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    }

    texture->anchorX = 0;
    texture->anchorY = 0;

    _add_to_list(&_texture_list, texture, textureName, id, __func__);
    return texture;
}

/**
 * Closes a font by name
 * \param fontName The name of the font
 */
SSGEDECL void SSGE_Text_CloseFont(char *name) {
    _assert_engine_init
    SSGE_Font *font = SSGE_Array_FindPop(&_font_list, _find_font_name, name);
    if (font == NULL) 
        SSGE_ErrorEx("Font not found: %s", name);
    TTF_CloseFont(font->font);
    free(font->name);
    free(font);
}

/**
 * Closes all fonts
 */
SSGEDECL void SSGE_Text_CloseAllFonts() {
    _assert_engine_init
    SSGE_Array_Destroy(&_font_list, _destroy_font);
    SSGE_Array_Create(&_font_list);
}