#ifndef UNFREEZE_BUTTON_CODE_H
#define UNFREEZE_BUTTON_CODE_H
#include "engine/core/bbCoreInbox.h"
#include "engine/network/bbNetwork.h"

typedef enum
{
    bbCoreInbox_testMessage = bbCoreInbox_numTypes,
    bbCoreInbox_netpauseButton,
    bbCoreInbox_unfreezeButton,
    bbCoreInbox_testClick,
    bbCoreInbox_testClick2,
    bbCoreInbox_testClick3,
    bbCoreInbox_setGoalpoint,
    bbCoreInbox_freeze,
    bbCoreInbox_receiveMessage,
    bbCoreInbox_clickMonster,
    bbCoreInbox_clickPlayer,
    bbCoreInbox_clickUnit,
    bbCoreInbox_clickMap,
    bbCoreInbox_keyPress,
    bbCoreInbox_setPlayerEntity,
    bbCoreInbox_numVTypes
} bbCoreInbox_vType;



bbFlag bbCoreInbox_TestMessage(bbCore* core);
bbFlag bbCoreInbox_SetGoalpoint(bbCore* core, bbHandle entity, bbMapCoords goalpoint);

bbFlag bbCoreInbox_Freeze(bbCore* core);

bbFlag bbCoreInbox_ReceiveMessage(bbCore* core,bbNetwork* Network, bbHandle threaded_pool_handle);

bbFlag bbCoreInbox_ClickMonster(bbCore* core, bbHandle entity_handle);


bbFlag bbCoreInbox_ClickUnit(bbCore* core, bbHandle entity_handle, U64 control_keys);
bbFlag bbCoreInbox_ClicMap(bbCore* core, bbMapCoords, U64 control_keys);
bbFlag bbCoreInbox_KeyPress(bbCore* core, U64 key, U64 control_keys);

bbFlag bbCoreInbox_clickUnit_fn(bbCore* core, struct bbCoreInboxMessage* message);
bbFlag bbCoreInbox_clickMap_fn(bbCore* core, struct bbCoreInboxMessage* message);
bbFlag bbCoreInbox_keyPress_fn(bbCore* core, struct bbCoreInboxMessage* message);
#endif //UNFREEZE_BUTTON_CODE_H