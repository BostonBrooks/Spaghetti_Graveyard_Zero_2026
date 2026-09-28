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

    drawable->functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    drawable->functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    drawable->functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    drawable->functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");


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


    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;



    if (entity_handle.u64 != no_handle.u64)
    {
        bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
        bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_idle;
    unit->entity_handle = entity_handle;


    //bbDebug("system = %u\n", entity_handle.system.system);

    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "MONSTER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 5;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 626;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
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

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");


    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
        bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);


    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "PLAYER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 6;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 626;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;




    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }

    bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}


bbFlag bbViewportSpawnCow(bbViewportApp* viewport_app,
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

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
       // bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);

    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "MONSTER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 7;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 704;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }


        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}


bbFlag bbViewportSpawnFox(bbViewportApp* viewport_app,
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

    unit->drawable.md.random_seed = bbRand();

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "SPAWN FOXES");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "UNSPAWN FOXES");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SPAWN FOXES");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "UNSPAWN FOXES");

    unit->drawable.last_state_change = 0; //set this value from the simulation thread when spawned
    unit->drawable.last_wander_time = 0;

    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
       // bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);


    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "MONSTER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
         "UNIT_GROUP2",

         &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 12;
    unit->drawable.md.frames[0].start_time =  -(rand()%6);
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 711;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }



    // bbRenderUnitGroup_spawnKey(viewport_app->renderUnits,
    //                            "FOXES",
    //                            &unit->drawable);

    bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}

bbFlag bbViewportSpawnLizard(bbViewportApp* viewport_app,
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

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
        //bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);


    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "MONSTER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 9;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 707;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;



    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }


        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}


bbFlag bbViewportSpawnDevil(bbViewportApp* viewport_app,
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

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
        //bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);


    bbVPMouseFunctions_getTableHandle(
        &home.viewport_app.mouse.functions,
        &unit->mouse.mouse_table,
        "MONSTER" );
    unit->mouse.hover = 0;
    unit->mouse.selected = 0;
    unit->mouse.rect.top = 4*POINTS_PER_TILE;
    unit->mouse.rect.left = POINTS_PER_TILE;
    unit->mouse.rect.height = 4*POINTS_PER_TILE;
    unit->mouse.rect.width = 2*POINTS_PER_TILE;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 11;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 714;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }


        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}

bbFlag bbViewportSpawnFireball(bbViewportApp* viewport_app,
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

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;
    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }
    if (moveable_handle.u64 != no_handle.u64){
        //bbHandle* moveable_unit;
        //bbVPool_allocFromHandle(home.viewport_app.moveable_units, (void**)&moveable_unit, moveable_handle);
        //*moveable_unit = unit_handle;
    }
    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);

    unit->mouse.mouse_table.u64 = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 8;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = -1; //drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 612;
    unit->drawable.md.frames[1].start_time =  -(rand()%6);
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);

    unit->drawable.md.frames[2].draw_function = -1;//drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 704;
    unit->drawable.md.frames[2].start_time =  -(rand()%6);
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "HEALTH_POINTS",
             &drawfunctionHandle);

    unit->drawable.md.frames[3].draw_function = -1;// drawfunctionHandle.u64;
    unit->drawable.md.frames[3].asset_handle.u64 = 626;
    unit->drawable.md.frames[3].start_time =  -(rand()%6);
    unit->drawable.md.frames[3].framerate = 1;
    unit->drawable.md.frames[3].offset.x = 0;
    unit->drawable.md.frames[3].offset.y = 0;

    for (I32 k = 4; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
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
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnCow,
                          "COW");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnLizard,
                          "LIZARD");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnFireball,
                          "FIREBALL");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnFox,
                          "FOX");
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnDevil,
                          "DEVIL");
}