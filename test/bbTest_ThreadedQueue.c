#include <stddef.h>

#include "engine/threadsafe/bbThreadedQueue.h"


thread_local char* thread;

U64 test_time = 0;

typedef struct
{


    bbListElement_Handle list_element;
} bbTest_Struct;

int main (void)
{
    bbThreadedQueue queue;

    bbFlag flag = bbThreadedQueue_init(&queue, NULL, sizeof(bbTest_Struct), 7, offsetof(bbTest_Struct, list_element));

    for (I32 i=0; i<7; i++)
    {
        bbTest_Struct* test_struct;

        bbThreadedQueue_alloc(&queue, (void**)&test_struct);

        bbThreadedQueue_pushL(&queue, test_struct);
    }
    for (I32 i=0; i<7; i++)
    {
        bbTest_Struct* test_struct;


        bbThreadedQueue_popR(&queue, (void**)&test_struct);

        bbThreadedQueue_free(&queue, (void**)&test_struct);
    }
    for (I32 i=0; i<7; i++)
    {
        bbTest_Struct* test_struct;

        bbThreadedQueue_alloc(&queue, (void**)&test_struct);

        bbThreadedQueue_pushL(&queue, test_struct);
    }
    for (I32 i=0; i<7; i++)
    {
        bbTest_Struct* test_struct;


        bbThreadedQueue_popR(&queue, (void**)&test_struct);
        bbThreadedQueue_free(&queue, (void**)&test_struct);
    }
}