#include "engine/network/bbNetwork.h"

#include "engine/logic/bbTerminal.h"
#include "engine/network/bbNetworkPacket.h"

extern _Thread_local char* thread;

bbFlag bbNetwork_init(bbNetwork* network,
    bbNetwork_PacketToStruct* packet_to_struct,
    bbNetwork_StructToPacket* struct_to_packet,
    bbNetwork_onConnect* on_connect,
    bbNetwork_onDisconnect* on_disconnect,
    bbNetwork_filterInbox* filter_inbox,
    bbNetwork_filterOutbox* filter_outbox,
    void* extra_data)
{
    //not a thread
    //thread = "init";
    const I32 queue_length = 500;

    network->packet_to_struct = packet_to_struct;
    network->struct_to_packet = struct_to_packet;
    network->on_connect = on_connect;
    network->on_disconnect = on_disconnect;
    network->filter_inbox = filter_inbox;
    network->filter_outbox = filter_outbox;
    network->extra_data = extra_data;

    bbThreadedQueue_init(&network->inbox,NULL,sizeof(bbNetworkPacket),queue_length,offsetof(bbNetworkPacket, list_element));
    bbThreadedQueue_init(&network->outbox,NULL,sizeof(bbNetworkPacket),queue_length,offsetof(bbNetworkPacket, list_element));

    network->quit = false;
    return bbSuccess;
}

bbFlag bbNetwork_connect(bbNetwork* network, sfIpAddress address, I32 port)
{
    network->send_ready = false;
    network->receive_ready = false;
    //Not a thread
    //thread = "connect";
    printf("Hello Connect\n");
    network->address = address;
    network->port = port;

    pthread_create(&network->receive_thread,NULL, bbNetwork_spawn, network);

    return bbSuccess;
}

void* bbNetwork_spawn(void* Network)
{
    thread = "spawn";
    printf("Hello Spawn\n");

    const I32 connect_timeout = 20;
    bbNetwork* network = (bbNetwork*)Network;

    sfSocketStatus status;
    sfTcpSocket* socket = sfTcpSocket_create();

    sfTcpSocket_setBlocking(socket, sfTrue);
    bbAssert(socket!=NULL, "bad socket constructor\n");

    bbHere()

    status = sfTcpSocket_connect(socket, network->address, network->port, sfSeconds(connect_timeout));
    if (status == sfSocketNotReady)
    {
        network->on_disconnect(NULL);
        return NULL;
    }
    //workaround because status is always sfSocketDone
    sfIpAddress address = sfTcpSocket_getRemoteAddress(socket);
    U32 addressInt = sfIpAddress_toInteger(address);
    U32 noneInt = sfIpAddress_toInteger(sfIpAddress_None);

    if (addressInt == noneInt)
    {
        sfTcpSocket_destroy(socket);
        network->on_disconnect(NULL);
        return NULL;
    }

    network->socket = socket;

    if (network->on_connect != NULL) network->on_connect(NULL);
    pthread_create(&network->send_thread,NULL, bbNetwork_sendThread, network);
    bbNetwork_receiveThread(network);

    //handle cleanup?
    return NULL;
}


void* bbNetwork_receiveThread(void* args)
{

    thread = "receive";
    printf("Hello Receive\n");
    bbNetwork* network = args;
    bbThreadedQueue* queue = &network->inbox;
    sfTcpSocket* socket = network->socket;
    sfPacket* packet = sfPacket_create();
    sfSocketStatus status;


    network->receive_ready = true;
    I32 i = 0;
    while (1)
    {
        if (network->quit)
        {
            network->receive_ready = false;
            if (network->send_ready == false)
            {
                network->on_disconnect(network);
            }
            return NULL;
        }
        status = sfTcpSocket_receivePacket(socket, packet);

        if (status != sfSocketDone) continue;

        char message[512];
        //TODO Convert network packet to struct
        bbNetworkPacket* test;
        bbThreadedQueue_alloc(queue, (void**)&test);
        bbNetworkPacket_toStruct(packet, test);

        if (network->filter_inbox != NULL)
        {
            bbFlag flag = network->filter_inbox(network,test);
            if (flag != bbSuccess)
            {
                bbThreadedQueue_free(queue, (void**)&test);
                continue;
            }
        }

        if (test->type == PACKETTYPE_SETGOALPOINT)
        {
          //  bbDebug("in receive thread: i = %d, j = %d, k = %d\n",
          //      test->data.map_coords.i,
           //     test->data.map_coords.j,
           //     test->data.map_coords.k);
        }
        bbThreadedQueue_pushL(queue, test);

        sfPacket_clear(packet);
        i++;
    }

    sfSleep(sfSeconds(1));
    return NULL;
}

/// Take struct from queue, Convert struct to packet, send
void* bbNetwork_sendThread(void* args)
{
    thread = "send";

    printf("Hello Send\n");
    bbNetwork* network = args;
    sfTcpSocket* socket = network->socket;
    sfPacket* packet = sfPacket_create();
    sfSocketStatus status;
    network->send_ready = true;

    char message[512];
    while (1)
    {
        if (network->quit)
        {
            network->send_ready = false;
            if (network->receive_ready == false)
            {
                network->on_disconnect(network);
            }
            return NULL;
        }

        bbNetworkPacket* test;

        bbFlag flag = bbThreadedQueue_popR_block(&network->outbox, (void**)&test);

        if (flag == bbNone) continue;

        bbFlag flag2 = network->filter_outbox(network,test);

        if (flag2 != bbSuccess)
        {
            bbThreadedQueue_free(&network->outbox, (void**)&test);

            continue;
        }

        bbNetworkPacket_fromStruct(packet, test);
        status = sfTcpSocket_sendPacket(socket, packet);


        sfPacket_clear(packet);

        bbThreadedQueue_free(&network->outbox, (void**)&test);

        //sfSocketStatus_print(status);


    }
    return NULL;

}
