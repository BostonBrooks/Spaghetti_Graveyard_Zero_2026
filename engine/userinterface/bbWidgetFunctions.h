
#ifndef BB_WIDGET_FUNCTION_ENUMS_H
#define BB_WIDGET_FUNCTION_ENUMS_H


typedef enum
{
    WidgetConstructor,
    WidgetUpdate,
    WidgetDestructor,
    WidgetOnCommand,
    WidgetOnTimer,
    WidgetHide,
    WidgetUnhide
} bbWidgetFunctionType;

typedef enum bbWidgetCommandType
{
    bbWC_putChar,
    bbWC_putStr,
    bbWC_setBounds,
    bbWC_setDimensions,
    bbWC_setStr,
    bbWC_clrStr
} bbWidgetCommandType;

#endif //BB_WIDGET_FUNCTION_ENUMS_H

#ifndef BB_WIDGET_FUNCTIONS_H
#define BB_WIDGET_FUNCTIONS_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbFlag.h"

typedef struct bbWidget bbWidget;
typedef struct bbWidgets bbWidgets;

typedef bbFlag bbWidget_Constructor (bbWidget** self,
                                      bbWidgets* widgets,
                                      bbWidget* parent,
                                      char* name,
                                      bbScreenPoints screen_points,
                                      bbGraphicsApp* graphics
                                      );

typedef struct bbWidgetFunctions {
    bbWidget_Constructor** constructors;
    bbDictionary* constructor_dict;
    I32 constructor_available;
} bbWidgetFunctions;


bbFlag bbWidgetFunctions_new(bbWidgetFunctions** self);
bbFlag bbWidgetFunctions_populate(bbWidgetFunctions* self);

bbFlag bbWidgetFunctions_add(bbWidgetFunctions* functions,
                              bbWidgetFunctionType function_type,
                              void* function_pointer,
                              char* key );

bbFlag bbWidgetFunctions_getFunction(void** function,
                                    bbWidgetFunctions* functions,
                                    bbWidgetFunctionType function_type,
                                    char* key);

I32 bbWidgetFunctions_getInt(bbWidgetFunctions* functions,
                             bbWidgetFunctionType function_type,
                             char* key);




#endif //BB_WIDGET_FUNCTIONS_H