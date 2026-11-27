

#include <stdio.h>


#include "engine/core/bbCore.h"
#include "engine/core/bbCoreDiscard.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/profiling/bbProfiling.h"

pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;


U64 bbArith_rand = 193;
bbTime test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];
bbHome home;

bbMapCoords testGoalPoint;


static void test_core(void) {

    printf("Initializing core\n");
    bbCore core;
    bbCore_init(&home.core.core);
    bbCore_initMap(&home.core.core.map);

    sfClock *clock =sfClock_create();
    bbAssert(clock != NULL, "sfClock_create failed");


    for (U32 i = 0; i < 1000000; i++) {
        bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input,(bbHandle){0});
        bbCore_react(&home.core.core);
    }

    sfTime time = sfClock_restart(clock);
    printf("bbCI_setTime() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    bbCore_rewindUntil(&home.core.core, 0);


    time = sfClock_restart(clock);
    printf("bbCore_rewindUntil() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    bbCore_react(&home.core.core);

    time = sfClock_restart(clock);
    printf("bbCore_react() takes %lld microseconds\n", sfTime_asMicroseconds(time));

    printf("test_core passed\n");
}


static void stress_test_actions(void) {

  //  bool warnings_were_fatal = bbWarningsAreFatal;
  //  bbWarningsAreFatal = 0;

    printf("Initializing core\n");
    //Initialise dispatch table for instruction callbacks
    bbCore_init(&home.core.core);
    bbCore_initVInstructions(&home.core.core);
    bbCore_initInboxMessages(&home.core.core);
    bbCore_initActions(&home.core.core);
    bbCore_initDiscard(&home.core.core);
    bbCore_initMap(&home.core.core.map);


    bbFlag flag;
    for (U32 i = 0; i < 7; i++) {
        flag = bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input,(bbHandle){0});
        bbAssert(flag == bbSuccess,"bbCI_setTime() returns  flag=%d\n", flag);
        flag = bbCore_react(&home.core.core);
        bbAssert(flag == bbSuccess,"bbCore_react() returns  flag=%d\n", flag);

        flag = bbCoreInput_checkActions(&home.core.core,i,bbInstructionSource_input,(bbHandle){0});
        bbAssert(flag == bbSuccess,"checkActions() returns  flag=%d\n", flag);
        flag = bbCore_react(&home.core.core);
        bbAssert(flag == bbSuccess,"bbCore_react() returns  flag=%d\n", flag);

    }

    for (U32 i = 7; i < 1000; i++) {



        flag = bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input,(bbHandle){0});
        bbAssert(flag == bbSuccess,"bbCI_setTime() returns  flag=%d\n", flag);
        flag = bbCore_react(&home.core.core);
        bbAssert(flag == bbSuccess,"bbCore_react() returns  flag=%d\n", flag);

        char expected[128];
        sprintf(expected,"Expected TIME: %u",1);
        bbAction_setString(&home.core.core,
                   0,
                   i,
                   i,
                   1,
                   expected);

        flag = bbCoreInput_checkActions(&home.core.core,i,bbInstructionSource_input,(bbHandle){0});
        bbAssert(flag == bbSuccess,"checkActions() returns  flag=%d\n", flag);
        flag = bbCore_react(&home.core.core);
        bbAssert(flag == bbSuccess,"bbCore_react() returns  flag=%d\n", flag);
    }


    bbVPool_delete(home.core.core.instruction_pool);
    bbVPool_delete(home.core.core.action_pool);


    printf("stress_test_actions passed\n");

}


int main(void) {

    //test_core();

    BB_PROFILE_INIT();
    for (I32 i = 0; i < 50; i++) {
        stress_test_actions();
    }


    BB_PROFILE_PRINT();
    printf("All enabled set time tests passed!\n");

    return 0;
}
