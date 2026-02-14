#include "engine/network/bbNetworkApp.h"

#include <stdio.h>
#include <string.h>

#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"

//typedef bbFlag bbNetwork_onConnect (void* network);
//Notify user interface of network connection
bbFlag bbConnect(void* network);
//typedef bbFlag bbNetwork_onDisconnect (void* network);
//Notify user interface of network connection
bbFlag bbDisconnect(void* network);

//initialize the system
bbFlag bbNetworkApp_init(bbNetwork* network)
{
    bbFlag flag;
    bbNetworkTime* network_time = (bbNetworkTime*)malloc(sizeof(bbNetworkTime));
    bbNetworkTime_init(network_time);

    flag = bbNetwork_init(network,
        bbNetworkPacket_toStruct,
        bbNetworkPacket_fromStruct,
        bbConnect,bbDisconnect,
        bbNetworkTime_filterInbox,bbNetworkTime_filterOutbox,network_time);

    return bbSuccess;
}

//connect to the network and change address and port to actual
bbFlag bbNetworkApp_connect(bbNetwork* network, char* address, char* port)
{
    sfIpAddress ip_address;
    I32 port_number;
    bbFlag flag;
    if (strlen(address) == 0)
    {
        bbStr_setStr(address ,"127.0.0.1", 64);
        bbStr_setStr(port ,"1701", 64);

        network->on_disconnect(NULL);
        return bbNone;
    }
    ip_address = sfIpAddress_fromString(address);


    U32 addressInt = sfIpAddress_toInteger(ip_address);

    if (addressInt == 0)
    {
        bbStr_setStr(address ,"127.0.0.1", 64);
        bbStr_setStr(port ,"1701", 64);

        network->on_disconnect(NULL);
        return bbNone;
    }
    sfIpAddress_toString(ip_address, address);

    I32 len = strlen(port);
    if (len == 0)
    {
        bbStr_setStr(address ,"127.0.0.1", 64);
        bbStr_setStr(port ,"1701", 64);

        network->on_disconnect(NULL);
        return bbNone;
    }

    char digits[] = "0123456789";
    I32 int_len = strspn(port, digits);
    if (len != int_len)
    {
        bbStr_setStr(address ,"127.0.0.1", 64);
        bbStr_setStr(port ,"1701", 64);

        network->on_disconnect(NULL);
        return bbNone;
    }
    port_number = atoi(port);

    flag = bbNetwork_connect(network, ip_address, port_number);

    bbFlag_print(flag);
    return flag;

}

bbFlag bbNetworkApp_sendGoalpoint(void* Network, bbMapCoords* coords)
{
    bbNetwork* network = (bbNetwork*)Network;
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->actTick = 1337;
    packet->sendTick = 8008135;
    packet->type = PACKETTYPE_SETGOALPOINT;
    packet->data.map_coords.i = coords->i;
    packet->data.map_coords.j = coords->j;
    packet->data.map_coords.k = coords->k;
    bbThreadedQueue_pushL(&network->outbox, (void*)packet);

    return bbSuccess;
}

bbFlag bbNetworkApp_sendString(bbNetwork* network, char* string)
{
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_STRING;
    bbStr_setStr(packet->data.str, string, 64);
    bbThreadedQueue_pushL(&network->outbox, (void*)packet);

    return bbSuccess;
}
bbFlag bbNetworkApp_sendTime(bbNetwork* network){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_REQUESTTIMESTAMP;

    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}
bbFlag bbNetworkApp_sendNetworkPacket(bbNetwork* network, void* packet);

bbFlag bbNetworkApp_checkInbox(bbNetwork* network)
{
    while (1)
    {
        bbFlag flag;
        bbNetworkPacket* packet;
        flag = bbThreadedQueue_popR(&network->inbox, (void**)&packet);
        if (flag != bbSuccess) return bbSuccess;



        if (packet->type == PACKETTYPE_STRING)
        {
            printf("packet received: %s\n", packet->data.str);
        }
        if (packet->type == PACKETTYPE_SETGOALPOINT)
        {
            //bbDebug("coords received: i = %d, j = %d, k = %d\n",
            //    packet->data.map_coords.i, packet->data.map_coords.j, packet->data.map_coords.k);
        }

        if (packet->type == PACKETTYPE_UNFREEZEBUTTON)
        {
            bbDebug("add incoming message to bbCore?\n");

            bbLocalMessage_UnfreezeButton2(&home.core.core);
        }
        bbThreadedQueue_free(&network->inbox, (void**)&packet);
    }
}

//Notify user interface of network connection
bbFlag bbConnect(void* network)
{
    bbDebug("Connect to server in thread %s\n", thread);
/*
    bbPool_Handle handle;
    bbWidgets* widgets = &home.private.UI.widgets;
    bbWidget *widget;
    bbDictionary_lookup(widgets->dict, "DISCONNECT",&handle);
    bbVPool_lookup(widgets->pool, (void**)&widget, handle);

    widget->isFrozen = false;


    bbDictionary_lookup(widgets->dict, "CONNECT",&handle);
    bbVPool_lookup(widgets->pool, (void**)&widget, handle);

    widget->isFrozen = true;
*/
    return bbSuccess;
}
//typedef bbFlag bbNetwork_onDisconnect (void* network);
//Notify user interface of network connection
bbFlag bbDisconnect(void* network)
{
    bbDebug("Disconnect from server in thread %s\n", thread);
/*
    bbPool_Handle handle;
    bbWidgets* widgets = &home.private.UI.widgets;
    bbWidget *widget;
    bbDictionary_lookup(widgets->dict, "CONNECT",&handle);
    bbVPool_lookup(widgets->pool, (void**)&widget, handle);

    widget->isFrozen = false;

    bbDictionary_lookup(widgets->dict, "DISCONNECT",&handle);
    bbVPool_lookup(widgets->pool, (void**)&widget, handle);

    widget->isFrozen = true;*/
    return bbSuccess;
}



bbFlag bbNetworkApp_netsendButton(bbNetwork* network){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_UNFREEZEBUTTON;

    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}
