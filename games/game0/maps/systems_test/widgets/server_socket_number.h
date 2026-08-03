#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"


bbFlag bbWidget_Constructor_Socket_Number(bbWidget** self,
                                        bbWidgets* widgets,
                                        bbWidget* parent,
                                        char* name,
                                        bbScreenPoints screen_points,
                                        bbGraphicsApp* graphics)
{
    bbHere()
    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0 * SCREEN_PPP;
    rect.height = 0 * SCREEN_PPP;

    widget->rect = rect;
    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                 "SOCKET_NUMBER",
                 &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;
    widget->frames[0].offset.x = 3*SCREEN_PPP;
    widget->frames[0].offset.y = 3*SCREEN_PPP;


    if (self!=NULL) *self = widget;

    return bbSuccess;
}

