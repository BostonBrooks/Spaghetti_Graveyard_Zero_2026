#include <stddef.h>

#include "engine/threadsafe/bbThreadedQueue.h"


thread_local char* thread;

U64 test_time = 0;

bbThreadedQueue queue;

typedef struct
{
    bbListElement_Handle list_element;
} bbTest_Struct;


void* consumer_thread(void* arg)
{

    thread = "CONSUMER THREAD";


    while (1)
    {bbHere()
        bbTest_Struct* test_struct;

        bbThreadedQueue_popR_block(&queue, (void**)&test_struct);

        bbThreadedQueue_free(&queue, (void**)&test_struct);
    }
}

int main (void)
{

    thread = "PRODUCER THREAD";


    bbFlag flag = bbThreadedQueue_init(&queue, NULL, sizeof(bbTest_Struct), 7, offsetof(bbTest_Struct, list_element));



    pthread_t consumer_pthread;
    pthread_create(&consumer_pthread, NULL, consumer_thread, NULL);

    while (1)
    {bbHere()
        bbTest_Struct* test_struct;

        bbThreadedQueue_alloc(&queue, (void**)&test_struct);

        bbThreadedQueue_pushL(&queue, test_struct);
    }
}