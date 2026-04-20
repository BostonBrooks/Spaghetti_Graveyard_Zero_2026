
#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbMapIcons.h"

bbFlag spawnDrawables(void)
{
    bbViewportApp* app = &home.viewport_app;
    for (I32 i = 0; i<24;i++){
        for (I32 j=0; j<24;j++){

            bbMapCoords MC;
            MC.i = i * (POINTS_PER_SQUARE / 2);
            MC.j = j * (POINTS_PER_SQUARE / 2);
            MC.k = 0;

            MC.i += rand()%(100 * POINTS_PER_PIXEL);
            MC.j += rand()%(100 * POINTS_PER_PIXEL);

            bbDrawable* drawable;
            bbDrawable_newTree(&drawable, app->drawables, &home.UI.graphics,
                               MC);
            bbMapIcon* mapicon;
            bbMapIcon_new(&mapicon, app->mapIcons,&home.UI.graphics, MC);
            //bbAvoidable* avoidable;
            //bbAvoidable_new(&avoidable, app->avoidables, MC, 160);


        }
    }

    for (I32 i = 0; i<8;i++) {
        for (I32 j = 0; j < 8; j++) {
            bbMapCoords MC;

            MC.i = i * (POINTS_PER_SQUARE);
            MC.j = j * (POINTS_PER_SQUARE);
            MC.k = 0;

            MC.i += rand()%(100 * POINTS_PER_PIXEL);
            MC.j += rand()%(100 * POINTS_PER_PIXEL);

            //bbAgent* agent;
            //bbAgent_constructor(&agent, home.shared.agents, "SKELETON", "SKELETON", MC);

        }
    }

    //bbUnit* player;
    //bbUnit_newCat(&player, app->units, &home.constant.graphics,
    ///           home.private.viewportApp.viewport.viewpoint);
    //home.shared.player = NULL;

    bbMapCoords MC;
    MC.i = 50;
    MC.j = 100;
    MC.k = 0;
    bbUnit* cat;

    bbUnit_newSkeleton(&cat,home.viewport_app.units, &home.UI.graphics, MC, 0);
    cat->drawable.frames[0].handle.u64 = 10;

    for (I32 i = 1; i<numMoveables;i++)
    {
        MC.i = i*POINTS_PER_TILE;
        MC.j = i*POINTS_PER_TILE;
        bbUnit_newSkeleton(&cat,home.viewport_app.units, &home.UI.graphics, MC, i);
    }
    //bbAgent* agent;
    //bbAgent_constructor(&agent, home.shared.agents, "PLAYER", "PLAYER", MC);

    return bbSuccess;
}

