#ifndef WIDGET_FUNCTIONS
#define WIDGET_FUNCTIONS
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/map0/widgets/textbox.h"
#include "games/game0/maps/map0/widgets/gamewidget.h"
#include "games/game0/maps/map0/widgets/button.h"


bbFlag bbWidget_Constructor_NULL (bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics)
{
    bbHere();
}

bbFlag bbWidget_Constructor_Kitty(bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics){

    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0;
    rect.height = 0;

    widget->rect = rect;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_ANIMATION",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->animations->dictionary,
                        "KITTY", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, name, handle);

    if (self!=NULL) *self = widget;

    return bbSuccess;
}

bbFlag bbWidget_Constructor_Clock(bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics){

    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0;
    rect.height = 0;

    widget->rect = rect;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_MAPTIME_ANIMATION",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->animations->dictionary,
                        "CLOCK", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, name, handle);

    if (self!=NULL) *self = widget;

    return bbSuccess;
}

bbFlag bbWidgetFunctions_populate(bbWidgetFunctions* self)
{
    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_NULL,
        "NULL");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Kitty,
        "KITTY");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Textbox,
        "TEXTBOX");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Game,
        "GAME");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Clock,
        "CLOCK");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Button,
        "BUTTON");



    bbWidgetFunctions_add(self,
        WidgetCommand,
        Textbox_OnCommand,
        "TEXTBOX");

    return bbSuccess;
}



#endif //WIDGET_FUNCTIONS