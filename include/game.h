#ifndef __GAME_H__
#define __GAME_H__

#include "engine.h"

typedef struct _Game {
    int width, height;
    int nb_players;
    Player *players[4];
} Game;

typedef struct _Player {
    char *name;
    int x, y;
} Player;

void update(void *game);
void draw(void *game);
void event_handler(SDL_Event event, void *game);

#endif // __GAME_H__