#include "engine/viewport/bbDrawables.h"
#include "engine/logic/bbNestedList.h"
//#include "engine/viewport/bbUnits.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbFlag bbUnit_newCat(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    I32 square_index = bbDrawables_getSquareIndex(SC.i, SC.j, units->squares_i);
    bbUnitSquare unitSquare = units->squares[square_index];

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

    home.viewport_app.unit_array[index] = unit_handle;
    bbList_sortL(&unitSquare.list, unit);
    *self = unit;
    return bbSuccess;
}

bbFlag bbUnit_newSkeleton(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    I32 square_index = bbDrawables_getSquareIndex(SC.i, SC.j, units->squares_i);
    bbUnitSquare unitSquare = units->squares[square_index];

    bbUnit* unit;
    bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

    bbHandle unit_handle;
    bbVPool_reverseLookup(pool, unit, &unit_handle);

    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;



    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_ANIMATION_ANGLE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 9;
    unit->drawable.frames[0].start_time=  -(rand()%6);
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    home.viewport_app.unit_array[index] = unit_handle;
    bbList_sortL(&unitSquare.list, unit);
    *self = unit;
    return bbSuccess;
}



bbFlag bbUnits_consumeBuffer(bbUnits* units, bbHandle* unit_array, bbMoveables_snapshot* snapshot)
{
    bbHandle unit_handle;
    bbVPool* pool = units->pool;
    bbUnit* unit;
    bbDrawable* drawable;



    for (I32 i = 0; i < numMoveables; i++)
    {

        unit_handle = unit_array[i];
        bbFlag flag =  bbVPool_lookup(pool, (void**)&unit, unit_handle);
        drawable = &unit->drawable;


        if (drawable == NULL)
        {
            bbHere()
                        continue;
        }

        if (snapshot->time >= unit->next_time)
        {
            unit->prev_coords = unit->next_coords;
            unit->prev_goalpoint = unit->next_goalpoint;
            unit->prev_time = unit->next_time;

            unit->next_coords = snapshot->moveables[i].position;
            unit->next_goalpoint = snapshot->moveables[i].goalpoint;
            unit->next_time = snapshot->time;
            I32 delta_i = unit->next_goalpoint.i - unit->prev_coords.i;
            I32 delta_j = unit->next_goalpoint.j - unit->prev_coords.j;
            float rotation = atan2(delta_i, delta_j);
            drawable->rotation = rotation;
        }
        bbMilliCoords position
            = bbMillicoords_interpolate(unit->prev_coords, unit->next_coords, unit->prev_time,
                home.UI.clock2_handle.map_tick, unit->next_time);

        bbMapCoords MC = bbMilliCoords_getMapCoords(position);

        if (i==0) home.viewport_app.viewport.viewpoint = MC;

        //TODO add list of out of bounds units
        if (MC.i<0 || MC.j < 0) continue;
        if (MC.i >= POINTS_PER_MAP || MC.j >= POINTS_PER_MAP) continue;
        bbDrawable_setLocation(drawable, units,MC);


    }
    return bbSuccess;
}