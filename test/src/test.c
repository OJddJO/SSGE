#include "SSGE/SSGE.h"

void update(void *);

int main() {
    SSGE_Init("Test", 800, 500, 75);
    // SSGE_SetVSync(true);

    uint32_t id;

    SSGE_Animation *anim = SSGE_Animation_CreateFrames(&id, NULL, 4, 100, 100);
    
    SSGE_Animation_AddFrame(anim, 2, "1.png");
    SSGE_Animation_AddFrame(anim, 2, "2.png");
    SSGE_Animation_AddFrame(anim, 2, "3.png");
    SSGE_Animation_AddFrame(anim, 2, "4.png");

    SSGE_Animation_Play(anim, 0, 0, -1, false, false);
    SSGE_Run(NULL, NULL, NULL, NULL);
    
    SSGE_Quit();
    return 0;
}
