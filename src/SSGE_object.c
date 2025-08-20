#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_object.h"
#include "SSGE/SSGE_animation.h"

SSGEAPI SSGE_Object *SSGE_Object_Create(uint32_t *id, char *name, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) 
        SSGE_Error("Failed to allocate memory for object")

    object->spriteType = SSGE_SPRITE_STATIC;
    object->texture = NULL;
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->hitbox = hitbox;
    object->data = data;
    object->destroyData = destroyData;

    _add_to_list(&_object_list, object, name, id, __func__);
    return object;
}

SSGEAPI SSGE_Object *SSGE_Object_Instantiate(uint32_t *id, SSGE_ObjectTemplate *template, char *name, int x, int y, void *data) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Object_Create(id, name, x, y, template->width, template->height, template->hitbox, data, template->destroyData);
    if (template->spriteType == SSGE_SPRITE_STATIC)
        SSGE_Object_BindTexture(object, template->texture);
    else
        SSGE_Object_BindAnimation(object, template->animation);
    return object;
}

SSGEAPI bool SSGE_Object_Exists(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
    return ptr == NULL ? false : true;
}

static bool _find_object_name(void *ptr, void *name) {
    return strcmp(((SSGE_Object *)ptr)->name, (char *)name) == 0 ;
}

SSGEAPI bool SSGE_Object_ExistsName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    return ptr == NULL ? false : true;
}

SSGEAPI void SSGE_Object_Draw(SSGE_Object *object) {
    _assert_engine_init
    if (object->width == 0 || object->height == 0 ||
        object->spriteType == SSGE_SPRITE_ANIM) return;

    if (object->spriteType == SSGE_SPRITE_STATIC) {
        SSGE_Texture *texture = object->texture;
        if (texture == NULL)
            return;
        SDL_Rect rect = {object->x + texture->anchorX, object->y + texture->anchorY, object->width, object->height};
        SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
    }
}

SSGEAPI void SSGE_Object_DrawAll() {
    _assert_engine_init
    for (uint32_t i = 0; i < _object_list.count; i++) {
        SSGE_Object *object = SSGE_Array_Get(&_object_list, i);
        if (object == NULL) continue;

        if (object->width == 0 || object->height == 0 ||
            object->spriteType == SSGE_SPRITE_ANIM) continue;

        if (object->spriteType == SSGE_SPRITE_STATIC) {
            SSGE_Texture *texture = object->texture;
            if (texture == NULL) continue;
            SDL_Rect rect = {object->x + texture->anchorX, object->y + texture->anchorY, object->width, object->height};
            SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
        }
    }
}

SSGEAPI void SSGE_Object_Move(SSGE_Object *object, int x, int y) {
    _assert_engine_init
    object->x = x;
    object->y = y;
    if (object->spriteType == SSGE_SPRITE_ANIM)
        SSGE_Animation_Move(object->animation, x, y);
}

SSGEAPI void SSGE_Object_BindTexture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init
    object->spriteType = SSGE_SPRITE_STATIC;
    object->texture = texture;
}

SSGEAPI void SSGE_Object_BindAnimation(SSGE_Object *object, SSGE_Animation *animation) {
    _assert_engine_init
    object->spriteType = SSGE_SPRITE_ANIM;
    object->animation = SSGE_Animation_Play(animation, object->x, object->y, -1, false, false);
}

SSGEAPI void SSGE_Object_RemoveSprite(SSGE_Object *object) {
    _assert_engine_init
    object->spriteType = SSGE_SPRITE_STATIC;
    object->texture = NULL;
}

SSGEAPI SSGE_Object *SSGE_Object_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %u", id)
    return ptr;
}

SSGEAPI SSGE_Object *SSGE_Object_GetName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    return ptr;
}

SSGEAPI void SSGE_Object_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_Pop(&_object_list, id);
    if (object == NULL)
        SSGE_ErrorEx("Object not found: %u", id)
    _destroy_object(object);
}

SSGEAPI void SSGE_Object_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_FindPop(&_object_list, _find_object_name, name);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    _destroy_object(object);
}

SSGEAPI void SSGE_Object_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_object_list, _destroy_object);
    SSGE_Array_Create(&_object_list);
}

SSGEAPI bool SSGE_Object_IsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}