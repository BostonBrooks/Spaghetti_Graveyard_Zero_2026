#include "engine/logic/bbHandle.h"
#include "games/game0/maps/systems_test/core/player_goalpoint.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/data/bbHome.h"
#include "core/instructions.h"
#include "core/core_inbox.h"
#include "entity_spawner/live_spawn.h"

extern U32 collision;



bbFlag bbCoreInput_testClick4(bbCore* core,
                                  bbMapCoords MC,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action)
{

    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_testClick4;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}




bbFlag bbInstruction_testClick4_fn(bbCore* core, bbInstruction* instruction)
{

    bbNetworkApp_setClick4(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbNetworkApp_setClick4(bbNetwork* Network, bbMapCoords MC, U64 time, U32 collision)
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
