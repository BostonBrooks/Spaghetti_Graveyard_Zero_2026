#include "bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"

bbFlag bbLocalMessage_SetString(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_setString;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_UnfreezeButton(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_unfreezeButton;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_ActionUnfreeze(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_actionUnfreeze;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_RetroactionUnfreeze(bbCore* core, char* string)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_retroactionUnfreeze;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetsendButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netsendButton;

    bbStr_setStr(message->data.key, key, KEY_LENGTH);

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetcodeButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netcodeButton;
    bbStr_setStr(message->data.key, key, KEY_LENGTH);
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
    bbStr_setStr(message->data.key, key, KEY_LENGTH);
    message->act_time = home.core.core.actual_time;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_NetpauseButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_netpauseButton;

    bbStr_setStr(message->data.key, key, KEY_LENGTH);

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

bbFlag bbLocalMessage_SpawnBanana(bbCore* core, bbMapCoords mapCoords)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_spawnBanana;

    message->act_time = home.core.core.actual_time;
    message->data.map_coords = mapCoords;

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_SpawnUnit(bbCore* core, bbMapCoords mapCoords, bbMapCoords mapCoords2, char* key)
{

    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_spawnUnit;

    message->act_time = home.core.core.actual_time;
    message->data.spawn_unit_out.position = mapCoords;
    message->data.spawn_unit_out.goal_point = mapCoords2;


    bbStr_putStr(message->data.spawn_unit_out.key, key, KEY_LENGTH);


    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}

bbFlag bbLocalMessage_MapClick(bbCore* core, bbMapCoords mapCoords, I32 mouse_button)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_mapClick;

    message->act_time = home.core.core.actual_time;
    message->data.map_click.coords = mapCoords;
    message->data.map_click.button = mouse_button;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}

bbFlag bbLocalMessage_SetGoalpoint(bbCore* core, bbMapCoords mapCoords)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_setGoalpoint;

    message->act_time = home.core.core.actual_time;
    message->data.map_coords = mapCoords;

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}


bbFlag bbLocalMessage_SwitchCharacterButton(bbCore* core, char* key)
{
    bbLocalMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbLocalMessage_switchCharacterButton;

    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}