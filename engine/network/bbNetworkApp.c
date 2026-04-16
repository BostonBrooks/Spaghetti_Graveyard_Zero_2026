#include "engine/network/bbNetworkApp.h"

#include <stdio.h>
#include <string.h>

#include "engine/core/bbAction.h"
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
    packet->act_tick = 1337;
    packet->send_tick = 8008135;
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
            bbLocalMessage_UnfreezeButton(&home.core.core,packet->data.str);
            bbCore_react(&home.core.core);

        }

        if (packet->type == PACKETTYPE_NETCODEBUTTON)
        {
            bbAction_unfreezeButton(&home.core.core,
                packet->player,
                0,
                packet->send_tick,
                packet->act_tick,
                packet->data.str);
        }

        if (packet->type == PACKETTYPE_PAUSE)
        {
            bbClock2_setPause(&home.clock2,
                              packet->data.pause.reference_server_tick,
                              packet->data.pause.reference_map_tick,
                              packet->data.pause.is_paused);
            bbLocalMessage_UnfreezeButton(&home.core.core,"(UN)PAUSE");

        }

        if (packet->type == PACKETTYPE_KEYUP)
        {

            bbAction_setPaddleDirection(&home.core.core,
            packet->player,
            rand(),
            packet->send_tick,
            packet->act_tick,
             left_stop);
            bbDebug("key up: %d\n", packet->data.integer);
        }
        if (packet->type == PACKETTYPE_KEYDOWN)
        {
            if (packet->data.integer == 73)
            {
                bbAction_setPaddleDirection(&home.core.core,
                packet->player,
                rand(),
                packet->send_tick,
                packet->act_tick,left_up);
            }
            if (packet->data.integer == 74)
            {
                bbAction_setPaddleDirection(&home.core.core,
                packet->player,
                rand(),
                packet->send_tick,
                packet->act_tick,left_down);
            }
            bbDebug("key down: %d\n", packet->data.integer);

        }
        if (packet->type == PACKETTYPE_PADDLEVELOCITY)
        {


            bbAction_setPaddleVelocity(&home.core.core,
                                        packet->data.paddle_and_velocity.x,
                                        packet->collision,
                                        packet->send_tick,
                                        packet->act_tick,
                                        packet->data.paddle_and_velocity.y);
        }

        if (packet->type == PACKETTYPE_VIEWPOINT)
        {
            bbAction_setViewpoint(&home.core.core,
                                        packet->data.map_coords,
                                        packet->collision,
                                        packet->send_tick,
                                        packet->act_tick);
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



bbFlag bbNetworkApp_netsendButton(bbNetwork* network, char* key){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_UNFREEZEBUTTON;

    bbStr_setStr(packet->data.str, key, 64);
    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}



bbFlag bbNetworkApp_netpauseButton(bbNetwork* network, char* key){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_PAUSE;

    packet->data.pause.is_paused = !home.core.clock2_handle.clock_paused;
    packet->data.pause.reference_server_tick = home.core.clock2_handle.server_tick;
    packet->data.pause.reference_map_tick = home.core.clock2_handle.map_tick;

    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}



bbFlag bbNetworkApp_netcodeButton(bbNetwork* network, char* key, U64 time){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_NETCODEBUTTON;
    packet->act_tick = time;
    bbStr_setStr(packet->data.str, key, 64);
    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}

bbFlag bbNetworkApp_keyUp(bbNetwork* network, I32 key_code, U64 time, U32 collision){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_KEYUP;
    packet->act_tick = time;
    packet->data.integer = key_code;
    packet->collision = collision;
    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}
bbFlag bbNetworkApp_keyDown(bbNetwork* network, I32 key_code, U64 time, U32 collision){
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_KEYDOWN;
    packet->act_tick = time;
    packet->data.integer = key_code;
    packet->collision = collision;
    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}


bbFlag bbNetworkApp_setViewpointOut(bbNetwork* network, bbMapCoords MC, U64 time, U32 collision)
{
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);
    packet->type = PACKETTYPE_VIEWPOINT;
    packet->act_tick = time;
    packet->data.map_coords = MC;
    packet->collision = collision;
    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}