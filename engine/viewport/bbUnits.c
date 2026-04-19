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