#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/viewport/bbViewportApp.h"

bbFlag bbProjectile_spawnArrow(bbViewportApp* viewport_app,
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
    bbVPool_lookup(home.viewport_app.units->pool,(void**)&caster,caster_unit_handle);
    if (caster!=NULL) {
        bbHere()
        MC.k += caster->drawable.md.projectile_spawn_point.k;
        MC.i += caster->drawable.md.projectile_spawn_point.i * sin(caster->drawable.md.angle)
                - caster->drawable.md.projectile_spawn_point.j * cos(caster->drawable.md.angle);

        MC.j += caster->drawable.md.projectile_spawn_point.i * cos(caster->drawable.md.angle)
                + caster->drawable.md.projectile_spawn_point.j * sin(caster->drawable.md.angle);
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
        unit->u.projectile.start_coords.i += caster->drawable.md.projectile_spawn_point.i * sin(caster->drawable.md.angle)
                - caster->drawable.md.projectile_spawn_point.j * cos(caster->drawable.md.angle);

        unit->u.projectile.start_coords.j += caster->drawable.md.projectile_spawn_point.i * cos(caster->drawable.md.angle)
                + caster->drawable.md.projectile_spawn_point.j * sin(caster->drawable.md.angle);
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

    bbList_sortL(&unitSquare->list, unit);

    unit->u.projectile.list.list_id = 0;
    unit->u.projectile.list.prev = viewport_app->projectiles.pool->null;
    unit->u.projectile.list.next = viewport_app->projectiles.pool->null;
    bbList_pushL(&viewport_app->projectiles,unit);

    return bbSuccess;
}
