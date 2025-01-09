#include "game.h"

static void update(Game *game);
static void draw(Game *game);
static void event_handler(SDL_Event event, Game *game);

static void create_hitboxes();

int main(int argc, char *argv[]) {
    // Initialize the engine
    engine_init("TicTacToe", WIN_W, WIN_H, FPS);
    // Load fonts with sizes 32 and 64
    load_font("assets/font.ttf", 32, "font_32");
    load_font("assets/font.ttf", 64, "font_64");

    // Load audio files
    load_audio("audio/start.ogg", "start");
    load_audio("audio/click.ogg", "click");
    load_audio("audio/tie.ogg", "tie");
    load_audio("audio/win.ogg", "win");

    // Set the window properties
    window_resizable(false);
    window_fullscreen(false);
    set_manual_update(true);
    set_background_color((Color){23, 15, 71, 255});

    // Create the game structure
    Game *game = (Game *)malloc(sizeof(Game));
    if (game == NULL) {
        fprintf(stderr, "[GAME] Failed to allocate memory for game\n");
        exit(1);
    }
    init_game(game);

    // Create hitboxes for the tic-tac-toe grid
    // The hitboxes are invisible buttons that will be used to detect mouse clicks
    create_hitboxes();

    // Run the engine
    play_audio_by_name("start", -1);
    engine_run(update, draw, event_handler, game);

    // Quit the engine
    destroy_all_objects();
    destroy_all_textures();
    destroy_all_templates();
    close_all_audios();
    close_all_fonts();
    engine_quit();

    return 0;
}

/**
 * Create hitboxes for the tic-tac-toe grid
 * \note The hitboxes are created as objects with the name "hitbox_i_j" where i and j are the row and column of the hitbox
 */
static void create_hitboxes() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char name[10];
            sprintf(name, "hitbox_%d_%d", i, j);
            create_hitbox(name, i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
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
            draw_line_thick(0, i * TILE_SIZE, WIN_W, i * TILE_SIZE, (Color){66, 50, 166, 255}, 5);
        }
        for (int i = 0; i < 4; i++) {
            draw_line_thick(i * TILE_SIZE, 0, i * TILE_SIZE, WIN_H, (Color){66, 50, 166, 255}, 5);
        }
        //draw X and O
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (game->matrix[i][j] == 1) {
                    draw_text("font_64", "X", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (Color){255, 255, 255, 255}, CENTER);
                } else if (game->matrix[i][j] == 2) {
                    draw_text("font_64", "O", i * TILE_SIZE + TILE_SIZE / 2, j * TILE_SIZE + TILE_SIZE / 2, (Color){255, 255, 255, 255}, CENTER);
                }
            }
        }
    } else {
        char text[20];
        if (game->winner == -1) {
            sprintf(text, "It's a draw!");
            play_audio_by_name("tie", -1);
        } else {
            sprintf(text, "Player %d wins!", game->winner);
            play_audio_by_name("win", -1);
        }
        draw_text("font_32", text, WIN_W / 2, WIN_H / 2, (Color){255, 255, 255, 255}, CENTER);
    }
}

/**
 * Event handler for the game
 * \param event The SDL event
 * \param game The game structure
 */
void event_handler(SDL_Event event, Game *game) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN: // handle mouse click
            if (game->winner == 0) { // if the game is not over
                // get the mouse position
                int x, y;
                get_mouse_position(&x, &y);
                int i = x / TILE_SIZE;
                int j = y / TILE_SIZE;
                char name[10];

                // check if an hitbox is clicked
                sprintf(name, "hitbox_%d_%d", i, j);
                if (object_is_hovered_by_name(name)) { // if the hitbox is clicked
                    // play the click sound
                    play_audio_by_name("click", -1);
                    // update the game datas
                    game->matrix[i][j] = game->current_player;
                    game->current_player = game->current_player == 1 ? 2 : 1;
                    game->turn++;
                    // destroy the hitbox to prevent further clicks
                    destroy_object_by_name(name);
                    // update the game
                    manual_update(); // note that we used set_manual_update(true) in the main function
                }
            } else { // if the game is over
                // restart the game
                destroy_all_objects();
                init_game(game);
                create_hitboxes();
                manual_update();
            }
    }
}