#include <stdlib.h>
#include <stdatomic.h>

#define SSGE_GET_SDL

#include "SSGE/SSGE.h"
#include "SSGE/SSGE_local.h"

static int _eventFilter(void *userdata, SDL_Event *event) {
    switch (event->type) {
        case SSGE_EVENT_FIRSTEVENT:
        case SSGE_EVENT_QUIT:
        case SSGE_EVENT_WINDOWEVENT:
        case SSGE_EVENT_KEYDOWN:
        case SSGE_EVENT_KEYUP:
        case SSGE_EVENT_MOUSEMOTION:
        case SSGE_EVENT_MOUSEBUTTONDOWN:
        case SSGE_EVENT_MOUSEBUTTONUP:
        case SSGE_EVENT_MOUSEWHEEL:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

SSGEAPI const SSGE_Engine * const SSGE_Init(char *title, uint16_t width, uint16_t height, uint16_t fps) {
    if (_engine.initialized == true)
        SSGE_Error("Engine already initialized")

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SSGE_ErrorEx("Failed to initialize SDL: %s", SDL_GetError())

    if (TTF_Init() != 0)
        SSGE_ErrorEx("Failed to initialize TTF: %s", TTF_GetError())

    _engine.title = (_windowReq.title = (char *)malloc(strlen(title)));
    strcpy(_engine.title, title);
    _engine.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (_windowReq.width = width), (_windowReq.height = height), 0);
    if (_engine.window == NULL)
        SSGE_ErrorEx("Failed to create window: %s", SDL_GetError())

    _engine.renderer = SDL_CreateRenderer(_engine.window, -1, SDL_RENDERER_ACCELERATED);
    if (_engine.renderer == NULL)
        SSGE_ErrorEx("Failed to create renderer: %s", SDL_GetError())

    if (SDL_SetRenderDrawBlendMode(_engine.renderer, SDL_BLENDMODE_BLEND) != 0)
        SSGE_ErrorEx("Failed to set renderer to blend mode: %s", SDL_GetError())

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_WAVPACK) == 0) 
        SSGE_ErrorEx("Failed to initialize audio mixer: %s", SDL_GetError())

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) 
        SSGE_ErrorEx("Failed to open audio device for playback: %s", SDL_GetError())

    SDL_SetEventFilter(_eventFilter, NULL);

    SDL_SetRenderDrawColor(_engine.renderer, 0, 0, 0, 255);

    SSGE_Array_Create(&_textureList);
    SSGE_Array_Create(&_objectList);
    SSGE_Array_Create(&_objectTemplateList);
    SSGE_Array_Create(&_fontList);
    SSGE_Array_Create(&_audioList);
    SSGE_Array_Create(&_animationList);
    SSGE_Array_Create(&_playingAnim);

    _engine.icon = NULL;
    _engine.fps = fps;
    _engine.maxFrameskip = _MAX_FRAMESKIP;
    _engine.vsync = false;
    _engine.vsyncRate = 0;
    _engine.isRunning = false;
    _engine.initialized = true;
    _engine.fullscreen = false;
    _engine.resizable = false;

    return &_engine;
}

