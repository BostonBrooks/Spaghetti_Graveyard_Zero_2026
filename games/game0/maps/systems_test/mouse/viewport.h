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

    //send command to player,
    //player sends request to server,
    //message from server spawns skelly
    bbCoreInbox_TestClick(&home.core.core, MC2,0);

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


         bbScreenPoints screen_points = mouse->position;
         bbViewportCoords VC = bbScreenPoints_getViewportPoints(&home.viewport_app.viewport, screen_points);
         bbMapCoords MC = bbViewportCoords_getMapCoords(VC);
        //
        // bbDrawable* drawable;
        // bbDrawable_newPoint(&drawable, home.viewport_app.drawables,
        //                   &home.UI.graphics, MC);

        if (!bbVPool_handleIsEqual(pool,handle,mouse->is_over))
        {
            mouse->was_over = mouse->is_over;
            mouse->is_over = handle;
        }
        return bbBreak;
    }
    return bbContinue;
}
