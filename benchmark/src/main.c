#include <SSGE/SSGE.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define WWIDTH 1080
#define WHEIGHT 720
#define OBJPOW2 13

typedef struct _BenchData {
    clock_t start;
    uint64_t frame;
    uint64_t update;
} BenchData;

void update(BenchData *game);
void draw(BenchData *data);

int main(int argc, char *argv[]) {
    bool nothread = false;
    for (int i = 0; i < argc; i++)
    if (strcmp(argv[i], "nothread") == 0) {
        puts("Threading disabled");
        nothread = true;
    }

    BenchData data = {0};

    SSGE_Init("Benchmark", WWIDTH, WHEIGHT, 60);
    SSGE_WindowResizable(true);
    
    uint32_t id;
    SSGE_Font_Create("font.ttf", 32, "font");
    SSGE_Texture *texture = SSGE_Texture_Create(&id, "image.png", "texture");

    srand(time(NULL));

    for (int i = 0; i < (1<<OBJPOW2); i++) {
        char name[50];
        sprintf(name, "test_%u", 1);
        SSGE_Object *obj = SSGE_Object_Create(&id, name, 0, 0, rand()%1200, rand()%1200, false, NULL, NULL);
        SSGE_Object_BindTexture(obj, texture);
    }

    data.start = clock();
    SSGE_Run((SSGE_UpdateFunc)update, (SSGE_DrawFunc)draw, NULL, &data, nothread);

    SSGE_Quit();
    return 0;
}

void update(BenchData *data) {
    for (int i = 0; i < (1<<OBJPOW2); i++) {
        SSGE_Object_Move(SSGE_Object_Get(i), rand()%WWIDTH, rand()%WHEIGHT);
    }
    ++data->update;
}

void draw(BenchData *data) {
    double fpt = (double)++data->frame / (double)(clock() - data->start);
    double upt = data->update / (double)(clock() - data->start);
    char fptchar[50];
    sprintf(fptchar, "Avg. FPT: %f | Avg. UPT: %f", fpt, upt);
    SSGE_Text_Draw("font", fptchar, 2, 2, (SSGE_Color){0, 0, 0, 255}, SSGE_NW);
    SSGE_Text_Draw("font", fptchar, 0, 0, (SSGE_Color){255, 255, 255, 255}, SSGE_NW);
}