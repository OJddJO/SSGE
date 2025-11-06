#ifndef __SSGE_H__
#define __SSGE_H__

#define SSGE_VERSION_MAJOR 3
#define SSGE_VERSION_MINOR 1
#define SSGE_VERSION_PATCH 1

#include <stdbool.h>
#include <stdint.h>

#include "SSGE/SSGE_config.h"
#include "SSGE/SSGE_types.h"

#include "SSGE/SSGE_texture.h"
#include "SSGE/SSGE_animation.h"
#include "SSGE/SSGE_tilemap.h"
#include "SSGE/SSGE_object.h"
#include "SSGE/SSGE_objtemplate.h"
#include "SSGE/SSGE_geometry.h"
#include "SSGE/SSGE_text.h"
#include "SSGE/SSGE_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the engine
 * \param title The title of the window
 * \param width The window width
 * \param height The window height
 * \param fps The frames per second
 */
SSGEAPI void SSGE_Init(const char *title, uint16_t width, uint16_t height, uint16_t fps);

/**
 * Quit the engine
 * \note This function must be called at the end of the program
 */
SSGEAPI void SSGE_Quit();

/**
 * Run the engine
 * \param update The update function, can be `NULL`
 * \param background The function to draw the background, can be `NULL`
 * \param draw The draw function, can be `NULL`
 * \param eventHandler The event handler function, can be `NULL`
 * \param data The `void *` to pass to the functions (update, background, draw, eventHandler), can be `NULL`
 * \warning The engine runs in an infinite loop until the window is closed
 * \note The order of execution is as follows: Event handling, Update, (Clear screen), (Object Rendering), Draw
 */
SSGEAPI void SSGE_Run(SSGE_UpdateFunc update, SSGE_DrawFunc background, SSGE_DrawFunc draw, SSGE_EventHandler eventHandler, void *data);

// Window functions

/**
 * Set the window title
 * \param title The title of the window
 */
SSGEAPI void SSGE_SetWindowTitle(const char *title);

/**
 * Set the window icon
 * \param filename The path to the icon
 */
SSGEAPI void SSGE_SetWindowIcon(const char *filename);

/**
 * Resize the window
 * \param width The target width
 * \param height The target height
 */
SSGEAPI void SSGE_WindowResize(uint16_t width, uint16_t height);

/**
 * Set if the window is resizable
 * \param resizable True if the window should be resizable, false otherwise
 */
SSGEAPI void SSGE_WindowResizable(bool resizable);

/**
 * Set the window screen mode
 * \param fullscreen True if the window should be fullscreen, false otherwise
 */
SSGEAPI void SSGE_WindowFullscreen(SSGE_WindowMode fullscreen);

/**
 * Set the max number of frame to skip when frameskipping
 * \param max The max to set
 */
SSGEAPI void SSGE_SetFrameskipMax(uint8_t max);

/**
 * Enable or disable VSync
 * \param vsync If VSync should be enabled or disabled
 */
SSGEAPI void SSGE_SetVSync(bool vsync);

/**
 * Enable or disable the manual update mode
 * \param manualUpdate True if the manual update mode should be enabled, false otherwise
 * \note This function should be called before the `SSGE_Run` function
 * \note When the manual update mode is enabled, the screen will only be cleared and updated when the `SSGE_ManualUpdate` function is called.
 * \note Setting the manual update mode may be more efficient when the screen does not need to be updated every frame
 * \note If VSync is enabled, the manual update mode is ignored
 */
SSGEAPI void SSGE_SetManualUpdate(bool manualUpdate);

/**
 * Manually updates the screen
 * \note This function should be called when the manual update mode is enabled
 * \note It does nothing if the manual update mode is disabled
 */
SSGEAPI void SSGE_ManualUpdate();

/**
 * Change the background color
 * \param color The color to set
 */
SSGEAPI void SSGE_SetBackgroundColor(SSGE_Color color);

/**
 * Get the mouse position
 * \param x The variable to store the x coordinate of the mouse
 * \param y The variable to store the y coordinate of the mouse
 * \note To get the mouse position in the event handler, please use the coordinates in the `SSGE_Event` struct
 */
SSGEAPI void SSGE_GetMousePosition(int *x, int *y);

/**
 * Get the window size
 * \param width The variable to store the width of the window
 * \param height The variable to store the height of the window
 */
SSGEAPI void SSGE_GetWindowSize(uint16_t *width, uint16_t *height);

/**
 * Get if the window is resizable
 * \return True if the window is resizable, false otherwise
 */
SSGEAPI bool SSGE_GetWindowResizable();

/**
 * Get if the window is borderless
 * \return True if the window is borderless, false otherwise
 */
SSGEAPI bool SSGE_GetWindowBorderless();

/**
 * Get the window fullscreen mode
 * \return The fullscreen mode of the window
 */
SSGEAPI SSGE_WindowMode SSGE_GetWindowFullscreen();

/**
 * Get the window FPS
 * \return The FPS of the window
 */
SSGEAPI uint16_t SSGE_GetWindowFPS();

/**
 * Get the max frameskip
 * \return The max frameskip
 */
SSGEAPI uint8_t SSGE_GetMaxFrameskip();

/**
 * Get if VSync is enabled
 * \return True if VSync is enabled, false otherwise
 */
SSGEAPI bool SSGE_GetVSync();

// For advanced usage
/**
 * Get the SDL_Renderer used by the engine
 * \return The SDL_Renderer pointer
 */
SSGEAPI struct SDL_Renderer *SSGE_getRenderer();

#ifdef __cplusplus
}
#endif

#endif // __SSGE_H__