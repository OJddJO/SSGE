#include "SSGE_local.h"
#include "SSGE/SSGE_object.h"
#include "SSGE/SSGE_animation.h"

SSGEAPI SSGE_Object *SSGE_Object_Create(uint32_t *id, const char *name, int x, int y, int width, int height, bool hitbox) {
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) 
        SSGE_Error("Failed to allocate memory for object")

    *object = (SSGE_Object) {
        .spriteType = SSGE_SPRITE_NONE,
        .texture = {
            .texture = NULL,
            .renderDataIdx = 0,
        },
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .hitbox = hitbox,
        .data = NULL,
        .destroyData = NULL,
    };

    _addToList(&_objectList, object, name, id, __func__);
    return object;
}

SSGEAPI SSGE_Object *SSGE_Object_Instantiate(uint32_t *id, SSGE_ObjectTemplate *template, char *name, int x, int y) {
    SSGE_Object *object = SSGE_Object_Create(id, name, x, y, template->width, template->height, template->hitbox);
    if (template->spriteType == SSGE_SPRITE_STATIC)
        SSGE_Object_BindTexture(object, template->texture);
    else if (template->spriteType == SSGE_SPRITE_ANIM)
        SSGE_Object_BindAnimation(object, template->animation, false, false);
    return object;
}

SSGEAPI bool SSGE_Object_Exists(uint32_t id) {
    SSGE_Object *ptr = SSGE_Array_Get(&_objectList, id);
    return ptr != NULL;
}

inline static bool _find_object_name(void *ptr, void *name) {
    return strcmp(((SSGE_Object *)ptr)->name, (char *)name) == 0 ;
}

SSGEAPI bool SSGE_Object_ExistsName(const char *name) {
    SSGE_Object *ptr = SSGE_Array_Find(&_objectList, _find_object_name, (void *)name);
    return ptr != NULL;
}

SSGEAPI void SSGE_Object_Move(SSGE_Object *object, int x, int y) {
    object->x = x;
    object->y = y;
    switch (object->spriteType) {
        case SSGE_SPRITE_ANIM:
            SSGE_Animation_Move(object->animation, x, y);
            break;
        case SSGE_SPRITE_STATIC:
            _SSGE_RenderData *renderData = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
            renderData->dest.x = x;
            renderData->dest.y = y;
            break;
        default:
            break;
    }
}

SSGEAPI void SSGE_Object_MoveRel(SSGE_Object *object, int dx, int dy) {
    object->x += dx;
    object->y += dy;
    switch (object->spriteType) {
        case SSGE_SPRITE_ANIM:
            SSGE_Animation_Move(object->animation, object->x, object->y);
            break;
        case SSGE_SPRITE_STATIC:
            _SSGE_RenderData *renderData = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
            renderData->dest.x = object->x;
            renderData->dest.y = object->y;
            break;
        default:
            break;
    }
}

SSGEAPI void SSGE_Object_BindData(SSGE_Object *object, void *data, SSGE_DestroyData destroy) {
    if (object->data && object->destroyData)
        object->destroyData(object->data);
    object->data = data;
    object->destroyData = destroy;
}

SSGEAPI void SSGE_Object_RemoveData(SSGE_Object *object) {
    if (object->data && object->destroyData)
        object->destroyData(object->data);
    object->data = NULL;
    object->destroyData = NULL;
}

SSGEAPI void SSGE_Object_BindTexture(SSGE_Object *object, SSGE_Texture *texture) {
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_STATIC;
    _SSGE_RenderData *renderData = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData));
    *renderData = (_SSGE_RenderData){
        .dest = {
            .x = object->x,
            .y = object->y,
            .w = object->width,
            .h = object->height
        },
        .once = false
    };
    object->texture.renderDataIdx = SSGE_Array_Add(&texture->queue, renderData);
    object->texture.texture = texture;
}

SSGEAPI void SSGE_Object_BindAnimation(SSGE_Object *object, SSGE_Animation *animation, bool reversed, bool pingpong) {
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_ANIM;
    object->animation = SSGE_Animation_Play(animation, object->x, object->y, -1, reversed, pingpong);
}

