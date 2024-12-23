#ifndef __GAME_H__
#define __GAME_H__

#include "engine.h"

#define FPS 60
#define TILE_SIZE 128
// #define MAP_W 40
// #define MAP_H 26
// #define WIN_W MAP_W * TILE_SIZE
// #define WIN_H MAP_H * TILE_SIZE

#define MAP_W 3
#define MAP_H 3
#define WIN_W MAP_W * TILE_SIZE
#define WIN_H MAP_H * TILE_SIZE

typedef struct _Game {
    int matrix[3][3];
    int current_player;
    int winner;
    int turn;
} Game;

void init_game(Game *game);
int check_winner(Game *game);

#endif // __GAME_H__