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
    bbMapCoords MC = bbViewportCoords_getMapCoords(VC);


    bbDebug("spawn banana i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);

    bbLocalMessage_SpawnBanana(&home.core.core, MC);

    MC.i+=1000;
    bbLocalMessage_SpawnUnit(&home.core.core, MC, "KITTY");
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


    bbDebug("move to i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);
    return bbSuccess;
}