#include "engine/interthread/bbClock.h"


void* clock_thread(void* arg)
{
    thread = "CLOCK";
    bbClock *clock = (bbClock *) arg;

    while (1)
    {
        bbNetworkTime_waitInt(clock->network_time, clock->current_tick+1);
        clock->current_tick++;

        if (clock->current_tick % 300 == 0)
        {
            bbHere()
        }
    }
}


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time)
{
    clock->network_time = network_time;

    I64 time;
    bbNetworkTime_get(network_time, &time);

    clock->current_tick = time / (1000000 / 60);

    clock->clock_running = true;
    pthread_create(&clock->clock_thread, NULL, clock_thread, clock);

    return EXIT_SUCCESS;
}