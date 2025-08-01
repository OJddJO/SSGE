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
SSGEDECL SSGE_Tilemap *SSGE_Tilemap_Create(char *filename, uint32_t tileWidth, uint32_t tileHeight, uint32_t spacing, uint16_t nbRows, uint16_t nbCols) {
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
 * \param row The row of the tile
 * \param col The column of the tile
 * \return The tile, NULL if error
 * \note The tile must be destroyed after use
 */
SSGEDECL SSGE_Tile *SSGE_Tilemap_GetTile(SSGE_Tilemap *tilemap, uint16_t row, uint16_t col) {
    _assert_engine_init
    if (row >= tilemap->nbRows || col >= tilemap->nbCols)
        SSGE_ErrorEx2("Tile out of bounds: (row: %u col: %u)", row, col);

    SSGE_Tile *tile = (SSGE_Tile *)malloc(sizeof(SSGE_Tile));
    if (tile == NULL)
        SSGE_Error("Failed to allocate memory for tile");

    tile->tilemap = tilemap;
    tile->row = row;
    tile->col = col;

    return tile;
}

/**
 * Gets a texture from a tilemap
 * \param tilemap The tilemap to use
 * \param row The row of the tile
 * \param col The column of the tile
 * \param name The name of the texture
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Tilemap_GetTileAsTexture(SSGE_Tilemap *tilemap, uint16_t row, uint16_t col, char *name) {
    _assert_engine_init
    SSGE_Texture *texture = (SSGE_Texture *)malloc(sizeof(SSGE_Texture));
    if (texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    texture->texture = SDL_CreateTexture(_engine.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tilemap->tileWidth, tilemap->tileHeight);
    if (texture->texture == NULL) 
        SSGE_Error("Failed to allocate memory for texture");

    SDL_SetRenderTarget(_engine.renderer, texture->texture);
    SSGE_Tilemap_DrawTile(tilemap, row, col, 0, 0);
    SDL_SetRenderTarget(_engine.renderer, NULL);

    texture->anchorX = 0;
    texture->anchorY = 0;

    return _add_to_list(&_texture_list, texture, name, __func__);
}

/**
 * Draws a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param row The row of the tile
 * \param col The column of the tile
 * \param x The x coordinate at which the tile is drawn
 * \param y The y coordinate at which the tile is drawn
 */
SSGEDECL void SSGE_Tilemap_DrawTile(SSGE_Tilemap *tilemap, uint16_t row, uint16_t col, int x, int y) {
    _assert_engine_init
    if (row >= tilemap->nbRows || col >= tilemap->nbCols) 
        SSGE_ErrorEx2("Tile out of bounds (row: %u col: %u)", row, col);

    SDL_Rect src = {col * (tilemap->tileWidth + tilemap->spacing), row * (tilemap->tileHeight + tilemap->spacing), tilemap->tileWidth, tilemap->tileHeight};
    SDL_Rect dest = {x, y, tilemap->tileWidth, tilemap->tileHeight};
    SDL_RenderCopy(_engine.renderer, tilemap->texture, &src, &dest);
}

/**
 * Draws a tile from a tilemap
 * \param tilemap The tilemap to use
 * \param row The row of the tile
 * \param col The column of the tile
 * \param x The x coordinate at which the tile is drawn
 * \param y The y coordinate at which the tile is drawn
 * \param width The width of the drawn tile
 * \param height The height of the drawn tile
 */
SSGEDECL void SSGE_Tilemap_DrawTileSize(SSGE_Tilemap *tilemap, uint16_t row, uint16_t col, int x, int y, uint32_t width, uint32_t height) {
    _assert_engine_init
    if (row >= tilemap->nbRows || col >= tilemap->nbCols) 
        SSGE_ErrorEx2("Tile out of bounds (row: %u col: %u)", row, col);

    SDL_Rect src = {col * (tilemap->tileWidth + tilemap->spacing), row * (tilemap->tileHeight + tilemap->spacing), tilemap->tileWidth, tilemap->tileHeight};
    SDL_Rect dest = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, tilemap->texture, &src, &dest);
}

/**
 * Draws a tile
 * \param tile The tile to draw
 * \param x The x coordinate at which the tile is drawn
 * \param y The y coordinate at which the tile is drawn
 */
SSGEDECL void SSGE_Tilemap_DrawTileAlt(SSGE_Tile *tile, int x, int y) {
    _assert_engine_init
    SDL_Rect src = {tile->col * (tile->tilemap->tileWidth + tile->tilemap->spacing), tile->row * (tile->tilemap->tileHeight + tile->tilemap->spacing), tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_Rect dest = {x, y, tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_RenderCopy(_engine.renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Draws a tile with the specified width and height
 * \param tile The tile to draw
 * \param x The x coordinate at which the tile is drawn
 * \param y The y coordinate at which the tile is drawn
 * \param width The width of the drawn tile
 * \param height The height of the drawn tile
 */
SSGEDECL void SSGE_Tilemap_DrawTileSizeAlt(SSGE_Tile *tile, int x, int y, uint16_t width, uint16_t height) {
    _assert_engine_init
    SDL_Rect src = {tile->col * (tile->tilemap->tileWidth + tile->tilemap->spacing), tile->row * (tile->tilemap->tileHeight + tile->tilemap->spacing), tile->tilemap->tileWidth, tile->tilemap->tileHeight};
    SDL_Rect dest = {x, y, width, height};
    SDL_RenderCopy(_engine.renderer, tile->tilemap->texture, &src, &dest);
}

/**
 * Destroys a tile
 * \param tile The tile to destroy
 * \note This function does not destroy the tilemap
 */
SSGEDECL void SSGE_Tilemap_DestroyTile(SSGE_Tile *tile) {
    free(tile);
}

/**
 * Destroys a tilemap
 * \param tilemap The tilemap to destroy
 */
SSGEDECL void SSGE_Tilemap_Destroy(SSGE_Tilemap *tilemap) {
    _assert_engine_init
    SDL_DestroyTexture(tilemap->texture);
    free(tilemap);
}
