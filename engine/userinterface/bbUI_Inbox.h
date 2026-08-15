#ifndef BB_UI_INBOX_H
#define BB_UI_INBOX_H

#include "engine/core/bbCoreInbox.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/threadsafe/bbThreadedPool.h"

typedef enum
{
    bbUI_Inbox_unpressButton,
    bbUI_Inbox_unpressButton2,
    bbUI_Inbox_setWidgetPosition,
    bbUI_Inbox_setViewpoint,
    bbUI_Inbox_newSkelly,
    bbUI_Inbox_setUnitState,
    bbUI_Inbox_setEntityState,
    bbUI_Inbox_newBanana,
    bbUI_Inbox_deleteUnit,
    bbUI_Inbox_newUnit,
    bbUI_Inbox_setUnitHP,
    bbUI_Inbox_newTree,
    bbUI_Inbox_newDrawable
} bbUI_Inbox_type;

typedef struct
{
    bbHandle handle;
} bbUI_Inbox_handle;

typedef struct
{
    char string[KEY_LENGTH];
} bbUI_Inbox_string;



typedef struct
{
    bbUI_Inbox_handle handle;
    bbUI_Inbox_string string;
    bbHandle entity_handle;
    bbHandle moveable_handle;
    bbMapCoords coords;
    I32 integer;
    I32 integer2;
    I32 integer3;
    float floating;
} bbUI_Inbox_data;

typedef struct
{
    bbUI_Inbox_type type;
    bbUI_Inbox_data data;
    bbListElement_Handle list_element;

    //may or may not need to redo local messages
    bbHandle redo;
} bbUI_Inbox_message;

typedef struct
{
    bbVPool* local_message_pool;
    ///Receive messages from GUI, FIFO
    bbThreadedQueue local_message_queue;
} bbUI_Inbox;

bbFlag bbUI_Inbox_init(bbUI_Inbox* inbox);
bbFlag bbUI_Inbox_SetEntityState(bbUI_Inbox* inbox, bbHandle entity, I32 state);
bbFlag bbUI_Inbox_check(bbUI_Inbox* inbox);
bbFlag bbUI_Inbox_unpressButton_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_unpressButton2_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_newSkelly_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_setUnitState_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_newBanana_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_deleteUnit_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);

bbFlag bbUI_Inbox_setUnitHP_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);


bbFlag bbUI_Inbox_UnpressButton(bbUI_Inbox* inbox);
bbFlag bbUI_Inbox_UnpressButton2(bbUI_Inbox* inbox, char* key);
bbFlag bbUI_Inbox_SetWidgetPosition(bbUI_Inbox* inbox, char* key, bbHandle position);
bbFlag bbUI_Inbox_NewSkelly(bbUI_Inbox* inbox, bbMapCoords coords, I32 movable);
bbFlag bbUI_Inbox_SetUnitState(bbUI_Inbox* inbox, bbHandle unit, I32 state);


bbFlag bbUI_Inbox_NewUnit(bbUI_Inbox* inbox, I32 type_index, bbMapCoords MC, I32 entity_index, I32 movable_index);
bbFlag bbUI_Inbox_newUnit_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);

bbFlag bbUI_Inbox_NewBanana(bbUI_Inbox* inbox, bbMapCoords MC, bbHandle entity_handle, bbHandle moveable_handle);

bbFlag bbUI_Inbox_DeleteUnit(bbUI_Inbox* inbox, bbHandle entity_handle, bbHandle moveable_handle);

bbFlag bbUI_Inbox_SetUnitHP(bbUI_Inbox* inbox, bbHandle unit, float HP);

bbFlag bbUI_Inbox_NewTree(bbUI_Inbox* inbox, char* tree_type, bbMapCoords position);
bbFlag bbUI_Inbox_newTree_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);

bbFlag bbUI_Inbox_NewDrawable(bbUI_Inbox* inbox, I32 type_index, bbMapCoords MC, bbHandle entity_handle, bbHandle moveable_handle);
bbFlag bbUI_Inbox_newDrawable_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);



bbFlag bbUI_Inbox_SetViewpoint(bbUI_Inbox* inbox,  bbHandle entity_handle);

bbFlag bbUI_Inbox_setViewpoint_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
#endif // BB_UI_INBOX_H