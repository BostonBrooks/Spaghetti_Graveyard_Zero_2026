#ifndef WIDGET_FUNCTIONS
#define WIDGET_FUNCTIONS
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/userinterface/bbWidgets.h"

bbFlag bbWidget_Constructor_NULL (bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics
)
{
    bbHere();
}



bbFlag bbWidgetFunctions_populate(bbWidgetFunctions* self)
{
    bbWidgetFunctions_add(self,
        WidgetConstructor,
        bbWidget_Constructor_NULL,
        "NULL");

    return bbSuccess;
}

#endif //WIDGET_FUNCTIONS