#ifndef BB_LOCAL_MESSAGE_H
#define BB_LOCAL_MESSAGE_H
#include "bbInstruction.h"

typedef struct bbCore bbCore;
typedef enum
{
    bbCoreInbox_setString,
    bbCoreInbox_unfreezeButton,
    bbCoreInbox_testClick,
    bbCoreInbox_numTypes
} bbCoreInbox_type;

typedef struct bbCoreInboxMessage
{
    U32 type;
    bbInstruction_data data;
    U64 act_time;
    bbListElement_Handle list_element;
} bbCoreInboxMessage;

bbFlag bbCore_initInboxMessages(bbCore* core);

bbFlag bbCore_checkInbox(bbCore* core);

bbFlag bbCoreInbox_setString_fn(bbCore* core, bbCoreInboxMessage* message);
bbFlag bbCoreInbox_testClick_fn(bbCore* core, bbCoreInboxMessage* message);

#endif // BB_LOCAL_MESSAGE_H