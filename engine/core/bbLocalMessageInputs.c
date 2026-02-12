#include "engine/core/bbLocalMessageInputs.h"

#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbString.h"


bbFlag bbLocalMessage_PrintString(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_printString;
    bbStr_setStr(message->data.string.string, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}
