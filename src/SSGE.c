#include <stdlib.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE.h"
#include "SSGE/SSGE_local.h"

static int _event_filter(void *userdata, SDL_Event *event) {
    switch (event->type) {
        case SSGE_FIRSTEVENT:
        case SSGE_QUIT:
        case SSGE_WINDOWEVENT:
        case SSGE_KEYDOWN:
        case SSGE_KEYUP:
        case SSGE_MOUSEMOTION:
        case SSGE_MOUSEBUTTONDOWN:
        case SSGE_MOUSEBUTTONUP:
        case SSGE_MOUSEWHEEL:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

SSGEDECL SSGE_Engine *SSGE_Init(char *title, uint16_t width, uint16_t height, uint16_t fps) {
    if (_engine.initialized)
        SSGE_Error("Engine already initialized")

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SSGE_ErrorEx("Failed to initialize SDL: %s", SDL_GetError())

    if (TTF_Init() != 0)
        SSGE_ErrorEx("Failed to initialize TTF: %s", TTF_GetError())

    _engine.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (_engine.window == NULL)
        SSGE_ErrorEx("Failed to create window: %s", SDL_GetError())

    _engine.renderer = SDL_CreateRenderer(_engine.window, -1, SDL_RENDERER_ACCELERATED);
    if (_engine.renderer == NULL)
        SSGE_ErrorEx("Failed to create renderer: %s", SDL_GetError())

    if (SDL_SetRenderDrawBlendMode(_engine.renderer, SDL_BLENDMODE_BLEND) != 0)
        SSGE_ErrorEx("Failed to set renderer to blend mode: %s", SDL_GetError())

    if (Mix_Init(MIX_INIT_MP3 || MIX_INIT_OGG || MIX_INIT_WAVPACK) == 0) 
        SSGE_ErrorEx("Failed to initialize audio mixer: %s", SDL_GetError())

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) 
        SSGE_ErrorEx("Failed to open audio device for playback: %s", SDL_GetError())

    SDL_SetEventFilter(_event_filter, NULL);

    SDL_SetRenderDrawColor(_engine.renderer, 0, 0, 0, 255);

    SSGE_Array_Create(&_texture_list);
    SSGE_Array_Create(&_object_list);
    SSGE_Array_Create(&_object_template_list);
    SSGE_Array_Create(&_font_list);
    SSGE_Array_Create(&_audio_list);
    SSGE_Array_Create(&_animation_list);
    SSGE_Array_Create(&_playingAnim);

    _engine.isRunning = false;
    _engine.width = width;
    _engine.height = height;
    _engine.fps = fps;
    _engine.initialized = true;

    return &_engine;
}

SSGEDECL void SSGE_Quit() {
    _assert_engine_init

    SSGE_Array_Destroy(&_texture_list, _destroy_texture);
    SSGE_Array_Destroy(&_object_list, _destroy_object);
    SSGE_Array_Destroy(&_object_template_list, _destroy_template);
    SSGE_Array_Destroy(&_font_list, _destroy_font);
    SSGE_Array_Destroy(&_audio_list, _destroy_audio);
    SSGE_Array_Destroy(&_animation_list, _destroy_animation);
    SSGE_Array_Destroy(&_playingAnim, free);

    SDL_DestroyRenderer(_engine.renderer);
    SDL_DestroyWindow(_engine.window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

// For update threading
static SDL_mutex *_updateMutex;
static SDL_cond *_updateCond;
static bool _updateDone = 0;
typedef struct _updThreadData {
    void (*update)(void *);
    void *data;
} updThreadData;

static int updateThreadFunc(void *data) {
    while (_engine.isRunning) {
        SDL_LockMutex(_updateMutex);
        while (_updateDone)
            SDL_CondWait(_updateCond, _updateMutex);

        ((updThreadData *)data)->update(((updThreadData *)data)->data);
        _updateDone = 1;
        SDL_CondSignal(_updateCond);
        SDL_UnlockMutex(_updateMutex);
    }
    return 0;
}

SSGEDECL void SSGE_Run(void (*update)(void *), void (*draw)(void *), void (*eventHandler)(SSGE_Event, void *), void *data) {
    _assert_engine_init

    _updateMutex = SDL_CreateMutex();
    _updateCond = SDL_CreateCond();
    _updateDone = 0;

    SDL_CreateThread((SDL_ThreadFunction)updateThreadFunc, "Update Thread", (&(updThreadData){update, data}));

    uint32_t frameStart;
    int frameTime;

    _engine.isRunning = true;

    // first update
    if (update) {
        SDL_LockMutex(_updateMutex);
        _updateDone = 0;
        SDL_CondSignal(_updateCond);
    }

    while (_engine.isRunning) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent((SDL_Event *)&_event)) {
            if (_event.type == SDL_QUIT)
                _engine.isRunning = false;
            if (eventHandler)
                eventHandler(_event, data);
        }

        if (update) { // Synchronize update thread with main thread
            while (!_updateDone)
                SDL_CondWait(_updateCond, _updateMutex);
            SDL_UnlockMutex(_updateMutex);
        }

        if (_update_frame || !_manual_update_frame) {
            SDL_SetRenderDrawColor(_engine.renderer, _bg_color.r, _bg_color.g, _bg_color.b, _bg_color.a);
            SDL_RenderClear(_engine.renderer);
            SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
            if (draw) draw(data);
            _update_frame = false;
        }

        if (update) {
            SDL_LockMutex(_updateMutex);
            _updateDone = 0;
            SDL_CondSignal(_updateCond);
        }

        if (_update_frame || !_manual_update_frame)
            SDL_RenderPresent(_engine.renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if ((double)frameTime < (double)1000 / _engine.fps)
            SDL_Delay((1000 / _engine.fps) - frameTime);
    }

}

SSGEDECL void SSGE_SetWindowTitle(char *title) {
    _assert_engine_init
    SDL_SetWindowTitle(_engine.window, title);
}

SSGEDECL void SSGE_SetWindowIcon(char *filename) {
    _assert_engine_init
    SDL_Surface *icon = IMG_Load(filename);
    if (icon == NULL) {
        SSGE_ErrorEx("Failed to load icon: %s", IMG_GetError())
        return;
    }
    SDL_SetWindowIcon(_engine.window, icon);
    SDL_FreeSurface(icon);
}

SSGEDECL void SSGE_WindowResize(uint16_t width, uint16_t height) {
    _assert_engine_init
    SDL_SetWindowSize(_engine.window, width, height);
}

SSGEDECL void SSGE_WindowResizable(bool resizable) {
    _assert_engine_init
    SDL_SetWindowResizable(_engine.window, resizable ? SDL_TRUE : SDL_FALSE);
}

SSGEDECL void SSGE_WindowFullscreen(bool fullscreen) {
    _assert_engine_init
    SDL_SetWindowFullscreen(_engine.window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

SSGEDECL void SSGE_SetManualUpdate(bool manualUpdate) {
    _assert_engine_init
    _manual_update_frame = manualUpdate;
}

SSGEDECL void SSGE_ManualUpdate() {
    _update_frame = true;
}

SSGEDECL void SSGE_SetColor(SSGE_Color color) {
    _assert_engine_init
    _color = color;
    SDL_SetRenderDrawColor(_engine.renderer, color.r, color.g, color.b, color.a);
}

SSGEDECL void SSGE_SetBackgroundColor(SSGE_Color color) {
    _assert_engine_init
    _bg_color = color;
}

SSGEDECL void SSGE_GetMousePosition(int *x, int *y) {
    _assert_engine_init
    SDL_GetMouseState(x, y);
}

SSGEDECL bool SSGE_ObjectIsHovered(SSGE_Object *object) {
    _assert_engine_init
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return mouseX >= object->x && mouseX <= object->x + object->width && mouseY >= object->y && mouseY <= object->y + object->height;
}

static bool _is_hovered(SSGE_Object *ptr, int *mousePos) {
    int mouseX = mousePos[0],
        mouseY = mousePos[1];
    return mouseX >= ptr->x && mouseX <= ptr->x + ptr->width && mouseY >= ptr->y && mouseY <= ptr->y + ptr->height;
}

SSGEDECL SSGE_Object *SSGE_GetHoveredObject() {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
    return SSGE_Array_Find(&_object_list, _is_hovered, mousePos);
    #pragma GCC diagnostic pop
}

SSGEDECL uint32_t SSGE_GetHoveredObjects(SSGE_Object *objects[], uint32_t size) {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);
    
    uint32_t i = 0, count = 0;
    while ((i < _object_list.count || i >= _object_list.size) && count < size) {
        SSGE_Object *obj = _object_list.array[i++];

        if (obj != NULL && _is_hovered(obj, mousePos))
            objects[count++] = obj;
        
        i++;
    }
    return count;
}
