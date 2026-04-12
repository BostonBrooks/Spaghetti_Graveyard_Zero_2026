#ifndef BBVIEWPORTAPP_H
#define BBVIEWPORTAPP_H
#include "engine/viewport/bbMapIcons.h"
#include "engine/viewport/bbUnits.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/avoidance/bbAvoidables.h"
#include "engine/logic/bbFlag.h"
#include "engine/viewport/bbViewport.h"
typedef struct
{
    bbDrawables* drawables;
    bbMapIcons* mapIcons;
    bbUnits* units;
    //todo move from bbUIApp
    bbViewport viewport;

    //TODO We'll put this here for now...
    bbAvoidables* avoidables;


} bbViewportApp;

bbFlag bbViewportApp_init(bbViewportApp* app);

#endif //BBVIEWPORTAPP_H