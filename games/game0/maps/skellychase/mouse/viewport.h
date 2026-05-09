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
    bbLocalMessage_SetViewpoint(&home.core.core, MC);
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
    return bbSuccess;
}