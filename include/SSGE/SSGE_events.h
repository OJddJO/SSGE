/* This header file essentially remap all SDL_Event, for more information see SDL_events.h*/

#ifndef __SSGE_EVENTS_H__
#define __SSGE_EVENTS_H__

#include <stdint.h>

typedef enum _SSGE_EventType {
    SSGE_FIRSTEVENT         = 0, // UNUSED
    // Application events
    SSGE_QUIT               = 0x100,
    SSGE_DISPLAYEVENT       = 0x150,
    // Window events
    SSGE_WINDOWEVENT        = 0x200,
    // Keyboard events
    SSGE_KEYDOWN            = 0x300,
    SSGE_KEYUP,
    // Mouse events
    SSGE_MOUSEMOTION        = 0x400,
    SSGE_MOUSEBUTTONDOWN,
    SSGE_MOUSEBUTTONUP,
    SSGE_MOUSEWHEEL,
} SSGE_EventType;

typedef struct _SSGE_CommonEvent {
    uint32_t type;
    uint32_t timestamp;
} SSGE_CommonEvent;

typedef struct _SSGE_DisplayEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t display;
    uint8_t event;
    uint8_t padding1;
    uint8_t padding2;
    uint8_t padding3;
    int32_t data1;
} SSGE_DisplayEvent;

typedef struct _SSGE_WindowEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint8_t event;
    uint8_t padding1;
    uint8_t padding2;
    uint8_t padding3;
    int32_t data1;
    int32_t data2;
} SSGE_WindowEvent;

typedef struct _SSGE_Keysym {
    uint32_t scancode;
    int32_t sym;
    uint16_t mod;
    uint32_t unused;
} SSGE_Keysym;

typedef struct _SSGE_KeyboardEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint8_t state;
    uint8_t repeat;
    uint8_t padding2;
    uint8_t padding3;
    SSGE_Keysym keysym;
} SSGE_KeyboardEvent;

typedef struct _SSGE_MouseMotionEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    uint32_t state;
    int32_t x;
    int32_t y;
    int32_t xrel;
    int32_t yrel;
} SSGE_MouseMotionEvent;

typedef struct _SSGE_MouseButtonEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    int8_t button;
    int8_t state;
    int8_t clicks;
    int8_t padding1;
    int32_t x;
    int32_t y;
} SSGE_MouseButtonEvent;

typedef struct _SSGE_MouseWheelEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint32_t which;
    int32_t x;
    int32_t y;
    uint32_t direction;
    float preciseX;
    float preciseY;
    int32_t mouseX;
    int32_t mouseY;
} SSGE_MouseWheelEvent;

typedef struct _SSGE_QuitEvent {
    uint32_t type;
    uint32_t timestamp;
} SSGE_QuitEvent;

typedef union _SSGE_Event {
    uint32_t type;
    SSGE_CommonEvent common;
    SSGE_DisplayEvent display;
    SSGE_WindowEvent window;
    SSGE_KeyboardEvent key;
    SSGE_MouseMotionEvent motion;
    SSGE_MouseButtonEvent button;
    SSGE_MouseWheelEvent wheel;
    SSGE_QuitEvent quit;
    uint8_t padding[sizeof(void *) <= 8 ? 56 : sizeof(void *) == 16 ? 64 : 3 * sizeof(void *)];
} SSGE_Event;

#endif //__SSGE_EVENTS_H__