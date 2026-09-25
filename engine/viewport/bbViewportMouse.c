#include "engine/viewport/bbViewportMouse.h"

#include "bbViewportApp.h"
#include "engine/geometry/bbGroundCoords.h"
#include "engine/logic/bbNestedListR.h"


bbFlag bbVPMouse_Init(bbVPMouse* vpmouse, void* viewportApp, bbDrawables* units, bbMouse* mouse, bbGraphicsApp* graphics)
{
    vpmouse->position.x = 0;
    vpmouse->position.y = 0;
    vpmouse->previous_position.x = 0;
    vpmouse->previous_position.y = 0;
    vpmouse->drag_origin.x = 0;
    vpmouse->drag_origin.y = 0;

    vpmouse->left_down = false;
    vpmouse->right_down = false;
    vpmouse->left_changed = false;
    vpmouse->right_changed = false;
    vpmouse->mouse_within_viewport = false;
    vpmouse->was_over = units->pool->null;
    vpmouse->is_over = units->pool->null;
    vpmouse->selected = units->pool->null;

    //TODO choose an icon
    vpmouse->default_mouse_icon = 0;
    bbVPMouseFunctions_init(&vpmouse->functions);
    //bbVPMouseFunctions_populate(&vpmouse->functions);
    vpmouse->units = units;
    vpmouse->viewportApp = viewportApp;
    vpmouse->mouse = mouse;
    vpmouse->graphics = graphics;

    bbViewportApp* ViewportApp = (bbViewportApp*)viewportApp;
    ViewportApp->viewport.mouse = vpmouse;
    return bbSuccess;
}


bbFlag bbVPMouse_Event(bbVPMouse* vpmouse, bbVPMouse_event* event)
{
    bbVPMouse_button button;
    switch (event->type)
    {
    case bbEvtMouseMoved:
        bbDebug("mouse moved over viewport\n");
        break;

    case bbEvtMouseButtonPressed:
        bbDebug("mouse clicked viewport\n");
        button = event->button;
        if (button == bbMouseLeft)
        {
            vpmouse->left_changed = true;
            vpmouse->left_down = true;
            vpmouse->previous_position = vpmouse->position;

        } else if (button == bbMouseRight)
        {
            vpmouse->right_changed = true;
            vpmouse->right_down = true;
            vpmouse->previous_position = vpmouse->position;
        }
        break;

    case bbEvtMouseButtonReleased:
        bbDebug("mouse unclicked viewport\n");
        button = event->button;
        if (button == bbMouseLeft)
        {
            vpmouse->left_changed = true;
            vpmouse->left_down = false;
            vpmouse->previous_position = vpmouse->position;
        } else if (button == bbMouseRight)
        {
            vpmouse->right_changed = true;
            vpmouse->right_down = false;
            vpmouse->previous_position = vpmouse->position;
        }
        break;

    case bbEvtMouseEnter:
        bbDebug("mouse entered viewport\n");
        break;

    case bbEvtMouseLeave:
        bbDebug("mouse left viewport\n");
        break;
    case bbEvtDrag:
        bbDebug("mouse drag viewport\n");
        break;

    }
    return bbSuccess;
}

