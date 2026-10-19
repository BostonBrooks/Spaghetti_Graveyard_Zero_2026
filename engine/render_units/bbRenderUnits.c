#include "engine/render_units/bbRenderUnits.h"

#include "engine/data/bbHome.h"
#include "engine/geometry/bbViewportCoords.h"
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

    bbList_init(&render_units->soldier_list, render_units->pool,NULL,offsetof(bbRenderUnitGroup, list_element)
            ,NULL,bbSystem_RenderUnits);

    bbList_init(&render_units->projectile_list, render_units->pool,NULL,offsetof(bbRenderUnitGroup, list_element)
            ,NULL,bbSystem_RenderProjectile);

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

    bbList_remove(&render_units->soldier_list, group);
    bbVPool_free(render_units->pool, group);

    return bbSuccess;

}

bbFlag bbRenderUnitGroup_spawn_hoplites(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbWarning(drawable->group == NULL, "trying to respawn an existing drawable group\n");
    if (drawable->group != NULL){return bbSuccess;}
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    group->spacing = POINTS_PER_TILE;
    group->randomness = 1.f;

    bbHandle drawfunctionHandle;
    bbMinimalDrawable fox_drawable;
    fox_drawable.coords = drawable->md.coords;
    fox_drawable.state = bbDrawableState_idle;
    fox_drawable.class = bbDrawableClass_renderUnit;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "FRONT_ROW",
                    &drawfunctionHandle);

    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[0].asset_handle.u64 = 13;
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
        U64 seed = bbArith64_hashIndex(drawable->md.random_seed,i);
        float framerate = 1.0 + (bbArith64_hashIndex(seed,1) % 16) / 160.f;
        U32 start_time = bbArith64_hashIndex(seed, 2) % 60;

        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].avoidance_type = bbRU_avoidanceType_minimium;
        group->units[i].movement_type = bbRU_movementType_wander;

        if (drawable->md.state == bbDrawableState_dead) {
            group->units[i].avoidance_type = bbRU_avoidanceType_none;
        }

        group->units[i].md = fox_drawable;
        group->units[i].md.random_seed = seed;
        group->units[i].md.angle =0;
        group->units[i].md.frames[0].framerate = framerate;
        group->units[i].md.frames[0].start_time = start_time;


    }



    bbList_pushL(&render_units->soldier_list, group);

    if (Group != NULL) *Group = group;
    return bbSuccess;

}


bbFlag bbRenderUnitGroup_spawn_archers(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbWarning(drawable->group == NULL, "trying to respawn an existing drawable group\n");
    if (drawable->group != NULL){return bbSuccess;}
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    group->spacing = POINTS_PER_TILE;
    group->randomness = 1.f;

    bbHandle drawfunctionHandle;
    bbMinimalDrawable fox_drawable;
    fox_drawable.coords = drawable->md.coords;
    fox_drawable.state = bbDrawableState_idle;
    fox_drawable.class = bbDrawableClass_renderUnit;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "PARENT_STATE",
                    &drawfunctionHandle);

    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[0].asset_handle.u64 = 12;
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
        U64 seed = bbArith64_hashIndex(drawable->md.random_seed,i);
        float framerate = 1.0 + (bbArith64_hashIndex(seed,1) % 16) / 160.f;
        U32 start_time = bbArith64_hashIndex(seed, 2) % 60;

        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].avoidance_type = bbRU_avoidanceType_minimium;
        group->units[i].movement_type = bbRU_movementType_wander;

        if (drawable->md.state == bbDrawableState_dead) {
            group->units[i].avoidance_type = bbRU_avoidanceType_none;
        }

        group->units[i].md = fox_drawable;
        group->units[i].md.random_seed = seed;
        group->units[i].md.angle =0;
        group->units[i].md.frames[0].framerate = framerate;
        group->units[i].md.frames[0].start_time = start_time;


    }



    bbList_pushL(&render_units->soldier_list, group);

    if (Group != NULL) *Group = group;
    return bbSuccess;

}

