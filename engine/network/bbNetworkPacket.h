#ifndef BB_NETWORK_PACKET_H
#define BB_NETWORK_PACKET_H

#include <SFML/Network.h>

#include "bbNetworkTime.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/network/bbNetworkTime.h"
#include "engine/network/bbNetworkTime_pause.h"

#define PACKETTYPE_STRING            0
#define PACKETTYPE_TIMESTAMP         1
#define PACKETTYPE_REQUESTTIMESTAMP  2
#define PACKETTYPE_SETGOALPOINT      3
#define PACKETTYPE_UNFREEZEBUTTON    4
#define PACKETTYPE_NETCODEBUTTON     5
#define PACKETTYPE_PAUSE             6
#define PACKETTYPE_KEYUP             7
#define PACKETTYPE_KEYDOWN           8
#define PACKETTYPE_PADDLEVELOCITY    9

#define bbPacketType_print(flag)\
{\
switch (flag)\
{\
case PACKETTYPE_STRING:\
bbDebug ("Packet Type: PACKETTYPE_STRING\n");\
break;\
case PACKETTYPE_TIMESTAMP:\
bbDebug ("Packet Type: PACKETTYPE_TIMESTAMP\n");\
break;\
case PACKETTYPE_REQUESTTIMESTAMP:\
bbDebug ("Packet Type: PACKETTYPE_REQUESTTIMESTAMP\n");\
break;\
case PACKETTYPE_SETGOALPOINT:\
bbDebug ("Packet Type: PACKETTYPE_SETGOALPOINT\n");\
break;\
case PACKETTYPE_UNFREEZEBUTTON:\
bbDebug ("Packet Type: PACKETTYPE_UNFREEZEBUTTON\n");\
break;\
case PACKETTYPE_NETCODEBUTTON:\
bbDebug ("Packet Type: PACKETTYPE_NETCODEBUTTON\n");\
break;\
case PACKETTYPE_PAUSE:\
bbDebug ("Packet Type: PACKETTYPE_PAUSE\n");\
break;\
case PACKETTYPE_KEYUP:\
bbDebug ("Packet Type: PACKETTYPE_KEYUP\n");\
break;\
case PACKETTYPE_KEYDOWN:\
bbDebug ("Packet Type: PACKETTYPE_KEYDOWN\n");\
break;\
case PACKETTYPE_PADDLEVELOCITY:\
bbDebug ("Packet Type: PACKETTYPE_PADDLEVELOCITY\n");\
break;\
\
}\
}\

typedef union
{
    I32 integer;
    bbNetwork_timestamp timestamp;
    bbNetwork_pauseMessage pause;
    char str[64];
    bbMapCoords map_coords;

    I32x2 paddle_and_velocity;
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