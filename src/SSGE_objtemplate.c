#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_objtemplate.h"

/***********************************************
 * Object template functions
 ***********************************************/

SSGEDECL SSGE_ObjectTemplate *SSGE_Template_CreateStatic(uint32_t *id, char *name, SSGE_Texture *texture, uint16_t width, uint16_t height, bool hitbox, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template");
    template->spriteType = SSGE_SPRITE_STATIC;
    template->texture = texture;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;
    template->destroyData = destroyData;

    _add_to_list(&_object_template_list, template, name, id, __func__);
    return template;
}

SSGEDECL SSGE_ObjectTemplate *SSGE_Template_CreateAnim(uint32_t *id, char *name, SSGE_Animation *animation, uint16_t width, uint16_t height, bool hitbox, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template");
    template->spriteType = SSGE_SPRITE_ANIM;
    template->animation = animation;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;
    template->destroyData = destroyData;

    _add_to_list(&_object_template_list, template, name, id, __func__);
    return template;
}

SSGEDECL SSGE_ObjectTemplate *SSGE_Template_Get(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Get(&_object_template_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id);
    return ptr;
}

static bool _find_template_name(void *ptr, void *name) {
    return strcmp(((SSGE_ObjectTemplate *)ptr)->name, name) == 0;
}

SSGEDECL SSGE_ObjectTemplate *SSGE_Template_GetName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Find(&_object_template_list, _find_template_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name);
    return ptr;
}

SSGEDECL void SSGE_Template_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_Pop(&_object_template_list, id);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id);
    free(template->name);
    free(template);
}

SSGEDECL void SSGE_Template_DestroyName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_FindPop(&_object_template_list, _find_template_name, name);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name);
    free(template->name);
    free(template);
}

SSGEDECL void SSGE_Template_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_object_template_list, _destroy_template);
    SSGE_Array_Create(&_object_template_list);
}