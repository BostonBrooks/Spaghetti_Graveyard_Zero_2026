#ifndef UNFREEZE_BUTTON_CODE_H
#define UNFREEZE_BUTTON_CODE_H
#include "engine/core/bbCoreInbox.h"

typedef enum
{
    bbCoreInbox_testMessage = bbCoreInbox_numTypes,
    bbCoreInbox_netpauseButton,
    bbCoreInbox_unfreezeButton,
    bbCoreInbox_testClick,
    bbCoreInbox_testClick2,
    bbCoreInbox_setGoalpoint
} bbCoreInbox_vType;



bbFlag bbCoreInbox_TestMessage(bbCore* core);
bbFlag bbCoreInbox_SetGoalpoint(bbCore* core, bbHandle entity, bbMapCoords goalpoint);

#endif //UNFREEZE_BUTTON_CODE_H