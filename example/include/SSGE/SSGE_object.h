#ifndef __SSGE_OBJECT_H__
#define __SSGE_OBJECT_H__

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates an object
 * \param name The name of the object
 * \param texture The texture of the object
 * \param x The x position of the object
 * \param y The y position of the object
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox True if the object has a hitbox, false otherwise
 * \param data The data of the object
 * \param destroyData The function to destroy the data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Object_Create(char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *));

/**
 * Instantiates an object from an object template
 * \param template The object template to instantiate
 * \param x The x position of the object
 * \param y The y position of the object
 * \param data The data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Object_Instantiate(SSGE_ObjectTemplate *template, char *name, int x, int y, void *data);

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
 * Changes the texture of an object
 * \param object The object to change the texture of
 * \param texture The new texture of the object
 */
SSGEDECL void SSGE_Object_ChangeTexture(SSGE_Object *object, SSGE_Texture *texture);

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

// Hitbox functions

/**
 * Creates a hitbox
 * \param name The name of the hitbox
 * \param x The x position of the hitbox
 * \param y The y position of the hitbox
 * \param width The width of the hitbox
 * \param height The height of the hitbox
 * \return The hitbox id
 * \note The hitbox is an object with a hitbox property set to true, it does not have texture
 * \note The hitbox is stored internally as an object and can be accessed by its name or its id
 * \warning The hitbox must be destroyed at the exit of the program, as texture
 */
SSGEDECL uint32_t SSGE_Hitbox_Create(char *name, int x, int y, int width, int height);

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