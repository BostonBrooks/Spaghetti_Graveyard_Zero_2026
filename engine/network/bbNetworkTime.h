
#include "engine/logic/bbIntTypes.h"

//This prevents a circular dependency
#ifndef BBNETWORKTIMESTAMP
#define BBNETWORKTIMESTAMP

typedef struct
{
    U64 packetN;
    U64 receive_time;
    U64 send_time;
} bbNetwork_timestamp;

#endif // BBNETWORKTIMESTAMP

#ifndef BBNETWORKTIME_H
#define BBNETWORKTIME_H

#include <SFML/System.h>

#include "bbNetwork.h"
#include "bbNetworkPacket.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbList.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/network/bbNetworkPacket.h"
#include "engine/network/bbNetwork.h"

typedef struct
{
    I64 time_difference;
    bbListElement_Handle chronological;
    bbListElement_Handle sorted;
} bbNetworkTime_maths;

typedef struct
{
    U64 packetN;
    U64 local_send_time;
    U64 server_receive_time;
    U64 server_send_time;
    U64 local_receive_time;
    I64 time_difference;
    U64 round_trip_time;
    bbListElement_Handle list_element;
} bbNetworkTime_record;

// keep data to track difference between clocks
typedef struct
{
    sfClock* localClock;
    U64 packets_sent;
    U64 network_tick_time;

    bbThreadedQueue pending;
    bbThreadedQueue completed;

    I32 numMathsElements;
    bbVPool* mathsPool;
    bbList mathsChronological;
    bbList mathsSorted;
    I64 timeDifference;
    bool timeCalibrated;
} bbNetworkTime;

// get in synch with server
bbFlag bbNetworkTime_init(bbNetworkTime* network_time);

// get the time since the server started
bbFlag bbNetworkTime_get(bbNetworkTime* network_time, I64* time);

// get the number of ticks/frames since the server started
bbFlag bbNetworkTime_getInt(bbNetworkTime* network_time, U64 tick);

// wait until the start of tick N
bbFlag bbNetworkTime_waitInt(bbNetworkTime* network_time, U64 tick);

//create timestamp immediately before sending packet
bbFlag bbNetworkTimeStamp_request(void* network, bbNetworkTime* network_time, bbNetwork_timestamp* timestamp);

///react immediately to incoming message
bbFlag bbNetworkTime_filterInbox (void* network, void* Struct);
///react to outgoing message immediately before sending
bbFlag bbNetworkTime_filterOutbox (void* network, void* Struct);

bbFlag bbNetworkTime_ping(void* network);

bbFlag bbNetworkTime_updateTimeDiff(bbNetworkTime* network_time);

#endif //BBNETWORKTIME_H
