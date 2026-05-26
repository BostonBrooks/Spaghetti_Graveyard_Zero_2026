#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/geometry/bbViewportCoords.h"

bbFlag Viewport_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;

    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbMapCoords MC = home.viewport_app.viewport.viewpoint;
    bbMapCoords MC2 = bbViewportCoords_getMapCoords(VC);
    //bbLocalMessage_SpawnUnit(&home.core.core, MC, MC2, "BALLOON");


    bbLocalMessage_MapClick(&home.core.core, MC2, 0);
    //bbDebug("goal.i = %d, goal.j = %d\n", MC2.i, MC2.j);
    return bbSuccess;
}

bbFlag Viewport_RightDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{

    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;

    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbMapCoords MC = bbViewportCoords_getMapCoords(VC);
    bbLocalMessage_SetGoalpoint(&home.core.core, MC);


    //bbDebug("move to i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);
    return bbSuccess;
}