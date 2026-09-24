#include "engine/viewport/bbRenderUnits.h"

#include "engine/logic/bbSystemPool.h"

#define MAX_SPAWN_FUNCS 193

bbFlag bbRenderUnits_init(bbRenderUnits* render_units)
{
    bbVPool_newSystem(&render_units->pool,
                      bbSystem_RenderUnits,
                      sizeof(bbRenderUnitGroup),
                      10,
                      1000,
                      "RENDER UNITS");

    bbList_init(&render_units->list, render_units->pool,NULL,offsetof(bbRenderUnitGroup, list_element)
            ,NULL,bbSystem_RenderUnits);

    render_units->spawnFunction_num = 0;
    bbDictionary_new(&render_units->spawnFunction_dict,MAX_SPAWN_FUNCS);

    render_units->spawnFunctions = calloc(MAX_SPAWN_FUNCS, sizeof (bbRenderUnitGroup_spawn_fn*));


    return bbSuccess;
}


bbFlag bbRenderUnits_addSpawnFunction(bbRenderUnits* render_units,
                                      bbRenderUnitGroup_spawn_fn* spawn_function,
                                      char* key)
{
    I32 available = render_units->spawnFunction_num++;
    render_units->spawnFunctions[available] = spawn_function;
    bbHandle dict_entry;
    dict_entry.u64 = available;
    bbDictionary_add(render_units->spawnFunction_dict,key, dict_entry);
    return bbSuccess;
}

bbFlag bbRenderUnitGroup_spawn(bbRenderUnits* render_units,
                                      I32 spawn_function_index,
                                      bbDrawable* drawable)
{
    bbRenderUnitGroup_spawn_fn* spawn_fn = render_units->spawnFunctions[spawn_function_index];
    return spawn_fn(render_units, drawable);
}

bbFlag bbRenderUnitGroup_delete(bbRenderUnits* render_units,
                                      bbRenderUnitGroup* group)
{
    bbDrawable* drawable = &group->units[0].drawable;
    drawable->group = NULL;

    bbList_remove(&render_units->list, drawable);
    bbVPool_free(render_units->pool, drawable);

    return bbSuccess;

}

bbFlag bbUnit_newSkeleton(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index);

bbFlag bbRenderUnitGroup_spawn_foxes(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    bbHandle drawfunctionHandle;
    bbDrawable fox_drawable;
    fox_drawable.coords = drawable->coords;
    fox_drawable.state = bbDrawableState_idle;


    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].drawfunction = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "UNIT_ANIMATION_ANGLE",
                    &drawfunctionHandle);

    fox_drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    fox_drawable.frames[0].handle.u64 = 21;
    fox_drawable.frames[0].start_time=  -(rand()%60);
    fox_drawable.frames[0].framerate = 1;
    fox_drawable.frames[0].offset.x = 0;
    fox_drawable.frames[0].offset.y = 0;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++ )
    {
        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].movement_type = bbRU_movementType_rigid;
        group->units->drawable = fox_drawable;
        fox_drawable.coords.i += i * POINTS_PER_TILE;


    }

    bbList_pushL(&render_units->list, group);

    return bbSuccess;

}
