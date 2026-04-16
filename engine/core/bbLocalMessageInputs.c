#include "bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"

#include "engine/data/bbHome.h"
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

bbFlag bbLocalMessage_RetroactionUnfreeze(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_retroactionUnfreeze;
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

bbFlag bbLocalMessage_NetcodeButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netcodeButton;
    bbStr_setStr(message->data.string, key, KEY_LENGTH);
    message->act_time = home.core.core.actual_time + 60;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;

}

bbFlag bbLocalMessage_KeyDown(bbCore* core, I32 key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_keyDown;
    message->data.three_handles.handle1.u64 = key;
    message->act_time = home.core.core.actual_time;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;

}

bbFlag bbLocalMessage_KeyUp(bbCore* core, I32 key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_keyUp;
    message->data.three_handles.handle1.u64 = key;
    message->act_time = home.core.core.actual_time;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;

}

bbFlag bbLocalMessage_ActionLoop(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_actionLoop;
    bbStr_setStr(message->data.string, key, KEY_LENGTH);
    message->act_time = home.core.core.actual_time;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetpauseButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netpauseButton;

    bbStr_setStr(message->data.string, key, KEY_LENGTH);

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}


bbFlag bbLocalMessage_SetViewpoint(bbCore* core, bbMapCoords mapCoords)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_setViewpoint;

    message->act_time = home.core.core.actual_time;
    message->data.map_coords = mapCoords;

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}