#include "engine/network/bbNetworkTime.h"

#include "engine/logic/bbBloatedPool.h"
#include "engine/network/bbNetwork.h"
#include "engine/logic/bbTerminal.h"
#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/threadsafe/bbThreadedQueue_search.h"

I32 sortByTimeDifference(void* a, void* b)
{
    bbNetworkTime_maths* A = a;
    bbNetworkTime_maths* B = b;
    return (A->time_difference > B->time_difference);
}


bbFlag bbNetworkTime_init(bbNetworkTime* network_time)
{
    I32 queue_size = 64;
    network_time->localClock = sfClock_create();
    network_time->packets_sent = 0;
    bbThreadedQueue_init(&network_time->pending, NULL, sizeof(bbNetworkTime_record),
    queue_size, offsetof(bbNetworkTime_record, list_element));

    bbThreadedQueue_init(&network_time->completed,network_time->pending.pool, sizeof(bbNetworkTime_record),
        queue_size, offsetof(bbNetworkTime_record, list_element));

    network_time->numMathsElements = 0;
    bbVPool_newBloated(&network_time->mathsPool,sizeof(bbNetworkTime_maths), 100,100);
    bbList_init(&network_time->mathsChronological, network_time->mathsPool, NULL,offsetof(bbNetworkTime_maths, chronological),NULL);
    bbList_init(&network_time->mathsSorted, network_time->mathsPool, NULL,offsetof(bbNetworkTime_maths, sorted),sortByTimeDifference);
    network_time->timeCalibrated = false;

    return bbSuccess;
}


bbFlag bbNetworkTime_filterOutbox (void* Network, void* Struct)
{

        bbNetworkPacket* packet = Struct;
    if (packet->type == PACKETTYPE_REQUESTTIMESTAMP)
    {

        bbNetwork* network = Network;
        bbNetworkTime* network_time = (bbNetworkTime*)network->extra_data;
        bbNetworkTime_record* record;
        bbThreadedPool* pool = network_time->pending.pool->pool;
        bbThreadedQueue_alloc(&network_time->pending, (void**)&record);
        packet->data.timestamp.packetN = network_time->packets_sent;
        record->packetN = network_time->packets_sent;

        network_time->packets_sent++;
        record->local_send_time = sfTime_asMicroseconds(sfClock_getElapsedTime(network_time->localClock));
        bbThreadedQueue_pushL(&network_time->pending,record);
        return bbSuccess;
    }

    return bbSuccess;
}

bbFlag packetN_equals(void* Callback, bbHandle handle)
{

    bbCallback* callback = (bbCallback*)Callback;
    I32 packet_n = callback->args.u64;
    bbNetworkTime_record* record = handle.ptr;

    if (record->packetN == packet_n) return bbSuccess;
    return bbNone;
}

bbFlag bbNetworkTime_filterInbox (void* Network, void* Struct)
{

    bbNetwork* network = Network;
    bbNetworkTime* network_time = (bbNetworkTime*)network->extra_data;
    bbNetworkPacket* packet = Struct;


    //bbDebug("sendTick = %llu, actTick = %llu\n", packet->sendTick, packet->actTick);

    if (packet->type == PACKETTYPE_SETGOALPOINT)
    {
        //bbDebug("packet coords i = %d, j = %d, k = %d\n",
        //    packet->data.map_coords.i, packet->data.map_coords.j, packet->data.map_coords.k);

       return bbSuccess;
    }

    if (packet->type == PACKETTYPE_TIMESTAMP)

        {
        bbCallback callback;
        callback.function = packetN_equals;
        callback.args.u64 = packet->data.timestamp.packetN;

        bbNetworkTime_record* record;

        bbFlag flag = bbThreadedQueue_search(&network_time->pending,  (void**)&record, &callback);

        if (flag == bbSuccess)
        {
            record->local_receive_time = sfTime_asMicroseconds(sfClock_getElapsedTime(network_time->localClock));
            record->server_receive_time = packet->data.timestamp.receive_time;
            record->server_send_time = packet->data.timestamp.send_time;

            //printf("packetN = %llu, local_send_time = %llu, server_receive_time = %llu, server_send_time = %llu, local_receive_time = %llu\n",
            //    record->packetN, record->local_send_time, record->server_receive_time, record->server_send_time, record->local_receive_time);


            bbThreadedQueue_pushL(&network_time->completed,record);
        }
        // "None" implies that the packet does not have to be placed in the network inbox
        return bbNone;
    }

return bbSuccess;
}

