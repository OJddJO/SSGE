#include "game.h"

void update(void *game);
void draw(void *game);
void event_handler(Event event, void *game);

int main(int argc, char *argv[]) {
    engine_init("TinyWar", WIN_W, WIN_H, FPS);

    window_resizable(false);
    window_fullscreen(false);

    Game *game = init_game();

    engine_run(update, draw, event_handler, game);

    destroy_all_objects();
    destroy_all_textures();
    destroy_all_templates();
    engine_quit();

    return 0;
}

void update(void *game) {
}

void draw(void *game) {
    for (int row = 0; row < MAP_H; row++) {
        for (int col = 0; col < MAP_W; col++) {
            Tile *grass = NULL;
            switch (((Game *)game)->map[row][col]) {
                case 0:
                    grass = get_tile(((Game *)game)->tilemap, 0, 0);
                    break;
                case 1:
                    grass = get_tile(((Game *)game)->tilemap, 0, 1);
                    break;
                case 2:
                    grass = get_tile(((Game *)game)->tilemap, 0, 2);
                    break;
            }
            draw_tile_with_size(grass, col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
}

void event_handler(Event event, void *game) {
}