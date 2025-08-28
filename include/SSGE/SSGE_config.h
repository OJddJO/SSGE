// This file should not be included
// It is used internally by the SSGE library

#ifndef __SSGE_CONFIG_H__
#define __SSGE_CONFIG_H__

#ifdef _WIN32
    #ifndef SSGE_STATIC
        #define SSGEDECL __declspec(dllexport)
        #define SSGEIMPORT __declspec(dllimport)
    #else
        #define SSGEDECL
        #define SSGEIMPORT
    #endif
#else
    #define SSGEDECL
    #define SSGEIMPORT
#endif

#ifdef _WIN32
    #define SSGECALL __cdecl
#else
    #define SSGECALL
#endif

#ifdef SSGE_BUILD
    #define SSGEAPI SSGEDECL SSGECALL
#else
    #define SSGEAPI SSGEIMPORT SSGECALL
#endif


#ifdef SSGE_GET_SDL
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_mixer.h>
    #undef SSGE_GET_SDL
#endif

#endif // __SSGE_CONFIG_H__