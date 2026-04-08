#include "engine/interthread/bbClock2.h"

#include "bbClock.h"
#include "engine/threadsafe/bbThreadedPool.h"

#define CLOCK_MESSAGE_POOL_SIZE 4096

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

    bbVPool_newThreaded(&clock->message_pool,sizeof(bbClock2_message),CLOCK_MESSAGE_POOL_SIZE);

    bbThreadedQueue_init(&clock->inbox,
                       clock->message_pool,
                       sizeof(bbClock2_message),
                       CLOCK_MESSAGE_POOL_SIZE,
                       offsetof(bbClock2_message, list_element));

    for (I32 i = 0; i < MAX_CONNECTIONS; i++)
    {
        bbThreadedQueue_init(&clock->connections[i].outbox,
                      clock->message_pool,
                      sizeof(bbClock2_message),
                      CLOCK_MESSAGE_POOL_SIZE,
                      offsetof(bbClock2_message, list_element));

        clock->connections[i].wait_until_tick = 0;
        clock->connections[i].update_when_paused = 0;
        clock->connections[i].in_use = false;


    }

    clock->is_running = true;

    //debug code:
    bbClock2_message* message;
    bbThreadedQueue_alloc(&clock->inbox, (void**)&message);

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
            if (flag != bbSuccess) break;
            if (message_in->clock_thread_index >= 3)
            {
                bbDebug("index (%d) too large! message_type = %d\n",
                    message_in->clock_thread_index, message_in->message_type );
                bbThreadedQueue_free(&clock->inbox,(void**)&message_in);
                continue;
            }

            if (message_in->clock_thread_index < 0)
            {
                bbDebug("index (%d) negative! message_type = %d\n",
                    message_in->clock_thread_index, message_in->message_type );
                bbThreadedQueue_free(&clock->inbox,(void**)&message_in);
                continue;
            }

            if (message_in->message_type == bbClock2MessageType_request)
            {
                bbClock2_connection* connection;
                connection = &clock->connections[message_in->clock_thread_index];

                connection->wait_until_tick = message_in->map_tick;
                bbThreadedQueue_free(&clock->inbox,(void**)&message_in);
            }
            else
            {
                bbNotHere()
            }
        }

        if (clock->is_paused == false)
        {
            for (I32 i = 0; i < MAX_CONNECTIONS; i++)
            {

                bbClock2_connection* connection;
                connection = &clock->connections[i];

                if (connection->in_use == false) continue;

                if (connection->wait_until_tick <= clock->map_tick)
                {
                    bbClock2_message* message_out;
                    bbThreadedQueue_alloc(&connection->outbox,(void**)&message_out);
                    message_out->message_type = bbClock2MessageType_send;
                    message_out->server_tick = clock->server_tick;
                    message_out->map_tick = clock->map_tick;
                    message_out->clock_paused = clock->is_paused;
                    connection->wait_until_tick = 0xFFFFFFFFFFFFFFFF;
                    bbThreadedQueue_pushR(&connection->outbox,message_out);


                }
            }
        } else
        {
            for (I32 i = 0; i < MAX_CONNECTIONS; i++)
            {
                bbClock2_connection* connection;
                connection = &clock->connections[i];

                if (connection->in_use == false) continue;

                if (connection->update_when_paused > 0
                    && clock->server_tick % connection->update_when_paused == 0)
                {
                    bbClock2_message* message_out;
                    bbThreadedQueue_alloc(&connection->outbox,(void**)&message_out);
                    message_out->message_type = bbClock2MessageType_sendPaused;
                    message_out->server_tick = clock->server_tick;
                    message_out->map_tick = clock->map_tick;
                    message_out->clock_paused = clock->is_paused;

                    bbThreadedQueue_pushR(&connection->outbox,message_out);
                }

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

//TODO not threadsafe, test and set clock->connections[i].in_use or use mutex
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


bbFlag bbClock2_waitTick(bbClock2* clock, bbClock2_handle* handle, U64 until_map_tick)
{
    bbClock2_message* message;
    bbThreadedQueue_alloc(&clock->inbox, (void**)&message);

    message->message_type = bbClock2MessageType_request;
    message->map_tick = until_map_tick;
    message->clock_thread_index = handle->clock_thread_index;
    bbAssert(handle->clock_thread_index < MAX_CONNECTIONS, "index too large\n");

    bbThreadedQueue_pushR(&clock->inbox,message);

    bbThreadedQueue_popL_block(
        &clock->connections[handle->clock_thread_index].outbox,
        (void**)&message);

    //todo what is message.message_type?
    handle->map_tick = message->map_tick;
    handle->server_tick = message->server_tick;
    handle->clock_paused = message->clock_paused;

    bbThreadedQueue_free(
        &clock->connections[handle->clock_thread_index].outbox,
        (void**)&message);

    //todo return bbPaused
    return bbSuccess;
}

bbFlag bbClock2_setPause(bbClock2* clock,
                U64 reference_server_tick,
                U64 reference_map_tick,
                bool is_paused)
{
    clock->reference_server_tick = reference_server_tick;
    clock->reference_map_tick = reference_map_tick;
    clock->is_paused = is_paused;
    return bbSuccess;
}


bbFlag bbClock2_testPause(bbClock2* clock,bool is_paused)
{
    bbClock2_setPause(clock,
                clock->server_tick,
                clock->map_tick,
                is_paused);
    return bbSuccess;
}