#ifndef __SSGE_TEXT_H__
#define __SSGE_TEXT_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Loads a font
 * \param filename The path to the font
 * \param size The size of the font
 * \param name The name of the font
 */
SSGEDECL void SSGE_Text_CreateFont(char *filename, int size, char *name);

/**
 * Draws text
 * \param fontName The name of the font
 * \param text The text to draw
 * \param x The x coordinate to draw the text
 * \param y The y coordinate to draw the text
 * \param color The color of the text
 * \param anchor The anchor of the text
 */
SSGEDECL void SSGE_Text_Draw(char *fontName, char *text, int x, int y, SSGE_Color color, SSGE_Anchor anchor);

/**
 * Creates text as a texture
 * \param fontName The name of the font
 * \param text The text to draw
 * \param color The color of the text
 * \param textureName The name of the texture
 */
SSGEDECL uint32_t SSGE_Text_Create(char *fontName, char *text, SSGE_Color color, char *textureName);

/**
 * Closes a font by name
 * \param fontName The name of the font
 */
SSGEDECL void SSGE_Text_CloseFont(char *name);

/**
 * Closes all fonts
 */
SSGEDECL void SSGE_Text_CloseAllFonts();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_TEXT_H__