bbFlag bbVPMouse_isOver(bbVPMouse* vpmouse, bbHandle* unit_handle)
{
    bbViewportApp* viewportApp = (bbViewportApp*)vpmouse->viewportApp;
    bbViewportCoords viewport_coords = vpmouse->position;
    bbDrawables* units = viewportApp->units;
    bbMapCoords MC = bbViewportCoords_getMapCoords(viewport_coords);

    bbWidget* vp_widget = viewportApp->viewport.widget;
    if (vp_widget->mtable.hover == 0)
    {
        if (unit_handle != NULL)
        {
            unit_handle->u64 = 0;
        }

        vpmouse->is_over.u64 = 0;
        return bbFail;
    }

    MC.i -= POINTS_PER_SQUARE /2;
    MC.j -= POINTS_PER_SQUARE /2;

    I32 squares_i = units->squares_i;
    I32 squares_j = units->squares_j;

    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);

    bbVPMouse_isOver_cl cl;
    cl.mouse = vpmouse;
    cl.handle = units->pool->null;
    cl.unit = NULL;

    bbNestedList list;
    bbNestedListR_init(&list);

    I32 i = SC.i;
    I32 j = SC.j;
    //bbNotImplemented()
    // for (I32 i = square_i_min; i < square_i_max; ++i) {
    //     for (I32 j = square_j_min; j < square_j_max; ++j) {
    I32 n = i + squares_i * j;
    if (n >=0 && n < squares_i * squares_j )
        bbNestedListR_attach(&list, &units->squares[n].list);


    n = (i+1) + squares_i * j;
    if (n >=0 && n < squares_i * squares_j )
        bbNestedListR_attach(&list, &units->squares[n].list);

    n = i + squares_i * (j+1);
    if (n >=0 && n < squares_i * squares_j )
        bbNestedListR_attach(&list, &units->squares[n].list);

    n = (i+1) + squares_i * (j+1);
    if (n >=0 && n < squares_i * squares_j )
        bbNestedListR_attach(&list, &units->squares[n].list);




    //bbDebug("pool name: %s\n",   units->squares[n].list.pool->pool_name);
    //bbNotImplemented() - doesnt work when nested list has one element
    //bbNestedList_attach(&list, &units->squares[n+1].list);

    bbNestedListR_map(&list, bbVPMouse_isOverFunc, &cl);

    //bbDebug("handle = (%u, %u, %u)\n", cl.handle.system.system, cl.handle.system.index, cl.handle.system.generation);


    if (unit_handle != NULL)
    {
        *unit_handle = cl.handle;
    }

    if (bbSuccess != bbVPool_handleIsEqual(units->pool,vpmouse->is_over, cl.handle))
    {
        bbUnit* was_over;
        bbFlag flag2 = bbVPool_lookup(units->pool,(void**)&was_over,vpmouse->was_over);
        if (flag2 == bbSuccess) was_over->mouse.hover = false;
        if (cl.unit!=NULL)
        {
            bbUnit* is_over = cl.unit;
            is_over->mouse.hover = true;
        }

        vpmouse->was_over = vpmouse->is_over;
        vpmouse->is_over = cl.handle;

    }
    vpmouse->selected_unit = cl.unit;
    return bbSuccess;

}

///bbVPMouse_isOverFunc is mapped to each unit until one is found under the viewport mouse
bbFlag bbVPMouse_isOverFunc(void* node, void* cl)
{

    //printf("test bbVPMouse_isOverFunc\n");
    bbUnit* unit = (bbUnit*)node;
    bbVPMouse_isOver_cl* data = cl;
    bbVPMouse* vpmouse = data->mouse;
    bbViewportApp* viewportApp = vpmouse->viewportApp;
    bbViewportCoords mouse_coords = vpmouse->position;
    bbDrawables* units = viewportApp->units;
    bbViewportCoords unit_coords;

    bbHandle mouse_table_handle = unit->mouse.mouse_table;

    bbMouseTable* mouse_table;

    bbFlag flag = bbVPool_lookup(vpmouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);

    if (flag != bbSuccess)
    {
        return bbContinue;
    }

    I32 func_int = mouse_table->is_over;

    if (func_int == -1)
    {
        return bbContinue;
    }

    bbVPMouse_IsOver* function;
    bbVPMouseFunctions_getFunction(&vpmouse->functions, (void**)&function, VPMouseIsOver, func_int);

    if (function == NULL)
    {
        return bbContinue;
    }

    flag = function(vpmouse, units, node);

    if (flag == bbContinue) return bbContinue;

    bbHandle unit_handle;
    bbVPool_reverseLookup(units->pool,unit, &unit_handle);

    data->handle = unit_handle;
    data->unit = unit;

    return bbBreak;

}

