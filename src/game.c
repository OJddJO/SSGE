#include "game.h"

Game *init_game() {
    Game *game = (Game *)malloc(sizeof(Game));
    if (game == NULL) {
        fprintf(stderr, "[GAME] Failed to allocate memory for game\n");
        exit(1);
    }
    Tilemap *tm = create_tilemap("assets/tilemap.png", 16, 16, 1, 11, 18);
    game->tilemap = tm;

    srand(time(NULL));

    game->nb_players = 4;
    for (int row = 0; row < MAP_H; row++) {
        for (int col = 0; col < MAP_W; col++) {
            game->map[row][col] = rand() % 3;
        }
    }

    return game;
}