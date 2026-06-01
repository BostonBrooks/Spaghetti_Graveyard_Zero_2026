///I'm starting things over, network connection must not block spawning thread
///bbTime will need immediate access to the network, not using bbThreadedQueue
#ifndef BB_NETWORK_H
#define BB_NETWORK_H

#include <stdatomic.h>
#include <SFML/Network.h>

#include "bbNetworkPacket.h"
#include "engine/threadsafe/bbThreadedQueue.h"


typedef bbFlag bbNetwork_ProcessPacket (void* network, void* args);
typedef bbFlag bbNetwork_onConnect (void* network);
typedef bbFlag bbNetwork_onDisconnect (void* network);


typedef bbFlag bbNetwork_PacketToStruct (sfPacket* packet, void* Struct);
typedef bbFlag bbNetwork_StructToPacket (sfPacket* packet, void* Struct);

///react immediately to incoming message
typedef bbFlag bbNetwork_filterInbox (void* network, void* Struct);
///react to outgoing message immediately before sending
typedef bbFlag bbNetwork_filterOutbox (void* network, void* Struct);

typedef struct bbNetwork
{
    sfIpAddress address;
    I32 port;
    sfTcpSocket* socket;
    I32 server_socket_number;

    //receive packets over network and add to inbox
    pthread_t receive_thread;
    bbThreadedQueue inbox;

    //take packets from outbox and send over network
    pthread_t send_thread;
    bbThreadedQueue outbox;

    // behaviors are defined outside this module:

    bbNetwork_PacketToStruct* packet_to_struct;
    bbNetwork_StructToPacket* struct_to_packet;
    //notify front end
    bbNetwork_onConnect* on_connect;
    //notify front end
    bbNetwork_onDisconnect* on_disconnect;


    bbNetwork_filterInbox* filter_inbox;
    bbNetwork_filterOutbox* filter_outbox;
    void* extra_data; //Used by bbTime
//if true,
    atomic_bool quit;
    atomic_bool send_ready;
    atomic_bool receive_ready;

} bbNetwork;


///set values in existing object
bbFlag bbNetwork_init(bbNetwork* network,
    bbNetwork_PacketToStruct* packet_to_struct,
    bbNetwork_StructToPacket* struct_to_packet,
    bbNetwork_onConnect* on_connect,
    bbNetwork_onDisconnect* on_disconnect,
    bbNetwork_filterInbox* filter_inbox,
    bbNetwork_filterOutbox* filter_outbox,
    void* extra_data);

///Asynchronously try to connect to a server, if successful, call bbNetwork_onConnect, otherwise call bbNetwork_onDisconnect
///Launch bbNetwork_receiveThread
///Then wait on outbox to send packets
bbFlag bbNetwork_connect(bbNetwork* network, sfIpAddress address, I32 port);

///called by bbNetwork_connect to asynchronously connect
void* bbNetwork_spawn(void* Network);
///signal threads to exit
bbFlag bbNetwork_disconnect(bbNetwork* network);

//one of the following is spawned as a new thread, and the other is called like a normal function
/// this thread waits on inbox and sends packets over network
void* bbNetwork_sendThread(void* args);

/// this thread waits for incoming packets then outputs to inbox
void* bbNetwork_receiveThread(void* args);

//Functions to send to outbox or receive to inbox are called by main

#endif //BB_NETWORK_H
