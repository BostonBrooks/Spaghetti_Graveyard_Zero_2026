#include "engine/viewport/bbViewportMouse.h"

#include "bbViewportApp.h"


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
        bbDebug("mouse moved over viewport");
        break;

    case bbEvtMouseButtonPressed:
        bbDebug("mouse clicked viewport");
        break;

    case bbEvtMouseButtonReleased:
        bbDebug("mouse unclicked viewport");
        break;

    case bbEvtMouseEnter:
        bbDebug("mouse entered viewport");
        break;

    case bbEvtMouseLeave:
        bbDebug("mouse left viewport");
        break;

    }
    return bbSuccess;
}