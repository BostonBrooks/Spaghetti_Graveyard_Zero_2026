
#include <stdio.h>

#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"


pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;

U64 bbArith_rand = 193;
bbTime test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];

bbMapCoords testGoalPoint;

bbFlag bbCore_checkMap(bbInstructionMap* map,void*,void*) {
    return bbSuccess;
}

static void test_core(void) {

    printf("Initializing core\n");
    bbCore core;
    bbCore_init(&core);

    sfClock *clock =sfClock_create();
    bbAssert(clock != NULL, "sfClock_create failed");


    for (U32 i = 0; i < 1000000; i++) {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input,(bbHandle){0});
        bbCore_react(&core);
    }

    sfTime time = sfClock_restart(clock);
    printf("bbCI_setTime() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    bbCore_rewindUntil(&core, 0);


    time = sfClock_restart(clock);
    printf("bbCore_rewindUntil() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    bbCore_react(&core);

    time = sfClock_restart(clock);
    printf("bbCore_react() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    printf("test_core passed\n");
}


int main(void) {

    test_core();
    printf("All enabled set time tests passed!\n");

    return 0;
}
