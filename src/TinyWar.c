#include "game.h"

int main(int argc, char *argv[]) {
    engine_init("TinyWar", 800, 600, 60);
    window_resizable(true);
    engine_run(update, draw, event_handler, NULL);
    return 0;
}

void update(void *game) {
}

void draw(void *game) {
    draw_texture_from_path("assets/tile_0001.png", 0, 0, 32, 32);
}

void event_handler(SDL_Event event, void *game) {
}