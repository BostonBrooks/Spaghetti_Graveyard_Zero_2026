#include "engine/interthread/bbClock2.h"

#include "bbClock.h"
#include "engine/threadsafe/bbThreadedPool.h"

void* clock2_thread(void* arg)
{
    thread = "CLOCK_THREAD";
    bbClock2 *clock = (bbClock2*) arg;

    I64 time;
    bbNetworkTime* network_time = clock->network_time;
    bbNetworkTime_get(network_time, &time);

    clock->server_tick = time / (1000000 / 60);
    clock->map_tick = 0;

    clock->reference_server_tick = clock->server_tick;
    clock->reference_map_tick = 0;
    clock->is_paused = true;

    bbVPool_newThreaded(&clock->message_pool,sizeof(bbClock_message),193);

    bbThreadedQueue_init(&clock->inbox,
                       clock->message_pool,
                       sizeof(bbClock2_message),
                       193,
                       offsetof(bbClock2_message, list_element));

    for (I32 i = 0; i < MAX_CONNECTIONS; i++)
    {
        bbThreadedQueue_init(&clock->connections[i].outbox,
                      clock->message_pool,
                      sizeof(bbClock2_message),
                      193,
                      offsetof(bbClock2_message, list_element));

        clock->connections[i].wait_until_tick = 0;
        clock->connections[i].update_when_paused = 0;
        clock->connections[i].in_use = false;


    }

    clock->is_running = true;

    while (1)
    {
        bbNetworkTime_waitInt(clock->network_time, clock->server_tick+1);
        clock->server_tick++;

        if (clock->is_paused) clock->map_tick = clock->reference_map_tick;
        else
        {
            clock->map_tick = clock->reference_map_tick
                      + (clock->server_tick - clock->reference_server_tick);
        }


        //check inbox

        bbClock2_message* message_in;

        while (1)
        {
            bbFlag flag = bbThreadedQueue_popR(&clock->inbox,(void**)&message_in);
            if (flag == bbNone) break;
            if (message_in->message_type == bbClock2MessageType_request)
            {
                bbClock2_connection* connection;
                connection = &clock->connections[message_in->clock_thread_index];

                connection->wait_until_tick = message_in->map_tick;
                bbThreadedQueue_free(&clock->inbox,(void**)message_in);
            }
            else
            {
                bbHere();
            }
        }


        //wake up threads with messages
    }
}

bbFlag bbClock2_init(bbClock2* clock, bbNetworkTime* network_time)
{
    clock->network_time = network_time;

    pthread_create(&clock->thread_id, NULL, clock2_thread, clock);

    return bbSuccess;
}

bbFlag bbClock2_handle_init(bbClock2* clock,
                            bbClock2_handle* handle,
                            U8 update_when_paused)
{
    for (I32 i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (clock->connections[i].in_use == false)
        {
            clock->connections[i].in_use = true;
            clock->connections[i].update_when_paused = update_when_paused;

            handle->server_tick = clock->server_tick;
            handle->map_tick = clock->map_tick;
            handle->clock_thread_index = i;
            handle->clock_paused = clock->is_paused;


            return bbSuccess;
        }
    }
    return bbFail;
}


bbFlag bbClock2_waitTick(bbClock2* clock, bbClock2_handle* handle, U64 until_tick)
{
    bbClock2_message* message;
    bbThreadedQueue_alloc(&clock->inbox, (void**)&message);

    message->message_type = bbClock2MessageType_request;
    message->map_tick = until_tick;
    message->clock_thread_index = handle->clock_thread_index;

    bbThreadedQueue_pushR(&clock->inbox,message);

    bbThreadedQueue_popL_block(
        &clock->connections[handle->clock_thread_index].outbox,
        (void**)&message);

    handle->map_tick = message->map_tick;
    handle->server_tick = message->server_tick;
    handle->clock_paused = message->clock_paused;

    bbThreadedQueue_free(
        &clock->connections[handle->clock_thread_index].outbox,
        (void**)&message);

    return bbSuccess;
}