#ifndef __SSGE_OBJECT_H__
#define __SSGE_OBJECT_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates an object
 * \param id Where to store the id of the object
 * \param name The name of the object
 * \param texture The texture of the object
 * \param x The x coordinate of the object
 * \param y The y coordinate of the object
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox True if the object has a hitbox, false otherwise
 * \param data The data of the object
 * \param destroyData The function to destroy the data of the object
 * \return The object
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL SSGE_Object *SSGE_Object_Create(uint32_t *id, char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *));

/**
 * Instantiates an object from an object template
 * \param id Where to store the id of the object
 * \param template The object template to instantiate
 * \param x The x coordinate of the object
 * \param y The y coordinate of the object
 * \param data The data of the object
 * \return The object
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL SSGE_Object *SSGE_Object_Instantiate(uint32_t *id, SSGE_ObjectTemplate *template, char *name, int x, int y, void *data);

/**
 * Checks if an object exists
 * \param id The id of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_Object_Exists(uint32_t id);

/**
 * Checks if an object exists
 * \param name The name of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_Object_ExistsName(char *name);

/**
 * Draws an object
 * \param object The object to draw
 */
SSGEDECL void SSGE_Object_Draw(SSGE_Object *object);

/**
 * Moves an object
 * \param object The object to move
 * \param x The new x coordinate
 * \param y The new y coordinate
 */
SSGEDECL void SSGE_Object_Move(SSGE_Object *object, int x, int y);

/**
 * Changes the texture of an object
 * \param object The object to change the texture of
 * \param texture The new texture of the object
 */
SSGEDECL void SSGE_Object_ChangeTexture(SSGE_Object *object, SSGE_Texture *texture);

/**
 * Remove the texure of an object
 * \param object The object to remove the texture of
 */
SSGEDECL void SSGE_Object_RemoveTexture(SSGE_Object *object);

/**
 * Gets an object by id
 * \param id The id of the object
 * \return The object
 */
SSGEDECL SSGE_Object *SSGE_Object_Get(uint32_t id);

/**
 * Gets an object by name
 * \param name The name of the object
 * \return The object with the given name
 */
SSGEDECL SSGE_Object *SSGE_Object_GetName(char *name);

/**
 * Destroys an object by id
 * \param id The id of the object
 */
SSGEDECL void SSGE_Object_Destroy(uint32_t id);

/**
 * Destroys all objects with a given name
 * \param name The name of the object
 */
SSGEDECL void SSGE_Object_DestroyName(char *name);

/**
 * Destroys all objects
 */
SSGEDECL void SSGE_Object_DestroyAll();

/**
 * Checks if a hitbox is colliding with another hitbox
 * \param hitbox1 The first hitbox
 * \param hitbox2 The second hitbox
 * \return True if the hitboxes are colliding, false otherwise
 */
SSGEDECL bool SSGE_Hitbox_IsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2);

#ifdef __cplusplus
}
#endif

#endif // __SSGE_OBJECT_H__