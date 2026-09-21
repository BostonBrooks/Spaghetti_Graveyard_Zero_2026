#include "core/core_inbox.h"
#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbVPMouse_IsOver_Player (void* Mouse, void* units, void* Unit)
{


    bbUnit* unit = Unit;
    I32 closeness = unit->drawable.coords.i - unit->drawable.coords.j;

    bbVPMouse* mouse = (bbVPMouse*)Mouse;
    bbViewportApp* app = (bbViewportApp*)mouse->viewportApp;
    bbMapCoords mouse_coords = bbViewportCoords_getMapCoords_k_fixed(mouse->position,0,&app->viewport);

    //bbFlag flag = bbMapCoords_withinCircle(mouse_coords, unit->drawable.coords, POINTS_PER_TILE);
    //bbDebug("the unit's closeness is %d\n", closeness);

    bbGameWorldRect rect;

    rect.top = 4*POINTS_PER_TILE;
    rect.left = POINTS_PER_TILE;
    rect.height = 4*POINTS_PER_TILE;
    rect.width = 2*POINTS_PER_TILE;
    bbFlag flag  = bbMapCoords_withinRect(mouse_coords, unit->drawable.coords, rect);

    if (flag == bbSuccess) return bbBreak;

    return bbContinue;
}


bbFlag bbVPMouse_Enter_Player (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("entering player character\n");
}
bbFlag bbVPMouse_Leave_Player (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("leaving player character\n");
}


bbFlag bbCoreInbox_ClickPlayer(bbCore* core, bbHandle entity_handle);
bbFlag bbVPMouse_LeftDown_Player (void* Mouse, void* widgets, void* Unit,
void* graphics)
{
    bbUnit* unit = Unit;

    bbCoreInbox_ClickPlayer(&home.core.core, unit->entity_handle);


    bbDebug("clicking player character\n");
}
bbFlag bbVPMouse_LeftUp_Player (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("unclicking player character\n");
}
bbFlag bbVPMouse_LeftDrag_Player (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("left player character\n");
}
bbFlag bbVPMouse_RightDown_Player (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right player character\n");
}
bbFlag bbVPMouse_RightUp_Player (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right player character\n");
}
bbFlag bbVPMouse_RightDrag_Player (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right player character\n");
}