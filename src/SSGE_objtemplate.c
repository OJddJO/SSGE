#include "SSGE_local.h"
#include "SSGE/SSGE_objtemplate.h"

SSGEAPI SSGE_ObjectTemplate *SSGE_Template_Create(uint32_t *id, char *name, uint16_t width, uint16_t height, bool hitbox) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template")
    template->spriteType = SSGE_SPRITE_NONE;
    template->texture = NULL;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;

    _addToList(&_objectTemplateList, template, name, id, __func__);
    return template;
}

SSGEAPI SSGE_ObjectTemplate *SSGE_Template_CreateStatic(uint32_t *id, char *name, SSGE_Texture *texture, uint16_t width, uint16_t height, bool hitbox) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template")
    template->spriteType = SSGE_SPRITE_STATIC;
    template->texture = texture;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;

    _addToList(&_objectTemplateList, template, name, id, __func__);
    return template;
}

SSGEAPI SSGE_ObjectTemplate *SSGE_Template_CreateAnim(uint32_t *id, char *name, SSGE_Animation *animation, uint16_t width, uint16_t height, bool hitbox) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)malloc(sizeof(SSGE_ObjectTemplate));
    if (template == NULL) 
        SSGE_Error("Failed to allocate memory for object template")
    template->spriteType = SSGE_SPRITE_ANIM;
    template->animation = animation;
    template->width = width;
    template->height = height;
    template->hitbox = hitbox;

    _addToList(&_objectTemplateList, template, name, id, __func__);
    return template;
}

SSGEAPI SSGE_ObjectTemplate *SSGE_Template_Get(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Get(&_objectTemplateList, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id)
    return ptr;
}

inline static bool _find_template_name(void *ptr, void *name) {
    return strcmp(((SSGE_ObjectTemplate *)ptr)->name, name) == 0;
}

SSGEAPI SSGE_ObjectTemplate *SSGE_Template_GetName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *ptr = SSGE_Array_Find(&_objectTemplateList, _find_template_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name)
    return ptr;
}

SSGEAPI void SSGE_Template_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_Pop(&_objectTemplateList, id);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %u", id)
    free(template->name);
    free(template);
}

SSGEAPI void SSGE_Template_DestroyName(char *name) {
    _assert_engine_init
    SSGE_ObjectTemplate *template = SSGE_Array_FindPop(&_objectTemplateList, _find_template_name, name);
    if (template == NULL) 
        SSGE_ErrorEx("Object template not found: %s", name)
    free(template->name);
    free(template);
}

SSGEAPI void SSGE_Template_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_objectTemplateList, (SSGE_DestroyData)destroyTemplate);
    SSGE_Array_Create(&_objectTemplateList);
}