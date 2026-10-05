#include "engine/render_units/bbRenderUnits.h"

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
bbFlag bbRenderUnitGroup_spawnKey(bbRenderUnits* render_units,
                                      char* key,
                                      bbDrawable* drawable) {
    bbHandle dict_entry;
    bbDictionary_lookup(render_units->spawnFunction_dict,key, &dict_entry);

    bbRenderUnitGroup_spawn(render_units,
                            dict_entry.u64,
                            drawable);
}


bbFlag bbRenderUnitGroup_delete(bbRenderUnits* render_units,
                                      bbRenderUnitGroup* group)
{
    bbDrawable* drawable = group->units[0].owner;
    drawable->group = NULL;

    bbList_remove(&render_units->list, group);
    bbVPool_free(render_units->pool, group);

    return bbSuccess;

}


bbFlag bbRenderUnitGroup_spawn_foxes(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbAssert(drawable->group == NULL, "trying to respawn an existing drawable group\n");
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    bbHandle drawfunctionHandle;
    bbMinimalDrawable fox_drawable;
    fox_drawable.coords = drawable->md.coords;
    fox_drawable.state = bbDrawableState_idle;
    fox_drawable.class = bbDrawableClass_renderUnit;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "UNIT_DRAWBUFFER",
                    &drawfunctionHandle);

    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[0].asset_handle.u64 = 25;
    fox_drawable.frames[0].start_time=  0;
    fox_drawable.frames[0].framerate = 1;
    fox_drawable.frames[0].offset.x = 0;
    fox_drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
            "DRAWABLE_SHADOW",
            &drawfunctionHandle);
    fox_drawable.frames[1].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[1].asset_handle.u64 = 1077;
    fox_drawable.frames[1].start_time=  0;
    fox_drawable.frames[1].framerate = 0;
    fox_drawable.frames[1].offset.x = 0;
    fox_drawable.frames[1].offset.y = 0;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++ )
    {
        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].movement_type = bbRU_movementType_wander;
        group->units[i].md = fox_drawable;
        group->units[i].md.random_seed = bbArith64_hashIndex(drawable->md.random_seed,i);


    }



    bbList_pushL(&render_units->list, group);

    if (Group != NULL) *Group = group;
    return bbSuccess;

}

bbFlag bbRenderUnitGroup_spawnFoxes_fn(struct bbRenderUnits* render_units, bbDrawable* drawable)
{

    bbRenderUnitGroup_spawn_foxes(NULL,
                                  render_units,
                                  drawable,
                                  &home.UI.graphics);

    return bbSuccess;
}

bbFlag bbRenderUnits_updateMovement(bbRenderUnits* render_units)
{
    bbRenderUnitGroup* group;
    bbFlag flag = bbList_setHead(&render_units->list,(void**)&group);
    while (flag == bbSuccess)
    {
        bbDrawable* drawable = group->units[0].owner;
        bbUnit* parent_unit = (bbUnit*)group->units[0].owner;

        float theta = drawable->md.rotation;
        float spacing = POINTS_PER_TILE;
        I32 num_units = 12;

        bbMapCoords delta_coords, new_coords;

        float c_theta = cos(theta);
        float s_theta = sin(theta);

        for (I32 i = 0; i < UNITS_PER_GROUP; i++)
        {

            bbRenderUnit* unit = &group->units[i];

            if (unit->movement_type == bbRU_movementType_rigid) {
                I32 row_N = i / 4;
                I32 column_M = i %4;

                delta_coords.i = (1.5-column_M)*spacing*c_theta - (-1+row_N)*spacing*s_theta;
                delta_coords.j = -(1.5-column_M)*spacing*s_theta - (-1+row_N)*spacing*c_theta;
                delta_coords.k = 0;

                unit->md.coords = drawable->md.coords;
                unit->md.coords.i += delta_coords.i;
                unit->md.coords.j += delta_coords.j;
                unit->md.coords.k = bbMapCoords_getElevation(&home.ground_surface, unit->md.coords);
                unit->md.rotation = drawable->md.rotation;
            }

            if (unit->movement_type == bbRU_movementType_wander) {


                U64 coefficients = bbArith64_hash(unit->md.random_seed);
                U64 mask = 0xFF;

                U64 c0 = coefficients & mask;
                double f0 = ((double)c0 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c1 = coefficients & mask;
                double f1 = ((double)c1 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c2 = coefficients & mask;
                double f2 = ((double)c2 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c3 = coefficients & mask;
                double f3 = ((double)c3 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c4 = coefficients & mask;
                double f4 = ((double)c4 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c5 = coefficients & mask;
                double f5 = ((double)c5 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c6 = coefficients & mask;
                double f6 = ((double)c6 - 128.0) / 512.0;
                coefficients >>= 8;

                U64 c7 = coefficients & mask;
                double f7 = ((double)c7 - 128.0) / 512.0;

               // bbDebug("(%f, %f, %f, %f,%f, %f, %f, %f_\n",
               //     f0, f1, f2, f3, f4, f5, f6, f7)

                bbTime current_time = home.UI.clock2_handle.map_tick;
                //TODO bbTime last_update; bbTime last_wander
                bbTime wander_time = parent_unit->drawable.last_wander_time;
                if (parent_unit->drawable.md.state == bbDrawableState_moving) {
                    wander_time += current_time- parent_unit->drawable.last_state_change;
                }
                double wander_time_d = wander_time / 60.0;

                double delta_i = f0*sin(wander_time_d*1.1)
                                +f1*cos(wander_time_d*1.1)
                                +f2*sin(wander_time_d)
                                +f3*cos(wander_time_d);

                double delta_j = f4*sin(wander_time_d*1.1)
                                +f5*cos(wander_time_d*1.1)
                                +f6*sin(wander_time_d)
                                +f7*cos(wander_time_d);


                double row_N = i / 4;
                double column_M = i %4;

                double rn =  row_N + delta_i;
                double cm =  column_M + delta_j;

                delta_coords.i = (1.5-cm)*spacing*c_theta - (-1+rn)*spacing*s_theta;
                delta_coords.j = -(1.5-cm)*spacing*s_theta - (-1+rn)*spacing*c_theta;
                delta_coords.k = 0;

                unit->md.coords = drawable->md.coords;
                unit->md.coords.i += delta_coords.i;
                unit->md.coords.j += delta_coords.j;
                unit->md.rotation = drawable->md.rotation;


                unit->md.coords.k = bbMapCoords_getElevation(&home.ground_surface, unit->md.coords);
            }
        }
        flag = bbList_increment(&render_units->list,(void**)&group);
    }
}