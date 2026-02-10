///bbClock uses bbNetworkTime to synchronise with the server, and loop exactly
///60 times per second.
///
///Threads are stored in a waiting state and then reactivated at a set time.

#ifndef BB_BBCLOCK_H
#define BB_BBCLOCK_H

#include "engine/network/bbNetworkTime.h"

typedef enum
{
    bbClockMessageType_request,
    bbClockMessageType_send
} bbClock_message_type;

//send messages to and from bbClock
typedef struct bbClock_message
{
    bbClock_message_type type;
    U64 tick_time;
    bbListElement_Handle list_element;
} bbClock_message;

typedef struct
{
    pthread_t clock_thread;
    bbNetworkTime* network_time;
    U64 current_tick;
    bool clock_running;

    bbVPool* message_pool;

    //Later, there will be one of each of these per thread
    bbThreadedQueue inbox;
    bbThreadedQueue outbox;

} bbClock;


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time);

bbFlag bbClock_getTick(bbClock* clock, U64* tick_time);

bbFlag bbClock_waitTick(bbClock* clock, U64 tick_time);

#endif// BB_BBCLOCK_H