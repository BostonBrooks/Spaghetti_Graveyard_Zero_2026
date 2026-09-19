#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/logic/bbSegmentedDeque.h"
#include "engine/logic/bbTerminal.h"


thread_local char* thread;
thread_local bool debug_off;
U64 test_time = 0;


typedef struct {
    char string[32];
} test_struct;

int main(void)
{
    debug_off = false;
    bbSegmentedDeque deque;


    bbSegmentedDeque_init(&deque, 12);

    test_struct* test;


    for (I32 i = 0; i < 100; i++)
    {
        bbSegmentedDeque_allocFront(&deque, (void**)&test);

        sprintf(test->string, "%d", i);

        bbSegmentedDeque_pushFront(&deque, &test);
    }
    test_struct* test2;
    for (I32 i = 0; i < 100; i++){
        bbHere()
        bbSegmentedDeque_peakFront(&deque, (void**)&test2);
        bbHere()
        printf("pointer = %p\n", test2);
        printf("test: %s\n", test2->string);
        bbHere()
        bbSegmentedDeque_popFront(&deque, (void**)&test2);
        bbHere()
    }

    return EXIT_SUCCESS;
}


