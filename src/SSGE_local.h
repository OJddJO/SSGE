#ifndef __SSGE_LOCAL_H__
#define __SSGE_LOCAL_H__

#include <string.h>
#include <stdatomic.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "SSGE/SSGE_types.h"
#include "SSGE/SSGE_array.h"
#include "SSGE/SSGE_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _PLAYING_ANIM_INITIAL_SIZE  64
#define _PLAYING_ANIM_GROWTH_FACTOR 2
#define _MAX_FRAMESKIP              3

typedef struct {
    int         x;
    int         y;
    uint16_t    width;
    uint16_t    height;
    SSGE_Flip   flip;
    SSGE_Point  rotationCenter;
    double      angle;
    bool        once;
} _SSGE_RenderData;

// Dummy type
typedef struct _DummyType {
    char *name;
    uint32_t id;
} DummyType;

extern SSGE_Engine  _engine;
extern SSGE_Array   _textureList;
extern SSGE_Array   _objectList;
extern SSGE_Array   _objectTemplateList;
extern SSGE_Array   _fontList;
extern SSGE_Array   _audioList;
extern SSGE_Array   _animationList;
extern SSGE_Array   _playingAnim;
extern SSGE_Color   _color;
extern SSGE_Color   _bgColor;
extern bool         _manualUpdateFrame;
extern bool         _updateFrame;

#define _assert_engine_init \
if (!_engine.initialized) {\
    fprintf(stderr, "[SSGE][%s] Engine not initialized\n", __func__);\
    exit(1);\
}\

inline void _addToList(SSGE_Array *list, void *element, char *name, uint32_t *id, const char *funcname) {
    if (name) {
        ((DummyType *)element)->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
        if (((DummyType *)element)->name == NULL) {
            fprintf(stderr, "[SSGE][%s] Failed to allocate memory for name\n", funcname);
            exit(1);
        }
        strcpy(((DummyType *)element)->name, name);
    } else ((DummyType *)element)->name = NULL;

    if (id == NULL) {
        fprintf(stderr, "[SSGE][%s] ID is discarded\n", funcname);
        ((DummyType *)element)->id = SSGE_Array_Add(list, element);
        return;
    }
    *id = (((DummyType *)element)->id = SSGE_Array_Add(list, element));
}

inline void _initTextureFields(SSGE_Texture *texture) {
    texture->anchorX = 0;
    texture->anchorY = 0;

    SSGE_Array_Create(&texture->queue);
}

void destroyTexture(SSGE_Texture *ptr);
void destroyObject(SSGE_Object *ptr);
void destroyTemplate(SSGE_ObjectTemplate *ptr);
void destroyFont(SSGE_Font *ptr);
void destroyAudio(SSGE_Audio *ptr);
void destroyAnimation(SSGE_Animation *ptr);

void textureAcquire(SSGE_Texture *texture);
void textureRelease(SSGE_Texture *texture);

void destroyBufferedRenderItem(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // __SSGE_LOCAL_H__