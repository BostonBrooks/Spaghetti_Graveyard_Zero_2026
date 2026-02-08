#ifndef BBNWETWORKAPP_H
#define BBNWETWORKAPP_H

#include "engine/network/bbNetwork.h"
#include "engine/logic/bbFlag.h"

//initialize the system
bbFlag bbNetworkApp_init(bbNetwork* network);

//connect to the network and change address and port to actual
bbFlag bbNetworkApp_connect(bbNetwork* network, char* address, char* port);

bbFlag bbNetworkApp_sendString(bbNetwork* network, char* string);
bbFlag bbNetworkApp_sendTime(bbNetwork* network);
bbFlag bbNetworkApp_sendNetworkPacket(bbNetwork* network, void* packet);

bbFlag bbNetworkApp_checkInbox(bbNetwork* network);


bbFlag bbNetworkApp_sendGoalpoint(void* Network, bbMapCoords* coords);

#endif // BBNWETWORKAPP_H