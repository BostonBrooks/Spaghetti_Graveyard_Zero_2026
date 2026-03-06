#include "bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/logic/bbString.h"

bbFlag bbLocalMessage_SetString(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_setString;
    bbStr_setStr(message->data.string, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_UnfreezeButton(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_unfreezeButton;
    bbStr_setStr(message->data.string, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_ActionUnfreeze(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_actionUnfreeze;
    bbStr_setStr(message->data.string, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetsendButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netsendButton;

    bbStr_setStr(message->data.string, key, KEY_LENGTH);

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}