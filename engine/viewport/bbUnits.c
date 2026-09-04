#include "engine/viewport/bbDrawables.h"
#include "engine/logic/bbNestedList.h"
#include "engine/viewport/bbUnits.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
//extern bool interp_positions;

bbFlag bbUnit_newSkeleton(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index){

    bbVPool* pool = units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

    bbUnit* unit;
    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&unit,&unit_handle);


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

    //home.agents_app.entities.movable_units[index] = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}



bbFlag bbUnits_consumeBuffer(bbUnits* units, bbVPool* entity_units, bbMoveables_snapshot* snapshot)
{
    bbHandle* unit_handle;
    bbUnit* unit;
    bbDrawable* drawable;

    I64 map_time = home.UI.clock2_handle.map_tick;

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        if (snapshot->moveables[i].type == bbMoveableType_Unused) continue;
        if (snapshot->moveables[i].type == bbMoveableType_Dead) continue;



        bbVPool_lookup(home.viewport_app.entity_units,(void**)&unit_handle,snapshot->moveables[i].ECS_entity_handle);
        if (unit_handle == NULL) continue;
        bbVPool_lookup(home.viewport_app.units->pool,(void**)&unit,*unit_handle);
        if (unit == NULL) continue;
        drawable = &unit->drawable;

        if (snapshot->time > unit->next_time)
        {
            unit->prev_coords = unit->next_coords;
            unit->prev_goalpoint = unit->next_goalpoint;
            unit->prev_time = unit->next_time;

            unit->next_coords = snapshot->moveables[i].position;
            unit->next_goalpoint = snapshot->moveables[i].goalpoint;
            unit->next_time = snapshot->time;


            I32 delta_i = unit->next_coords.i - unit->prev_coords.i;
            I32 delta_j = unit->next_coords.j - unit->prev_coords.j;

            // if (drawable->state == bbDrawableState_attacking)
            // {
            //
            //     delta_i = unit->next_goalpoint.i - unit->prev_coords.i;
            //     delta_j = unit->next_goalpoint.j - unit->prev_coords.j;
            // }

            //TODO this condition is always true
            if (delta_i * delta_i + delta_j * delta_j > POINTS_PER_PIXEL*POINTS_PER_PIXEL)
            {
                float rotation = atan2(delta_i, delta_j);
                drawable->rotation = rotation;
            }
        }
        bbMapCoords position;
        //if (interp_positions)
        //{
            position
               = bbMapCoords_interpolate(unit->prev_coords, unit->next_coords, unit->prev_time,
                   map_time, unit->next_time);
        //}else{
        //    position = unit->next_coords;
        //}
        //If the drawable is out of bounds, it will be put in a separate bin
        bbDrawable_setLocation(drawable, units,position);



    }
    return bbSuccess;
}