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
    switch (event->type)
    {
    case bbEvtMouseMoved:
        bbDebug("mouse moved over viewport\n");
        break;

    case bbEvtMouseButtonPressed:
        bbDebug("mouse clicked viewport\n");
        break;

    case bbEvtMouseButtonReleased:
        bbDebug("mouse unclicked viewport\n");
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

    MC.i -= POINTS_PER_SQUARE /2;
    MC.j -= POINTS_PER_SQUARE /2;

    I32 squares_i = units->squares_i;
    I32 squares_j = units->squares_j;

    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);

    bbVPMouse_isOver_cl cl;
    cl.mouse = vpmouse;
    cl.handle = units->pool->null;

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

    debug_off = false;



    //bbDebug("pool name: %s\n",   units->squares[n].list.pool->pool_name);
    //bbNotImplemented() - doesnt work when nested list has one element
    //bbNestedList_attach(&list, &units->squares[n+1].list);

    bbNestedListR_map(&list, bbVPMouse_isOverFunc, &cl);

    debug_off = true;


    *unit_handle = cl.handle;

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

    bbFlag_print(flag)
    if (flag == bbContinue) return bbContinue;

    unit->drawable.state = bbDrawableState_idle;
    bbHere()
    return bbBreak;

}