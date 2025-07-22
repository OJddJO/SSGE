#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_tilemap.h"

/***********************************************
 * Tilemap functions
 ***********************************************/

/**
 * Creates a tilemap
 * \param filename The path to the tilemap
 * \param tileWidth The width of the tile
 * \param tileHeight The height of the tile
 * \param spacing The spacing between tiles
 * \param nbRows The number of rows in the tilemap
 * \param nbCols The number of columns in the tilemap
 * \return The tilemap
 */
SSGEDECL SSGE_Tilemap *SSGE_CreateTilemap(char *filename, int tileWidth, int tileHeight, int spacing, int nbRows, int nbCols) {
    _assert_engine_init
    SSGE_Tilemap *tilemap = (SSGE_Tilemap *)malloc(sizeof(SSGE_Tilemap));
    if (tilemap == NULL) 
        SSGE_Error("Failed to allocate memory for tilemap");

    tilemap->texture = IMG_LoadTexture(_engine.renderer, filename);
    if (tilemap->texture == NULL) 
        SSGE_ErrorEx("Failed to load tilemap: %s", IMG_GetError());

    tilemap->tileWidth = tileWidth;
    tilemap->tileHeight = tileHeight;
    tilemap->spacing = spacing;
    tilemap->nbRows = nbRows;
    tilemap->nbCols = nbCols;

    return tilemap;
}

/**
 * Gets a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param tileRow The row of the tile
 * \param tileCol The column of the tile
 * \return The tile
 * \note The tile must be destroyed after use
 */
SSGEDECL SSGE_Tile *SSGE_GetTile(SSGE_Tilemap *tilemap, int tileRow, int tileCol) {
    _assert_engine_init
    if (tileRow >= tilemap->nbRows || tileCol >= tilemap->nbCols) 
        SSGE_Error("Tile out of bounds");

    SSGE_Tile *tile = (SSGE_Tile *)malloc(sizeof(SSGE_Tile));
    if (tile == NULL) 
        SSGE_Error("Failed to allocate memory for tile");

    tile->tilemap = tilemap;
    tile->row = tileRow;
    tile->col = tileCol;

    return tile;
}

/**
 * Gets a texture from a tilemap
 * \param name The name of the texture
 * \param tilemap The tilemap to use
 * \param tileRow The row of the tile
 * \param tileCol The column of the tile
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_GetTileAsTexture(char *name, SSGE_Tilemap *tilemap, int tileRow, int tileCol) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilemap->tileWidth, tilemap->tileHeight);
    SDL_SetRenderTarget(_engine.renderer, texture->texture);
    SSGE_DrawTileFromTilemap(tilemap, tileRow, tileCol, 0, 0);
    SDL_SetRenderTarget(_engine.renderer, NULL);

    return _add_to_list(&_texture_list, texture, name, "SSGE_GetTileAsTexture");
}

/**
 * Draws a tile
 * \param tile The tile to draw
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 */
SSGEDECL void SSGE_DrawTile(SSGE_Tile *tile, int x, int y) {
    _assert_engine_init
    SDL_Rect src = {tile->col * (tile->tilemap->tileWidth + tile->tilemap->spacing), tile->row * (tile->tilemap->tileHeight + tile->tilemap->spacing), tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_Rect dest = {x, y, tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_RenderCopy(_engine.renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Draws a tile with the specified width and height
 * \param tile The tile to draw
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 * \param width The width of the tile
 * \param height The height of the tile
 */
SSGEDECL void SSGE_DrawTileWithSize(SSGE_Tile *tile, int x, int y, int width, int height) {
    _assert_engine_init
    SDL_Rect src = {tile->col * (tile->tilemap->tileWidth + tile->tilemap->spacing), tile->row * (tile->tilemap->tileHeight + tile->tilemap->spacing), tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_Rect dest = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Draws a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param tileRow The row of the tile
 * \param tileCol The column of the tile
 * \param x The x position to draw the tile
 * \param y The y position to draw the tile
 */
SSGEDECL void SSGE_DrawTileFromTilemap(SSGE_Tilemap *tilemap, int tileRow, int tileCol, int x, int y) {
    _assert_engine_init
    if (tileRow >= tilemap->nbRows || tileCol >= tilemap->nbCols) 
        SSGE_Error("Tile out of bounds");

    SDL_Rect src = {tileCol * (tilemap->tileWidth + tilemap->spacing), tileRow * (tilemap->tileHeight + tilemap->spacing), tilemap->tileWidth, tilemap->tileHeight};
    SDL_Rect dest = {x, y, tilemap->tileWidth, tilemap->tileHeight};
    SDL_RenderCopy(_engine.renderer, tilemap->texture, &src, &dest);
}

/**
 * Destroys a tile
 * \param tile The tile to destroy
 * \note This function does not destroy the tilemap
 */
SSGEDECL void SSGE_DestroyTile(SSGE_Tile *tile) {
    free(tile);
}

/**
 * Destroys a tilemap
 * \param tilemap The tilemap to destroy
 */
SSGEDECL void SSGE_DestroyTilemap(SSGE_Tilemap *tilemap) {
    _assert_engine_init
    SDL_DestroyTexture(tilemap->texture);
    free(tilemap);
}
