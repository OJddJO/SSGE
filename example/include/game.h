#ifndef __GAME_H__
#define __GAME_H__

#include "SSGE/SSGE.h"

#define FPS 60
#define TILE_SIZE 128

#define MAP_W 3
#define MAP_H 3
#define WIN_W MAP_W * TILE_SIZE
#define WIN_H MAP_H * TILE_SIZE

typedef struct _Game {
    uint32_t hitboxes[9];
    int matrix[3][3];
    int current_player;
    int winner;
    int turn;
} Game;

enum _audios {
    A_START,
    A_CLICK,
    A_TIE,
    A_WIN,
};

#endif // __GAME_H__