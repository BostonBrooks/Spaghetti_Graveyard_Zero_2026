#ifndef BB_VP_MOUSE_H
#define BB_VP_MOUSE_H
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbVPool.h"

///Every unit has one of these
typedef struct
{
    ///How does the unit treat the mouse
    bbHandle mouse_table;
    ///what is the clickbox of the unit
    bbGameWorldRect rect;

    ///book keeping data
    bool hover;
    bool selected;
} bbUnit_mouse;

typedef enum
{
    VPMouseIsOver,
    VPMouseEnter,
    VPMouseLeave,
    VPMouseLeftDown,
    VPMouseLeftUp,
    VPMouseLeftDrag,
    VPMouseRightDown,
    VPMouseRightUp,
    VPMouseRightDrag,
} bbVPMouseFunctionType;

typedef struct
{
    //virtual functions
    I32 is_over;
    I32 enter;
    I32 leave;
    I32 left_down;
    I32 right_down;
    I32 left_up;
    I32 right_up;
    I32 left_drag;
    I32 right_drag;
    I32 drop;
    I32 mouse_icon;
    I32 drag_icon;
    I32 on_click;
    I32 on_unclick;

    char key[KEY_LENGTH];
} bbVPMouseTable;

//TODO what are the arguments for these function types?

typedef bbFlag bbVPMouse_IsOver (void* mouse, void* units, void* unit);
typedef bbFlag bbVPMouse_Enter (void* mouse, void* widgets, void* widget, void* graphics);
typedef bbFlag bbVPMouse_Leave (void* mouse, void* widgets, void* widget, void* graphics);
typedef bbFlag bbVPMouse_LeftDown (void* mouse, void* widgets, void* widget,
                                 void* graphics);
typedef bbFlag bbVPMouse_LeftUp (void* mouse, void* widgets, void* widget,
                               void* graphics);
typedef bbFlag bbVPMouse_LeftDrag (void* mouse, void* widgets, void* widget,
void* graphics);
typedef bbFlag bbVPMouse_RightDown (void* mouse, void* widgets, void* widget,
                                 void* graphics);
typedef bbFlag bbVPMouse_RightUp (void* mouse, void* widgets, void* widget,
                               void* graphics);
typedef bbFlag bbVPMouse_RightDrag (void* mouse, void* widgets, void* widget,
                               void* graphics);

typedef struct
{
    bbVPMouse_IsOver** IsOver;
    bbDictionary* IsOver_dict;
    I32 IsOver_available;

    bbVPMouse_Enter** Enter;
    bbDictionary* Enter_dict;
    I32 Enter_available;

    bbVPMouse_Leave** Leave;
    bbDictionary* Leave_dict;
    I32 Leave_available;

    bbVPMouse_LeftDown** LeftDown;
    bbDictionary* LeftDown_dict;
    I32 LeftDown_available;

    bbVPMouse_LeftUp** LeftUp;
    bbDictionary* LeftUp_dict;
    I32 LeftUp_available;

    bbVPMouse_LeftDrag** LeftDrag;
    bbDictionary* LeftDrag_dict;
    I32 LeftDrag_available;

    bbVPMouse_LeftDown** RightDown;
    bbDictionary* RightDown_dict;
    I32 RightDown_available;

    bbVPMouse_LeftUp** RightUp;
    bbDictionary* RightUp_dict;
    I32 RightUp_available;

    bbVPMouse_LeftDrag** RightDrag;
    bbDictionary* RightDrag_dict;
    I32 RightDrag_available;

    bbVPool* mouse_tables;
    bbDictionary* mouse_table_dict;
}bbVPMouseFunctions;

bbFlag bbVPMouseFunctions_init(bbVPMouseFunctions* functions);
bbFlag bbVPMouseFunctions_populate(bbVPMouseFunctions* self);
bbFlag bbVPMouseFunctions_add(bbVPMouseFunctions* functions, bbVPMouseFunctionType fnType, void* fnPointer, char* key );

///pass table by reference, table is copied into pool, new handle returned by reference
bbFlag bbVPMouseFunctions_addTable(bbVPMouseFunctions* functions, bbVPMouseTable* table, bbHandle* table_handle, char* key);
///return a pointer to the function table
bbFlag bbVPMouseFunctions_getTable(bbVPMouseFunctions* functions, bbVPMouseTable** table, bbHandle table_handle);
bbFlag bbVPMouseFunctions_getTableHandle(bbVPMouseFunctions* functions, bbHandle* table_handle, char* key);
bbFlag bbVPMouseFunctions_getFunction(bbVPMouseFunctions* functions, void** function, bbVPMouseFunctionType fnType, I32 index);
I32 bbVPMouseFunctions_getInt(bbVPMouseFunctions* functions, bbVPMouseFunctionType fnType, char* key);


bbFlag bbVPMouse_IsOver_NULL (void* mouse, void* units, void* unit);

#endif //BB_VP_MOUSE_H