bbFlag bbRenderUnitGroup_spawn_foxes(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics)
{
    bbWarning(drawable->group == NULL, "trying to respawn an existing drawable group\n");
    if (drawable->group != NULL){return bbSuccess;}
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    group->spacing = POINTS_PER_TILE;
    group->randomness = 1.f;

    bbHandle drawfunctionHandle;
    bbMinimalDrawable fox_drawable;
    fox_drawable.coords = drawable->md.coords;
    fox_drawable.state = bbDrawableState_idle;
    fox_drawable.class = bbDrawableClass_renderUnit;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "FRONT_ROW",
                    &drawfunctionHandle);

    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;
    fox_drawable.frames[0].asset_handle.u64 = 10;
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
        U64 seed = bbArith64_hashIndex(drawable->md.random_seed,i);
        float framerate = 1.0 + (bbArith64_hashIndex(seed,1) % 16) / 160.f;
        U32 start_time = bbArith64_hashIndex(seed, 2) % 60;

        group->units[i].owner = drawable;
        group->units[i].index = i;
        group->units[i].avoidance_type = bbRU_avoidanceType_minimium;
        group->units[i].movement_type = bbRU_movementType_wander;

        if (drawable->md.state == bbDrawableState_dead) {
            group->units[i].avoidance_type = bbRU_avoidanceType_none;
        }

        group->units[i].md = fox_drawable;
        group->units[i].md.random_seed = seed;
        group->units[i].md.angle =0;
        group->units[i].md.frames[0].framerate = framerate;
        group->units[i].md.frames[0].start_time = start_time;


    }



    bbList_pushL(&render_units->soldier_list, group);

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

bbFlag bbRenderUnitGroup_spawnHoplites_fn(struct bbRenderUnits* render_units, bbDrawable* drawable)
{

    bbRenderUnitGroup_spawn_hoplites(NULL,
                                  render_units,
                                  drawable,
                                  &home.UI.graphics);

    return bbSuccess;
}

bbFlag bbRenderUnitGroup_spawnArchers_fn(struct bbRenderUnits* render_units, bbDrawable* drawable)
{

    bbRenderUnitGroup_spawn_archers(NULL,
                                  render_units,
                                  drawable,
                                  &home.UI.graphics);

    return bbSuccess;
}

bbFlag bbRenderUnits_updatePositions(bbRenderUnits* render_units)
{
    bbRenderUnitGroup* group;
    bbFlag flag = bbList_setHead(&render_units->soldier_list,(void**)&group);
    while (flag == bbSuccess)
    {
        bbDrawable* drawable = group->units[0].owner;
        bbUnit* parent_unit = (bbUnit*)group->units[0].owner;

        float theta = drawable->md.angle;
        float spacing = group->spacing;
        float randomness = group->randomness;

        float wander_time_f = 0;

        if (group->units->movement_type == bbRU_movementType_wander) {
            bbTime wander_time = parent_unit->drawable.last_wander_time;
            if (parent_unit->drawable.md.state == bbDrawableState_moving) {
                wander_time += home.UI.clock2_handle.map_tick- parent_unit->drawable.last_state_change;
            }
            wander_time_f = wander_time / 60.f;
        }
        bbMapCoords new_coords[UNITS_PER_GROUP];

        bbRenderUnitGroup_calcPositions(new_coords,
                                       drawable->md.coords,
                                       drawable->md.random_seed,
                                       wander_time_f,
                                       theta,
                                       spacing,
                                       randomness);

        for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
            bbRenderUnit* unit = &group->units[i];
            unit->md.coords = new_coords[i];
            unit->md.angle = drawable->md.angle;

            unit->forces.i = 0;
            unit->forces.j = 0;
            unit->forces.k = 0;
            unit->correction.i = 0;
            unit->correction.j = 0;
            unit->correction.k = 0;
        }

        flag = bbList_increment(&render_units->soldier_list,(void**)&group);
    }

    return bbSuccess;
}


