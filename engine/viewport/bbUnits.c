#include "engine/viewport/bbDrawables.h"
#include "engine/logic/bbNestedList.h"
#include "engine/viewport/bbUnits.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


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

    //home.agents_app.entities.movable_units[index] = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}



bbFlag bbUnits_consumeBuffer(bbUnits* units, bbVPool* entity_units, bbMoveables_snapshot* snapshot)
{
    bbHandle* unit_handle;
    bbVPool* pool = units->pool;
    bbUnit* unit;
    bbDrawable* drawable;

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {


        if (snapshot->moveables[i].type == bbMoveableType_Unused) continue;
        if (snapshot->moveables[i].type == bbMoveableType_Dead) continue;

        //bbDebug("index = %d\n", snapshot->moveables[i].ECS_entity_handle.bloated.index);
    }

}