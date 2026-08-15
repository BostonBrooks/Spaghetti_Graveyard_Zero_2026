#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewportApp.h"
#include "engine/data/bbHome.h"

I32 viewport_spawn_function_count = 12;

bbFlag bbViewportSpawnCat(bbViewportApp* viewport_app,
                          bbMapCoords MC,
                          bbHandle entity_handle,
                          bbHandle moveable_handle)
{
    bbDrawable* drawable;
    bbDrawable_newCat(&drawable,
                      viewport_app->drawables,
                      &home.UI.graphics,
                      MC);

    return bbSuccess;
}

bbFlag bbViewportSpawnSkelly(bbViewportApp* viewport_app,
                          bbMapCoords MC,
                          bbHandle entity_handle,
                          bbHandle moveable_handle){


    bbUnit* unit;
    bbUnits* units = home.viewport_app.units;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&unit,&unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        bbHandle* entity_unit;
        bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        *entity_unit = unit_handle;
    }
    if (moveable_handle.u64 != no_handle.u64){
        bbHandle* moveable_unit;
        bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        *moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.state = bbDrawableState_idle;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 5;
    unit->drawable.frames[0].start_time= 0;
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[1].handle.u64 = 612;
    unit->drawable.frames[1].start_time =  -(rand()%6);
    unit->drawable.frames[1].framerate = 1;
    unit->drawable.frames[1].offset.x = 0;
    unit->drawable.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[2].handle.u64 = 626;
    unit->drawable.frames[2].start_time =  -(rand()%6);
    unit->drawable.frames[2].framerate = 1;
    unit->drawable.frames[2].offset.x = 0;
    unit->drawable.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.frames[3].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[3].handle.u64 = 626;
    unit->drawable.frames[3].start_time =  -(rand()%6);
    unit->drawable.frames[3].framerate = 1;
    unit->drawable.frames[3].offset.x = 0;
    unit->drawable.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }


        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}

bbFlag bbViewportSpawnZombie(bbViewportApp* viewport_app,
                          bbMapCoords MC,
                          bbHandle entity_handle,
                          bbHandle moveable_handle){


    bbUnit* unit;
    bbUnits* units = home.viewport_app.units;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&unit,&unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        bbHandle* entity_unit;
        bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        *entity_unit = unit_handle;
    }
    if (moveable_handle.u64 != no_handle.u64){
        bbHandle* moveable_unit;
        bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        *moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.state = bbDrawableState_moving;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 6;
    unit->drawable.frames[0].start_time= 0;
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[1].handle.u64 = 612;
    unit->drawable.frames[1].start_time =  -(rand()%6);
    unit->drawable.frames[1].framerate = 1;
    unit->drawable.frames[1].offset.x = 0;
    unit->drawable.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[2].handle.u64 = 626;
    unit->drawable.frames[2].start_time =  -(rand()%6);
    unit->drawable.frames[2].framerate = 1;
    unit->drawable.frames[2].offset.x = 0;
    unit->drawable.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.frames[3].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[3].handle.u64 = 626;
    unit->drawable.frames[3].start_time =  -(rand()%6);
    unit->drawable.frames[3].framerate = 1;
    unit->drawable.frames[3].offset.x = 0;
    unit->drawable.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }


        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}

bbFlag bbViewportSpawner_populate(bbViewportSpawner* viewport_spawner)
{
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnCat,
                          "CAT");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnSkelly,
                          "SKELLY");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnZombie,
                          "ZOMBIE");
}