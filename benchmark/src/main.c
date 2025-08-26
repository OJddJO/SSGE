#include <SSGE/SSGE.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define WWIDTH 1080
#define WHEIGHT 720
#define OBJPOW2 13

typedef struct _Game {
    clock_t start;
    uint64_t frame;
} Game;

Game data = {0};

void update(Game *game);
void draw(Game *data);

int main(int argc, char *argv[]) {
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
    SSGE_Run((SSGE_UpdateFunc)update, (SSGE_DrawFunc)draw, NULL, &data);

    SSGE_Quit();
    return 0;
}

void update(Game *game) {
    for (int i = 0; i < (1<<OBJPOW2); i++) {
        SSGE_Object_Move(SSGE_Object_Get(i), rand()%WWIDTH, rand()%WHEIGHT);
    }
}

void draw(Game *data) {
    double fpt = (double)++data->frame / (double)(clock() - data->start);
    char fptchar[50];
    sprintf(fptchar, "Avg. FPT: %f", fpt);
    SSGE_Text_Draw("font", fptchar, 2, 2, (SSGE_Color){0, 0, 0, 255}, SSGE_NW);
    SSGE_Text_Draw("font", fptchar, 0, 0, (SSGE_Color){255, 255, 255, 255}, SSGE_NW);
}