bbFlag bbRenderUnitGroup_calcPositions(bbMapCoords coords[UNITS_PER_GROUP],
                                       bbMapCoords centre,
                                       U64 random_seed,
                                       float wander_time,
                                       float rotation,
                                       float spacing,
                                       float randomness) {

    float c_theta = cosf(rotation);
    float s_theta = sinf(rotation);

    bbMapCoords delta_coords;
    for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
        U64 coefficients = bbArith64_hashIndex(random_seed,i);
        U64 mask = 0xFF;


        U64 c0 = coefficients & mask;
        float f0 = ((float)c0 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c1 = coefficients & mask;
        float f1 = ((float)c1 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c2 = coefficients & mask;
        float f2 = ((float)c2 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c3 = coefficients & mask;
        float f3 = ((float)c3 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c4 = coefficients & mask;
        float f4 = ((float)c4 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c5 = coefficients & mask;
        float f5 = ((float)c5 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c6 = coefficients & mask;
        float f6 = ((float)c6 - 128.0f) / 512.0f;
        coefficients >>= 8;

        U64 c7 = coefficients & mask;
        float f7 = ((float)c7 - 128.0f) / 512.0f;

        float delta_i = f0*sinf(wander_time*1.1f)
                +f1*cosf(wander_time*1.1f)
                +f2*sinf(wander_time)
                +f3*cosf(wander_time);

        float delta_j = f4*sinf(wander_time*1.1f)
                        +f5*cosf(wander_time*1.1f)
                        +f6*sinf(wander_time)
                        +f7*cosf(wander_time);


        delta_i *= randomness;
        delta_j *= randomness;

        I32 row_N = i / 4;
        I32 column_M = i %4;

        float row_f =  row_N + delta_i;
        float column_f =  column_M + delta_j;

        delta_coords.i = (1.5f-row_f)*spacing*c_theta - (-1.f+row_f)*spacing*s_theta;
        delta_coords.j = -(1.5f-column_f)*spacing*s_theta - (-1.f+column_f)*spacing*c_theta;
        delta_coords.k = 0;

        coords[i].i = centre.i + delta_coords.i;
        coords[i].j = centre.j + delta_coords.j;
        coords[i].k = bbMapCoords_getElevation(&home.ground_surface, coords[i]);
    }

    return bbSuccess;

}


bbFlag bbRenderUnits_updateProjectiles(bbRenderUnits* render_units) {

    bbRenderUnitGroup* group;
    bbFlag flag = bbList_setHead(&render_units->projectile_list,(void**)&group);
    while (flag == bbSuccess) {

        bbUnit* parent_unit = (bbUnit*)group->units[0].owner;
        float gravity = parent_unit->u.projectile.gravity;

        bbTime start_time = parent_unit->u.projectile.start_time;
        bbTime end_time = parent_unit->u.projectile.end_time;
        bbTime current_time = home.UI.clock2_handle.map_tick;


        for (I32 i=0; i<UNITS_PER_GROUP; i++) {


            bbRenderUnit* render_unit = &group->units[i];

            bbTime reference_time = current_time + render_unit->time_deviation;

            bbMapCoords start_coords = render_unit->start_point;
            bbMapCoords end_coords = render_unit->end_point;
            bbMapCoords interp_coords = end_coords;

            if (reference_time  > end_time) {

                continue;
            }



                interp_coords = bbMapCoords_interpolateGravity(start_coords,
                                                                          end_coords,
                                                                          gravity,
                                                                          start_time,
                                                                          reference_time ,
                                                                          end_time);

            bbFloat3D velocity = bbMapCoords_getVelocity(
                start_coords,
                end_coords,
                gravity,
                start_time,
                reference_time ,
                end_time);


            float rotation = bbVelocity_getRotation(velocity);
            float angle = bbVelocity_getAngle(velocity);

            group->units[i].md.angle = angle;
            group->units[i].md.rotation = rotation;
            group->units[i].md.coords = interp_coords;

           // debug_off = false;
           //  bbDebug("start time = %lld, end_time = %lld\n", start_time, end_time);
           //  bbDebug("start (i, j, k) = (%d, %d, %d)\n", start_coords.i, start_coords.j, start_coords.k);
           //  bbDebug("end (i, j, k) = (%d, %d, %d)\n", end_coords.i, end_coords.j, end_coords.k);
           //  bbDebug("interp (i, j, k) = (%d, %d, %d)\n", interp_coords.i, interp_coords.j, interp_coords.k);
           //  debug_off = true;
        }

        flag = bbList_increment(&render_units->projectile_list,(void**)&group);
    }
}