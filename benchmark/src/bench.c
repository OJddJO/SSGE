#include <SSGE/SSGE.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define WWIDTH 1080
#define WHEIGHT 720

typedef struct _BenchData {
    clock_t start;
    uint64_t frame;
    uint64_t update;
    int nbobj;
    bool fullscreen;
} BenchData;

void update(BenchData *data);
void draw(BenchData *data);
void eventHandler(SSGE_Event ev, BenchData *data);

int main(int argc, char *argv[]) {
    BenchData data = {0};
    bool vsync = false;
    uint16_t fps = 60;
    data.nbobj = 1 << 12;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-nbobj") == 0) {
            if (i >= argc) return 1;
            data.nbobj = atoi(argv[++i]);
        }
        if (strcmp(argv[i], "-vsync") == 0) {
            vsync = true;
        }
        if (strcmp(argv[i], "-fps") == 0) {
            if (i >= argc) return 1;
            fps = (uint16_t)atoi(argv[++i]);
        }
    }

    printf("nb. obj. : %d, fps: %u, vsync: %d\n", data.nbobj, fps, vsync);
    SSGE_Init("Benchmark", WWIDTH, WHEIGHT, fps);
    SSGE_WindowResizable(true);
    SSGE_SetVSync(vsync);

    uint32_t id;
    SSGE_Font_Create("font.ttf", 24, "font");
    SSGE_Texture *texture = SSGE_Texture_Create(&id, "image.png", "texture");

    srand(time(NULL));

    for (int i = 0; i < data.nbobj; i++) {
        char name[50];
        sprintf(name, "test_%u", 1);
        SSGE_Object *obj = SSGE_Object_Create(&id, name, 0, 0, rand()%1200, rand()%1200, false, NULL, NULL);
        SSGE_Object_BindTexture(obj, texture);
    }

    data.start = clock();
    SSGE_Run((SSGE_UpdateFunc)update, (SSGE_DrawFunc)draw, (SSGE_EventHandler)eventHandler, &data);

    SSGE_Quit();
    return 0;
}

void update(BenchData *data) {
    for (int i = 0; i < data->nbobj; i++) {
        SSGE_Object_Move(SSGE_Object_Get(i), rand()%WWIDTH, rand()%WHEIGHT);
    }
    ++data->update;
}

void draw(BenchData *data) {
    double fps = (double)++data->frame / (double)(clock() - data->start) * CLOCKS_PER_SEC;
    double ups = data->update / (double)(clock() - data->start) * CLOCKS_PER_SEC;
    char fptchar[200];
    sprintf(fptchar, "Avg. FPS: %.3f | Avg. UPS: %.3f", fps, ups);
    SSGE_Text_Draw("font", fptchar, 2, 2, (SSGE_Color){0, 0, 0, 255}, SSGE_NW);
    SSGE_Text_Draw("font", fptchar, 0, 0, (SSGE_Color){255, 255, 255, 255}, SSGE_NW);
}

void eventHandler(SSGE_Event ev, BenchData *data) {
    printf("Event type: %d\n", ev.type);
    switch (ev.type) {
        case SSGE_EVENT_KEYDOWN:
            if (ev.key.keysym.scancode == SSGE_SCANCODE_F11)
                SSGE_WindowFullscreen((data->fullscreen = !data->fullscreen));
    }
}