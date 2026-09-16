#include "engine/geometry/bbGroundCoords.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbVPMouse_IsOver_NULL (void* Mouse, void* units, void* Unit)
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


bbFlag bbVPMouse_Enter_NULL (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("entering unit with null enter function\n");
}
bbFlag bbVPMouse_Leave_NULL (void* mouse, void* widgets, void* unit, void* graphics)
{
    bbDebug("leaving unit with null enter function\n");
}
bbFlag bbVPMouse_LeftDown_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("clicking unit with null enter function\n");
}
bbFlag bbVPMouse_LeftUp_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("unclicking unit with null enter function\n");
}
bbFlag bbVPMouse_LeftDrag_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("left drag unit with null enter function\n");
}
bbFlag bbVPMouse_RightDown_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right click unit with null enter function\n");
}
bbFlag bbVPMouse_RightUp_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right unclick unit with null enter function\n");
}
bbFlag bbVPMouse_RightDrag_NULL (void* mouse, void* widgets, void* unit,
void* graphics)
{
    bbDebug("right drag unit with null enter function\n");
}