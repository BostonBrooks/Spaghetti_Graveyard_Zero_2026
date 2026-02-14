#include "engine/core/bbLocalMessageInputs.h"

#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbLocalMessage_PrintString(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_printString;
    bbStr_setStr(message->data.string.string, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}


bbFlag bbLocalMessage_UnfreezeButton(bbCore* core)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_unfreezeButton;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetsendButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netsendButton;

    bbStr_setStr(message->data.string.string, key, KEY_LENGTH);

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_UnfreezeButton2(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_unfreezeButton2;
    bbStr_setStr(message->data.string.string, key, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}