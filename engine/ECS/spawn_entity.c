#include "engine/logic/bbHandle.h"
#include "engine/ECS/spawn_entity.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/data/bbHome.h"

extern U32 collision;

bbFlag bbCoreInbox_TestClick2(bbCore* core, bbMapCoords MC)
{

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_testClick;
    message->act_time = home.core.core.actual_time;
    message->data.map_coords = MC;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}



bbFlag bbCoreInbox_testClick2_fn(bbCore* core, bbCoreInboxMessage* message)
{

    bbCoreInput_spawnEntityOut(core, message->data.map_coords, message->act_time, bbInstructionSource_input, no_handle);
    return bbSuccess;
}

bbFlag bbCoreInput_spawnEntityOut(bbCore* core,
                                  bbMapCoords MC,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action)
{

    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnEntityOut;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}




bbFlag bbInstruction_spawnEntityOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_spawnEntityOut(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbNetworkApp_spawnEntityOut(bbNetwork* Network, bbMapCoords MC, U64 time, U32 collision)
{
    bbNetwork* network = (bbNetwork*)Network;
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->act_tick = time;
    packet->collision = collision;
    packet->type = PACKETTYPE_TESTCLICK;
    packet->data.map_coords.i = MC.i;
    packet->data.map_coords.j = MC.j;
    packet->data.map_coords.k = MC.k;
    bbThreadedQueue_pushL(&network->outbox, (void*)packet);

    return bbSuccess;
}


bbFlag bbAction_spawnEntity(void* Core,
                            bbMapCoords map_coords,
                            bbHandle server_entity,
                            U32 collision,
                            U64 act_tick)
{
    bbCore* core = (bbCore*)Core;


    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_spawnEntity;
    action->header.collision = collision;
    action->header.act_tick = act_tick - 15;
    action->map_coords = map_coords;
    action->handle = server_entity;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}


