#ifndef WIDGET_FUNCTIONS
#define WIDGET_FUNCTIONS
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/map0/widgets/textbox.h"
#include "games/game0/maps/map0/widgets/gamewidget.h"
#include "games/game0/maps/map0/widgets/react_button.h"
#include "games/game0/maps/map0/widgets/netsend_button.h"
#include "games/game0/maps/map0/widgets/action_button.h"
#include "games/game0/maps/map0/widgets/retroaction_button.h"
#include "games/game0/maps/map0/widgets/netcode_button.h"
#include "games/game0/maps/map0/widgets/netpause_button.h"
#include "games/game0/maps/map0/widgets/loop_button.h"
#include "games/game0/maps/map0/widgets/pause_button.h"
#include "games/game0/maps/map0/widgets/composition.h"


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

bbFlag bbWidget_Constructor_Butterfly(bbWidget** self,
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
    rect.width = 80*SCREEN_PPP;
    rect.height = 80*SCREEN_PPP;

    widget->rect = rect;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_ANGLE",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->animations->dictionary,
                        "BUTTERFLIES", &widget->frames[0].handle);

    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    widget->angle = rand()%8;

    int funcInt;
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseIsOver,
                                  "BUTTERFLY");
    widget->mtable.is_over = funcInt;


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
                     "WIDGET_GRAPHICSTIME_ANIMATION",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->animations->dictionary,
                        "REDCLOCK", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                 "WIDGET_CORETIME_ANIMATION",
                 &drawfunctionHandle);

    widget->frames[1].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->animations->dictionary,
                        "CLOCK", &widget->frames[1].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[1].offset.x = 0;
    widget->frames[1].offset.y = 0;

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
        bbWidget_Constructor_ReactButton,
        "REACT");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_NetsendButton,
        "NETSEND");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_ActionButton,
        "ACTION");



    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_RetroactionButton,
        "RETROACTION");


    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Composition,
        "COMPOSITION");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_Butterfly,
        "BUTTERFLY");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_NetcodeButton,
        "NETCODE");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_LoopButton,
        "LOOP_BUTTON");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_PauseButton,
        "PAUSE_BUTTON");

    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_NetpauseButton,
        "NETPAUSE_BUTTON");



    bbWidgetFunctions_add(self,
        WidgetCommand,
        Textbox_OnCommand,
        "TEXTBOX");

    return bbSuccess;
}



#endif //WIDGET_FUNCTIONS