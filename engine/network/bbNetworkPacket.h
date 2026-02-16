#ifndef BB_NETWORK_PACKET_H
#define BB_NETWORK_PACKET_H

#include <SFML/Network.h>

#include "bbNetworkTime.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/network/bbNetworkTime.h"

#define PACKETTYPE_STRING            0
#define PACKETTYPE_TIMESTAMP         1
#define PACKETTYPE_REQUESTTIMESTAMP  2
#define PACKETTYPE_SETGOALPOINT      3
#define PACKETTYPE_UNFREEZEBUTTON    4
#define PACKETTYPE_NETCODEBUTTON     5


typedef union
{
    bbNetwork_timestamp timestamp;
    char str[64];
    bbMapCoords map_coords;
} bbNetworkPacket_data;



typedef struct
{
    bbListElement_Handle list_element;
    I32 type;
    U64 send_tick;
    U64 act_tick;
    U8 player;
    bbNetworkPacket_data data;

} bbNetworkPacket;


bbFlag bbNetworkPacket_toStruct (sfPacket* packet, void* Struct);
bbFlag bbNetworkPacket_fromStruct (sfPacket* packet, void* Struct);

//depends on above functions, so not included in bbNetwork.c, which is meant to be more generic
bbFlag bbNetwork_sendStr(void* Network, char* str);


bbFlag bbNetwork_requestTimestamp(void* Network);

#endif //BB_NETWORK_PACKET_H