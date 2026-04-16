#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbViewportCoords.h"

bbFlag Viewport_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;

    bbScreenPoints screen_points = mouse->position;
    screen_points.x -= widget->rect.left;
    screen_points.y -= widget->rect.top;

    bbMapCoords MC = bbScreenCoords_getMapCoords(screen_points, widget->extra_data);
    bbLocalMessage_SetViewpoint(&home.core.core, MC);
    return bbSuccess;
}

bbFlag Viewport_RightDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{

    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;

    bbScreenPoints screen_points = mouse->position;
    screen_points.x -= widget->rect.left;
    screen_points.y -= widget->rect.top;

    bbMapCoords MC = bbScreenCoords_getMapCoords(screen_points, widget->extra_data);
    bbLocalMessage_SetGoalpoint(&home.core.core, MC);
    return bbSuccess;
}