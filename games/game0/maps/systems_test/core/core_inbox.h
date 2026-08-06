#include "engine/core/bbCoreInbox.h"

typedef enum
{
    bbCoreInbox_testMessage = bbCoreInbox_numTypes,
    bbCoreInbox_netpauseButton
} bbCoreInbox_vType;



bbFlag bbCoreInbox_TestMessage(bbCore* core);