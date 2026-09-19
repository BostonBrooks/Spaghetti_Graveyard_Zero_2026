#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/logic/bbSegmentedDeque.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbSQ_Macros.h"

thread_local char* thread;
thread_local bool debug_off;
U64 test_time = 0;



typedef struct {
    char string[32];
} test_struct;

DECLARE_SQ_HEADER(bbTest,sizeof(test_struct),12)

int main(void)
{
    debug_off = false;
    bbSegmentedDeque deque;


    bbSegmentedDeque_init(&deque, 12);

    test_struct *test, *test2;

    bbTest_deque macro_deque;
    bbTest_deque_init(&macro_deque, 12);

    for (I32 i = 0; i < 100; i++)
    {
        bbTest_deque_allocBack(&macro_deque, (void**)&test);

        sprintf(test->string, "%d", i);

        bbTest_deque_pushBack(&macro_deque, &test);
    }
    for (I32 i = 0; i < 1000; i++){

        printf("%d\n", i);
        bbFlag flag = bbTest_deque_peakBack(&macro_deque, (void**)&test2);
        bbAssert(flag == bbSuccess, "peak failed first\n");

        printf("macro test: %s\n", test2->string);

        flag = bbTest_deque_popBack(&macro_deque, (void**)&test2);
        bbAssert(flag == bbSuccess, "pop failed first\n");

    }

    for (I32 i = 0; i < 100; i++)
    {
        bbSegmentedDeque_allocBack(&deque, (void**)&test);

        sprintf(test->string, "%d", i);

        bbSegmentedDeque_pushBack(&deque, &test);
    }

    for (I32 i = 0; i < 1000; i++){

        printf("%d\n", i);
        bbFlag flag = bbSegmentedDeque_peakBack(&deque, (void**)&test2);
        bbAssert(flag == bbSuccess, "peak failed first\n");

        printf("test: %s\n", test2->string);

        flag = bbSegmentedDeque_popBack(&deque, (void**)&test2);
        bbAssert(flag == bbSuccess, "pop failed first\n");

    }

    return EXIT_SUCCESS;
}

DECLARE_SQ_BODY(bbTest,sizeof(test_struct),12)
