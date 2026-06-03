#include "engine/network/bbNetworkPacket.h"

#include "bbNetwork.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"


extern _Thread_local char* thread;

bbFlag bbNetworkPacket_toStruct (sfPacket* packet, void* Struct)
{
    bbNetworkPacket* struct1 = Struct;


    struct1->type = sfPacket_readInt32(packet);

    U64 sendTick_lower = sfPacket_readUint32(packet);
    U64 sendTick_upper = sfPacket_readUint32(packet);

    struct1->send_tick = sendTick_upper * 0x100000000 + sendTick_lower;

    U64 actTick_lower = sfPacket_readUint32(packet);
    U64 actTick_upper = sfPacket_readUint32(packet);

    struct1->act_tick = actTick_upper * 0x100000000 + actTick_lower;

    struct1->player = sfPacket_readUint32(packet);
    struct1->collision = sfPacket_readUint32(packet);

    switch (struct1->type)
    {
        case PACKETTYPE_STRING:
            sfPacket_readString(packet, struct1->data.str);
        break;
        case PACKETTYPE_TIMESTAMP:
        case PACKETTYPE_REQUESTTIMESTAMP:

            U64 packetN_lower = sfPacket_readUint32(packet);
            U64 packetN_upper = sfPacket_readUint32(packet);
            U64 receive_time_lower = sfPacket_readUint32(packet);
            U64 receive_time_upper = sfPacket_readUint32(packet);
            U64 send_time_lower = sfPacket_readUint32(packet);
            U64 send_time_upper = sfPacket_readUint32(packet);



            struct1->data.timestamp.packetN = packetN_upper * 0x100000000 + packetN_lower;
            struct1->data.timestamp.receive_time = receive_time_upper * 0x100000000  + receive_time_lower;
            struct1->data.timestamp.send_time = send_time_upper * 0x100000000 + send_time_lower;

        break;
    case PACKETTYPE_SETGOALPOINT:
        struct1->data.map_coords.i = sfPacket_readInt32(packet);
        struct1->data.map_coords.j = sfPacket_readInt32(packet);
        struct1->data.map_coords.k = sfPacket_readInt32(packet);
        break;
    case PACKETTYPE_UNFREEZEBUTTON:
    case PACKETTYPE_NETCODEBUTTON:
            sfPacket_readString(packet, struct1->data.str);
        break;
    case PACKETTYPE_PAUSE:

        U64 server_tick_lower = sfPacket_readUint32(packet);
        U64 server_tick_upper = sfPacket_readUint32(packet);
        U64 map_tick_lower = sfPacket_readUint32(packet);
        U64 map_tick_upper = sfPacket_readUint32(packet);


        struct1->data.pause.reference_server_tick = server_tick_upper * 0x100000000 + server_tick_lower;
        struct1->data.pause.reference_map_tick = map_tick_upper * 0x100000000 + map_tick_lower;
       struct1->data.pause.is_paused = sfPacket_readBool(packet);

        break;

    case PACKETTYPE_KEYUP:
    case PACKETTYPE_KEYDOWN:
        struct1->data.integer = sfPacket_readInt32(packet);
        break;
    case PACKETTYPE_PADDLEVELOCITY:
        struct1->data.paddle_and_velocity.x = sfPacket_readInt32(packet);
        struct1->data.paddle_and_velocity.y = sfPacket_readInt32(packet);

        break;
    case PACKETTYPE_VIEWPOINT:
    case PACKETTYPE_GOALPOINT:

        struct1->data.map_coords.i = sfPacket_readInt32(packet);
        struct1->data.map_coords.j = sfPacket_readInt32(packet);
        struct1->data.map_coords.k = sfPacket_readInt32(packet);

        break;
    case PACKETTYPE_SPAWNBANANA:
    case PACKETTYPE_SPAWNUNIT:

        struct1->data.unit.position.i = sfPacket_readInt32(packet);
        struct1->data.unit.position.j = sfPacket_readInt32(packet);
        struct1->data.unit.position.k = sfPacket_readInt32(packet);

        struct1->data.unit.goalpoint.i = sfPacket_readInt32(packet);
        struct1->data.unit.goalpoint.j = sfPacket_readInt32(packet);
        struct1->data.unit.goalpoint.k = sfPacket_readInt32(packet);


       // bbDebug("goal.i = %d, goal.j = %d\n", struct1->data.unit.goalpoint.i, struct1->data.unit.goalpoint.j);

        struct1->data.unit.entity_index = sfPacket_readInt32(packet);
        struct1->data.unit.movable_index = sfPacket_readInt32(packet);
        struct1->data.unit.type_index = sfPacket_readInt32(packet);
        break;
    case PACKETTYPE_SETSOCKETNUMBER:
        struct1->data.integer = sfPacket_readInt32(packet);
        bbHere()
        break;
    }
    return bbSuccess;
}
bbFlag bbNetworkPacket_fromStruct (sfPacket* packet, void* Struct)
{
    bbNetworkPacket* struct1 = Struct;

    sfPacket_writeInt32(packet, struct1->type);

    U64 sendTick_lower = struct1->send_tick & 0xFFFFFFFF;
    U64 sendTick_upper = struct1->send_tick / 0x100000000;

    sfPacket_writeUint32(packet, (U32)sendTick_lower);
    sfPacket_writeUint32(packet, (U32)sendTick_upper);

    U64 actTick_lower = struct1->act_tick & 0xFFFFFFFF;
    U64 actTick_upper = struct1->act_tick / 0x100000000;

    sfPacket_writeUint32(packet, (U32)actTick_lower);
    sfPacket_writeUint32(packet, (U32)actTick_upper);

    sfPacket_writeUint32(packet, struct1->player);

    sfPacket_writeUint32(packet, struct1->collision);

    switch (struct1->type)
    {
    case PACKETTYPE_STRING:
        sfPacket_writeString(packet, struct1->data.str);
        break;
    case PACKETTYPE_TIMESTAMP:
    case PACKETTYPE_REQUESTTIMESTAMP:

        U64 packetN_lower = struct1->data.timestamp.packetN & 0xFFFFFFFF;
        U64 packetN_upper = struct1->data.timestamp.packetN / 0x100000000;
        U64 receive_time_lower = struct1->data.timestamp.receive_time & 0xFFFFFFFF;
        U64 receive_time_upper = struct1->data.timestamp.receive_time / 0x100000000;
        U64 send_time_lower = struct1->data.timestamp.send_time & 0xFFFFFFFF;
        U64 send_time_upper = struct1->data.timestamp.send_time / 0x100000000;



        sfPacket_writeUint32(packet, (U32)packetN_lower);
        sfPacket_writeUint32(packet, (U32)packetN_upper);
        sfPacket_writeUint32(packet, (U32)receive_time_lower);
        sfPacket_writeUint32(packet, (U32)receive_time_upper);
        sfPacket_writeUint32(packet, (U32)send_time_lower);
        sfPacket_writeUint32(packet, (U32)send_time_upper);
        break;
    case PACKETTYPE_SETGOALPOINT:

        sfPacket_writeInt32(packet, struct1->data.map_coords.i);
        sfPacket_writeInt32(packet, struct1->data.map_coords.j);
        sfPacket_writeInt32(packet, struct1->data.map_coords.k);
        break;
    case PACKETTYPE_UNFREEZEBUTTON:
    case PACKETTYPE_NETCODEBUTTON:

        sfPacket_writeString(packet, struct1->data.str);

        break;
    case PACKETTYPE_PAUSE:

        U64 server_tick_lower = struct1->data.pause.reference_server_tick & 0xFFFFFFFF;
        U64 server_tick_upper = struct1->data.pause.reference_server_tick / 0x100000000;
        U64 map_tick_lower = struct1->data.pause.reference_map_tick & 0xFFFFFFFF;
        U64 map_tick_upper = struct1->data.pause.reference_map_tick / 0x100000000;

        sfPacket_writeUint32(packet, (U32)server_tick_lower);
        sfPacket_writeUint32(packet, (U32)server_tick_upper);
        sfPacket_writeUint32(packet, (U32)map_tick_lower);
        sfPacket_writeUint32(packet, (U32)map_tick_upper);
        sfPacket_writeBool(packet, struct1->data.pause.is_paused);

        break;
    case PACKETTYPE_KEYUP:
    case PACKETTYPE_KEYDOWN:
        sfPacket_writeInt32(packet, struct1->data.integer);

        break;

    case PACKETTYPE_PADDLEVELOCITY:
        sfPacket_writeInt32(packet, struct1->data.paddle_and_velocity.x);
        sfPacket_writeInt32(packet, struct1->data.paddle_and_velocity.y);

        break;

    case PACKETTYPE_VIEWPOINT:
    case PACKETTYPE_GOALPOINT:
        sfPacket_writeInt32(packet, struct1->data.map_coords.i);
        sfPacket_writeInt32(packet, struct1->data.map_coords.j);
        sfPacket_writeInt32(packet, struct1->data.map_coords.k);
        break;
    case PACKETTYPE_SPAWNBANANA:
    case PACKETTYPE_SPAWNUNIT:

        sfPacket_writeInt32(packet,struct1->data.unit.position.i);
        sfPacket_writeInt32(packet,struct1->data.unit.position.j);
        sfPacket_writeInt32(packet,struct1->data.unit.position.k);

        sfPacket_writeInt32(packet,struct1->data.unit.goalpoint.i);
        sfPacket_writeInt32(packet,struct1->data.unit.goalpoint.j);
        sfPacket_writeInt32(packet,struct1->data.unit.goalpoint.k);

        //bbDebug("goal.i = %d, goal.j = %d\n", struct1->data.unit.goalpoint.i, struct1->data.unit.goalpoint.j);

        sfPacket_writeInt32(packet,struct1->data.unit.entity_index);
        sfPacket_writeInt32(packet,struct1->data.unit.movable_index);
        sfPacket_writeInt32(packet,struct1->data.unit.type_index);
        break;
    case PACKETTYPE_SETSOCKETNUMBER:
        sfPacket_writeInt32(packet, struct1->data.integer);
        bbHere()
        break;
    }

    return bbSuccess;
}


bbFlag bbNetwork_sendStr(void* Network, char* str)
{
    bbNetwork* network = (bbNetwork*)Network;
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_STRING;
    bbStr_setStr(packet->data.str, str, 64);
    bbThreadedQueue_pushL(&network->outbox, (void*)packet);

    return bbSuccess;
}



