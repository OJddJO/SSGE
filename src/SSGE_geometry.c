#define SSGE_GET_SDL

#include "SDL2/SDL2_gfxPrimitives.h"

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_geometry.h"

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
SSGEDECL void SSGE_DrawLine(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    lineRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a rectangle
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 * \param color The color of the rectangle
 */
SSGEDECL void SSGE_DrawRect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    rectangleRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws an ellipse
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 */
SSGEDECL void SSGE_DrawEllipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    ellipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a circle
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 */
SSGEDECL void SSGE_DrawCircle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    circleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
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
SSGEDECL void SSGE_DrawLineThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickLineRGBA(_engine.renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
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
SSGEDECL void SSGE_DrawRectThick(int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    for (int i = 0; i < thickness; i++) {
        rectangleRGBA(_engine.renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
    }
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws a circle with a specified thickness
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 * \param thickness The thickness of the circle
 */
SSGEDECL void SSGE_DrawCircleThick(int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
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
SSGEDECL void SSGE_DrawEllipseThick(int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init
    if (color.a == 0) return;
    thickEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills a rectangle
 * \param x1 The x position of the point at the top-left corner of the rectangle
 * \param y1 The y position of the point at the top-left corner of the rectangle
 * \param x2 The x position of the point at the bottom-right corner of the rectangle
 * \param y2 The y position of the point at the bottom-right corner of the rectangle
 */
SSGEDECL void SSGE_FillRect(int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    boxRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills a circle
 * \param x The x position of the circle
 * \param y The y position of the circle
 * \param radius The radius of the circle
 * \param color The color of the circle
 */
SSGEDECL void SSGE_FillCircle(int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    filledCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Fills an ellipse
 * \param x The x position of the ellipse
 * \param y The y position of the ellipse
 * \param rx The x radius of the ellipse
 * \param ry The y radius of the ellipse
 * \param color The color of the ellipse
 */
SSGEDECL void SSGE_FillEllipse(int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    if (color.a == 0) return;
    filledEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
}

/**
 * Draws geometry from a texture
 * \param texture The texture to draw
 * \param x The x position to draw the texture
 * \param y The y position to draw the texture
 */
SSGEDECL void SSGE_DrawGeometry(SSGE_Texture *texture, int x, int y) {
    _assert_engine_init
    SDL_Rect rect = {x, y, _engine.width, _engine.height};
    SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
    SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
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
SSGEDECL uint32_t SSGE_CreateLine(char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

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

    return _add_to_list(&_texture_list, texture, name, __func__);
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
SSGEDECL uint32_t SSGE_CreateRect(char *name, int x1, int y1, int x2, int y2, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        rectangleRGBA(_engine.renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, __func__);
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
SSGEDECL uint32_t SSGE_CreateCircle(char *name, int x, int y, int radius, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        filledCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, __func__);
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
SSGEDECL uint32_t SSGE_CreateEllipse(char *name, int x, int y, int rx, int ry, SSGE_Color color) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        ellipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, "SSGE_CreateEllipse");
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
SSGEDECL uint32_t SSGE_CreateLineThick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        thickLineRGBA(_engine.renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, "SSGE_CreateLineThick");
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
SSGEDECL uint32_t SSGE_CreateRectThick(char *name, int x1, int y1, int x2, int y2, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        for (int i = 0; i < thickness; i++) {
            rectangleRGBA(_engine.renderer, x1 + i, y1 + i, x2 - i, y2 - i, color.r, color.g, color.b, color.a);
        }
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, "SSGE_CreateRectThick");
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
SSGEDECL uint32_t SSGE_CreateCircleThick(char *name, int x, int y, int radius, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);
    
        thickCircleRGBA(_engine.renderer, x, y, radius, color.r, color.g, color.b, color.a, thickness);
    
        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, "SSGE_CreateCircleThick");
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
SSGEDECL uint32_t SSGE_CreateEllipseThick(char *name, int x, int y, int rx, int ry, SSGE_Color color, int thickness) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _engine.width, _engine.height);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture name");
    if (color.a != 0) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(_engine.renderer, texture->texture);

        thickEllipseRGBA(_engine.renderer, x, y, rx, ry, color.r, color.g, color.b, color.a, thickness);

        SDL_SetRenderTarget(_engine.renderer, NULL);
        SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
    }

    return _add_to_list(&_texture_list, texture, name, "SSGE_CreateEllipseThick");
}
