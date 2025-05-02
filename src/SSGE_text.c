#include <stdio.h>

#define SSGE_WANT_SDL2

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
SSGEDECL void SSGE_LoadFont(char *filename, int size, char *name) {
    _assert_engine_init();
    SSGE_Font *font = (SSGE_Font *)malloc(sizeof(SSGE_Font));
    if (font == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for font\n");
        exit(1);
    }

    font->font = TTF_OpenFont(filename, size);
    if (font->font == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    font->name = (char *)malloc(sizeof(char) * strlen(filename) + 1);
    if (font->name == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for font name\n");
        exit(1);
    }
    strcpy(font->name, name);

    SSGE_Array_Add(_font_list, font);
}

static bool _find_font_name(void *ptr, void *name) {
    return strcmp(((SSGE_Font *)ptr)->name, (char *)name) == 0 ? 1 : 0;
}

static SSGE_Font *_get_font(char *name) {
    SSGE_Font *ptr = SSGE_Array_Find(_font_list, _find_font_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Font not found: %s\n", name);
        exit(1);
    }
    return ptr;
}

/**
 * Draws text
 * \param fontName The name of the font
 * \param text The text to draw
 * \param x The x position to draw the text
 * \param y The y position to draw the text
 * \param color The color of the text
 * \param anchor The anchor of the text
 */
SSGEDECL void SSGE_DrawText(char *fontName, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor) {
    _assert_engine_init();

    if (color.a == 0) return;

    SSGE_Font *font_struct = _get_font(fontName);
    SDL_Surface *surface = TTF_RenderText_Solid(font_struct->font, text, *(SDL_Color *)&color);
    if (surface == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to draw text: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_engine->renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to draw text: %s\n", SDL_GetError());
        exit(1);
    }

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

    SDL_RenderCopy(_engine->renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

/**
 * Creates text as a texture
 * \param fontName The name of the font
 * \param text The text to draw
 * \param color The color of the text
 * \param textureName The name of the texture
 */
SSGEDECL uint32_t SSGE_CreateText(char *fontName, char *text, SSGE_Color color, char *textureName) {
    _assert_engine_init();
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for texture\n");
        exit(1);
    }

    if (color.a != 0) {
        SSGE_Font *font_struct = _get_font(fontName);
        SDL_Surface *surface = TTF_RenderText_Solid(font_struct->font, text, *(SDL_Color *)&color);
        if (surface == NULL) {
            fprintf(stderr, "[SSGE][CORE] Failed to render text: %s\n", TTF_GetError());
            exit(1);
        }

        texture->texture = SDL_CreateTextureFromSurface(_engine->renderer, surface);
        if (texture->texture == NULL) {
            fprintf(stderr, "[SSGE][CORE] Failed to create texture from surface: %s\n", SDL_GetError());
            exit(1);
        }

        SDL_FreeSurface(surface);
    } else {
        texture->texture = SDL_CreateTexture(_engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine->width, _engine->height);
    }

    return _add_texture_to_list(texture, textureName);
}

/**
 * Closes a font by name
 * \param fontName The name of the font
 */
SSGEDECL void SSGE_CloseFont(char *name) {
    _assert_engine_init();
    SSGE_Font *font = SSGE_Array_FindPop(_font_list, _find_font_name, name);
    if (font == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Font not found: %s\n", name);
        exit(1);
    }
    TTF_CloseFont(font->font);
    free(font->name);
    free(font);
}

/**
 * Closes all fonts
 */
SSGEDECL void SSGE_CloseAllFonts() {
    _assert_engine_init();
    SSGE_Array_Destroy(_font_list, _destroy_font);
    _font_list = SSGE_Array_Create();
}