#ifndef BB_MOUSE_H
#define BB_MOUSE_H

#include "engine/logic/bbTree.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/data/CSFML.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/userinterface/bbMouseFunctions.h"

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

    bbMouse_onClick* OnClick;
    bbMouse_onClick* OnUnClick;

    //book keeping data
    unsigned int hover : 1;
    unsigned int selected : 1;
} bbMouseTable;


typedef struct bbMouse
{
    bbScreenPoints position;
    bbScreenPoints previous_position;
    bbScreenPoints drag_origin;
    unsigned int left_down : 1;
    unsigned int right_down : 1;
    unsigned int left_changed : 1;
    unsigned int right_changed : 1;
    //what if mouse button is down but nothing is selected?
    // PS we have bbVPool.null
    // and I32 bbVPool_handleIsEqual(bbVPool* pool, bbPool_Handle A, bbPool_Handle B);

    //wasOver == null indicates isover has not change
    bbHandle was_over;
    bbHandle is_over;
    bbHandle selected;
    I32 default_mouse_icon;

    bbMouseFunctions functions;
} bbMouse;

//Used by bbTree_ascendingMapVisible()
typedef struct
{
    void* widgets;
} bbMouse_isOver_cl;

//Needs to know the pool used for widgets...
bbFlag bbMouse_Init(bbMouse* mouse, void* widgets, bbGraphicsApp* graphics);

//Figure out what widget the mouse is over
bbFlag bbMouse_isOver(bbMouse* mouse, void* widgets);

bbFlag bbMouse_Event(bbMouse* mouse, sfEvent* event);
//Interact with widgets, may have to look up sprite int in dictionary
bbFlag bbMouse_Update(bbMouse* mouse, void* widgets, bbGraphicsApp* graphics);

//Draw mouse to screen, depending on what widget is selected and what is under the mouse
bbFlag bbMouse_Draw(bbMouse* mouse, void* widgets, bbGraphicsApp* graphics, sfRenderWindow* window);

bbFlag bbMouse_EnterWidget(void* mouse, void* widgets, void* widget, void* graphics);

bbFlag bbMouse_LeaveWidget(void* mouse, void* widgets, void* widget, void* graphics);

bbFlag bbMouse_LeftDownWidget(void* mouse, void* widgets, void* widget,
                                void* graphics);
bbFlag bbMouse_LeftUpWidget(void* mouse, void* widgets, void* widget,
                                  void* graphics);


bbFlag bbMouse_LeftDragWidget(void* mouse, void* widgets, void* widget,
void* graphics);

bbFlag bbMouse_RightDownWidget(void* mouse, void* widgets, void* widget,
                                void* graphics);
bbFlag bbMouse_RightUpWidget(void* mouse, void* widgets, void* widget,
                                  void* graphics);


bbFlag bbMouse_RightDragWidget(void* mouse, void* widgets, void* widget,
                            void* graphics);

//Function to be called recursively
bbFlag bbMouse_isOverFunc(bbTree* tree, void* node, void* cl);

#endif//BB_MOUSE_H