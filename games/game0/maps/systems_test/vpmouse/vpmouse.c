


#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/viewport/bbViewportMouseFunctions.h"
#include "games/game0/maps/systems_test/vpmouse/null_mouse_functions.h"
#include "games/game0/maps/systems_test/vpmouse/monster_mouse_functions.h"
#include "games/game0/maps/systems_test/vpmouse/PC_mouse_functions.h"

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

    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseIsOver, bbVPMouse_IsOver_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseEnter, bbVPMouse_Enter_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeave, bbVPMouse_Leave_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDown, bbVPMouse_LeftDown_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftUp, bbVPMouse_LeftUp_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDrag, bbVPMouse_LeftDrag_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDown, bbVPMouse_RightDown_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightUp, bbVPMouse_RightUp_Monster, "MONSTER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDrag, bbVPMouse_RightDrag_Monster, "MONSTER");

    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseIsOver, bbVPMouse_IsOver_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseEnter, bbVPMouse_Enter_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeave, bbVPMouse_Leave_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDown, bbVPMouse_LeftDown_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftUp, bbVPMouse_LeftUp_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseLeftDrag, bbVPMouse_LeftDrag_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDown, bbVPMouse_RightDown_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightUp, bbVPMouse_RightUp_Player, "PLAYER");
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseRightDrag, bbVPMouse_RightDrag_Player, "PLAYER");

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

    table.is_over =  bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseIsOver,"MONSTER");
    table.enter = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseEnter,"MONSTER");
    table.leave = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeave,"MONSTER");
    table.left_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDown,"MONSTER");
    table.right_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDown,"MONSTER");
    table.left_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftUp,"MONSTER");
    table.right_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightUp,"MONSTER");
    table.left_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDrag,"MONSTER");
    table.right_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDrag,"MONSTER");
    table.drop = -1;
    table.mouse_icon = 133;
    table.drag_icon = -1;
    table.on_click = -1;
    table.on_unclick = -1;

    bbStr_setStr(table.key,"MONSTER", KEY_LENGTH);

    bbVPMouseFunctions_addTable(&home.viewport_app.mouse.functions,&table,NULL,"MONSTER");

    table.is_over =  bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseIsOver,"PLAYER");
    table.enter = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseEnter,"PLAYER");
    table.leave = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeave,"PLAYER");
    table.left_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDown,"PLAYER");
    table.right_down = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDown,"PLAYER");
    table.left_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftUp,"PLAYER");
    table.right_up = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightUp,"PLAYER");
    table.left_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseLeftDrag,"PLAYER");
    table.right_drag = bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseRightDrag,"PLAYER");
    table.drop = -1;
    table.mouse_icon = 133;
    table.drag_icon = -1;
    table.on_click = -1;
    table.on_unclick = -1;

    bbStr_setStr(table.key,"PLAYER", KEY_LENGTH);

    bbVPMouseFunctions_addTable(&home.viewport_app.mouse.functions,&table,NULL,"PLAYER");

    return bbSuccess;
}
