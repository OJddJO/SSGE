#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_object.h"

/***********************************************
 * Object functions
 ***********************************************/

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
SSGEDECL uint32_t SSGE_CreateObject(char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *)) {
    _assert_engine_init();
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for object\n");
        exit(1);
    }

    object->texture = texture->texture;
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->hitbox = hitbox;
    object->data = data;
    object->destroyData = destroyData;

    return _add_object_to_list(object, name);
}

/**
 * Instantiates an object from an object template
 * \param template The object template to instantiate
 * \param x The x position of the object
 * \param y The y position of the object
 * \param data The data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_InstantiateObject(SSGE_ObjectTemplate *template, char *name, int x, int y, void *data) {
    _assert_engine_init();
    return SSGE_CreateObject(name, template->texture, x, y, template->width, template->height, template->hitbox, data, template->destroyData);
}

/**
 * Checks if an object exists
 * \param id The id of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_ObjectExists(uint32_t id) {
    _assert_engine_init();
    SSGE_Object *ptr = SSGE_Array_Get(_object_list, id);
    return ptr == NULL ? false : true;
}

static bool _find_object_name(void *ptr, void *name) {
    return strcmp(((SSGE_Object *)ptr)->name, (char *)name) == 0 ? 1 : 0;
}

/**
 * Checks if an object exists
 * \param name The name of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_ObjectExistsByName(char *name) {
    _assert_engine_init();
    SSGE_Object *ptr = SSGE_Array_Find(_object_list, _find_object_name, name);
    return ptr == NULL ? false : true;
}

/**
 * Draws an object
 * \param object The object to draw
 */
SSGEDECL void SSGE_DrawObject(SSGE_Object *object) {
    _assert_engine_init();
    SDL_Rect rect = {object->x, object->y, object->width, object->height};
    SDL_RenderCopy(_engine->renderer, object->texture, NULL, &rect);
}

/**
 * Changes the texture of an object
 * \param object The object to change the texture of
 * \param texture The new texture of the object
 */
SSGEDECL void SSGE_ChangeObjectTexture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init();
    object->texture = texture->texture;
}

/**
 * Gets an object by id
 * \param id The id of the object
 * \return The object
 */
SSGEDECL SSGE_Object *SSGE_GetObject(uint32_t id) {
    _assert_engine_init();
    SSGE_Object *ptr = SSGE_Array_Get(_object_list, id);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object not found: %u\n", id);
        exit(1);
    }
    return ptr;
}

/**
 * Gets an object by name
 * \param name The name of the object
 * \return The object with the given name
 */
SSGEDECL SSGE_Object *SSGE_GetObjectByName(char *name) {
    _assert_engine_init();
    SSGE_Object *ptr = SSGE_Array_Find(_object_list, _find_object_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object not found: %s\n", name);
        exit(1);
    }
    return ptr;
}

/**
 * Destroys an object by id
 * \param id The id of the object
 */
SSGEDECL void SSGE_DestroyObject(uint32_t id) {
    _assert_engine_init();
    SSGE_Object *object = SSGE_Array_Pop(_object_list, id);
    if (object == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object not found: %u\n", id);
        exit(1);
    }
    free(object->name);
    if (object->destroyData != NULL)
        object->destroyData(object->data);
    free(object);
}

/**
 * Destroys all objects with a given name
 * \param name The name of the object
 */
SSGEDECL void SSGE_DestroyObjectByName(char *name) {
    _assert_engine_init();
    SSGE_Object *object = SSGE_Array_FindPop(_object_list, _find_object_name, name);
    if (object == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object not found: %s\n", name);
        exit(1);
    }
    free(object->name);
    if (object->destroyData != NULL)
        object->destroyData(object->data);
    free(object);
}

/**
 * Destroys all objects
 */
SSGEDECL void SSGE_DestroyAllObjects() {
    _assert_engine_init();
    SSGE_Array_Destroy(_object_list, _destroy_object);
    _object_list = SSGE_Array_Create();
}

/***********************************************
 * Hitbox functions
 ***********************************************/

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
SSGEDECL uint32_t SSGE_CreateHitbox(char *name, int x, int y, int width, int height) {
    _assert_engine_init();
    SSGE_Object *new_hitbox = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (new_hitbox == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for hitbox\n");
        exit(1);
    }
    new_hitbox->x = x;
    new_hitbox->y = y;
    new_hitbox->width = width;
    new_hitbox->height = height;
    new_hitbox->hitbox = true;
    new_hitbox->texture = NULL; // No texture for hitbox
    new_hitbox->data = NULL;
    new_hitbox->destroyData = NULL; // No data to destroy for hitbox

    return _add_object_to_list(new_hitbox, name);
}

/**
 * Checks if a hitbox is colliding with another hitbox
 * \param hitbox1 The first hitbox
 * \param hitbox2 The second hitbox
 * \return True if the hitboxes are colliding, false otherwise
 */
SSGEDECL bool SSGE_HitboxIsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}