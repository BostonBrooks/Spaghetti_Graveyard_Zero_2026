
#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbMapIcons.h"

bbFlag spawnDrawables(void)
{
    bbViewportApp* app = &home.viewport_app;


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

    for (I32 i = 0; i<NUM_AGENTS;i++)
    {

        MC.i = (i%8)*POINTS_PER_TILE*4+10208;
        MC.j = (i/8)*POINTS_PER_TILE*4+10028;
        MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
        bbUnit_newSkeleton(&cat,home.viewport_app.units, &home.UI.graphics, MC, i);
        cat->drawable.frames[0].handle.u64 = 10;


        bbHandle drawfunctionHandle;
        bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "MAPICON_TEST",
                    &drawfunctionHandle);




        cat->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
        cat->drawable.frames[2].handle.u64 = 614;
        cat->drawable.frames[2].start_time =  -(rand()%6);
        cat->drawable.frames[2].framerate = 1;
        cat->drawable.frames[2].offset.x = 0;
        cat->drawable.frames[2].offset.y = 0;

    }
    for (I32 i = NUM_AGENTS; i<numMoveables;i++)
    {

        MC.i = (i%8)*POINTS_PER_TILE*4+10028;
        MC.j = (i/8)*POINTS_PER_TILE*4+10028;
        MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
        bbUnit_newSkeleton(&cat,home.viewport_app.units, &home.UI.graphics, MC, i);
    }
    //bbAgent* agent;
    //bbAgent_constructor(&agent, home.shared.agents, "PLAYER", "PLAYER", MC);

    return bbSuccess;
}