SSGEAPI void SSGE_Quit() {
    _assert_engine_init

    SSGE_Array_Destroy(&_objectList, (_SSGE_Destroy)destroyObject);
    SSGE_Array_Destroy(&_objectTemplateList, (_SSGE_Destroy)destroyTemplate);
    SSGE_Array_Destroy(&_fontList, (_SSGE_Destroy)destroyFont);
    SSGE_Array_Destroy(&_audioList, (_SSGE_Destroy)destroyAudio);
    SSGE_Array_Destroy(&_animationList, (_SSGE_Destroy)destroyAnimation);
    SSGE_Array_Destroy(&_playingAnim, free);
    SSGE_Array_Destroy(&_textureList, (_SSGE_Destroy)destroyTexture);

    free(_engine.title);
    SDL_FreeSurface(_engine.icon);

    SDL_DestroyRenderer(_engine.renderer);
    SDL_DestroyWindow(_engine.window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

inline static void _initDoubleBuffering(_SSGE_DoubleRenderBuffer *doubleBuffer) {
    SSGE_Array_Create(&doubleBuffer->buffers[0].renderQueue);
    SSGE_Array_Create(&doubleBuffer->buffers[1].renderQueue);

    doubleBuffer->buffers[0].ready = false;
    doubleBuffer->buffers[1].ready = false;
    doubleBuffer->buffers[0].inUse = false;
    doubleBuffer->buffers[1].inUse = false;
    
    atomic_store(&doubleBuffer->writeBuffer, 0);
    atomic_store(&doubleBuffer->readBuffer, 1);
    
    atomic_store(&doubleBuffer->framesGenerated, 0);
    atomic_store(&doubleBuffer->framesRendered, 0);

    atomic_store(&doubleBuffer->evHandlerBusy, false);
    atomic_store(&doubleBuffer->evQueueBusy, false);
}

inline static bool _isTextureVisible(int x, int y, int width, int height) {
    if (x + width < 0) return false;
    if (x > _engine.width) return false;
    if (y + height < 0) return false;
    if (y > _engine.height) return false;
    
    return true;
}

inline static void _renderTextures(_SSGE_DoubleRenderBuffer *doubleBuffer) {
    int readIdx = atomic_load(&doubleBuffer->readBuffer);
    _SSGE_RenderBuffer *readBuffer = &doubleBuffer->buffers[readIdx];

    if (!atomic_load(&readBuffer->ready)) return;


    for (int i = 0; i < readBuffer->renderQueue.count; i++) {
        _SSGE_BufferedRenderItem *item = SSGE_Array_Get(&readBuffer->renderQueue, i);
        if (!item) continue;

        SSGE_Texture *texture = item->texture;
        if (atomic_load(&texture->markedForDestroy))
            continue;

        SDL_Texture *sdlTexture = texture->texture;
        _SSGE_RenderData *array = item->renderDatas;

        for (int j = 0; j < item->count; j++) {
            _SSGE_RenderData data = array[j];
            if (!_isTextureVisible(data.x, data.y, data.width, data.height)) 
                continue;
    
            SDL_Rect rect = {
                data.x + texture->anchorX, 
                data.y + texture->anchorY, 
                data.width, 
                data.height
            };
    
            if (data.angle == 0 && data.flip == 0) SDL_RenderCopy(_engine.renderer, sdlTexture, NULL, &rect);
            else SDL_RenderCopyEx(_engine.renderer, sdlTexture, NULL, &rect, data.angle, (SDL_Point *)&data.rotationCenter, data.flip);
        }
    }

    atomic_store(&readBuffer->inUse, false);
    atomic_fetch_add(&doubleBuffer->framesRendered, 1);
}

inline static void _copyGameStateToBuffer(_SSGE_RenderBuffer *buffer) {
    uint32_t count = _textureList.count;
    for (uint32_t i = 0; i < count; i++) {
        SSGE_Texture *texture = SSGE_Array_Get(&_textureList, i);
        if (!texture || texture->markedForDestroy || texture->queue.count == 0) continue;

        _SSGE_BufferedRenderItem *item = (_SSGE_BufferedRenderItem *)malloc(sizeof(_SSGE_BufferedRenderItem));
        _SSGE_RenderData *array = (_SSGE_RenderData *)malloc(sizeof(_SSGE_RenderData) * texture->queue.count);
        item->renderDatas = array;
        item->texture = texture;
        item->count = texture->queue.count;

        textureAcquire(texture);

        for (int j = 0; j < texture->queue.count; j++) {
            _SSGE_RenderData *data = SSGE_Array_Get(&texture->queue, j);
            if (!data) continue;

            item->renderDatas[j] = *data;

            if (data->once) free(SSGE_Array_Pop(&texture->queue, j)); // free the render data if once is set
        }

        SSGE_Array_Add(&buffer->renderQueue, item);
    }
}

static int _updateThreadFunc(_SSGE_UpdThreadData *data) {
    _SSGE_DoubleRenderBuffer *doubleBuffer = data->doubleBuffer;
    SSGE_Array *evQueue = &data->eventQueue;
    void (*update)(void *) = data->update;
    void (*eventHandler)(SSGE_Event, void *) = data->eventHandler;
    void *updData = data->data;

    while (_engine.isRunning) {
        if (eventHandler && !atomic_load(&doubleBuffer->evQueueBusy)) {
            atomic_store(&doubleBuffer->evHandlerBusy, true);
            uint32_t count = evQueue->count;
            uint32_t i = 0, done = 0;
            SSGE_Event *event;
            while (done < count) {
                event = SSGE_Array_Pop(evQueue, i++);
                if (!event) continue;
                eventHandler(*event, updData);
                free(event);
                ++done;
            }
            atomic_store(&doubleBuffer->evHandlerBusy, false);
        }

        int writeIdx = atomic_load(&doubleBuffer->writeBuffer);
        _SSGE_RenderBuffer *writeBuffer = &doubleBuffer->buffers[writeIdx];

        atomic_store(&writeBuffer->ready, false);

        SSGE_Array_Destroy(&writeBuffer->renderQueue, destroyBufferedRenderItem);
        SSGE_Array_Create(&writeBuffer->renderQueue);

        while (atomic_load(&doubleBuffer->evHandlerBusy) && _engine.isRunning) SDL_Delay(0);
        if (!_engine.isRunning) return 0;

        uintmax_t generated = atomic_fetch_add(&doubleBuffer->framesGenerated, 1);
        uintmax_t rendered = atomic_load(&doubleBuffer->framesRendered);
        int framesBehind = (int)(rendered - generated);
        if (framesBehind > 0) {
            int framesToCatchUp = (framesBehind > _engine.maxFrameskip) ? _engine.maxFrameskip : framesBehind;

            for (int i = 0; i < framesToCatchUp; i++) {
                update(updData);
                atomic_fetch_add(&doubleBuffer->framesGenerated, 1);
            }
        }

        update(updData);

        _copyGameStateToBuffer(writeBuffer);

        atomic_store(&writeBuffer->ready, true);
        atomic_store(&writeBuffer->inUse, true);

        int readIdx = atomic_load(&doubleBuffer->readBuffer);
        atomic_bool *inUse = &doubleBuffer->buffers[readIdx].inUse;
        while (atomic_load(inUse) && _engine.isRunning) SDL_Delay(0);
        if (!_engine.isRunning) return 0;

        atomic_store(&doubleBuffer->writeBuffer, readIdx);
        atomic_store(&doubleBuffer->readBuffer, writeIdx);
    }
    return 0;
}

inline static void changeWindowState() {
    if (_windowReq.width != _engine.width || _windowReq.height != _engine.height)
        SDL_SetWindowSize(_engine.window, (_engine.width = _windowReq.width), (_engine.height = _windowReq.height));
    if (_windowReq.fullscreen != _engine.fullscreen)
        SDL_SetWindowFullscreen(_engine.window, (_engine.fullscreen = _windowReq.fullscreen));
    if (_windowReq.resizable != _engine.resizable)
        SDL_SetWindowResizable(_engine.window, (_engine.resizable = _windowReq.resizable));
    if (_windowReq.title != _engine.title) {
        free(_engine.title);
        SDL_SetWindowTitle(_engine.window, (_engine.title = _windowReq.title));
    }
    if (_windowReq.icon != _engine.icon) {
        SDL_FreeSurface(_engine.icon);
        SDL_SetWindowIcon(_engine.window, (_engine.icon = _windowReq.icon));
    }

    _windowReq.changed = false;
}

SSGEAPI void SSGE_Run(void (*update)(void *), void (*draw)(void *), void (*eventHandler)(SSGE_Event, void *), void *data) {
    _assert_engine_init

    uint64_t frameStart;
    double targetFrameTime = 1000.0 / (double)_engine.fps;
    
    _SSGE_DoubleRenderBuffer doubleBuffer = {0};
    _initDoubleBuffering(&doubleBuffer);
    
    _SSGE_UpdThreadData threadData = {0};
    SDL_Thread *updateThread = NULL;
    if (update || eventHandler) {
        threadData = (_SSGE_UpdThreadData){
            .update = update,
            .eventHandler = eventHandler,
            .data = data,
            .doubleBuffer = &doubleBuffer
        };
        if (eventHandler) SSGE_Array_Create(&threadData.eventQueue);
        updateThread = SDL_CreateThread((SDL_ThreadFunction)_updateThreadFunc, "SSGE Update", &threadData);
        if (!updateThread)
            SSGE_ErrorEx("Failed to create update thread: %s", SDL_GetError());
    }

    _engine.isRunning = true;
    
    while (_engine.isRunning) {
        frameStart = SDL_GetTicks64();

        // Check if the window state has changed, and if it has, apply the changes
        if (_windowReq.changed) changeWindowState();

        while (atomic_load(&doubleBuffer.evHandlerBusy)) SDL_Delay(0);
        atomic_store(&doubleBuffer.evQueueBusy, true);
        while (SDL_PollEvent((SDL_Event *)&_event)) {
            if (_event.type == SDL_QUIT) {
                _engine.isRunning = false;
                SDL_WaitThread(updateThread, NULL);
                SSGE_Array_Destroy(&threadData.eventQueue, free);
                SSGE_Array_Destroy(&doubleBuffer.buffers[0].renderQueue, destroyBufferedRenderItem);
                SSGE_Array_Destroy(&doubleBuffer.buffers[1].renderQueue, destroyBufferedRenderItem);
                return;
            }
            if (eventHandler) {
                SSGE_Event *buffer = (SSGE_Event *)malloc(sizeof(SSGE_Event));
                *buffer = _event;
                SSGE_Array_Add(&threadData.eventQueue, buffer);
            }
        }
        atomic_store(&doubleBuffer.evQueueBusy, false);
        
        if (_updateFrame || !_manualUpdateFrame || _engine.vsync) {
            SDL_SetRenderDrawColor(_engine.renderer, _bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
            SDL_RenderClear(_engine.renderer);
            SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
            _renderTextures(&doubleBuffer);
            if (draw) draw(data);
            
            SDL_RenderPresent(_engine.renderer);
            _updateFrame = false;
        }

        if (!_engine.vsync) {
            uint64_t frameTime = SDL_GetTicks64() - frameStart;
            if ((double)frameTime < targetFrameTime)
                SDL_Delay((uint32_t)(targetFrameTime - (double)frameTime));
        }
    }
}

SSGEAPI void SSGE_SetWindowTitle(char *title) {
    _assert_engine_init
    _windowReq.title = (char *)malloc(strlen(title));
    strcpy(_windowReq.title, title);
    _windowReq.changed = true;
}

SSGEAPI void SSGE_SetWindowIcon(char *filename) {
    _assert_engine_init
    _windowReq.icon = IMG_Load(filename);
    if (_windowReq.icon == NULL) {
        SSGE_ErrorEx("Failed to load icon: %s", IMG_GetError())
        return;
    }
    _windowReq.changed = true;
}

SSGEAPI void SSGE_WindowResize(uint16_t width, uint16_t height) {
    _assert_engine_init
    _windowReq.width = width;
    _windowReq.height = height;
    _windowReq.changed = true;
}

SSGEAPI void SSGE_WindowResizable(bool resizable) {
    _assert_engine_init
    _windowReq.resizable = resizable;
    _windowReq.changed = true;
}

SSGEAPI void SSGE_WindowFullscreen(SSGE_WindowMode fullscreen) {
    _assert_engine_init
    _windowReq.fullscreen = fullscreen;
    _windowReq.changed = true;
}

SSGEAPI void SSGE_SetFrameskipMax(uint8_t max) {
    _assert_engine_init
    _engine.maxFrameskip = max;
}

SSGEAPI void SSGE_SetVSync(bool vsync) {
    _assert_engine_init
    if (SDL_RenderSetVSync(_engine.renderer, (_engine.vsync = vsync)) != 0) {
        _engine.vsync = false;
        SSGE_WarningEx("Failed to enable VSync, fallback to %d fps", _engine.fps);
    }
    if (vsync) {
        SDL_DisplayMode mode;
        int displayIndex = SDL_GetWindowDisplayIndex(_engine.window);
        if (SDL_GetCurrentDisplayMode(displayIndex, &mode) == 0) {
            _engine.vsyncRate = (uint32_t)mode.refresh_rate;
        } else _engine.vsyncRate = (uint32_t)_engine.fps; // fallback to fps
    }
}

SSGEAPI void SSGE_SetManualUpdate(bool manualUpdate) {
    _assert_engine_init
    _manualUpdateFrame = manualUpdate;
}

SSGEAPI void SSGE_ManualUpdate() {
    _updateFrame = true;
}

SSGEAPI void SSGE_SetColor(SSGE_Color color) {
    _assert_engine_init
    _color = color;
    SDL_SetRenderDrawColor(_engine.renderer, color.r, color.g, color.b, color.a);
}

SSGEAPI void SSGE_SetBackgroundColor(SSGE_Color color) {
    _assert_engine_init
    _bgColor = color;
}

SSGEAPI void SSGE_GetMousePosition(int *x, int *y) {
    _assert_engine_init
    SDL_GetMouseState(x, y);
}

SSGEAPI bool SSGE_ObjectIsHovered(SSGE_Object *object) {
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

SSGEAPI SSGE_Object *SSGE_GetHoveredObject() {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);

    return SSGE_Array_Find(&_objectList, (bool (*)(void *, void *))_is_hovered, mousePos);
}

SSGEAPI uint32_t SSGE_GetHoveredObjects(SSGE_Object *objects[], uint32_t size) {
    _assert_engine_init
    int mousePos[2];
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);
    
    uint32_t i = 0, count = 0;
    while ((i < _objectList.count || i >= _objectList.size) && count < size) {
        SSGE_Object *obj = _objectList.array[i++];

        if (obj != NULL && _is_hovered(obj, mousePos))
            objects[count++] = obj;
    }
    return count;
}
