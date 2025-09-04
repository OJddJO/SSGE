#include "SDL2/SDL2_gfxPrimitives.h"

#include "SSGE_local.h"
#include "SSGE/SSGE_geometry.h"

SSGEAPI void SSGE_DrawLine(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    lineRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawRect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    rectangleRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawEllipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    ellipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawCircle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    circleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawLineThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickLineRGBA(_engine.renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawRectThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    for (int i = 0; i < thickness; i++) {
        rectangleRGBA(_engine.renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
    }
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawCircleThick(int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawEllipseThick(int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_FillRect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    boxRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_FillCircle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    filledCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_FillEllipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    filledEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI void SSGE_DrawGeometry(SSGE_Texture *texture, int x, int y) {
    _assert_engine_init
    SDL_Rect rect = {x, y, _engine.width, _engine.height};
    SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

SSGEAPI SSGE_Texture *SSGE_CreateLine(uint32_t *id, char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        lineRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateRect(uint32_t *id, char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        rectangleRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateCircle(uint32_t *id, char *name, int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        filledCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateEllipse(uint32_t *id, char *name, int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        ellipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateLineThick(uint32_t *id, char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        thickLineRGBA(_engine.renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateRectThick(uint32_t *id, char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        for (int i = 0; i < thickness; i++) {
            rectangleRGBA(_engine.renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
        }
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateCircleThick(uint32_t *id, char *name, int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        thickCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}

SSGEAPI SSGE_Texture *SSGE_CreateEllipseThick(uint32_t *id, char *name, int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture")

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name")
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        thickEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    _initTextureFields(texture);

    _addToList(&_textureList, texture, name, id, __func__);
    return texture;
}