bbFlag bbVPMouse_Update(bbVPMouse* mouse, bbGraphicsApp* graphics)
{

    bbViewportApp* viewportApp = mouse->viewportApp;
    bbWidget* widget = viewportApp->viewport_widget;
    bbUnits* units = viewportApp->units;
    bbUnit* unit = mouse->selected_unit;

    if (unit!=NULL)
    {

        bbHandle mouse_table_handle = unit->mouse.mouse_table;
        bbVPMouseTable* mouse_table;
        bbFlag flag = bbVPool_lookup(mouse->functions.mouse_tables,(void**)&mouse_table,mouse_table_handle);

        //bbDebug("mouse_table_key = %s\n", mouse_table->key);

        if (flag != bbSuccess)
        {
            widget->mtable.mouse_icon = 85;
        } else
        {
            widget->mtable.mouse_icon = mouse_table->mouse_icon;
        }
    } else
    {
        widget->mtable.mouse_icon = 85;
    }


    //bbDebug("mouse.isover = %x\n", mouse->is_over.ptr);
    //bbDebug("mouse.wasover = %x\n", mouse->is_over.ptr);
    if(bbSuccess == bbVPool_handleIsNULL(units->pool, mouse->was_over))
    {
        bbUnit* enter_unit;
        bbUnit* leave_unit;
        bbFlag flag, flag2;
        if (bbSuccess == (flag = bbVPool_lookup(units->pool,(void**)&leave_unit,mouse->was_over)))
        {
            if (leave_unit != NULL) bbVPMouse_LeaveUnit(mouse, leave_unit);
            //bbDebug("unit = %p\n", leave_unit);
        }
        if (bbSuccess == (flag2 = bbVPool_lookup(units->pool,(void**)&enter_unit,mouse->is_over)))
        {//bbHere()

            if (enter_unit != NULL) bbVPMouse_EnterUnit(mouse, enter_unit);
            //bbDebug("unit = %p\n", enter_unit);
        }


        //bbFlag_print(flag);
        //bbFlag_print(flag2);
        mouse->was_over = units->pool->null;
    }
    bbUnit* unit2;
    if (mouse->left_down && mouse->left_changed)
    {
        bbVPool_lookup(units->pool, (void**)&unit2, mouse->is_over);
        bbVPMouse_LeftDownUnit(mouse, unit2);

        mouse->selected = mouse->is_over;
    }
    else if (!mouse->left_down && mouse->left_changed)
    {
        if (bbSuccess != bbVPool_handleIsEqual(units->pool, mouse->selected, units->pool->null))
        {
            bbVPool_lookup(units->pool, (void**)&unit2, mouse->selected);
            bbVPMouse_LeftUpUnit(mouse, unit2);
        }
    }
    else if (mouse->left_down && !mouse->left_changed)
    {
        if (bbSuccess != bbVPool_handleIsEqual(units->pool, mouse->selected, units->pool->null))
        {
            bbVPool_lookup(units->pool, (void**)&unit2, mouse->selected);
            bbVPMouse_LeftDragUnit(mouse, unit2);
        }
    }

    bbUnit* unit3;
    if (mouse->right_down && mouse->right_changed)
    {
        bbVPool_lookup(units->pool, (void**)&unit3, mouse->is_over);
        bbVPMouse_RightDownUnit(mouse, unit3);

        mouse->selected = mouse->is_over;
    }
    else if (!mouse->right_down && mouse->right_changed)
    {
        if (bbSuccess != bbVPool_handleIsEqual(units->pool, mouse->selected, units->pool->null))
        {
            bbVPool_lookup(units->pool, (void**)&unit3, mouse->selected);
            bbVPMouse_RightUpUnit(mouse, unit3);
        }
    }
    else if (mouse->right_down && !mouse->right_changed)
    {
        if (bbSuccess != bbVPool_handleIsEqual(units->pool, mouse->selected, units->pool->null))
        {
            bbVPool_lookup(units->pool, (void**)&unit3, mouse->selected);
            bbVPMouse_RightDragUnit(mouse, unit3);
        }
    }

    mouse->right_changed = false;
    mouse->left_changed = false;

    return bbSuccess;
}