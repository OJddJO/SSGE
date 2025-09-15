#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <stdint.h>

#include "SSGE/SSGE.h"
#include "SSGE_local.h"

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

SSGEAPI const SSGE_Engine *SSGE_Init(char *title, uint16_t width, uint16_t height, uint16_t fps) {
    if (_engine.initialized == true)
        SSGE_Error("Engine already initialized")

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SSGE_ErrorEx("Failed to initialize SDL: %s", SDL_GetError())

    if (TTF_Init() != 0)
        SSGE_ErrorEx("Failed to initialize TTF: %s", TTF_GetError())

    if (!title)
        SSGE_Error("title can't be NULL")
    _engine.title = (char *)malloc(strlen(title) + 1);
    strcpy(_engine.title, title);
    _engine.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (_engine.window == NULL)
        SSGE_ErrorEx("Failed to create window: %s", SDL_GetError())

    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "1");
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
    _engine.width = width;
    _engine.height = height;
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

    SSGE_Array_Destroy(&_objectList, (SSGE_DestroyData)destroyObject);
    SSGE_Array_Destroy(&_objectTemplateList, (SSGE_DestroyData)destroyTemplate);
    SSGE_Array_Destroy(&_fontList, (SSGE_DestroyData)destroyFont);
    SSGE_Array_Destroy(&_audioList, (SSGE_DestroyData)destroyAudio);
    SSGE_Array_Destroy(&_animationList, (SSGE_DestroyData)destroyAnimation);
    SSGE_Array_Destroy(&_playingAnim, free);
    SSGE_Array_Destroy(&_textureList, (SSGE_DestroyData)destroyTexture);

    if (_engine.title) free(_engine.title);
    if (_engine.icon) SDL_FreeSurface(_engine.icon);

    SDL_DestroyRenderer(_engine.renderer);
    SDL_DestroyWindow(_engine.window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

inline static bool _isTextureVisible(int x, int y, int width, int height) {
    return !((x + width) < 0 || x >= _engine.width || (y + height) < 0 || y >= _engine.height);
}

SSGEAPI void SSGE_Run(SSGE_UpdateFunc update, SSGE_DrawFunc draw, SSGE_EventHandler eventHandler, void *data) {
    _assert_engine_init

    uint64_t frameStart;
    double targetFrameTime = 1000.0 / (double)(_engine.fps);
    uint64_t nextUpdate = 0;

    _engine.isRunning = true;
    SSGE_Event event = {0};

    while (_engine.isRunning) {
        frameStart = SDL_GetTicks64();
        
        while (SDL_PollEvent((SDL_Event *)&event)) {
            switch (event.type) {
                case SDL_QUIT:
                _engine.isRunning = false;
                break;
                case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        _engine.width = event.window.data1;
                        _engine.height = event.window.data2;
                        break;
                }
                break;
            }
            eventHandler(event, data);
        }
        
        int updateLoops = 0;
        uint64_t currentTime = SDL_GetTicks64();
        if (update) while (currentTime > nextUpdate && updateLoops++ < _engine.maxFrameskip) {
            update(data);
            nextUpdate += (uint64_t)targetFrameTime;
        }
        
        if (_updateFrame || !_manualUpdateFrame || _engine.vsync) {
            SDL_SetRenderDrawColor(_engine.renderer, _bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
            SDL_RenderClear(_engine.renderer);
            SDL_SetRenderDrawColor(_engine.renderer, _color.r, _color.g, _color.b, _color.a);
            for (uint32_t i = 0, textureDone = 0; textureDone < _textureList.count && i < _textureList.size; i++) {
                SSGE_Texture *texture = SSGE_Array_Get(&_textureList, i);
                if (!texture) continue;

                ++textureDone;
                SDL_Texture *sdlTexture = texture->texture;
                uint32_t count = texture->queue.count;
                uint32_t size = texture->queue.size;

                for (uint32_t j = 0, dataDone = 0; dataDone < count && j < size; j++) {
                    _SSGE_RenderData *data = SSGE_Array_Get(&texture->queue, j);
                    if (!data) continue;
                    
                    ++dataDone;
                    if (!_isTextureVisible(data->x, data->y, data->width, data->height)) continue;
                    SDL_Rect rect = {
                        data->x + texture->anchorX,
                        data->y + texture->anchorY,
                        data->width,
                        data->height,
                    };

                    if (data->angle == 0 && data->flip == 0) SDL_RenderCopy(_engine.renderer, sdlTexture, NULL, &rect);
                    else SDL_RenderCopyEx(_engine.renderer, sdlTexture, NULL, &rect, data->angle, (SDL_Point *)&data->rotationCenter, data->flip);

                    if (data->once) free(SSGE_Array_Pop(&texture->queue, j));
                }
            }
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
    if (!title) return;
    if (_engine.title) free(_engine.title);
    _engine.title = (char *)malloc(strlen(title) + 1);
    strcpy(_engine.title, title);
}

SSGEAPI void SSGE_SetWindowIcon(char *filename) {
    _assert_engine_init
    SDL_Surface *icon = IMG_Load(filename);
    if (icon == NULL)
        SSGE_ErrorEx("Failed to load icon: %s", IMG_GetError())

    if (_engine.icon) SDL_FreeSurface(_engine.icon);
    _engine.icon = icon;
}

SSGEAPI void SSGE_WindowResize(uint16_t width, uint16_t height) {
    _assert_engine_init
    _engine.width = width;
    _engine.height = height;
    SDL_SetWindowSize(_engine.window, width, height);
}

SSGEAPI void SSGE_WindowResizable(bool resizable) {
    _assert_engine_init
    _engine.resizable = resizable;
    SDL_SetWindowResizable(_engine.window, resizable);
}

SSGEAPI void SSGE_WindowFullscreen(SSGE_WindowMode fullscreen) {
    _assert_engine_init
    _engine.fullscreen = fullscreen;
    SDL_SetWindowFullscreen(_engine.window, fullscreen);
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
            _engine.vsyncRate = (uint16_t)mode.refresh_rate;
        } else {
            SSGE_WarningEx("Failed to get VSync rate: %s", SDL_GetError());
            _engine.vsyncRate = _engine.fps; // fallback to fps
        }
    }
}

SSGEAPI void SSGE_SetManualUpdate(bool manualUpdate) {
    _assert_engine_init
    _manualUpdateFrame = manualUpdate;
}

SSGEAPI void SSGE_ManualUpdate() {
    _assert_engine_init
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
