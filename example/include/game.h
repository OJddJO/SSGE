#ifndef __GAME_H__
#define __GAME_H__

#include "SSGE/SSGE.h"
#include <stdatomic.h>

#define FPS 60
#define TILE_SIZE 128

#define MAP_W 3
#define MAP_H 3
#define WIN_W MAP_W * TILE_SIZE
#define WIN_H MAP_H * TILE_SIZE

typedef struct _Game {
    uint32_t            hitboxes[3][3]; // Hitboxes' ids
    int                 board[3][3];    // The board (0 is empty, 1 and 2 is player)
    uint8_t             current_player; // The current player (1 or 2)
    uint8_t             winner;         // The id of the winner
    uint8_t             turn;           // Current turn count
    bool                ended;          // If the game has ended and is waiting for a click
} Game;

enum _audios {
    A_START,
    A_CLICK,
    A_TIE,
    A_WIN,
};

#endif // __GAME_H__