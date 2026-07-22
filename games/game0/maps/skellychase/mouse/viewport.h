#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/spawn_entity.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/geometry/bbViewportCoords.h"

bbFlag Viewport_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{bbHere()
    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbMapCoords MC = home.viewport_app.viewport.viewpoint;
    bbMapCoords MC2 = bbViewportCoords_getMapCoords(VC);
    bbCoreInbox_TestClick2(&home.core.core, MC);

    return bbSuccess;
}

bbFlag Viewport_RightDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{
bbHere()
    // bbMouse* mouse = (bbMouse*)Mouse;
    // bbWidget* widget = (bbWidget*)Widget;
    //
    // bbScreenPoints screen_points = mouse->position;
    // bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    // bbMapCoords MC = bbViewportCoords_getMapCoords(VC);
    // bbLocalMessage_MapClick(&home.core.core, MC, 0);
    //
    //
    // //bbDebug("move to i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);
    return bbSuccess;
}