///place a PACKETTYPE_REQUESTTIMESTAMP in outbox
bbFlag bbNetworkTime_ping(void* Network)
{
    bbNetwork* network = Network;
    bbNetworkPacket* packet;
    bbThreadedQueue_alloc(&network->outbox, (void**)&packet);

    packet->send_tick = 696969696;
    packet->act_tick = 31415;
    packet->type = PACKETTYPE_REQUESTTIMESTAMP;

    bbThreadedQueue_pushL(&network->outbox,packet);

    return bbSuccess;
}


bbFlag bbNetworkTime_updateTimeDiff(bbNetworkTime* network_time)
{
    bbFlag flag;
    while (1)
    {
        bbNetworkTime_record* record;
        flag = bbThreadedQueue_popR(&network_time->completed, (void**)&record);
        if (flag != bbSuccess) return bbSuccess;




        //T1 = client send
        //T2 = server receive
        //T3 = server send
        //T4 = client receive
        //RTT = (T4-T1)-(T3-T2)
        //RTT = (client receive - client send) - (server send - server receive)

        U64 RTT = (record->local_receive_time - record->local_send_time) - (record->server_send_time - record->
            server_receive_time);

        //difference = ((T2-T1)+(T3-T4))/2;

        //difference = ((server receive-client send)+(server send-client receive))/2;

        I64 difference = (((I64)record->server_receive_time - (I64)record->local_send_time)+(
            + (I64)record->server_send_time - (I64)record->local_receive_time)) / 2;
        //printf("round trip time = %llu, time difference = %lld\n", RTT, difference);

        bbThreadedQueue_free(&network_time->completed, (void**)&record);

        bbNetworkTime_maths* maths;
        bbList_alloc(&network_time->mathsChronological,(void**)&maths);
        maths->sorted.prev = network_time->mathsSorted.pool->null;
        maths->sorted.next = network_time->mathsSorted.pool->null;

        maths->time_difference = difference;
        bbList_pushL(&network_time->mathsChronological, maths);

        bbList_sortL(&network_time->mathsSorted,maths);
        network_time->numMathsElements++;

        if (network_time->numMathsElements > 32)
        {



            bbList_popR(&network_time->mathsChronological,(void**)&maths);
            bbList_remove(&network_time->mathsSorted,maths);
            bbVPool_free(network_time->mathsPool,(void*)maths);

            I64 average = 0;
            bbNetworkTime_maths* maths;
            bbList_getNth(&network_time->mathsSorted,(void**)&maths, 15);
            average += maths->time_difference;
            bbList_getNth(&network_time->mathsSorted,(void**)&maths, 16);
            average += maths->time_difference;
            bbList_getNth(&network_time->mathsSorted,(void**)&maths, 17);
            average += maths->time_difference;
            bbList_getNth(&network_time->mathsSorted,(void**)&maths, 18);
            average += maths->time_difference;

            average /= 4;

            network_time->timeDifference = average;

            //bbDebug("timedifference = %ld\n", average);

            network_time->numMathsElements--;

            if (network_time->timeCalibrated == false)
            {
                network_time->timeCalibrated = true;
                I64 time;
                bbFlag flag1 = bbNetworkTime_get(network_time,&time);
                network_time->network_tick_time = time/(1000000/60);

                //bbDebug("time = %lld, tick time = %llu\n", time, network_time->network_tick_time);

            }
        }

    }
}

bbFlag bbNetworkTime_get(bbNetworkTime* network_time, I64* time)
{
    if (network_time->timeCalibrated == false)
    {
        *time = 0;
        return bbNone;
    }
    U64 local_time = sfTime_asMicroseconds(sfClock_getElapsedTime(network_time->localClock));
    *time = local_time + network_time->timeDifference;
    return bbSuccess;
}

bbFlag bbNetworkTime_waitInt(bbNetworkTime* network_time, U64 tick)
{
    I64 current_time;
    bbNetworkTime_get(network_time,&current_time);



    I64 target_time = tick * (1000000/60);

    I64 wait_time = target_time - current_time;



    if (wait_time <= 0)
    {
        network_time->network_tick_time = tick;
        //bbDebug("current_time = %lld, target_time = %lld, wait_time = %lld XXXXX wait_time negative\n",
        //        current_time, target_time, wait_time);
        return bbSuccess;
    }

    //bbDebug("current_time = %f, target_time = %f, wait_time = %f\n",
    //      current_time/1000000.f, target_time/1000000.f, wait_time/1000000.f);
    sfSleep(sfMicroseconds(wait_time));

    network_time->network_tick_time = tick;
    return bbSuccess;

}