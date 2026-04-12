#ifndef BB_VIEWPORTAPP_H
#define BB_VIEWPORTAPP_H


#include "engine/viewport/bbMapIcons.h"
//#include "engine/viewport/bbUnits.h"
#include "engine/viewport/bbDrawables.h"
//#include "engine/avoidance/bbAvoidables.h"
#include "engine/logic/bbFlag.h"
#include "engine/viewport/bbViewport.h"

typedef struct
{
    bbDrawables* drawables;
    bbMapIcons* mapIcons;
    //bbUnits* units;
    bbViewport viewport;

} bbViewportApp;

bbFlag bbViewportApp_init(bbViewportApp* app);

#endif //BB_VIEWPORTAPP_H