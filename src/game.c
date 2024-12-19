#include "game.h"

Game *init_game() {
    Game *game = (Game *)malloc(sizeof(Game));
    if (game == NULL) {
        fprintf(stderr, "[GAME] Failed to allocate memory for game\n");
        exit(1);
    }
    
    for (int i = 0; i < MAP_H; i++) {
        for (int j = 0; j < MAP_W; j++) {
            game->matrix[i][j] = 0;
        }
    }

    game->current_player = 1;
    game->winner = 0;

    return game;
}