#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
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

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseButtonPressed;
    event.button = bbMouseLeft;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);
    //send command to player,
    //player sends request to server,
    //message from server spawns skelly
    bbCoreInbox_TestClick(&home.core.core, MC2,1);

    return bbSuccess;
}

bbFlag Viewport_RightDown (void* Mouse, void* Widgets, void* Widget, void*
                          Graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbWidget* widget = (bbWidget*)Widget;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbMapCoords MC = home.viewport_app.viewport.viewpoint;
    bbMapCoords MC2 = bbViewportCoords_getMapCoords(VC);

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseButtonPressed;
    event.button = bbMouseRight;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    //send command to player,
    //player sends request to server,
    //message from server spawns skelly
    bbCoreInbox_TestClick(&home.core.core, MC2,0);

    return bbSuccess;
}

bbFlag Viewport_Enter (void* Mouse, void* widgets, void* widget, void* graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbWidget* Widget = widget;
    Widget->mtable.hover = 1;
bbHere()
    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseEnter;
    event.button = bbMouseRight;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;
}
bbFlag Viewport_Leave (void* Mouse, void* widgets, void* widget, void* graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
    bbWidget* Widget = widget;
    Widget->mtable.hover = 0;
bbHere()
    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseLeave;
    event.button = bbMouseRight;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;
}

bbFlag Viewport_LeftUp (void* Mouse, void* widgets, void* widget,
                               void* graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseButtonReleased;
    event.button = bbMouseLeft;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;
}
bbFlag Viewport_LeftDrag (void* Mouse, void* widgets, void* widget, void* graphics)
{

    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtDrag;
    event.button = bbMouseLeft;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;
}

bbFlag Viewport_RightUp (void* Mouse, void* widgets, void* widget,
                               void* graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtMouseButtonReleased;
    event.button = bbMouseRight;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;
}
bbFlag Viewport_RightDrag (void* Mouse, void* widgets, void* widget,
                               void* graphics)
{
    bbMouse* mouse = (bbMouse*)Mouse;
    bbScreenPoints screen_points = mouse->position;
    bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);

    bbVPMouse_event event;
    event.position = VC;
    event.type = bbEvtDrag;
    event.button = bbMouseRight;
    bbVPMouse_Event(&home.viewport_app.mouse, &event);

    return bbSuccess;

}

bbFlag IsOver_Viewport(bbMouse* mouse, bbWidgets* widgets, bbWidget* widget)
{

    bbScreenPointsRect rect = widget->rect;
    bbScreenPoints point = mouse->position;

    if (bbScreenPoints_inScreenPointsRect(point, rect)){
        bbVPool* pool = widgets->pool;
        bbHandle handle;
        bbVPool_reverseLookup(pool,widget,&handle);


         //bbScreenPoints screen_points = mouse->position;
         //bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
         //bbMapCoords MC = bbViewportCoords_getMapCoords(VC);
        //
        // bbDrawable* drawable;
        // bbDrawable_newPoint(&drawable, home.viewport_app.drawables,
        //                   &home.UI.graphics, MC);

        if (bbSuccess != bbVPool_handleIsEqual(pool,handle,mouse->is_over))
        {
            mouse->was_over = mouse->is_over;
            mouse->is_over = handle;
        }
        return bbBreak;
    }
    return bbContinue;
}

