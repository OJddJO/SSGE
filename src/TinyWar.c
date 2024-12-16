#include "game.h"

#define FPS 60
#define TILE_SIZE 32
#define MAP_W 40
#define MAP_H 26
#define WIN_W MAP_W * TILE_SIZE
#define WIN_H MAP_H * TILE_SIZE

static void init_textures();
static void init_map();
static void update(void *game);
static void draw(void *game);
static void event_handler(SDL_Event event, void *game);

int **map;

int main(int argc, char *argv[]) {
    engine_init("TinyWar", WIN_W, WIN_H, FPS);

    window_resizable(false);
    window_fullscreen(false);

    srand(time(NULL));
    init_textures();
    init_map();

    engine_run(update, draw, event_handler, NULL);

    destroy_all_objects();
    destroy_all_textures();
    destroy_all_templates();
    engine_quit();

    return 0;
}

static void init_map() {
    map = (int **)malloc(MAP_W * sizeof(int *));
    for (int x = 0; x < MAP_W; x++) {
        map[x] = (int *)malloc(MAP_H * sizeof(int));
        for (int y = 0; y < MAP_H; y++) {
            map[x][y] = rand() % 3;
        }
    }
}

static void init_textures() {
    load_texture("assets/tile_0000.png", "grass_0");
    load_texture("assets/tile_0001.png", "grass_1");
    load_texture("assets/tile_0002.png", "grass_2");
}

static void update(void *game) {
}

static void draw(void *game) {
    for (int i = 0; i < MAP_W; i++) {
        for (int j = 0; j < MAP_H; j++) {
            SDL_Texture *grass = NULL;
            switch (map[i][j]) {
                case 0:
                    grass = get_texture_by_name("grass_0");
                    break;
                case 1:
                    grass = get_texture_by_name("grass_1");
                    break;
                case 2:
                    grass = get_texture_by_name("grass_2");
                    break;
            }
            draw_texture(grass, i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
}

static void event_handler(SDL_Event event, void *game) {
}