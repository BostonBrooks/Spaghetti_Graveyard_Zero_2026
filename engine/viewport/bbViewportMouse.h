#ifndef BB_VPMOUSE_H
#define BB_VPMOUSE_H

#include "bbUnits.h"
#include "bbViewport.h"
#include "engine/logic/bbTree.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/data/CSFML.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/userinterface/bbMouseFunctions.h"
#include "engine/userinterface/bbMouse.h"
#include "engine/viewport/bbViewportMouseFunctions.h"

//What do we do when the mouse isnt over the viewport?
//what do we do when the mouse is actually over the viewport?
//update the mouse even when the mouse hasnt moved, the unit under the mouse may have moved


typedef struct bbVPMouse
{
    bbViewportCoords position;
    bbViewportCoords previous_position;
    bbViewportCoords drag_origin;
    bool left_down;
    bool right_down;
    bool left_changed;
    bool right_changed;
    bool mouse_within_viewport;
    bbHandle was_over;
    bbHandle is_over;
    bbHandle selected;

    //selected unit is only used to decide what mouse pointer to use
    void* selected_unit;
    I32 default_mouse_icon;

    bbVPMouseFunctions functions;
    bbDrawables* units;
    void* viewportApp;
    bbMouse* mouse;
    bbGraphicsApp* graphics;


} bbVPMouse;

//TODO bbNotImplemented()
typedef struct
{
    bbVPMouse* mouse;
    bbHandle handle;
    void* unit;
} bbVPMouse_isOver_cl;

typedef enum{
    bbEvtMouseMoved,
    bbEvtMouseButtonPressed,
    bbEvtMouseButtonReleased,
    bbEvtMouseEnter,
    bbEvtMouseLeave,
    bbEvtDrag,
}  bbVPMouse_eventType;

typedef enum
{
    bbMouseLeft,
    bbMouseRight
} bbVPMouse_button;

typedef struct
{
    //TODO bbNotImplemented()
    bbViewportCoords position;
    bbVPMouse_eventType type;
    bbVPMouse_button button;
} bbVPMouse_event;

bbFlag bbVPMouse_Init(bbVPMouse* vpmouse, void* viewportApp, bbDrawables* units, bbMouse* mouse, bbGraphicsApp* graphics);

///Figure out what unit the mouse is over
bbFlag bbVPMouse_isOver(bbVPMouse* vpmouse, bbHandle* unit_handle);

///mouse responds to event from viewport
bbFlag bbVPMouse_Event(bbVPMouse* vpmouse, bbVPMouse_event* event);

///Interact with units, may have to look up sprite int in dictionary
bbFlag bbVPMouse_Update(bbVPMouse* mouse, bbGraphicsApp* graphics);

///VP mouse interacts with how the mouse pointer is drawn
bbFlag bbVPMouse_Draw(bbVPMouse* vpmouse, bbMouse* mouse);

///call the unit's enter_unit function
bbFlag bbVPMouse_EnterUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///call the unit's leave_unit function
bbFlag bbVPMouse_LeaveUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///call the unit's left_down function
bbFlag bbVPMouse_LeftDownUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///call the unit's left_up function
bbFlag bbVPMouse_LeftUpUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///what happens when you try to drag the unit?
bbFlag bbVPMouse_LeftDragUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///call the unit's right_down function
bbFlag bbVPMouse_RightDownUnit(bbVPMouse* vpmouse, struct bbUnit* unit);
///call the unit's right_up function
bbFlag bbVPMouse_RightUpUnit(bbVPMouse* vpmouse, struct bbUnit* unit);

///what happens when you try to drag the unit with the right button?
bbFlag bbVPMouse_RightDragUnit(bbVPMouse* mouse, struct bbUnit* unit);

///Function to be called iteratively until a unit is found under the mouse pointer
bbFlag bbVPMouse_isOverFunc(void* node, void* cl);




#endif//BB_VPMOUSE_H