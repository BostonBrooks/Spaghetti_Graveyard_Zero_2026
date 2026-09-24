#include "engine/viewport/bbRenderUnits.h"

#include "engine/data/bbHome.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/logic/bbIterator.h"
#include "engine/logic/bbSystemPool.h"

#define MAX_SPAWN_FUNCS 193

bbFlag bbRenderUnits_new(bbRenderUnits** this)
{
    bbRenderUnits* render_units = calloc(1, sizeof(bbRenderUnits));
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

    *this = render_units;

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
    bbDrawable* drawable = group->units[0].owner;
    drawable->group = NULL;

    bbList_remove(&render_units->list, drawable);
    bbVPool_free(render_units->pool, drawable);

    return bbSuccess;

}


bbFlag bbRenderUnitGroup_spawn_foxes(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    bbHandle drawfunctionHandle;
    bbMinimalDrawable fox_drawable;
    fox_drawable.coords = drawable->md.coords;
    fox_drawable.state = bbDrawableState_idle;


    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "DRAWBUFFER_SPRITE",
                    &drawfunctionHandle);

    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[0].asset_handle.u64 = 709;
    fox_drawable.frames[0].start_time=  0;
    fox_drawable.frames[0].framerate = 1;
    fox_drawable.frames[0].offset.x = 0;
    fox_drawable.frames[0].offset.y = 0;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++ )
    {
        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].movement_type = bbRU_movementType_rigid;
        group->units[i].md = fox_drawable;
        group->units[i].md.coords.i += i * POINTS_PER_TILE;


    }

    bbList_pushL(&render_units->list, group);

    *Group = group;
    return bbSuccess;

}



bbFlag bbRenderUnits_updateMovement(bbRenderUnits* render_units)
{
    bbRenderUnitGroup* group;
    bbFlag flag = bbList_setHead(&render_units->list,(void**)&group);
    while (flag == bbSuccess)
    {
        bbDrawable* drawable = group->units[0].owner;
        float theta = drawable->md.rotation;
        float spacing = POINTS_PER_TILE;
        I32 num_units = 12;

        bbMapCoords delta_coords, new_coords;

        float c_theta = cos(theta);
        float s_theta = sin(theta);

        for (I32 i = 0; i < UNITS_PER_GROUP; i++)
        {
            I32 row_N = i / 4;
            I32 column_M = i %4;

            delta_coords.i = (1.5-column_M)*spacing*c_theta - (-1+row_N)*spacing*s_theta;
            delta_coords.j = -(1.5-column_M)*spacing*s_theta - (-1+row_N)*spacing*c_theta;
            delta_coords.k = 0;

            bbRenderUnit* unit = &group->units[i];
            unit->md.coords = drawable->md.coords;
            unit->md.coords.i += delta_coords.i;
            unit->md.coords.j += delta_coords.j;
            unit->md.coords.k = bbMapCoords_getElevation(&home.ground_surface, unit->md.coords);
            unit->md.rotation = drawable->md.rotation;

        }
        flag = bbList_increment(&render_units->list,(void**)&group);
    }
}