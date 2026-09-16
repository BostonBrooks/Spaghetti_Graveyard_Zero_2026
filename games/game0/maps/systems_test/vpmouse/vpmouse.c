


#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/viewport/bbViewportMouseFunctions.h"
#include "games/game0/maps/systems_test/vpmouse/mouse_functions.h"

bbFlag bbVPMouseFunctions_populate(bbVPMouseFunctions* self)
{
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseIsOver, bbVPMouse_IsOver_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseEnter, bbVPMouse_Enter_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeave, bbVPMouse_Leave_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDown, bbVPMouse_LeftDown_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftUp, bbVPMouse_LeftUp_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDrag, bbVPMouse_LeftDrag_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDown, bbVPMouse_RightDown_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightUp, bbVPMouse_RightUp_NULL, "NULL FUNCTION");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDrag, bbVPMouse_RightDrag_NULL, "NULL FUNCTION");



    bbVPMouseTable table;
    table.is_over =  bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseIsOver,"NULL FUNCTION");
    table.enter = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseEnter,"NULL FUNCTION");
    table.leave = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeave,"NULL FUNCTION");
    table.left_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDown,"NULL FUNCTION");
    table.right_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDown,"NULL FUNCTION");
    table.left_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftUp,"NULL FUNCTION");
    table.right_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightUp,"NULL FUNCTION");
    table.left_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDrag,"NULL FUNCTION");
    table.right_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDrag,"NULL FUNCTION");
    table.drop = -1;
    table.mouse_icon = 133;
    table.drag_icon = -1;
    table.on_click = -1;
    table.on_unclick = -1;

    bbStr_setStr(table.key,"NULL TABLE", KEY_LENGTH);

    bbVPMouseFunctions_addTable(&home.viewport_app.mouse.functions,&table,NULL,"NULL TABLE");

    return bbSuccess;
}
