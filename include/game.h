#ifndef __GAME_H__
#define __GAME_H__

#include "engine.h"

typedef struct _Game {
} Game;

void update(void *game);
void draw(void *game);
void event_handler(SDL_Event event, void *game);

#endif // __GAME_H__