#ifndef __SSGE_OBJTEMPLATE_H__
#define __SSGE_OBJTEMPLATE_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates an object template
 * \param name The name of the object template
 * \param texture The texture of the object template
 * \param width The width of the object template
 * \param height The height of the object template
 * \param hitbox True if objects created from this template have a hitbox, false otherwise
 * \param destroyData The function to destroy the data of the object
 * \return The object template id
 * \note The object template is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Template_Create(char *name, SSGE_Texture *texture, uint16_t width, uint16_t height, bool hitbox, void (*destroyData)(void *));

/**
 * Gets an object template by id
 * \param id The id of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_Template_Get(uint32_t id);

/**
 * Gets an object template by name
 * \param name The name of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_Template_GetName(char *name);

/**
 * Destroys an object template by id
 * \param id The id of the object template
 */
SSGEDECL void SSGE_Template_Destroy(uint32_t id);

/**
 * Destroys an object template by name
 * \param name The name of the object template
 */
SSGEDECL void SSGE_Template_DestroyName(char *name);

/**
 * Destroys all object templates
 */
SSGEDECL void SSGE_Template_DestroyAll();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_OBJTEMPLATE_H__