#include <stdio.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_objtemplate.h"

/***********************************************
 * Object template functions
 ***********************************************/

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
SSGEDECL uint32_t SSGE_Template_Create(char *name, SSGE_Texture *texture, int width, int height, bool hitbox, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template");
    template->texture = texture;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;
    template->destroyData = destroyData;

    return _add_to_list(&_object_template_list, template, name, __func__);
}

/**
 * Gets an object template by id
 * \param id The id of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_Template_Get(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Get(&_object_template_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id);
    return ptr;
}

static bool _find_template_name(void *ptr, void *name) {
    return strcmp(((SSGE_ObjectTemplate *)ptr)->name, name) == 0 ? 1 : 0;
}

/**
 * Gets an object template by name
 * \param name The name of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_Template_GetName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Find(&_object_template_list, _find_template_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name);
    return ptr;
}

/**
 * Destroys an object template by id
 * \param id The id of the object template
 */
SSGEDECL void SSGE_Template_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_Pop(&_object_template_list, id);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id);
    free(template->name);
    free(template);
}

/**
 * Destroys an object template by name
 * \param name The name of the object template
 */
SSGEDECL void SSGE_Template_DestroyName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_FindPop(&_object_template_list, _find_template_name, name);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name);
    free(template->name);
    free(template);
}

/**
 * Destroys all object templates
 */
SSGEDECL void SSGE_Template_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_object_template_list, _destroy_template);
    SSGE_Array_Create(&_object_template_list);
}