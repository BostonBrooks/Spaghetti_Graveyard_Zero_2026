#include "games/game0/maps/systems_test/render_units/tree_group.h"

#include "engine/data/bbHome.h"

bbFlag bbViewportSpawnTrees(bbViewportApp *viewport_app,
                            bbMapCoords MC,
                            U64 random_seed){


    bbDrawable* drawable;
    bbDrawables* drawables = home.viewport_app.drawables;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = drawables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbDrawableSquare* drawableSquare = bbDrawables_getSquare(drawables,SC.i, SC.j, drawables->squares_i, drawables->squares_j);

    bbHandle drawable_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&drawable,&drawable_handle);

    drawable->functions.on_enter = bbDrawables_getFunction(&home.viewport_app.table, "SPAWN TREES");
    drawable->functions.on_leave = bbDrawables_getFunction(&home.viewport_app.table, "UNSPAWN FOXES");
    drawable->functions.on_square_enter = bbDrawables_getFunction(&home.viewport_app.table, "SPAWN TREES");
    drawable->functions.on_square_leave = bbDrawables_getFunction(&home.viewport_app.table, "UNSPAWN FOXES");


    drawable->md.random_seed = random_seed;
    drawable->md.coords = MC;
    drawable->md.SC = SC;
drawable->md.state = bbDrawableState_idle;
    bbHandle drawfunctionHandle;
    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
         "TREE GROUP", &drawfunctionHandle);

    drawable->md.frames[0].draw_function = drawfunctionHandle.u64;
    drawable->md.frames[0].asset_handle.u64 = 12;
    drawable->md.frames[0].start_time =  -(rand()%6);
    drawable->md.frames[0].framerate = 1;
    drawable->md.frames[0].offset.x = 0;
    drawable->md.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
     "DRAWABLE_SHADOW", &drawfunctionHandle);

    drawable->md.frames[1].draw_function = drawfunctionHandle.u64;
    drawable->md.frames[1].asset_handle.u64 = 612;
    drawable->md.frames[1].start_time =  -(rand()%6);
    drawable->md.frames[1].framerate = 1;
    drawable->md.frames[1].offset.x = 0;
    drawable->md.frames[1].offset.y = 0;

    for (I32 k = 1; k < FRAMES_PER_DRAWABLE; k++){
        drawable->md.frames[k].draw_function = -1;
    }



    // bbRenderUnitGroup_spawnKey(viewport_app->renderUnits,
    //                            "TREES",
    //                            &unit->drawable);

    bbList_sortL(&drawableSquare->list, drawable);

    return bbSuccess;
}



bbFlag bbRenderUnitGroup_spawn_trees(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics) {
    bbRenderUnitGroup* group;
    bbVPool_alloc2(render_units->pool, (void**)&group, NULL);
    drawable->group = group;

    bbRenderUnit a_tree;
    a_tree.md.coords = drawable->md.coords;
    a_tree.md.state = bbDrawableState_idle;
    a_tree.movement_type = bbRU_movementType_none;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++) {
        a_tree.md.frames[k].draw_function = -1;
    }
    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                "DRAWBUFFER_SPRITE",
                &drawfunctionHandle);
    a_tree.md.frames[0].draw_function = drawfunctionHandle.u64;
    a_tree.md.frames[0].asset_handle.u64 = 1075;
    a_tree.md.frames[0].start_time=  0;
    a_tree.md.frames[0].framerate = 0;
    a_tree.md.frames[0].offset.x = 0;
    a_tree.md.frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                "DRAWABLE_SHADOW",
                &drawfunctionHandle);
    a_tree.md.frames[1].draw_function = drawfunctionHandle.u64;
    a_tree.md.frames[1].asset_handle.u64 = 1076;
    a_tree.md.frames[1].start_time=  0;
    a_tree.md.frames[1].framerate = 0;
    a_tree.md.frames[1].offset.x = 0;
    a_tree.md.frames[1].offset.y = 0;

    a_tree.owner = drawable;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++ )
    {
        group->units[i] = a_tree;
        group->units[i].index = i;
        group->units[i].md.random_seed = bbArith64_hashIndex(drawable->md.random_seed, i);
        group->units[i].md.coords.i
            += (i/3) * POINTS_PER_TILE *2
            + bbArith64_hashIndex(group->units[i].md.random_seed, i)%(POINTS_PER_TILE)
            - POINTS_PER_TILE * 3;
        group->units[i].md.coords.j
            += (i%3) * POINTS_PER_TILE *2
            + bbArith64_hashIndex(group->units[i].md.random_seed, i+12)%(POINTS_PER_TILE)
            - POINTS_PER_TILE * 2;
        group->units[i].md.coords.k = bbMapCoords_getElevation(&home.ground_surface, group->units[i].md.coords);
    }

    bbList_pushL(&render_units->list, group);

    if (Group != NULL) *Group = group;
    return bbSuccess;
}

bbFlag bbRenderUnitGroup_spawnTrees_fn(struct bbRenderUnits* render_units, bbDrawable* drawable)
{

    bbRenderUnitGroup_spawn_trees(NULL,
                                  render_units,
                                  drawable,
                                  &home.UI.graphics);

    return bbSuccess;
}