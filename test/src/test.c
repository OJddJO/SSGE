#include "SSGE/SSGE.h"

void update(void *);

SSGE_Object *obj = NULL;

int main() {
    SSGE_Init("Test", 800, 500, 75);
    SSGE_SetVSync(true);

    uint32_t id;

    SSGE_Animation *anim = SSGE_Animation_CreateFrames(&id, NULL, 4, 100, 100);
    
    SSGE_Animation_AddFrame(anim, 5, "1.png");
    SSGE_Animation_AddFrame(anim, 5, "2.png");
    SSGE_Animation_AddFrame(anim, 5, "3.png");
    SSGE_Animation_AddFrame(anim, 5, "4.png");

    obj = SSGE_Object_Create(&id, NULL, 50, 50, 200, 200, false);
    SSGE_Object_BindAnimation(obj, anim, false, false);

    SSGE_Object_Hide(obj);

    // id = SSGE_Animation_Play(anim, 0, 0, -1, false, false);
    // SSGE_Animation_Pause(id);

    SSGE_Run((SSGE_UpdateFunc)update, NULL, NULL, NULL);

    SSGE_Quit();
    return 0;
}

void update(void *) {
    SSGE_Object_Move(obj, (obj->x + 1)%800, (obj->y + 1)%500);
}