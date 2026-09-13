
#ifndef NETWORK_MESSAGE
#define NETWORK_MESSAGE
#include "games/game0/maps/systems_test/network/network_textbox_system.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkPacket.h"

bbFlag bbNetworkApp_sendMessage(void* network, bbHandle message_handle, U64 time, U32 collision);
bbFlag bbNetworkApp_receiveMessage(void* network, void* packet);

bbFlag bbNetworkPacket_toMessageHandle (sfPacket* packet, bbHandle* message_handle, bbTextbox_message** message)
{
    bbHandle handle;
    bbTextbox_message* msg;

    bbVPool_alloc2(home.textbox_app.textbox_system.threaded_pool,(void**)&msg,&handle)

    U64 timestamp_upper = sfPacket_readUint32(packet);
    U64 timestamp_lower = sfPacket_readUint32(packet);

    msg->timestamp = timestamp_upper * 0x100000000 + timestamp_lower;
    msg->length = sfPacket_readInt32(packet);
    msg->type = sfPacket_readInt32(packet);
    sfPacket_readString(packet, msg->text);

    if (message_handle != NULL) *message_handle = handle;
    if (message != NULL) *message = msg;

    return bbSuccess;


}
bbFlag bbNetworkPacket_fromMessageHandle (sfPacket* packet, bbHandle message_handle)
{
    bbTextbox_message* msg;
    bbDebug("message handle:\n%llu\n", message_handle.u64);
    bbVPool_lookup(home.textbox_app.textbox_system.threaded_pool,(void**)&msg,message_handle);


    U64 timestamp_lower = msg->timestamp & 0xFFFFFFFF;
    U64 timestamp_upper = msg->timestamp / 0x100000000;

    sfPacket_writeUint32(packet,timestamp_lower);
    sfPacket_writeUint32(packet,timestamp_upper);
    sfPacket_writeInt32(packet,msg->length);
    sfPacket_writeInt32(packet,msg->type);
    sfPacket_writeString(packet,msg->text);

    return bbSuccess;

}
//bbFlag bbNetworkApp_sendMessage(struct bbNetwork* network, bbHandle message_handle, U64 time, U32 collision);
///take a message from home.textbox->system->pool, put it onto home.textbox->system->threaded_pool and send
bbFlag bbNetworkApp_sendMessage(void* network, bbHandle message_handle, U64 time, U32 collision) {

    bbNetwork* Network = network;

    bbTextbox_message* message_in;
    bbVPool_lookup(home.textbox_app.textbox_system.pool,(void**)&message_in,message_handle);

    bbTextbox_message* message_out;
    bbHandle message_out_handle;
    bbVPool_alloc2(home.textbox_app.textbox_system.threaded_pool,(void**)&message_out,&message_out_handle);

    message_out->timestamp = message_in->timestamp;
    message_out->length = message_in->length;
    message_out->type = message_in->type;
    bbStr_setStr(message_out->text,message_in->text, MESSAGE_LENGTH);

    bbNetworkPacket* packet;

    bbThreadedQueue_alloc(&Network->outbox,(void**)&packet);
    packet->type = PACKETTYPE_MESSAGE;
    packet->act_tick = time;
    packet->collision = collision;

    packet->data.message.message_handle = message_out_handle;
    bbThreadedQueue_pushL(&Network->outbox, (void*)packet);
    bbDebug("message handle:\n%llu\n", message_out_handle.u64);
    return bbSuccess;
}

//bbFlag bbNetworkApp_receiveMessage(struct bbNetwork* network, struct bbNetworkPacket* packet);
bbFlag bbNetworkApp_receiveMessage(void* network, void* packet) {

    bbNetwork* Network = network;
    bbNetworkPacket* Packet = packet;

    bbTextbox_message* message_in;
    bbVPool_lookup(home.textbox_app.textbox_system.threaded_pool,(void**)&message_in,Packet->data.message.message_handle);

    bbDebug("PACKETTYPE_MESSAGE:\n%s\n", message_in->text);
    bbVPool_free(home.textbox_app.textbox_system.threaded_pool,message_in);

    return bbSuccess;
}

#endif //NETWORK_MESSAGE
