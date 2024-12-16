#ifndef __GAME_H__
#define __GAME_H__

#include "engine.h"

typedef struct _Player {
    char *name;
    int x, y;
} Player;

typedef struct _Game {
    int width, height;
    int nb_players;
    Player *players[4];
} Game;

#endif // __GAME_H__