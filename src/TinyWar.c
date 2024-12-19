#include "game.h"

void update(void *game);
void draw(void *game);
void event_handler(SDL_Event event, void *game);

static void create_hitboxes();
static int check_winner(Game *game);

int main(int argc, char *argv[]) {
    engine_init("TinyWar", WIN_W, WIN_H, FPS);
    load_font("assets/font.ttf", 32, "font_32");
    load_font("assets/font.ttf", 64, "font_64");

    window_resizable(false);
    window_fullscreen(false);
    set_manual_update(true);

    Game *game = init_game();

    create_hitboxes();

    engine_run(update, draw, event_handler, game);

    destroy_all_objects();
    destroy_all_textures();
    destroy_all_templates();
    engine_quit();

    return 0;
}

static void create_hitboxes() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            char name[10];
            sprintf(name, "hitbox_%d_%d", i, j);
            create_hitbox(name, i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
}

static int check_winner(Game *game) {
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
    return 0;
}

void update(void *_game) {
    Game *game = _game;
    game->winner = check_winner(game);
}

void draw(void *_game) {
    Game *game = _game;
    if (!game->winner) {
        //draw grid
        for (int i = 0; i < 4; i++) {
            draw_line_thick(0, i * TILE_SIZE, WIN_W, i * TILE_SIZE, (Color){255, 255, 255, 255}, 5);
        }
        for (int i = 0; i < 4; i++) {
            draw_line_thick(i * TILE_SIZE, 0, i * TILE_SIZE, WIN_H, (Color){255, 255, 255, 255}, 5);
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
        sprintf(text, "Player %d wins!", game->winner);
        draw_text("font_32", text, WIN_W / 2, WIN_H / 2, (Color){255, 255, 255, 255}, CENTER);
    }
}

void event_handler(SDL_Event event, void *_game) {
    Game *game = _game;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        get_mouse_position(&x, &y);
        int i = x / TILE_SIZE;
        int j = y / TILE_SIZE;
        char name[10];
        sprintf(name, "hitbox_%d_%d", i, j);
        if (object_is_hovered_by_name(name)) {
            game->matrix[i][j] = game->current_player;
            game->current_player = game->current_player == 1 ? 2 : 1;
            destroy_object_by_name(name);
            manual_update();
        }
    }
}