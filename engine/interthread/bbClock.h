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
    bbClockMessageType_subscribe,
    bbClockMessageType_send
} bbClock_message_type;

//send messages to and from bbClock
typedef struct bbClock_message
{
    bbClock_message_type type;
    bbHandle data;
    U8 index;
    bbListElement_Handle list_element;
} bbClock_message;

typedef struct
{
    bbThreadedQueue outbox;
    //send a wakeup message when send_time == current_tick
    U64 send_time;
    //send a wakeup message when "remainder" == "current_tick" mod "period"
    U32 period;
    U32 remainder;
    bool allocated;
} bbClock_connection;

#define MAX_CONNECTIONS 8
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
    bbClock_connection connections[MAX_CONNECTIONS];

} bbClock;


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time);

///Get the current time in multiples of 1/60 seconds
bbFlag bbClock_getTick(bbClock* clock, U64* tick_time);

///Assign outbox to thread by giving the thread an integer index
bbFlag bbClock_getOutboxIndex(bbClock* clock, U8* index);

///Wait until the time reaches tick_time
bbFlag bbClock_waitTick(bbClock* clock, U64 tick_time, U8 index);

///send a wakeup message when "remainder" == "current_tick" mod "period"
bbFlag bbClock_subscribe(bbClock* clock, U32 period, U32 remainder, U8 index);

///wake up when "remainder" == "current_tick" mod "period"
bbFlag bbClock_waitSubscription(bbClock* clock, U8 index);

#endif// BB_BBCLOCK_H