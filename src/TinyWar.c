#include "game.h"

int main(int argc, char *argv[]) {
    engine_init("TinyWar", 800, 600, 60);
    engine_run(update, draw, event_handler, NULL);
    return 0;
}

void update(void *game) {
    printf("update\n");
}

void draw(void *game) {
    printf("draw\n");
}

void event_handler(SDL_Event event, void *game) {
    printf("event_handler\n");
}