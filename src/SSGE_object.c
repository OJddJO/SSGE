#include "SSGE_local.h"
#include "SSGE/SSGE_object.h"
#include "SSGE/SSGE_animation.h"

SSGEAPI SSGE_Object *SSGE_Object_Create(uint32_t *id, char *name, int x, int y, int width, int height, bool hitbox) {
    _assert_engine_init
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) 
        SSGE_Error("Failed to allocate memory for object")

    object->spriteType = SSGE_SPRITE_NONE;
    object->texture.texture = NULL;
    object->texture.renderDataIdx = 0;
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
    object->hitbox = hitbox;
    object->data = NULL;
    object->destroyData = NULL;

    _addToList(&_objectList, object, name, id, __func__);
    return object;
}

SSGEAPI SSGE_Object *SSGE_Object_Instantiate(uint32_t *id, SSGE_ObjectTemplate *template, char *name, int x, int y) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Object_Create(id, name, x, y, template->width, template->height, template->hitbox);
    if (template->spriteType == SSGE_SPRITE_STATIC)
        SSGE_Object_BindTexture(object, template->texture);
    else if (template->spriteType == SSGE_SPRITE_ANIM)
        SSGE_Object_BindAnimation(object, template->animation);
    return object;
}

SSGEAPI bool SSGE_Object_Exists(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_objectList, id);
    return ptr == NULL ? false : true;
}

inline static bool _find_object_name(void *ptr, void *name) {
    return strcmp(((SSGE_Object *)ptr)->name, (char *)name) == 0 ;
}

SSGEAPI bool SSGE_Object_ExistsName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_objectList, _find_object_name, name);
    return ptr == NULL ? false : true;
}

SSGEAPI void SSGE_Object_Move(SSGE_Object *object, int x, int y) {
    _assert_engine_init
    object->x = x;
    object->y = y;
    switch (object->spriteType) {
        case SSGE_SPRITE_ANIM:
            SSGE_Animation_Move(object->animation, x, y);
            break;
        case SSGE_SPRITE_STATIC:
            _SSGE_RenderData *renderData = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
            renderData->x = x;
            renderData->y = y;
            break;
        default:
            break;
    }
}

SSGEAPI void SSGE_Object_MoveRel(SSGE_Object *object, int dx, int dy) {
    _assert_engine_init
    object->x += dx;
    object->y += dy;
    switch (object->spriteType) {
        case SSGE_SPRITE_ANIM:
            SSGE_Animation_Move(object->animation, object->x, object->y);
            break;
        case SSGE_SPRITE_STATIC:
            _SSGE_RenderData *renderData = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
            renderData->x = object->x;
            renderData->y = object->y;
            break;
        default:
            break;
    }
}

SSGEAPI void SSGE_Object_BindData(SSGE_Object *object, void *data, SSGE_DestroyData destroy) {
    _assert_engine_init
    if (object->data && object->destroyData)
        object->destroyData(object->data);
    object->data = data;
    object->destroyData = destroy;
}

SSGEAPI void SSGE_Object_RemoveData(SSGE_Object *object) {
    _assert_engine_init
    if (object->data && object->destroyData)
        object->destroyData(object->data);
}

SSGEAPI void SSGE_Object_BindTexture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_STATIC;
    _SSGE_RenderData *renderData = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData));
    *renderData = (_SSGE_RenderData){
        .x = object->x,
        .y = object->y,
        .width = object->width,
        .height = object->height,
        .once = false
    };
    object->texture.renderDataIdx = SSGE_Array_Add(&texture->queue, renderData);
    object->texture.texture = texture;
}

SSGEAPI void SSGE_Object_BindAnimation(SSGE_Object *object, SSGE_Animation *animation) {
    _assert_engine_init
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_ANIM;
    object->animation = SSGE_Animation_Play(animation, object->x, object->y, -1, false, false);
}

SSGEAPI void SSGE_Object_RemoveSprite(SSGE_Object *object) {
    _assert_engine_init
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_NONE;
}

SSGEAPI SSGE_Object *SSGE_Object_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_objectList, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %u", id)
    return ptr;
}

SSGEAPI SSGE_Object *SSGE_Object_GetName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_objectList, _find_object_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    return ptr;
}

SSGEAPI void *SSGE_Object_GetData(SSGE_Object *object) {
    _assert_engine_init
    return object->data;
}

SSGEAPI void SSGE_Object_Resize(SSGE_Object *object, uint16_t width, uint16_t height) {
    _assert_engine_init
    object->width = width;
    object->height = height;
    if (object->spriteType == SSGE_SPRITE_STATIC) {
        _SSGE_RenderData *data = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
        data->width = width;
        data->height = height;
    }
}

SSGEAPI void SSGE_Object_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_Pop(&_objectList, id);
    if (object == NULL)
        SSGE_ErrorEx("Object not found: %u", id)
    destroyObject(object);
}

SSGEAPI void SSGE_Object_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_FindPop(&_objectList, _find_object_name, name);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    destroyObject(object);
}

SSGEAPI void SSGE_Object_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_objectList, (SSGE_DestroyData)destroyObject);
    SSGE_Array_Create(&_objectList);
}

SSGEAPI bool SSGE_Object_IsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}

static bool _is_hovered(SSGE_Object *ptr, int *mousePos) {
    int mouseX = mousePos[0],
        mouseY = mousePos[1];
    return mouseX >= ptr->x && mouseX <= ptr->x + ptr->width && mouseY >= ptr->y && mouseY <= ptr->y + ptr->height;
}

SSGEAPI bool SSGE_Object_IsHovered(SSGE_Object *object) {
    _assert_engine_init
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
}

SSGEAPI SSGE_Object *SSGE_Object_GetAt(int x, int y) {
    _assert_engine_init
    int pos[2] = {x, y};
    return SSGE_Array_Find(&_objectList, (bool (*)(void *, void *))_is_hovered, pos);
}

SSGEAPI uint32_t SSGE_Object_GetAtList(int x, int y, SSGE_Object *objects[], uint32_t size) {
    _assert_engine_init
    int pos[2] = {x, y};

    uint32_t i = 0, count = 0;
    while (count < _objectList.count && i < _objectList.size && count < size) {
        SSGE_Object *obj = SSGE_Array_Get(&_objectList, i++);

        if (obj != NULL && _is_hovered(obj, pos))
            objects[count++] = obj;
    }
    return count;
}

SSGEAPI SSGE_Object *SSGE_Object_GetHovered() {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);

    return SSGE_Array_Find(&_objectList, (bool (*)(void *, void *))_is_hovered, mousePos);
}

SSGEAPI uint32_t SSGE_Objects_GetHoveredList(SSGE_Object *objects[], uint32_t size) {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);
    
    uint32_t i = 0, count = 0;
    while (count < _objectList.count && i < _objectList.size && count < size) {
        SSGE_Object *obj = SSGE_Array_Get(&_objectList, i++);

        if (obj != NULL && _is_hovered(obj, mousePos))
            objects[count++] = obj;
    }
    return count;
}
