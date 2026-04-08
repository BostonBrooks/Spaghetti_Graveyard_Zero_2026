#include <stddef.h>

#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"

#define head_tail(queue1){bbAssert((queue1.head < 0)==(queue1.tail < 0),\
"head = %d, tail = %d\n", queue1.head, queue1.tail)}

thread_local char* thread;

U64 test_time = 0;

bbThreadedQueue queue;

bbThreadedPool* pool = NULL;

typedef struct
{
    bbListElement_Handle list_element;
} bbTest_Struct;


void* consumer_thread(void* arg)
{

    thread = "CONSUMER THREAD";


    while (1)
    {
        head_tail(queue);
        bbTest_Struct* test_struct;

        bbDebug("in use = %d\n", pool->in_use);
        bbThreadedQueue_popR_block(&queue, (void**)&test_struct);
;
        bbThreadedQueue_free(&queue, (void**)&test_struct);

    }
}

int main (void)
{

    thread = "PRODUCER THREAD";


    bbFlag flag = bbThreadedQueue_init(&queue, NULL, sizeof(bbTest_Struct), 7, offsetof(bbTest_Struct, list_element));

    pool = queue.pool->pool;

    pthread_t consumer_pthread;
    pthread_create(&consumer_pthread, NULL, consumer_thread, NULL);

    while (1)
    {
        head_tail(queue);

        bbTest_Struct* test_struct;

        bbDebug("in use = %d\n", pool->in_use);
        bbThreadedQueue_alloc(&queue, (void**)&test_struct);

        bbThreadedQueue_pushL(&queue, test_struct);

    }
}