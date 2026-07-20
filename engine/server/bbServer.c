

#include <stdio.h>

#include "engine/data/CSFML.h"

#include "engine/logic/bbTerminal.h"
#include "engine/network/bbNetworkPacket.h"

thread_local char* thread;
U64 test_time = 0;

int main(void){

    //Use the tick values at the time the server was paused or unpaused as a reference
    U64 reference_server_tick = 0;
    U64 reference_map_tick = 0;
    bool is_paused = true;


    printf("Hello, server!\n");

    sfIpAddress localAddress;
    localAddress = sfIpAddress_getLocalAddress();
    char localAddressString[64];
    sfIpAddress_toString(localAddress, localAddressString);

    sfIpAddress publicAddress;
    publicAddress = sfIpAddress_getPublicAddress(sfSeconds(120));
    char publicAddressString[64];
    sfIpAddress_toString(publicAddress, publicAddressString);

    printf("Loopback address = 127.0.0.1\nLocal address = %s\nPublic address = %s\n",
        localAddressString, publicAddressString);


    sfClock* clock = sfClock_create();
    I32 port = bbGetInt("Input server port: ", 1701);
    bbClearLine(1);
    printf("The port is %d\n", port);

    sfTcpListener* listener = sfTcpListener_create();
    sfSocketStatus status = sfTcpListener_listen(listener,port,sfIpAddress_Any);
    //sfSocketStatus_print(status);
    if (status != sfSocketDone) {
        printf("Could not connect to the given port!");
        exit(EXIT_FAILURE);
    }
    sfTcpSocket* sockets[8];
    for (int i = 0; i < 8; ++i) {
        sockets[i] = NULL;
    }


    printf("Waiting for clients to connect...\n");

    sfSocketSelector* selector = sfSocketSelector_create();
    sfSocketSelector_addTcpListener(selector, listener);


    sfPacket* packet;
    packet = sfPacket_create();

    while(1){
        sfBool flag = sfFalse;
        while (flag == sfFalse) {
            flag = sfSocketSelector_wait(selector, sfSeconds(10));


        }

        if (sfSocketSelector_isTcpListenerReady(selector,listener) == sfTrue) {


            sfTcpSocket *socket = NULL;
            //socket = sfTcpSocket_create();
            status = sfTcpListener_accept(listener, &socket);
            //sfSocketStatus_print(status);
            if (status != sfSocketDone) {
                printf("Could not accept socket\n");
            } else {}

            int i = 0;
            while (sockets[i] != NULL) {
                i++;
                if (i==8) break;
                //bbDebug("i = %d\n", i)
            }
            if (i < 8) {

                sockets[i] = socket;
                sfSocketSelector_addTcpSocket(selector, socket);
                bbNetworkPacket packetStruct;
                packetStruct.type = PACKETTYPE_SETSOCKETNUMBER;
                packetStruct.data.integer = i;

                bbDebug("new connection socket %d\n", i);
                sfPacket_clear(packet);
                bbNetworkPacket_fromStruct(packet, &packetStruct);
                status = sfTcpSocket_sendPacket(sockets[i], packet);


            } else {

                sfTcpSocket_destroy(socket);
                printf("Socket array full\n");
            }

        }



        for(int i = 0; i < 8;i++){
            if (sockets[i] == NULL) continue;

            if(sfSocketSelector_isTcpSocketReady(selector, sockets[i]) ==
            sfTrue){



                status = sfTcpSocket_receivePacket(sockets[i], packet);
                if (status != sfSocketDone){

                    //This line causes the server to lock up Wrong order of operations?
                    //sfTcpSocket_destroy(sockets[i]);
                    sfSocketSelector_removeTcpSocket(selector, sockets[i]);
                    sockets[i] = NULL;
                    continue;
                }


                bbNetworkPacket packetStruct, packetStruct2;

                bbNetworkPacket_toStruct(packet, &packetStruct);

                //bbPacketType_print(packetStruct.type);

                if (packetStruct.type == PACKETTYPE_REQUESTTIMESTAMP)
                {
                    packetStruct.type = PACKETTYPE_TIMESTAMP;

                    U64 time = sfTime_asMicroseconds(sfClock_getElapsedTime(clock));
                    packetStruct.data.timestamp.receive_time = time;
                    packetStruct.data.timestamp.send_time = time;


                    sfPacket_clear(packet);
                    bbNetworkPacket_fromStruct(packet, &packetStruct);
                    status = sfTcpSocket_sendPacket(sockets[i], packet);
                    sfPacket_clear(packet);
                    //printf("Sent time\n");
                    continue;
                }




                for (int j = 0; j < 8; j++){
                    //send to self
                    //if (j == i) continue;
                    if(sockets[j] == NULL) continue;

                    status = sfTcpSocket_sendPacket(sockets[j], packet);

                    if (status != sfSocketDone){
                        printf("Failed to echo user %d's message to user %d!\n",i, j);
                    } else
                    {
                        printf("Sent packet\n");
                    }
                }
                sfPacket_clear(packet);

                continue;
            }

        }



    }

    printf("Server disconnected.\n");

    for (int i = 0; i < 8; i++){
        if(sockets[i]== NULL) continue;

        sfTcpSocket_disconnect(sockets[i]);
    }

    exit(EXIT_SUCCESS);



}
