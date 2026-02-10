#include "engine/interthread/bbClock.h"

#include "engine/logic/bbBloatedPool.h"
#include "engine/threadsafe/bbThreadedPool.h"


void* clock_thread(void* arg)
{
    thread = "CLOCK";
    bbClock *clock = (bbClock *) arg;
    I64 time;

    bbNetworkTime* network_time = clock->network_time;
    bbNetworkTime_get(network_time, &time);

    clock->current_tick = time / (1000000 / 60);

    bbVPool_newThreaded(&clock->message_pool,sizeof(bbClock_message),69);

    bbThreadedQueue_init(&clock->inbox,
                          clock->message_pool,
                          sizeof(bbClock_message),
                          69,
                          offsetof(bbClock_message, list_element));

    bbThreadedQueue_init(&clock->outbox,
                      clock->message_pool,
                      sizeof(bbClock_message),
                      69,
                      offsetof(bbClock_message, list_element));

    clock->clock_running = true;
    while (1)
    {
        bbNetworkTime_waitInt(clock->network_time, clock->current_tick+1);
        clock->current_tick++;

        bbClock_message* msg;
        //bbVPool_alloc(clock->message_pool, (void**)&msg);
        bbThreadedQueue_alloc(&clock->outbox,(void**)&msg);
        msg->type = bbClockMessageType_send;
        msg->tick_time = clock->current_tick;
        bbThreadedQueue_pushR(&clock->outbox,msg);

        if (clock->current_tick % 300 == 0)
        {
            bbHere()
        }
    }
}


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time)
{
    clock->network_time = network_time;

    pthread_create(&clock->clock_thread, NULL, clock_thread, clock);

    return EXIT_SUCCESS;
}