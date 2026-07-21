#include "bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"

bbFlag bbCoreInbox_SetString(bbCore* core, char* string)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_setString;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}


bbFlag bbCoreInbox_UnfreezeButton(bbCore* core, char* string)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_unfreezeButton;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}