SSGEAPI void SSGE_Object_RemoveSprite(SSGE_Object *object) {
    if (object->spriteType == SSGE_SPRITE_STATIC)
        free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
    object->spriteType = SSGE_SPRITE_NONE;
}

SSGEAPI void SSGE_Object_Hide(SSGE_Object *object) {
    if (object->hidden) return;
    object->hidden = true;
    switch (object->spriteType) {
        case SSGE_SPRITE_ANIM:
            SSGE_Animation_Pause(object->animation);
            break;
        case SSGE_SPRITE_STATIC:
            free(SSGE_Array_Pop(&object->texture.texture->queue, object->texture.renderDataIdx));
        default:
            break;
    }
}

SSGEAPI void SSGE_Object_Show(SSGE_Object *object) {
    if (object->hidden) {
        object->hidden = false;
        switch (object->spriteType) {
            case SSGE_SPRITE_ANIM:
                SSGE_Animation_Resume(object->animation);
                break;
            case SSGE_SPRITE_STATIC:
                _SSGE_RenderData *renderData = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData));
                *renderData = (_SSGE_RenderData){
                    .dest = {
                        .x = object->x,
                        .y = object->y,
                        .w = object->width,
                        .h = object->height
                    },
                    .once = false
                };
                object->texture.renderDataIdx = SSGE_Array_Add(&object->texture.texture->queue, renderData);
                break;
            default:
                break;
        }
    }
}

SSGEAPI SSGE_Object *SSGE_Object_Get(uint32_t id) {
    SSGE_Object *ptr = SSGE_Array_Get(&_objectList, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %u", id)
    return ptr;
}

SSGEAPI SSGE_Object *SSGE_Object_GetName(const char *name) {
    SSGE_Object *ptr = SSGE_Array_Find(&_objectList, _find_object_name, (void *)name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    return ptr;
}

SSGEAPI void *SSGE_Object_GetData(SSGE_Object *object) {
    return object->data;
}

SSGEAPI void SSGE_Object_Resize(SSGE_Object *object, uint16_t width, uint16_t height) {
    object->width = width;
    object->height = height;
    if (object->spriteType == SSGE_SPRITE_STATIC) {
        _SSGE_RenderData *data = SSGE_Array_Get(&object->texture.texture->queue, object->texture.renderDataIdx);
        data->dest.w = width;
        data->dest.h = height;
    }
}

SSGEAPI void SSGE_Object_Destroy(uint32_t id) {
    SSGE_Object *object = SSGE_Array_Pop(&_objectList, id);
    if (object == NULL)
        SSGE_ErrorEx("Object not found: %u", id)
    destroyObject(object);
}

SSGEAPI void SSGE_Object_DestroyName(const char *name) {
    SSGE_Object *object = SSGE_Array_FindPop(&_objectList, _find_object_name, (void *)name);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %s", name)
    destroyObject(object);
}

SSGEAPI void SSGE_Object_DestroyAll() {
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
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
}

SSGEAPI SSGE_Object *SSGE_Object_GetAt(int x, int y) {
    int pos[2] = {x, y};
    return SSGE_Array_Find(&_objectList, (bool (*)(void *, void *))_is_hovered, pos);
}

SSGEAPI uint32_t SSGE_Object_GetAtList(int x, int y, SSGE_Object *objects[], uint32_t size) {
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
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);

    return SSGE_Array_Find(&_objectList, (bool (*)(void *, void *))_is_hovered, mousePos);
}

SSGEAPI uint32_t SSGE_Objects_GetHoveredList(SSGE_Object *objects[], uint32_t size) {
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

SSGEAPI void SSGE_Object_GetSize(SSGE_Object *object, uint16_t *width, uint16_t *height) {
    *width = object->width;
    *height = object->height;
}

SSGEAPI void SSGE_Object_GetPosition(SSGE_Object *object, int *x, int *y) {
    *x = object->x;
    *y = object->y;
}

SSGEAPI bool SSGE_Object_IsHidden(SSGE_Object *object) {
    return object->hidden;
}
