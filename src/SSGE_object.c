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
 * \param x The x coordinate of the object
 * \param y The y coordinate of the object
 * \param width The width of the object
 * \param height The height of the object
 * \param hitbox True if the object has a hitbox, false otherwise
 * \param data The data of the object
 * \param destroyData The function to destroy the data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Object_Create(char *name, SSGE_Texture *texture, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) 
        SSGE_Error("Failed to allocate memory for object");

    object->texture = texture->texture;
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->hitbox = hitbox;
    object->data = data;
    object->destroyData = destroyData;

    return _add_to_list(&_object_list, object, name, __func__);
}

/**
 * Instantiates an object from an object template
 * \param template The object template to instantiate
 * \param x The x coordinate of the object
 * \param y The y coordinate of the object
 * \param data The data of the object
 * \return The object id
 * \note The object is stored internally and can be accessed by its name or its id
 */
SSGEDECL uint32_t SSGE_Object_Instantiate(SSGE_ObjectTemplate *template, char *name, int x, int y, void *data) {
    _assert_engine_init
    return SSGE_Object_Create(name, template->texture, x, y, template->width, template->height, template->hitbox, data, template->destroyData);
}

/**
 * Checks if an object exists
 * \param id The id of the object
 * \return True if the object exists, false otherwise
 */
SSGEDECL bool SSGE_Object_Exists(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
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
SSGEDECL bool SSGE_Object_ExistsName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    return ptr == NULL ? false : true;
}

/**
 * Draws an object
 * \param object The object to draw
 */
SSGEDECL void SSGE_Object_Draw(SSGE_Object *object) {
    _assert_engine_init
    if (object->width == 0 || object->height == 0) return;
    SDL_Rect rect = {object->x, object->y, object->width, object->height};
    SDL_RenderCopy(_engine.renderer, object->texture, NULL, &rect);
}

/**
 * Moves an object
 * \param object The object to move
 * \param x The new x coordinate
 * \param y The new y coordinate
 */
SSGEDECL void SSGE_Object_Move(SSGE_Object *object, int x, int y) {
    object->x = x;
    object->y = y;
}

/**
 * Changes the texture of an object
 * \param object The object to change the texture of
 * \param texture The new texture of the object
 */
SSGEDECL void SSGE_Object_ChangeTexture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init
    object->texture = texture->texture;
}

/**
 * Gets an object by id
 * \param id The id of the object
 * \return The object
 */
SSGEDECL SSGE_Object *SSGE_Object_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %u", id);
    return ptr;
}

/**
 * Gets an object by name
 * \param name The name of the object
 * \return The object with the given name
 */
SSGEDECL SSGE_Object *SSGE_Object_GetName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %s", name);
    return ptr;
}

/**
 * Destroys an object by id
 * \param id The id of the object
 */
SSGEDECL void SSGE_Object_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_Pop(&_object_list, id);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %u", id);
    free(object->name);
    if (object->destroyData != NULL)
        object->destroyData(object->data);
    free(object);
}

/**
 * Destroys all objects with a given name
 * \param name The name of the object
 */
SSGEDECL void SSGE_Object_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_FindPop(&_object_list, _find_object_name, name);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %s", name);
    free(object->name);
    if (object->destroyData != NULL)
        object->destroyData(object->data);
    free(object);
}

/**
 * Destroys all objects
 */
SSGEDECL void SSGE_Object_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_object_list, _destroy_object);
    SSGE_Array_Create(&_object_list);
}

/***********************************************
 * Hitbox functions
 ***********************************************/

/**
 * Creates a hitbox
 * \param name The name of the hitbox
 * \param x The x coordinate of the hitbox
 * \param y The y coordinate of the hitbox
 * \param width The width of the hitbox
 * \param height The height of the hitbox
 * \return The hitbox id
 * \note The hitbox is an object with a hitbox property set to true, it does not have texture
 * \note The hitbox is stored internally as an object and can be accessed by its name or its id
 * \warning The hitbox must be destroyed at the exit of the program, as texture
 */
SSGEDECL uint32_t SSGE_Hitbox_Create(char *name, int x, int y, int width, int height) {
    _assert_engine_init
    SSGE_Object *hitbox = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (hitbox == NULL) 
        SSGE_Error("Failed to allocate memory for hitbox");
    hitbox->x = x;
    hitbox->y = y;
    hitbox->width = width;
    hitbox->height = height;
    hitbox->hitbox = true;
    hitbox->texture = NULL; // No texture for hitbox
    hitbox->data = NULL;
    hitbox->destroyData = NULL; // No data to destroy for hitbox

    return _add_to_list(&_object_list, hitbox, name, "SSGE_Hitbox_Create");
}

/**
 * Checks if a hitbox is colliding with another hitbox
 * \param hitbox1 The first hitbox
 * \param hitbox2 The second hitbox
 * \return True if the hitboxes are colliding, false otherwise
 */
SSGEDECL bool SSGE_Hitbox_IsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}