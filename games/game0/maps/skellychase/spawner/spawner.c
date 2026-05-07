
#include <stdio.h>

#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/spawner/bbSpawner.h"


//typedef bbFlag bbSpawnFunction (char* string);

bbFlag bbSF_null(char* string)
{
    bbDebug("%s\n", string);
    return bbSuccess;
}

bbFlag bbSF_treeGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d", key, &MC.i, &MC.j);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);


    bbDebug("%s,%d,%d,%d\n", key, MC.i, MC.j, MC.j);

    bbDrawable* drawable;
    bbDrawable_newTree(&drawable, app->drawables, &home.UI.graphics,MC);
    bbMapIcon* mapicon;
    bbMapIcon_new(&mapicon, app->mapIcons,&home.UI.graphics, MC);

    return bbSuccess;
}

bbFlag bbSF_treeCore(char* string)
{
    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d", key, &MC.i, &MC.j);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);


    bbDebug("%s,%d,%d,%d\n", key, MC.i, MC.j, MC.j);


    bbAvoidable_newCircle(home.agents_app.avoidables, MC, 193);
    return bbSuccess;
}

bbFlag bbSF_skeletonGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbUnit* unit;
    bbUnit_newSkeleton(&unit,home.viewport_app.units, &home.UI.graphics, MC, index);

    return bbSuccess;
}

bbFlag bbSF_skeletonCore(char* string)
{

    bbHere()

    return bbSuccess;
}

bbFlag bbSpawner_populate(bbSpawner* spawner)
{
    bbSpawner_add(spawner,bbSF_null, bbSF_null, "NULL");
    bbSpawner_add(spawner,bbSF_treeCore, bbSF_treeGraphics, "TREE");
    bbSpawner_add(spawner,bbSF_skeletonCore, bbSF_skeletonGraphics, "SKELETON");
    return bbSuccess;
}
