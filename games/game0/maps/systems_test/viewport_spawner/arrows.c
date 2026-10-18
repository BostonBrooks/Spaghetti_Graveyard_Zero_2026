#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/viewport/bbViewportApp.h"

bbFlag bbProjectile_spawnArrows(bbViewportApp* viewport_app,
                                  bbProjectile_data data,
                                  bbHandle entity_handle,
                                  bbHandle caster_unit_handle,
                                  U64 random_seed) {

    bbUnit* unit;
    bbUnits* units = home.viewport_app.units;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = units->pool;
    bbMapCoords MC = data.start_coords;
    bbUnit* caster;
    bbDebug("caster = %d\n", caster_unit_handle)
    bbVPool_lookup(home.viewport_app.units->pool,(void**)&caster,caster_unit_handle);
    if (caster!=NULL) {
        bbHere()
        MC.k += caster->drawable.md.projectile_spawn_point.k;
        MC.i += caster->drawable.md.projectile_spawn_point.i * sin(caster->drawable.md.rotation)
                - caster->drawable.md.projectile_spawn_point.j * cos(caster->drawable.md.rotation);

        MC.j += caster->drawable.md.projectile_spawn_point.i * cos(caster->drawable.md.rotation)
                + caster->drawable.md.projectile_spawn_point.j * sin(caster->drawable.md.rotation);
    } else {
        bbHere()
    }



    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);
    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&unit,&unit_handle);

    unit->drawable.md.class = bbDrawableClass_unit;
    unit->drawable.md.random_seed = random_seed;
    //bbDebug("Fireball spawned with seed %lu\n", unit->drawable.md.random_seed);

    unit->drawable.functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "ENTER NULL");
    unit->drawable.functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "LEAVE NULL");
    unit->drawable.functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE ENTER NULL");
    unit->drawable.functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "SQUARE LEAVE NULL");



    unit->drawable.md.coords = MC;

    unit->drawable.md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }

    unit->type =  bbUnit_projectile;
    unit->u.projectile = data;

    if (caster!=NULL)
    {
        unit->u.projectile.start_coords.k += caster->drawable.md.projectile_spawn_point.k;
        unit->u.projectile.start_coords.i += caster->drawable.md.projectile_spawn_point.i * sin(caster->drawable.md.rotation)
                - caster->drawable.md.projectile_spawn_point.j * cos(caster->drawable.md.rotation);

        unit->u.projectile.start_coords.j += caster->drawable.md.projectile_spawn_point.i * cos(caster->drawable.md.rotation)
                + caster->drawable.md.projectile_spawn_point.j * sin(caster->drawable.md.rotation);
    }
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);

    unit->u.unit.mouse.mouse_table.u64 = 0;



    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "PROJECTILE_ROTATION_ANGLE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 35;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.md.frames[1].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[1].asset_handle.u64 = 89;
    unit->drawable.md.frames[1].start_time= 0;
    unit->drawable.md.frames[1].framerate = 1;
    unit->drawable.md.frames[1].offset.x = 0;
    unit->drawable.md.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "TREE_GROUP",
                    &drawfunctionHandle);
    unit->drawable.md.frames[2].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[2].asset_handle.u64 = 89;
    unit->drawable.md.frames[2].start_time= 0;
    unit->drawable.md.frames[2].framerate = 1;
    unit->drawable.md.frames[2].offset.x = 0;
    unit->drawable.md.frames[2].offset.y = 0;

    bbList_sortL(&unitSquare->list, unit);

    unit->u.projectile.list.list_id = 0;
    unit->u.projectile.list.prev = viewport_app->projectiles.pool->null;
    unit->u.projectile.list.next = viewport_app->projectiles.pool->null;
    bbList_pushL(&viewport_app->projectiles,unit);




    bbRenderUnitGroup* group;
    bbVPool_alloc2(viewport_app->renderUnits->pool, (void**)&group, NULL);

    unit->drawable.group = group;

    group->spacing = POINTS_PER_TILE;
    group->randomness = 1.f;

    bbMapCoords start_coords[12];
    bbMapCoords end_coords[12];

    bbRenderUnitGroup_calcPositions(start_coords,
                                       unit->u.projectile.start_coords,
                                       0,
                                       0.f,
                                       unit->drawable.md.rotation,
                                       POINTS_PER_TILE,
                                       0.f);

    bbRenderUnitGroup_calcPositions(end_coords,
                                       unit->u.projectile.end_coords,
                                       0,
                                       0.f,
                                       unit->drawable.md.rotation,
                                       POINTS_PER_TILE,
                                       0.f);

    bbMinimalDrawable fox_drawable;
    fox_drawable.state = bbDrawableState_idle;
    fox_drawable.class = bbDrawableClass_renderUnit;
    fox_drawable.random_seed = 0;
    fox_drawable.rotation = 0;
    fox_drawable.coords = start_coords[0];

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        fox_drawable.frames[k].draw_function = -1;
    }

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "PROJECTILES",
                    &drawfunctionHandle);
    fox_drawable.frames[0].draw_function = drawfunctionHandle.u64;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++ ) {

        bbRenderUnit* render_unit = &group->units[i];
        render_unit->start_point = start_coords[i];
        render_unit->end_point = end_coords[i];
        render_unit->md = fox_drawable;
        render_unit->owner = &unit->drawable;
        render_unit->index = i;
        render_unit->avoidance_type = bbRU_avoidanceType_none;
        render_unit->movement_type = bbRU_movementType_static;

         debug_off = false;
 bbDebug(" (i, j, k) = (%d, %d, %d)\n", render_unit->start_point.i, render_unit->start_point.j, render_unit->start_point.k);
 bbDebug(" (i, j, k) = (%d, %d, %d)\n", render_unit->end_point.i, render_unit->end_point.j, render_unit->end_point.k);
 debug_off = true;

    }

    bbList_pushL(&viewport_app->renderUnits->projectile_list, group);
    return bbSuccess;
}
