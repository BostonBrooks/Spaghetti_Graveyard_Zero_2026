#include "engine/network/bbNetworkClock.h"

#include "engine/logic/bbBloatedPool.h"

I32 bbNetworkClock_mathsSort(void* a, void* b)
{
    bbNetworkClock_maths* A = a;
    bbNetworkClock_maths* B = b;
    return (A->time_difference > B->time_difference);
}


void* bbNetwork_clockThread(void* Network_clock)
{
    bbNetworkClock* network_clock = (bbNetworkClock*) Network_clock;
    bbNetworkClock_pingRecord* ping_record;
    while (1)
    {
        bbThreadedQueue_popRblock(&network_clock->pending_pingRecords,(void**)&ping_record);
        //bbDebug("Round trip time = %lu, time_difference = %ld\n",
        //    ping_record->round_trip_time,
        //    ping_record->time_difference);


        bbNetworkClock_maths* maths;
        bbFlag flag = bbList_alloc(&network_clock->maths_list_chronological,(void**)&maths);


        //Since list element not initialized by previous statement
        maths->sorted.prev = network_clock->maths_list_sorted.pool->null;
        maths->sorted.next = network_clock->maths_list_sorted.pool->null;

        maths->time_difference = ping_record->time_difference;
        maths->round_trip_time = ping_record->round_trip_time;


        bbThreadedQueue_free(&network_clock->pending_pingRecords,(void**)&ping_record);

        bbList_pushL(&network_clock->maths_list_chronological,maths);
        bbList_sortL(&network_clock->maths_list_sorted,  maths);

        network_clock->maths_list_length++;
        if (network_clock->maths_list_length >32)
        {
            bbNetworkClock_maths* maths1;
            bbList_popR(&network_clock->maths_list_chronological,(void**)&maths1);
            bbList_remove(&network_clock->maths_list_sorted,(void*)maths1);
            bbVPool_free(network_clock->maths_pool,(void*)maths1);

            I64 average = 0;
            bbNetworkClock_maths* maths2;
            bbList_getNth(&network_clock->maths_list_sorted,(void**)&maths2, 15);
            average += maths2->time_difference;
            bbList_getNth(&network_clock->maths_list_sorted,(void**)&maths2, 16);
            average += maths2->time_difference;
            bbList_getNth(&network_clock->maths_list_sorted,(void**)&maths2, 17);
            average += maths2->time_difference;
            bbList_getNth(&network_clock->maths_list_sorted,(void**)&maths2, 18);
            average += maths2->time_difference;

            average /= 4;

            network_clock->time_difference = average;

            bbDebug("time difference = %ld\n", average);

            network_clock->maths_list_length--;

            if (network_clock->time_calibrated == false)
            {
                I64 time;
                //TODO bbNetworkClock_getMicroseconds(network_clock,&time);
                //network_clock->server_tick_time = time / (1000000/60);

                network_clock->time_calibrated = true;
            }

        }
    }
}

bbFlag bbNetworkClock_init(bbNetworkClock* network_clock, bbNetworkTime* network_time)
{
    network_clock->network_time = network_time;
    network_time->networkClock = network_clock;

    network_clock->server_tick_time = 0;

    bbThreadedQueue_init(&network_clock->pending_pingRecords,
        NULL,
        sizeof(bbNetworkClock_pingRecord),
        32,
        offsetof(bbNetworkClock_pingRecord, list_element));

    bbVPool_newBloated(&network_clock->maths_pool,sizeof(bbNetworkClock_maths),100,100);

    bbList_init(&network_clock->maths_list_chronological,
                network_clock->maths_pool,
                NULL,
                offsetof(bbNetworkClock_maths,chronological),
                NULL);

    bbList_init(&network_clock->maths_list_sorted,
            network_clock->maths_pool,
            NULL,
            offsetof(bbNetworkClock_maths,sorted),
            bbNetworkClock_mathsSort);

    network_clock->maths_list_length = 0;
    network_clock->time_difference = 0;
    network_clock->time_calibrated = false;

    pthread_create(&network_clock->clock_thread,NULL, bbNetwork_clockThread, network_clock);

    return bbSuccess;
}