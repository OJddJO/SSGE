#ifndef __SSGE_LOCAL_H__
#define __SSGE_LOCAL_H__

#include "SSGE/SSGE_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SSGE_Engine _engine;
extern SSGE_Array _texture_list;
extern SSGE_Array _object_list;
extern SSGE_Array _object_template_list;
extern SSGE_Array _font_list;
extern SSGE_Array _audio_list;
extern SSGE_Array _animation_list;
extern SSGE_Event _event;
extern SSGE_Color _color;
extern SSGE_Color _bg_color;
extern bool _manual_update_frame;
extern bool _update_frame;

#define Error(msg) {\
    fprintf(stderr, "[SSGE][%s] " msg "\n", __func__);\
    exit(1);\
}

#define ErrorEx(fmsg, arg) {\
    fprintf(stderr, "[SSGE][%s] " fmsg "\n", __func__, arg);\
    exit(1);\
}

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

void _destroy_texture(void *ptr);
void _destroy_object(void *ptr);
void _destroy_template(void *ptr);
void _destroy_font(void *ptr);
void _destroy_audio(void *ptr);

uint32_t _add_to_list(SSGE_Array *list, void *element, char *name, char *funcname);

#ifdef __cplusplus
}
#endif

#endif // __SSGE_LOCALVAR_H__