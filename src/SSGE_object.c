#define SSGE_GET_SDL

#include "SSGE/SSGE_local.h"
#include "SSGE/SSGE_object.h"

/***********************************************
 * Object functions
 ***********************************************/

SSGEDECL SSGE_Object *SSGE_Object_Create(uint32_t *id, char *name, int x, int y, int width, int height, bool hitbox, void *data, void (*destroyData)(void *)) {
    _assert_engine_init
    SSGE_Object *object = (SSGE_Object *)malloc(sizeof(SSGE_Object));
    if (object == NULL) 
        SSGE_Error("Failed to allocate memory for object");

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

SSGEDECL SSGE_Object *SSGE_Object_Instantiate(uint32_t *id, SSGE_ObjectTemplate *template, char *name, int x, int y, void *data) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Object_Create(id, name, x, y, template->width, template->height, template->hitbox, data, template->destroyData);
    if (template->spriteType == SSGE_SPRITE_STATIC)
        SSGE_Object_BindTexture(object, template->texture);
    else
        SSGE_Object_BindAnimation(object, template->animation);
    return object;
}

SSGEDECL bool SSGE_Object_Exists(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
    return ptr == NULL ? false : true;
}

static bool _find_object_name(void *ptr, void *name) {
    return strcmp(((SSGE_Object *)ptr)->name, (char *)name) == 0 ;
}

SSGEDECL bool SSGE_Object_ExistsName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    return ptr == NULL ? false : true;
}

SSGEDECL void SSGE_Object_Draw(SSGE_Object *object) {
    _assert_engine_init
    if (object->width == 0 || object->height == 0) return;

    if (object->spriteType == SSGE_SPRITE_STATIC) {
        SSGE_Texture *texture = object->texture;
        if (texture == NULL)
            SSGE_ErrorEx("Object '%s' has no texture", object->name);
        SDL_Rect rect = {object->x + texture->anchorX, object->y + texture->anchorY, object->width, object->height};
        SDL_RenderCopy(_engine.renderer, texture->texture, NULL, &rect);
    } else if (object->spriteType == SSGE_SPRITE_ANIM) {
        SSGE_AnimationState *state = object->animation;
        SSGE_Animation *anim = state->animation;
        switch (anim->type) {
            case SSGE_ANIM_FRAMES:
                SDL_Rect dest = {
                    state->x - anim->data.anchorX,
                    state->y - anim->data.anchorY,
                    anim->data.width,
                    anim->data.height,
                };
                SDL_RenderCopy(_engine.renderer, anim->data.frames[state->currentFrame], NULL, &dest);

                if (state->currentFrameTime >= anim->data.frametimes[state->currentFrame]) {
                    state->currentFrame += 1 - 2*state->reversed;
                    state->currentFrameTime = 0;
                }

                if (state->currentFrame >= anim->data.currentCount)
                    state->currentFrame = 0;

                break;

            case SSGE_ANIM_FUNCTION:
                anim->draw(state);
                break;
        }
    }
}

SSGEDECL void SSGE_Object_Move(SSGE_Object *object, int x, int y) {
    _assert_engine_init
    object->x = x;
    object->y = y;
    if (object->spriteType == SSGE_SPRITE_ANIM) {
        object->animation->x = x;
        object->animation->y = y;
    }
}

SSGEDECL void SSGE_Object_BindTexture(SSGE_Object *object, SSGE_Texture *texture) {
    _assert_engine_init
    if (object->spriteType == SSGE_SPRITE_ANIM)
        free(object->animation);

    object->spriteType = SSGE_SPRITE_STATIC;
    object->texture = texture;
}

SSGEDECL void SSGE_Object_BindAnimation(SSGE_Object *object, SSGE_Animation *animation) {
    _assert_engine_init
    object->spriteType = SSGE_SPRITE_ANIM;
    SSGE_AnimationState *state = (object->animation = (SSGE_AnimationState *)malloc(sizeof(SSGE_AnimationState)));
    if (state == NULL)
        SSGE_Error("Failed to allocate memory for animation")

    state->animation = animation;
    state->x = object->x;
    state->y = object->y;
    state->currentFrame = 0;
    state->currentFrameTime = 0;
    state->loop = 0; // Not used
    state->reversed = false; // Not used
    state->pingpong = false; // Not used
    state->isPlaying = true;
}

SSGEDECL void SSGE_Object_RemoveSprite(SSGE_Object *object) {
    _assert_engine_init
    if (object->spriteType == SSGE_SPRITE_ANIM)
        free(object->animation);

    object->spriteType = SSGE_SPRITE_STATIC;
    object->texture = NULL;
}

SSGEDECL SSGE_Object *SSGE_Object_Get(uint32_t id) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Get(&_object_list, id);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %u", id);
    return ptr;
}

SSGEDECL SSGE_Object *SSGE_Object_GetName(char *name) {
    _assert_engine_init
    SSGE_Object *ptr = SSGE_Array_Find(&_object_list, _find_object_name, name);
    if (ptr == NULL) 
        SSGE_ErrorEx("Object not found: %s", name);
    return ptr;
}

SSGEDECL void SSGE_Object_Destroy(uint32_t id) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_Pop(&_object_list, id);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %u", id);
    _destroy_object(object);
}

SSGEDECL void SSGE_Object_DestroyName(char *name) {
    _assert_engine_init
    SSGE_Object *object = SSGE_Array_FindPop(&_object_list, _find_object_name, name);
    if (object == NULL) 
        SSGE_ErrorEx("Object not found: %s", name);
    _destroy_object(object);
}

SSGEDECL void SSGE_Object_DestroyAll() {
    _assert_engine_init
    SSGE_Array_Destroy(&_object_list, _destroy_object);
    SSGE_Array_Create(&_object_list);
}

SSGEDECL bool SSGE_Object_IsColliding(SSGE_Object *hitbox1, SSGE_Object *hitbox2) {
    if (!hitbox1->hitbox || !hitbox2->hitbox) return false;
    return hitbox1->x < hitbox2->x + hitbox2->width && hitbox1->x + hitbox1->width > hitbox2->x && hitbox1->y < hitbox2->y + hitbox2->height && hitbox1->y + hitbox1->height > hitbox2->y;
}