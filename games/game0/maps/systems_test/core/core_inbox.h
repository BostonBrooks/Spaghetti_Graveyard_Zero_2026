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
    bbCoreInbox_numVTypes
} bbCoreInbox_vType;



bbFlag bbCoreInbox_TestMessage(bbCore* core);
bbFlag bbCoreInbox_SetGoalpoint(bbCore* core, bbHandle entity, bbMapCoords goalpoint);

bbFlag bbCoreInbox_Freeze(bbCore* core);

bbFlag bbCoreInbox_ReceiveMessage(bbCore* core,bbNetwork* Network, bbHandle threaded_pool_handle);

bbFlag bbCoreInbox_ClickMonster(bbCore* core, bbHandle entity_handle);
#endif //UNFREEZE_BUTTON_CODE_H