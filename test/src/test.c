#include "SSGE/SSGE.h"

int main() {
    SSGE_Init("Test", 800, 500, 75);
    SSGE_SetVSync(true);

    uint32_t id;
    SSGE_Texture *text;
    SSGE_Object *obj;

    // text = SSGE_CreateRectThick(&id, NULL, 100, 70, (SSGE_Color){255, 0, 0, 255}, 6);
    // obj = SSGE_Object_Create(&id, NULL, 10, 100, 100, 100, false);
    // SSGE_Object_BindTexture(obj, text);

    // text = SSGE_CreateCircleThick(&id, NULL, 20, (SSGE_Color){255, 0, 0, 255}, 7);
    // obj = SSGE_Object_Create(&id, NULL, 0, 0, 100, 100, false);
    // SSGE_Object_BindTexture(obj, text);

    // text = SSGE_CreateEllipseThick(&id, NULL, 50, 10, (SSGE_Color){255, 0, 0, 255}, 6);
    // obj = SSGE_Object_Create(&id, NULL, 0, 0, 100, 20, false);
    // SSGE_Object_BindTexture(obj, text);

    text = SSGE_CreateLineThick(&id, NULL, 50, (SSGE_Color){255, 0, 0, 255}, 6);
    obj = SSGE_Object_Create(&id, NULL, 10, 10, 50, 6, false);
    SSGE_Object_BindTexture(obj, text);

    SSGE_Run(NULL, NULL, NULL, NULL);

    SSGE_Quit();
    return 0;
}
