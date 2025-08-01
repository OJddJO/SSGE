#include <stdio.h>
#include <stdlib.h>
#include "game.h"

static void update(Game *game);
static void draw(Game *game);
static void event_handler(SSGE_Event event, Game *game);

static void create_hitboxes();

int main(int argc, char *argv[]) {
    // Initialize the engine
    SSGE_Init("TicTacToe", WIN_W, WIN_H, FPS);
    // Load fonts with sizes 32 and 64
    SSGE_Text_CreateFont("assets/font.ttf", 32, "font_32");
    SSGE_Text_CreateFont("assets/font.ttf", 64, "font_64");

    // Load audio files
    SSGE_Audio_Create("audio/start.ogg", "start");
    SSGE_Audio_Create("audio/click.ogg", "click");
    SSGE_Audio_Create("audio/tie.ogg", "tie");
    SSGE_Audio_Create("audio/win.ogg", "win");

    // Set the window properties
    SSGE_WindowResizable(false);
    SSGE_WindowFullscreen(false);
    SSGE_SetManualUpdate(true);
    SSGE_SetBackgroundColor((SSGE_Color){23, 15, 71, 255});

    // Create the game structure
    Game game;
    init_game(&game);

    // Create hitboxes for the tic-tac-toe grid
    // The hitboxes are invisible buttons that will be used to detect mouse clicks
    create_hitboxes();

    // Run the engine
    SSGE_Audio_PlayName("start", -1);

    SSGE_Run(update, draw, event_handler, &game);

    // Quit the engine
    SSGE_Quit();

    return 0;
}

/**
 * Create hitboxes for the tic-tac-toe grid
 * \note The hitboxes are created as objects with the name "hitbox_i_j" where i and j are the row and column of the hitbox
 */
static void create_hitboxes() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char name[30];
            sprintf(name, "hitbox_%d_%d", i, j);
            SSGE_Hitbox_Create(name, i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
}

/**
 * Update the game
 * \param game The game structure
 */
void update(Game *game) {
    game->winner = check_winner(game);
}

/**
 * Draw the board and the X and O
 * \param game The game structure
 */
void draw(Game *game) {
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
                if (game->matrix[i][j] == 1) {
                    SSGE_Text_Draw("font_64", "X", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
                } else if (game->matrix[i][j] == 2) {
                    SSGE_Text_Draw("font_64", "O", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
                }
            }
        }
    } else {
        char text[20];
        if (game->winner == -1) {
            sprintf(text, "It's a draw!");
            SSGE_Audio_PlayName("tie", -1);
        } else {
            sprintf(text, "Player %d wins!", game->winner);
            SSGE_Audio_PlayName("win", -1);
        }
        SSGE_Text_Draw("font_32", text, WIN_W / 2, WIN_H / 2, (SSGE_Color){255, 255, 255, 255}, SSGE_CENTER);
    }
}

/**
 * Event handler for the game
 * \param event The SDL event
 * \param game The game structure
 */
void event_handler(SSGE_Event event, Game *game) {
    switch (event.type) {
        case SSGE_MOUSEBUTTONDOWN: // handle mouse click
            if (game->winner == 0) { // if the game is not over
                // get the mouse position
                int x, y;
                SSGE_GetMousePosition(&x, &y);
                int i = x / TILE_SIZE;
                int j = y / TILE_SIZE;

                // check if a hitbox is clicked
                SSGE_Object *hitbox = SSGE_GetHoveredObject();
                if (hitbox != NULL && game->matrix[i][j] == 0) {
                    // play the click sound
                    SSGE_Audio_PlayName("click", -1);
                    // update the game datas
                    game->matrix[i][j] = game->current_player;
                    game->current_player = game->current_player == 1 ? 2 : 1;
                    game->turn++;
                    // update the game
                    SSGE_ManualUpdate(); // note that we used SSGE_SetManualUpdate(true) in the main function
                }
            } else { // if the game is over
                // restart the game
                SSGE_Object_DestroyAll();
                init_game(game);
                create_hitboxes();
                SSGE_ManualUpdate();
            }
    }
}