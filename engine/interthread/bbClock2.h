#ifndef BB_CLOCK2_H
#define BB_CLOCK2_H
#include "engine/logic/bbIntTypes.h"
#include "engine/network/bbNetworkTime.h"
#include "engine/threadsafe/bbThreadedQueue.h"

///Stored data returned by bbClock_waitTick()
typedef struct
{
    U64 server_tick;
    U64 map_tick;
    U8 clock_thread_index;
    bool clock_paused;
} bbClock2_handle;

typedef enum
{

    bbClock2MessageType_request,
    bbClock2MessageType_send,
    bbClock2MessageType_sendPaused
} bbClock2_message_type;

///bbClock_waitTick() sends a bbClock_message and waits for the response
typedef struct bbClock2_message
{
    bbClock2_message_type message_type;
    U64 server_tick;
    U64 map_tick;
    U8 clock_thread_index;
    bool clock_paused;
    bbListElement_Handle list_element;
} bbClock2_message;

typedef struct
{
    bbThreadedQueue outbox;
    //send a wakeup message when send_time == current_tick
    U64 wait_until_tick;

    ///if the game is paused, bbClock_waitTick() will wake up the thread every
    ///N clock ticks
    U8 update_when_paused;
    bool in_use;
} bbClock2_connection;

///I don't expect there will be more than 8 threads that need to synchronise clocks
#define MAX_CONNECTIONS 8

typedef struct
{
    pthread_t thread_id;
    bbNetworkTime* network_time;
    U64 server_tick;
    U64 map_tick;

    ///used to calculate difference between server_tick and map_tick
    U64 reference_server_tick;
    U64 reference_map_tick;
    bool is_paused;
    bool is_running;

    bbVPool* message_pool;
    bbThreadedQueue inbox;
    bbClock2_connection connections[MAX_CONNECTIONS];
} bbClock2;

bbFlag bbClock2_init(bbClock2* clock, bbNetworkTime* network_time);

bbFlag bbClock2_handle_init(bbClock2* clock,
                            bbClock2_handle* handle,
                            U8 update_when_paused);


bbFlag bbClock2_waitTick(bbClock2* clock, bbClock2_handle* handle, U64 until_map_tick);

bbFlag bbClock2_setPause(bbClock2* clock,
                U64 reference_server_tick,
                U64 reference_map_tick,
                bool is_paused);

bbFlag bbClock2_testPause(bbClock2* clock,bool is_paused);

#endif// BB_CLOCK2_H
