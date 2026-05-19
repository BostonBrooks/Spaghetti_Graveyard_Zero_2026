#ifndef BB_VIEWPORTAPP_H
#define BB_VIEWPORTAPP_H


#include "engine/agents/bbMoveables.h"
#include "engine/viewport/bbMapIcons.h"
#include "engine/viewport/bbUnits.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewport.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"



typedef struct
{
    bbDrawables* drawables;
    bbMapIcons* mapIcons;
    bbUnits* units;
    bbViewport viewport;
    bbWidget* viewport_widget;

    bbHandle unit_array[NUM_MOVEABLES];
} bbViewportApp;

bbFlag bbViewportApp_init(bbViewportApp* app);

bbFlag spawnDrawables(void);

#endif //BB_VIEWPORTAPP_H