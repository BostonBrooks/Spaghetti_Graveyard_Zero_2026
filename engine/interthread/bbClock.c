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

    for (I32 i = 0; i < MAX_CONNECTIONS; i++)
    {
        bbThreadedQueue_init(&clock->connections[i].outbox,
                      clock->message_pool,
                      sizeof(bbClock_message),
                      69,
                      offsetof(bbClock_message, list_element));

        clock->connections[i].send_time = 0;
        clock->connections[i].period = 0;
        clock->connections[i].remainder = 0;
        clock->connections[i].allocated = false;
    }

    clock->clock_running = true;
    while (1)
    {
        bbNetworkTime_waitInt(clock->network_time, clock->current_tick+1);
        clock->current_tick++;

        bbClock_message* msg1;
        while (1)
        {
            bbFlag flag = bbThreadedQueue_popL(&clock->inbox, (void**)&msg1);
            if (flag == bbNone) break;
            if (msg1->type == bbClockMessageType_request)
            {
                bbClock_connection* connection;
                connection = &clock->connections[msg1->index];
                connection->period = 0;
                connection->remainder = 0;
                connection->send_time = msg1->data.u64;
                bbThreadedQueue_free(&clock->inbox, (void**)&msg1);
            }
            else
            {
                bbHere()
            }
        }
        for (I32 i = 0; i < MAX_CONNECTIONS; i++)
        {
            bbClock_connection* connection;
            connection = &clock->connections[i];

            if (connection->allocated == false) continue;

            if (connection->send_time > 0)
            {
                if (connection->send_time <= clock->current_tick)
                {

                    bbClock_message* msg;
                    bbThreadedQueue_alloc(&connection->outbox,(void**)&msg);
                    msg->type = bbClockMessageType_send;
                    msg->data.u64 = clock->current_tick;
                    msg->index = i;
                    //bbDebug("prev = %llx, next = %llx, null = %llx\n",
                    //    msg->list_element.prev.u64,
                    //    msg->list_element.next.u64,
                    //    clock->message_pool->null.u64);
                    bbThreadedQueue_pushR(&connection->outbox,(void*)msg);
                    connection->send_time = 0;
                }
            } else
            {
                //check period and remainder
            }
        }

/* Since we're not using this, we need to comment out,
 * because otherwise it causes a memory leak
        bbClock_message* msg;
        bbThreadedQueue_alloc(&clock->outbox,(void**)&msg);
        msg->type = bbClockMessageType_send;
        msg->data.u64 = clock->current_tick;
        bbThreadedQueue_pushR(&clock->outbox,msg);
*/
    }
}


bbFlag bbClock_init(bbClock* clock, bbNetworkTime* network_time)
{
    clock->network_time = network_time;

    pthread_create(&clock->clock_thread, NULL, clock_thread, clock);

    return bbSuccess;
}

bbFlag bbClock_getTick(bbClock* clock, U64* tick_time)
{
    *tick_time = clock->current_tick;
    return bbSuccess;
}

///Assign outbox to thread by giving the thread an integer index
bbFlag bbClock_getOutboxIndex(bbClock* clock, U8* index)
{
    for (I32 i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (clock->connections[i].allocated == false)
        {
            clock->connections[i].allocated = true;
            *index = i;
            return bbSuccess;
        }
    }
    return bbFail;
}

//Wait until the time reaches tick_time
//It is possible that bbClock_waitTick comes in one tick late I don't know
bbFlag bbClock_waitTick(bbClock* clock, U64 tick_time, U8 index)
{
    if (tick_time <= clock->current_tick) return bbNone;
    bbClock_message* msg;
    bbThreadedQueue_alloc(&clock->inbox, (void**)&msg);
    msg->index = index;
    msg->data.u64 = tick_time;
    msg->type = bbClockMessageType_request;
    bbThreadedQueue_pushR(&clock->inbox,msg);
    bbThreadedQueue_popL_block(&clock->connections[index].outbox,(void**)&msg);
    bbThreadedQueue_free(&clock->connections[index].outbox,(void**)&msg);
    return bbSuccess;
}