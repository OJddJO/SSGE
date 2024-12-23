#include "game.h"

/**
 * Initialize the game
 * \param game The game structure to initialize
 */
void init_game(Game *game) {
    for (int i = 0; i < MAP_H; i++) {
        for (int j = 0; j < MAP_W; j++) {
            game->matrix[i][j] = 0;
        }
    }
    game->current_player = 1;
    game->winner = 0;
    game->turn = 0;
}

/**
 * Check if there is a winner
 * \param game The game structure
 * \return The winner, 0 if no winner, -1 if draw
 */
int check_winner(Game *game) {
    //check rows
    for (int i = 0; i < 3; i++) {
        if (game->matrix[i][0] == game->matrix[i][1] && game->matrix[i][1] == game->matrix[i][2] && game->matrix[i][0] != 0) {
            return game->matrix[i][0];
        }
    }
    //check columns
    for (int i = 0; i < 3; i++) {
        if (game->matrix[0][i] == game->matrix[1][i] && game->matrix[1][i] == game->matrix[2][i] && game->matrix[0][i] != 0) {
            return game->matrix[0][i];
        }
    }
    //check diagonals
    if (game->matrix[0][0] == game->matrix[1][1] && game->matrix[1][1] == game->matrix[2][2] && game->matrix[0][0] != 0) {
        return game->matrix[0][0];
    }
    if (game->matrix[0][2] == game->matrix[1][1] && game->matrix[1][1] == game->matrix[2][0] && game->matrix[0][2] != 0) {
        return game->matrix[0][2];
    }
    //check draw
    if (game->turn == 9) {
        return -1;
    }
    return 0;
}