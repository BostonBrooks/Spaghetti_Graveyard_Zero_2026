bbFlag bbProjectile_spawnFireball(bbViewportApp* viewport_app,
                                bbProjectile_data data,
                                bbHandle entity_handle,
                                bbUnit*
                                U64 random_seed) {

    bbUnit* unit;
    bbUnits* units = home.viewport_app.units;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = units->pool;
    bbMapCoords MC = data.start_coords;
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
        //bbHandle* entity_unit;
        //bbVPool_allocFromHandle(home.viewport_app.entity_units, (void**)&entity_unit, entity_handle);
        //*entity_unit = unit_handle;

        bbLookupTable_update(home.viewport_app.entity_units2,entity_handle,unit_handle);
    }

    unit->type =  bbUnit_unit;
    unit->u.projectile = data;
    unit->drawable.md.state = bbDrawableState_moving;
    unit->entity_handle = entity_handle;
    //bbDebug("system = %u\n", entity_handle.system.system);

    unit->u.unit.mouse.mouse_table.u64 = 0;



    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.md.frames[k].draw_function = -1;
    }

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "DRAWBUFFER_SPRITE",
                        &drawfunctionHandle);

    unit->drawable.md.frames[0].draw_function = drawfunctionHandle.u64;
    unit->drawable.md.frames[0].asset_handle.u64 = 700;
    unit->drawable.md.frames[0].start_time= 0;
    unit->drawable.md.frames[0].framerate = 1;
    unit->drawable.md.frames[0].offset.x = 0;
    unit->drawable.md.frames[0].offset.y = 0;
        bbList_sortL(&unitSquare->list, unit);

    return bbSuccess;
}
