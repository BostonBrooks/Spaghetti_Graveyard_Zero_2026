///Network clock runs in it's own thread. tick by at 60hz
///When bbNetworkTime_filterInbox sees a timestamp, it will place a message on a bbThreadedQueue
///and send that to bbNetworkClock.
///
///Will need access to network_time->localClock
///

#ifndef BBNETWORKCLOCK_H
#define BBNETWORKCLOCK_H

#include "engine/network/bbNetworkTime.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbHandle.h"
#include "engine/threadsafe/bbThreadedPool.h"

//Keep the last 32 of these and find the average of the middle 5
typedef struct
{
    I64 time_difference;
    U64 round_trip_time;
    bbListElement_Handle chronological;
    bbListElement_Handle sorted;
} bbNetworkClock_maths;

// bbNetworkTime_filterInbox will send these via bbNetworkClock.pending_pingRecords
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
} bbNetworkClock_pingRecord;

typedef struct
{
    bbNetworkTime* network_time;

    U64 server_tick_time;

    pthread_t clock_thread;
    bbThreadedQueue pending_pingRecords;

    bbVPool* maths_pool;
    bbList maths_list_chronological;
    bbList maths_list_sorted;
    I32 maths_list_length;

    I64 time_difference;
    bool time_calibrated;

    bbVPool waiting_threads_pool;
    bbList waiting_threads_list;

} bbNetworkClock;

///Launch network clock in new thread
bbFlag bbNetworkClock_init(bbNetworkClock* network_clock, bbNetworkTime* network_time);

///Get the server's current clock tick
bbFlag bbNetworkClock_getTick(bbNetworkClock* network_clock, U64* tick);

///Wait until the servers clock gets to the value 'tick'
///The thread doing the waiting might need to store some data
///May need to store a pool of waiting threads
bbFlag bbNetworkClock_waitTick(bbNetworkClock* network_clock, U64 tick);

///Request a time stamped packet from server to be sent to network clock via bbNetworkTime_filterInbox
bbFlag bbNetworkClock_ping(void* network);






#endif //BBNETWORKCLOCK_H