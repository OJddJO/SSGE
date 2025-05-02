#include <stdio.h>

#define SSGE_WANT_SDL2

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
SSGEDECL uint32_t SSGE_CreateObjectTemplate(char *name, SSGE_Texture *texture, int width, int height, bool hitbox, void (*destroyData)(void *)) {
    _assert_engine_init();
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) {
        fprintf(stderr, "[SSGE][CORE] Failed to allocate memory for object template\n");
        exit(1);
    }
    template->texture = texture;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;
    template->destroyData = destroyData;

    return _add_object_template_to_list(template, name);
}

/**
 * Gets an object template by id
 * \param id The id of the object template
 * \return The object template
 */
SSGEDECL SSGE_ObjectTemplate *SSGE_GetTemplate(uint32_t id) {
    _assert_engine_init();
    SSGE_ObjectTemplate *ptr = SSGE_Array_Get(_object_template_list, id);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object template not found: %u\n", id);
        exit(1);
    }
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
SSGEDECL SSGE_ObjectTemplate *SSGE_GetTemplateByName(char *name) {
    _assert_engine_init();
    SSGE_ObjectTemplate *ptr = SSGE_Array_Find(_object_template_list, _find_template_name, name);
    if (ptr == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object template not found: %s\n", name);
        exit(1);
    }
    return ptr;
}

/**
 * Destroys an object template by id
 * \param id The id of the object template
 */
SSGEDECL void SSGE_DestroyObjectTemplate(uint32_t id) {
    _assert_engine_init();
    SSGE_ObjectTemplate *template = SSGE_Array_Pop(_object_template_list, id);
    if (template == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object template not found: %u\n", id);
        exit(1);
    }
    free(template->name);
    free(template);
}

/**
 * Destroys an object template by name
 * \param name The name of the object template
 */
SSGEDECL void SSGE_DestroyObjectTemplateByName(char *name) {
    _assert_engine_init();
    SSGE_ObjectTemplate *template = SSGE_Array_FindPop(_object_template_list, _find_template_name, name);
    if (template == NULL) {
        fprintf(stderr, "[SSGE][ENGINE] Object template not found: %s\n", name);
        exit(1);
    }
    free(template->name);
    free(template);
}

/**
 * Destroys all object templates
 */
SSGEDECL void SSGE_DestroyAllTemplates() {
    _assert_engine_init();
    SSGE_Array_Destroy(_object_template_list, _destroy_template);
    _object_template_list = SSGE_Array_Create();
}