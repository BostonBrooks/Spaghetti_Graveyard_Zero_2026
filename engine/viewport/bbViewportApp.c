#include "engine/viewport/bbViewportApp.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbFlag bbViewportApp_init(bbViewportApp* app)
{
    bbDrawables_new((void**)&app->drawables,  12, 12);
    bbMapIcons_new((void**) &app->mapIcons,  12, 12);
    bbUnits_new((void**)&app->units, 12, 12);
    bbViewport_init(&app->viewport, 456, 466);

    //bbVPool_newBloated(&app->moveable_units, sizeof(bbHandle), 10,1000,"MOVEABLE_UNITS");
    //bbVPool_newBloated(&app->entity_units, sizeof(bbHandle), 10,1000,"ENTITY_UNITS");
    bbLookupTable_new(&app->entity_units2,bbECS_ECS,10,1000);
    return bbSuccess;
}


bbFlag bbViewportApp_updateViewpoint(bbViewportApp* app)
{
    if (app->viewport_focus.u64 == 0) return bbNone;

    bbHandle* unit_handle;
    //bbVPool_lookup(home.viewport_app.entity_units,(void**)&unit_handle,app->viewport_focus);


    bbHandle unit_handle2;
    bbFlag flag = bbLookupTable_lookup(home.viewport_app.entity_units2,app->viewport_focus,&unit_handle2);
    bbFlag_print(flag);

    if (unit_handle == NULL) return bbFail;
    bbUnit* unit;
    bbVPool_lookup(home.viewport_app.units->pool,(void**)&unit,unit_handle2);
    if (unit == NULL) return bbFail;

    app->viewport.viewpoint = unit->drawable.coords;

    return bbSuccess;

}