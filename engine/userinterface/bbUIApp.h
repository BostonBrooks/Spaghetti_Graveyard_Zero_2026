#ifndef BB_UIAPP_H
#define BB_UIAPP_H

#include "engine/userinterface/bbUI_Inbox.h"
#include "engine/interthread/bbClock.h"
#include "engine/userinterface/bbInput.h"

typedef struct
{

    sfRenderWindow* window;
    bbGraphicsApp graphics;
    bbWidgets widgets;
    bbUI_Inbox inbox;
    U64 UI_time;
    bbClock_handle clock2_handle;

    sfTexture* SplashTexture;
    sfSprite* SplashSprite;
    bbMouse mouse;
    bbInput input;

} bbUIApp;


bbFlag bbUIApp_spawnWidgets(bbUIApp* app);
bbFlag bbUIApp_init(bbUIApp* app);

bbFlag bbUIApp_draw(bbUIApp* app);
#endif //BB_UIAPP_H