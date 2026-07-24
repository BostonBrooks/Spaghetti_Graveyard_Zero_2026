#ifndef BB_VIEWPORTAPP_H
#define BB_VIEWPORTAPP_H


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

    //Convert from movable handle to unit handle
    bbVPool* moveable_units;
    //Convert from entity handle to unit handle
    bbVPool* entity_units;
} bbViewportApp;

bbFlag bbViewportApp_init(bbViewportApp* app);

bbFlag spawnDrawables(void);

#endif //BB_VIEWPORTAPP_H