///on being clicked,
///This button sets itself as frozen,
///Then sends a message to the core
///The core react and send a message to the GUI
///The GUI reacts and unfreezes the button


#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"

bbFlag bbWidget_Constructor_ReactButton(bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics){


    bbHere()
    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 75 * SCREEN_PPP;
    rect.height = 35 * SCREEN_PPP;

    widget->rect = rect;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "BUTTON_STATE",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->compositions->dictionary,
                        "BUTTON", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary, "WIDGET_TEXT",
                &drawfunctionHandle);
    widget->frames[1].drawfunction = drawfunctionHandle.u64;
    widget->frames[1].offset.x = 3*SCREEN_PPP;
    widget->frames[1].offset.y = 3*SCREEN_PPP;

    int funcInt;
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseIsOver,
                                  "HOVER");
    widget->mtable.is_over = funcInt;

    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseLeftDown,
                                  "BUTTON");
    widget->mtable.left_down = funcInt;

    widget->mtable.mouse_icon = 86;

    if (self!=NULL) *self = widget;

    return bbSuccess;
}