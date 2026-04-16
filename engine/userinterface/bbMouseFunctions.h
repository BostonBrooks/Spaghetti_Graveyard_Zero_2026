//Much like bbWidgetFunctions
#ifndef BBMOOSEFUNCTIONS_H
#define BBMOOSEFUNCTIONS_H

#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbIntTypes.h"

typedef enum
{
    MouseIsOver,
    MouseEnter,
    MouseLeave,
    MouseLeftDown,
    MouseLeftUp,
    MouseLeftDrag,
    MouseRightDown,
    MouseRightUp,
    MouseRightDrag,
} MouseFunctionType;

typedef bbFlag bbMouse_onClick (void* widget);
typedef bbFlag bbMouse_IsOver (void* mouse, void* widgets, void* widget);
typedef bbFlag bbMouse_Enter (void* mouse, void* widgets, void* widget, void* graphics);
typedef bbFlag bbMouse_Leave (void* mouse, void* widgets, void* widget, void* graphics);
typedef bbFlag bbMouse_LeftDown (void* mouse, void* widgets, void* widget,
                                 void* graphics);
typedef bbFlag bbMouse_LeftUp (void* mouse, void* widgets, void* widget,
                               void* graphics);
typedef bbFlag bbMouse_LeftDrag (void* mouse, void* widgets, void* widget,
void* graphics);
typedef bbFlag bbMouse_RightDown (void* mouse, void* widgets, void* widget,
                                 void* graphics);
typedef bbFlag bbMouse_RightUp (void* mouse, void* widgets, void* widget,
                               void* graphics);
typedef bbFlag bbMouse_RightDrag (void* mouse, void* widgets, void* widget,
                               void* graphics);

typedef struct
{
    bbMouse_IsOver** IsOver;
    bbDictionary* IsOver_dict;
    I32 IsOver_available;

    bbMouse_Enter** Enter;
    bbDictionary* Enter_dict;
    I32 Enter_available;

    bbMouse_Leave** Leave;
    bbDictionary* Leave_dict;
    I32 Leave_available;

    bbMouse_LeftDown** LeftDown;
    bbDictionary* LeftDown_dict;
    I32 LeftDown_available;

    bbMouse_LeftUp** LeftUp;
    bbDictionary* LeftUp_dict;
    I32 LeftUp_available;


    bbMouse_LeftDrag** LeftDrag;
    bbDictionary* LeftDrag_dict;
    I32 LeftDrag_available;

    bbMouse_LeftDown** RightDown;
    bbDictionary* RightDown_dict;
    I32 RightDown_available;

    bbMouse_LeftUp** RightUp;
    bbDictionary* RightUp_dict;
    I32 RightUp_available;


    bbMouse_LeftDrag** RightDrag;
    bbDictionary* RightDrag_dict;
    I32 RightDrag_available;

}bbMouseFunctions;

bbFlag bbMouseFunctions_init(bbMouseFunctions* functions);
bbFlag bbMouseFunctions_populate(bbMouseFunctions* self);
bbFlag bbMouseFunctions_add(bbMouseFunctions* functions, MouseFunctionType fnType, void* fnPointer, char* key );
bbFlag bbMouseFunctions_getFunction(void** function, bbMouseFunctions* functions, MouseFunctionType fnType, char* key);
I32 bbMouseFunctions_getInt(bbMouseFunctions* functions, MouseFunctionType fnType, char* key);


#endif
