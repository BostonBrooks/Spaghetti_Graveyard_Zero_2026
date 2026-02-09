///bbClock uses bbNetworkTime to synchronise with the server, and loop exactly
///60 times per second.
///
///Threads are stored in a waiting state and then reactivated at a set time.

#ifndef BB_BBCLOCK_H
#define BB_BBCLOCK_H

#include "engine/network/bbNetworkTime.h"

typedef struct
{
    pthread_t clock_thread;
    bbNetworkTime* network_time;
    U64 current_tick;
    bool clock_running;
} bbClock;


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time);

#endif// BB_BBCLOCK_H