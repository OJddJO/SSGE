#include <stdio.h>
#include <stdlib.h>
#include "game.h"

static void init_game(Game *game);
static void create_hitboxes(uint32_t hitboxes[3][3]);
static uint8_t check_winner(Game *game);

static void update(Game *game);
static void draw(Game *game);
static void event_handler(SSGE_Event event, Game *game);

int main(int argc, char *argv[]) {
    // Initialize the engine
    SSGE_Init("TicTacToe", WIN_W, WIN_H, FPS);

    // Set the window properties
    SSGE_WindowResizable(false);
    SSGE_WindowFullscreen(false);
    SSGE_SetManualUpdate(true);
    SSGE_SetBackgroundColor((SSGE_Color){23, 15, 71, 255});

    // Load fonts with sizes 32 and 64
    SSGE_Font_Create("font_32", "assets/font.ttf", 32);
    SSGE_Font_Create("font_64", "assets/font.ttf", 64);

    // Load audio files
    uint32_t audios[4] = {0};
    // Get the pointer of start for later use (line 42)
    SSGE_Audio *start = SSGE_Audio_Create(&audios[A_START], "start", "audio/start.ogg");
    SSGE_Audio_Create(&audios[A_CLICK], "click", "audio/click.ogg");
    SSGE_Audio_Create(&audios[A_TIE], "tie", "audio/tie.ogg");
    SSGE_Audio_Create(&audios[A_WIN], "win", "audio/win.ogg");

    // Create the game structure
    Game game;
    init_game(&game);

    // Create hitboxes for the tic-tac-toe grid
    // The hitboxes are invisible buttons that will be used to detect mouse clicks
    create_hitboxes(game.hitboxes);

    // Run the engine
    SSGE_Audio_Play(start, -1);

    SSGE_Run((SSGE_UpdateFunc)update, NULL, (SSGE_DrawFunc)draw, (SSGE_EventHandler)event_handler, &game);

    // Quit the engine
    SSGE_Quit();

    return 0;
}

/**
 * Create hitboxes for the tic-tac-toe grid
 * \note The hitboxes are created as objects with the name "hitbox_i_j" where i and j are the row and column of the hitbox
 */
static void create_hitboxes(uint32_t hitboxes[3][3]) {
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            char name[30];
            sprintf(name, "hitbox_%d_%d", i, j);
            SSGE_Object_Create(&hitboxes[i][j], name, i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, true);
        }
    }
}

/**
 * Init the game struct
 * \param game The game struct to init
 */
static void init_game(Game *game) {
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            game->hitboxes[i][j] = 0;
            game->board[i][j] = 0; // fill board with 0s
        }
    }

    game->current_player = 1;
    game->winner = 0;
    game->turn = 0;
    game->ended = false;
}

/**
 * Check if there is a winner
 * \param game The game structure
 * \return The winner, 0 if no winner, -1 if draw
 */
static uint8_t check_winner(Game *game) {
    //check rows
    for (int i = 0; i < 3; i++) {
        if (game->board[i][0] == game->board[i][1] && game->board[i][1] == game->board[i][2] && game->board[i][0] != 0) {
            return game->board[i][0];
        }
    }
    //check columns
    for (int i = 0; i < 3; i++) {
        if (game->board[0][i] == game->board[1][i] && game->board[1][i] == game->board[2][i] && game->board[0][i] != 0) {
            return game->board[0][i];
        }
    }
    //check diagonals
    if (game->board[0][0] == game->board[1][1] && game->board[1][1] == game->board[2][2] && game->board[0][0] != 0) {
        return game->board[0][0];
    }
    if (game->board[0][2] == game->board[1][1] && game->board[1][1] == game->board[2][0] && game->board[0][2] != 0) {
        return game->board[0][2];
    }
    //check draw
    if (game->turn == 9) {
        return -1;
    }
    return 0;
}

/**
 * Update the game
 * \param game The game structure
 */
static void update(Game *game) {
    game->winner = check_winner(game);
    if (game->winner && !game->ended)
        SSGE_ManualUpdate();
}

/**
 * Draw the board and the X and O
 * \param game The game structure
 */
static void draw(Game *game) {
    if (game->winner == 0) {
        //draw grid
        for (int i = 0; i < 4; i++) {
            SSGE_DrawLineThick(0, i * TILE_SIZE, WIN_W, i * TILE_SIZE, (SSGE_Color){66, 50, 166, 255}, 5);
        }
        for (int i = 0; i < 4; i++) {
            SSGE_DrawLineThick(i * TILE_SIZE, 0, i * TILE_SIZE, WIN_H, (SSGE_Color){66, 50, 166, 255}, 5);
        }
        //draw X and O
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (game->board[i][j] == 1) {
                    SSGE_Text_Draw("font_64", "X", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
                } else if (game->board[i][j] == 2) {
                    SSGE_Text_Draw("font_64", "O", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
                }
            }
        }
    } else if (!game->ended) {
        char text[20];
        if (game->winner == (uint8_t)-1) {
            sprintf(text, "It's a draw!");
            SSGE_Audio_Play(SSGE_Audio_Get(A_TIE), -1);
        } else {
            sprintf(text, "Player %d wins!", game->winner);
            SSGE_Audio_Play(SSGE_Audio_Get(A_WIN), -1);
        }
        SSGE_Text_Draw("font_32", text, WIN_W / 2, WIN_H / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
        game->ended = true;
    }
}

/**
 * Event handler for the game
 * \param event The SDL event
 * \param game The game structure
 */
static void event_handler(SSGE_Event event, Game *game) {
    switch (event.type) {
        case SSGE_EVENT_MOUSEBUTTONDOWN: // handle mouse click
            if (game->winner == 0) { // if the game is not over
                // get tile position
                int i = event.mouseClick.x / TILE_SIZE;
                int j = event.mouseClick.y / TILE_SIZE;

                // check if a hitbox is clicked
                SSGE_Object *hitbox = SSGE_Object_GetHovered();
                if (hitbox != NULL) {
                    // play the click sound
                    SSGE_Audio_Play(SSGE_Audio_Get(A_CLICK), -1);
                    // update the game datas
                    game->board[i][j] = game->current_player;
                    game->current_player = game->current_player == 1 ? 2 : 1;
                    game->turn++;
                    SSGE_Object_Destroy(game->hitboxes[i][j]);
                    // update the screen
                    SSGE_ManualUpdate(); // note that we used SSGE_SetManualUpdate(true) in the main function
                }
            } else { // if the game is over
                // restart the game
                SSGE_Object_DestroyAll();
                init_game(game);
                create_hitboxes(game->hitboxes);
                SSGE_ManualUpdate();
            }
            break;
    }
}