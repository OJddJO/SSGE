#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "SSGE/SSGE_local.h"

SSGE_Engine _engine             = {0};
SSGE_Array _texture_list        = {0};
SSGE_Array _object_list         = {0};
SSGE_Array _object_template_list = {0};
SSGE_Array _font_list           = {0};
SSGE_Array _audio_list          = {0};
SSGE_Array _animation_list      = {0};
SSGE_Event _event               = {0};
SSGE_Color _color               = {0, 0, 0, 255};
SSGE_Color _bg_color            = {0, 0, 0, 255};
bool _manual_update_frame       = false;
bool _update_frame              = true; // set to true to draw the first frame

uint32_t _add_to_list(SSGE_Array *list, void *element, char *name, const char *funcname) {
    ((DummyType *)element)->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (((DummyType *)element)->name == NULL) {
        fprintf(stderr, "[SSGE][%s] Failed to allocate memory for name\n", funcname);
        exit(1);
    }
    strcpy(((DummyType *)element)->name, name);

    return ((DummyType *)element)->id = SSGE_Array_Add(list, element);
}

void _destroy_texture(void *ptr) {
    SSGE_Texture *texture = (SSGE_Texture *)ptr;
    SDL_DestroyTexture(texture->texture);
    free(texture->name);
    free(texture);
}

void _destroy_object(void *ptr) {
    SSGE_Object *object = (SSGE_Object *)ptr;
    free(object->name);
    if (object->destroyData != NULL)
        object->destroyData(object->data);
    free(object);
}

void _destroy_template(void *ptr) {
    SSGE_ObjectTemplate *template = (SSGE_ObjectTemplate *)ptr;
    free(template->name);
    free(template);
}

void _destroy_font(void *ptr) {
    SSGE_Font *font = (SSGE_Font *)ptr;
    TTF_CloseFont(font->font);
    free(font->name);
    free(font);
}

void _destroy_audio(void *ptr) {
    SSGE_Audio *audio = (SSGE_Audio *)ptr;
    Mix_FreeChunk(audio->audio);
    free(audio->name);
    free(audio);
}

void _destroy_animation(void *ptr) {
    SSGE_Animation *animation = (SSGE_Animation *)ptr;
    if (animation->type == SSGE_ANIM_FRAMES) {
        for (uint32_t i = 0; i < animation->data.frameCount; i++) {
            if (animation->data.frames[i] != NULL) {
                SDL_DestroyTexture(animation->data.frames[i]);
            }
        }
        free(animation->data.frames);
    }
    free(animation->name);
    free(animation);
}
