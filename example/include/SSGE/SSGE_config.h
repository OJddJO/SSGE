#ifndef __SSGE_CONFIG_H__
#define __SSGE_CONFIG_H__

#ifdef DLL_EXPORT
    #define SSGEDECL __declspec(dllexport)
#else
    #define SSGEDECL
#endif

#ifdef SSGE_WANT_SDL2
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_mixer.h>
#elif defined(SSGE_WANT_FAKE_SDL2)
    typedef union SDL_Event;
    typedef struct SDL_Window;
    typedef struct SDL_Renderer;
    typedef struct SDL_Texture;
    typedef struct SDL_Surface;
    typedef struct _TTF_Font;
    typedef struct Mix_Chunk;
#endif

#ifdef SSGE_WANT_SDL2
#undef SSGE_WANT_SDL2
#endif
#ifdef SSGE_WANT_FAKE_SDL2
#undef SSGE_WANT_FAKE_SDL2
#endif

#endif // __SSGE_CONFIG_H__