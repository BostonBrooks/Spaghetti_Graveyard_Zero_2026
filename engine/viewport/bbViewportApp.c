#include "engine/viewport/bbViewportApp.h"

#include "engine/data/bbHome.h"

bbFlag bbViewportApp_init(bbViewportApp* app)
{
    bbDrawables_new((void**)&app->drawables,  12, 12);
    bbMapIcons_new((void**) &app->mapIcons,  12, 12);
    bbUnits_new((void**)&app->units, 12, 12);
    bbAvoidables_new((void**)&app->avoidables, 12, 12);
    bbViewport_init(&app->viewport, 456, 466);


}
