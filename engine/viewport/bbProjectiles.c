


#include "bbProjectiles.h"

#include "bbViewportApp.h"
#include "engine/data/bbHome.h"

bbFlag bbViewportSpawnSphere(bbViewportApp* viewport_app,
                             bbMapCoords MC,
                             bbHandle entity_handle,
                             bbHandle random_seed)
{


    bbProjectile* projectile;
    bbProjectiles* projectiles = home.viewport_app.projectiles;
    bbGraphicsApp* graphics = &home.UI.graphics;
    bbVPool* pool = projectiles->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(projectiles,SC.i, SC.j, projectiles->squares_i, projectiles->squares_j);

    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&projectile,&unit_handle);
    projectile->md.class = bbDrawableClass_unit;
    projectile->md.random_seed = random_seed.u64;

    projectile->md.coords = MC;
    projectile->md.SC = SC;
    bbHandle drawfunctionHandle;
    if (entity_handle.u64 != no_handle.u64)
    {

        bbLookupTable_update(home.viewport_app.entity_projectiles,entity_handle,unit_handle);
    }


    projectile->data.start_coords = MC;
    projectile->data.end_coords = MC;
    projectile->data.start_time = 0;
    projectile->data.end_time = 193;
    projectile->data.gravity = 1000;

    for (I32 k = 0; k < FRAMES_PER_DRAWABLE; k++){
        projectile->md.frames[k].draw_function = -1;
    }
    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "DRAWBUFFER_SPRITE",
                        &drawfunctionHandle);

    projectile->md.frames[0].draw_function = drawfunctionHandle.u64;
    projectile->md.frames[0].asset_handle.u64 = 4;
    projectile->md.frames[0].start_time= 0;
    projectile->md.frames[0].framerate = 1;
    projectile->md.frames[0].offset.x = 0;
    projectile->md.frames[0].offset.y = 0;

        bbList_sortL(&unitSquare->list, projectile);

    return bbSuccess;
}
