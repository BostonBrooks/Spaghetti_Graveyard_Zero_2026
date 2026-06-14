
#include "engine/logic/bbIntTypes.h"

//This prevents a circular dependency
#ifndef BBNETWORKTIMEPAUSEMESSAGE
#define BBNETWORKTIMEPAUSEMESSAGE

typedef struct
{
    U64 reference_server_tick;
    U64 reference_map_tick;
    bool is_paused;
} bbNetwork_pauseMessage;

#endif // BBNETWORKTIMEPAUSEMESSAGE



#ifndef BBNETWORKTIME_PAUSE_H
#define BBNETWORKTIME_PAUSE_H

#include <CSFML/System.h>

#include "bbNetwork.h"
#include "bbNetworkPacket.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbList.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/network/bbNetworkPacket.h"
#include "engine/network/bbNetwork.h"




#endif