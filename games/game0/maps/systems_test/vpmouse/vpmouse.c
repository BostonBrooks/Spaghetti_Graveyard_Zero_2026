


#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/viewport/bbViewportMouseFunctions.h"

bbFlag bbVPMouseFunctions_populate(bbVPMouseFunctions* self)
{
    bbVPMouseFunctions_add(&home.viewport_app.mouse.functions, VPMouseIsOver, bbVPMouse_IsOver_NULL, "NULL FUNCTION");



    bbVPMouseTable table;
    table.is_over =  bbVPMouseFunctions_getInt(&home.viewport_app.mouse.functions,VPMouseIsOver,"NULL FUNCTION");
    table.enter = -1;
    table.leave = -1;
    table.left_down = -1;
    table.right_down = -1;
    table.left_up = -1;
    table.right_up = -1;
    table.left_drag = -1;
    table.right_drag = -1;
    table.drop = -1;
    table.mouse_icon = 133;
    table.drag_icon = -1;
    table.on_click = -1;
    table.on_unclick = -1;

    bbStr_setStr(table.key,"NULL TABLE", KEY_LENGTH);

    bbVPMouseFunctions_addTable(&home.viewport_app.mouse.functions,&table,NULL,"NULL TABLE");

    return bbSuccess;
}
