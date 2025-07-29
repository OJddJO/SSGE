#ifndef __SSGE_TEXTURE_H__
#define __SSGE_TEXTURE_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a texture
 * \param filename The path to the texture
 * \param name The name of the texture
 * \return The texture id
 * \note The texture is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Texture_Create(char *filename, char *name);

/**
 * Gets a texture by id
 * \param id The id of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_Texture_Get(uint32_t id);

/**
 * Gets a texture by name
 * \param name The name of the texture
 * \return The texture
 */
SSGEDECL SSGE_Texture *SSGE_Texture_GetName(char *name);

/**
 * Draws a texture
 * \param texture The texture to draw
 * \param x The x coordinate at which the texture is drawn
 * \param y The y coordinate at which the texture is drawn
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_Texture_Draw(SSGE_Texture *texture, int x, int y, int width, int height);

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
SSGEDECL void SSGE_Texture_DrawEx(SSGE_Texture *texture, int x, int y, int width, int height, double angle, SSGE_Point *center, SSGE_Flip flip);

/**
 * Draws a texture from a file
 * \param filename The texture to draw
 * \param x The x coordinate at which the texture is drawn
 * \param y The y coordinate at which the texture is drawn
 * \param width The width of the texture
 * \param height The height of the texture
 */
SSGEDECL void SSGE_Texture_DrawFile(char *filename, int x, int y, int width, int height);

/**
 * Destroys a texture
 * \param id The id of the texture
 */
SSGEDECL void SSGE_Texture_Destroy(uint32_t id);

/**
 * Destroys a texture
 * \param name The name of the texture
 */
SSGEDECL void SSGE_Texture_DestroyName(char *name);

/**
 * Destroys all texture
 */
SSGEDECL void SSGE_Texture_DestroyAll();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TEXTURE_H__