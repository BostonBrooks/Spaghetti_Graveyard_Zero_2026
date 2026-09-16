#include "bbViewportMouse.h"
#include "engine/logic/bbVPool.h"
#include "engine/userinterface/bbMouse.h"
///call the unit's enter_unit function
bbFlag bbVPMouse_EnterUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{bbHere()
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->enter;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_Enter* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseEnter, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);



}

///call the unit's leave_unit function
bbFlag bbVPMouse_LeaveUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{bbHere()
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->leave;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_Leave* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseLeave, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);
}

///call the unit's left_down function
bbFlag bbVPMouse_LeftDownUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->left_down;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_LeftDown* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseLeftDown, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);
}


///call the unit's left_up function
bbFlag bbVPMouse_LeftUpUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->left_up;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_LeftUp* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseLeftUp, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);
}


///what happens when you try to drag the unit?
bbFlag bbVPMouse_LeftDragUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->left_drag;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_LeftDrag* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseLeftDrag, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);
}

///call the unit's right_down function
bbFlag bbVPMouse_RightDownUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{
    bbHandle mouse_table_handle = unit->mouse.mouse_table;
    bbMouseTable* mouse_table;
    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
    bbUnits* units = vpmouse->units;
    bbGraphicsApp* graphics_app = vpmouse->graphics;
    I32 func_int = mouse_table->right_down;
    if (func_int == -1) return bbSuccess;

    bbVPMouse_RightDown* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseRightDown, func_int);

    if (function == NULL) return bbSuccess;

    return function(vpmouse,units, unit, graphics_app);
}
///call the unit's right_up function
bbFlag bbVPMouse_RightUpUnit(bbVPMouse* vpmouse, struct bbUnit* unit)
{


        bbHandle mouse_table_handle = unit->mouse.mouse_table;
        bbMouseTable* mouse_table;
        bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
        bbUnits* units = vpmouse->units;
        bbGraphicsApp* graphics_app = vpmouse->graphics;
        I32 func_int = mouse_table->right_up;
        if (func_int == -1) return bbSuccess;

        bbVPMouse_RightUp* function;
        bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseRightUp, func_int);

        if (function == NULL) return bbSuccess;

        return function(vpmouse,units, unit, graphics_app);

}

///what happens when you try to drag the unit with the right button?
bbFlag bbVPMouse_RightDragWidget(bbVPMouse* vpmouse, struct bbUnit* unit)
{


        bbHandle mouse_table_handle = unit->mouse.mouse_table;
        bbMouseTable* mouse_table;
        bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);
        bbUnits* units = vpmouse->units;
        bbGraphicsApp* graphics_app = vpmouse->graphics;
        I32 func_int = mouse_table->right_drag;
        if (func_int == -1) return bbSuccess;

        bbVPMouse_RightDrag* function;
        bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseRightDrag, func_int);

        if (function == NULL) return bbSuccess;

        return function(vpmouse,units, unit, graphics_app);

}