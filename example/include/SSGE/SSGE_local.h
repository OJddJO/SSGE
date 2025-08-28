// This file should not be included
// It is used internally by the SSGE library

#ifndef __SSGE_LOCAL_H__
#define __SSGE_LOCAL_H__

#include <string.h>
#include "SSGE/SSGE_types.h"
#include "SSGE/SSGE_array.h"
#include "SSGE/SSGE_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _PLAYING_ANIM_INITIAL_SIZE  64
#define _PLAYING_ANIM_GROWTH_FACTOR 2
#define _MAX_FRAMESKIP              3

extern SSGE_Engine  _engine;
extern SSGE_Array   _textureList;
extern SSGE_Array   _objectList;
extern SSGE_Array   _objectTemplateList;
extern SSGE_Array   _fontList;
extern SSGE_Array   _audioList;
extern SSGE_Array   _animationList;
extern SSGE_Array   _playingAnim;
extern SSGE_Event   _event;
extern SSGE_Color   _color;
extern SSGE_Color   _bgColor;
extern bool         _manualUpdateFrame;
extern bool         _updateFrame;

#define _assert_engine_init \
if (!_engine.initialized) {\
    fprintf(stderr, "[SSGE][%s] Engine not initialized\n", __func__);\
    exit(1);\
}\

// Dummy type
typedef struct _DummyType {
    char *name;
    uint32_t id;
} DummyType;

inline void _add_to_list(SSGE_Array *list, void *element, char *name, uint32_t *id, const char *funcname) {
    ((DummyType *)element)->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    if (((DummyType *)element)->name == NULL) {
        fprintf(stderr, "[SSGE][%s] Failed to allocate memory for name\n", funcname);
        exit(1);
    }
    strcpy(((DummyType *)element)->name, name);

    if (id == NULL) {
        fprintf(stderr, "[SSGE][%s] ID is discarded\n", funcname);
        ((DummyType *)element)->id = SSGE_Array_Add(list, element);
        return;
    }
    *id = (((DummyType *)element)->id = SSGE_Array_Add(list, element));
}

void _destroy_texture(void *ptr);
void _destroy_object(void *ptr);
void _destroy_template(void *ptr);
void _destroy_font(void *ptr);
void _destroy_audio(void *ptr);
void _destroy_animation(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // __SSGE_LOCAL_H__