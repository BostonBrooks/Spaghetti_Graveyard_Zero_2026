#include "engine/viewport/bbViewportApp.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbFlag bbViewportApp_init(bbViewportApp* app)
{
    bbDrawables_new((void**)&app->drawables,  12, 12);
    bbMapIcons_new((void**) &app->mapIcons,  12, 12);
    bbUnits_new((void**)&app->units, 12, 12);
    bbViewport_init(&app->viewport, 456, 466);

    bbVPool_newBloated(&app->moveable_units, sizeof(bbHandle), 100,100,"MOVEABLE_UNITS");
    bbVPool_newBloated(&app->entity_units, sizeof(bbHandle), 100,100,"ENTITY_UNITS");
    return bbSuccess;
}
