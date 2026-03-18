
#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"


bbFlag bbWidget_Constructor_Ball(bbWidget** self,
bbWidgets* widgets,
bbWidget* parent,
char* name,
bbScreenPoints screen_points,
bbGraphicsApp* graphics){

    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);


    widget->ftable.command = bbWidgetFunctions_getInt(widgets->functions,WidgetCommand ,"BALL");

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0;
    rect.height = 0;

    widget->rect = rect;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_SPRITE",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->sprites->dictionary,
                        "BALL", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, name, handle);

    if (self!=NULL) *self = widget;

    return bbSuccess;
}

bbFlag Ball_OnCommand(bbWidget* widget, bbWidgetCommandType type, bbHandle data){
    switch(type)
    {
         case bbWC_setPosition:
             {
                 widget->rect.left = data.i32x2.x;
                 widget->rect.top = data.i32x2.y;


                 break;
        }

    }
    return bbSuccess;
}
