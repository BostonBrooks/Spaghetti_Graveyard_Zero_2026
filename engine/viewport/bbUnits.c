#include "engine/viewport/bbDrawables.h"
#include "engine/logic/bbNestedList.h"
//#include "engine/viewport/bbUnits.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbFlag bbUnit_newCat(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);


    bbUnit* unit;
    bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

    bbHandle unit_handle;
    bbVPool_reverseLookup(pool, unit, &unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;




    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_ANIMATION",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 7;
    unit->drawable.frames[0].start_time=  -(rand()%6);
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    home.entities_app.arrays.moveable_units[index] = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}

bbFlag bbUnit_newSkeleton(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

    bbUnit* unit;
    bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

    bbHandle unit_handle;
    bbVPool_reverseLookup(pool, unit, &unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;

    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;


    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "UNIT_ANIMATION_ANGLE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 9;
    unit->drawable.frames[0].start_time=  -(rand()%60);
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[1].handle.u64 = 612;
    unit->drawable.frames[1].start_time =  -(rand()%6);
    unit->drawable.frames[1].framerate = 1;
    unit->drawable.frames[1].offset.x = 0;
    unit->drawable.frames[1].offset.y = 0;

    for (I32 k = 2; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    home.entities_app.arrays.moveable_units[index] = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}


bbFlag bbUnit_newSkelly(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

    bbUnit* unit;
    bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

    bbHandle unit_handle;
    bbVPool_reverseLookup(pool, unit, &unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;

    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;
unit->drawable.state = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 5;
    unit->drawable.frames[0].start_time=  0;
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
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

    for (I32 k = 3; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    home.entities_app.arrays.moveable_units[index] = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}




bbFlag bbUnits_consumeBuffer(bbUnits* units, bbHandle* unit_array, bbMoveables_snapshot* snapshot)
{
    bbHandle unit_handle;
    bbVPool* pool = units->pool;
    bbUnit* unit;
    bbDrawable* drawable;



    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {

        unit_handle = unit_array[i];
        bbFlag flag =  bbVPool_lookup(pool, (void**)&unit, unit_handle);
        drawable = &unit->drawable;


        if (drawable == NULL)
        {
                        continue;
        }

        if (snapshot->time > unit->next_time)
        {
            unit->prev_coords = unit->next_coords;
            unit->prev_goalpoint = unit->next_goalpoint;
            unit->prev_time = unit->next_time;

            unit->next_coords = snapshot->moveables[i].position;
            unit->next_goalpoint = snapshot->moveables[i].goalpoint;
            unit->next_time = snapshot->time;
            I32 delta_i = unit->next_goalpoint.i - unit->prev_coords.i;
            I32 delta_j = unit->next_goalpoint.j - unit->prev_coords.j;

            if (delta_i * delta_i + delta_j * delta_j > POINTS_PER_PIXEL)
            {
                float rotation = atan2(delta_i, delta_j);
                drawable->rotation = rotation;
            }
        }

        bbMapCoords position
            = bbMapCoords_interpolate(unit->prev_coords, unit->next_coords, unit->prev_time,
                home.UI.clock2_handle.map_tick, unit->next_time);


        if (i==home.agents_app.player_entity) home.viewport_app.viewport.viewpoint = position;
        home.viewport_app.viewport.viewpoint.k += 1;
        //TODO add list of out of bounds units
        if (position.i<0 || position.j < 0) continue;
        if (position.i >= POINTS_PER_MAP || position.j >= POINTS_PER_MAP) continue;
        bbDrawable_setLocation(drawable, units,position);


    }
    return bbSuccess;
}