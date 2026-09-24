#include "core/core_inbox.h"
#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbVPMouse_IsOver_Monster (void* Mouse, void* units, void* Unit)
{


    bbUnit* unit = Unit;
    I32 closeness = unit->drawable.md.coords.i - unit->drawable.md.coords.j;

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
    bbFlag flag  = bbMapCoords_withinRect(mouse_coords, unit->drawable.md.coords, rect);

    if (flag == bbSuccess) return bbBreak;

    return bbContinue;
}


bbFlag bbVPMouse_Enter_Monster (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("entering monster\n");
}
bbFlag bbVPMouse_Leave_Monster (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("leaving monster\n");
}
bbFlag bbVPMouse_LeftDown_Monster (void* Mouse, void* widgets, void* Unit,
void* graphics)
{
    bbVPMouse* mouse = (bbVPMouse*)Mouse;
    bbViewportCoords VC = mouse->position;
    bbMapCoords MC2 = bbViewportCoords_getMapCoords(VC);
    bbUnit* unit = Unit;

    bbCoreInbox_ClickMonster(&home.core.core, unit->entity_handle);

    bbDebug("clicking monster\n");
}
bbFlag bbVPMouse_LeftUp_Monster (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("unclicking monster\n");
}
bbFlag bbVPMouse_LeftDrag_Monster (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("left drag monster\n");
}
bbFlag bbVPMouse_RightDown_Monster (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right click monster\n");
}
bbFlag bbVPMouse_RightUp_Monster (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right unclick monster\n");
}
bbFlag bbVPMouse_RightDrag_Monster (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right drag monster\n");
}