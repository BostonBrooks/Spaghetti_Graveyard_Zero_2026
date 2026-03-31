#include <stdio.h>
#include <pthread.h>
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbString.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

thread_local char* thread;
bbHome home;
U64 test_time = 0;
char test_string[KEY_LENGTH];

int main(void)
{
    bbCore core;
    bbCore_init(&core);

    test_time = core.actual_time = 0;
    bbStr_putStr(test_string, "Abera Kedabera",KEY_LENGTH);

    U32 collision = 0;

    char str[KEY_LENGTH];


    //game is running normally
    for (I32 i = 1; i < 9;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i,
                         i,
                         str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }
/*
//Some instructions come in late
    for (I32 i = 1; i < 8; i++)
    {
         sprintf(str, "(%d.1)", i);

         bbAction_setString(&core,
                     0,
                     collision++,
                     i,
                     i,
                     str);
    }


    // Game continues to run normally
    for (I32 i = 9; i < 12;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i,
                         i,
                         str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }
*/
    for (I32 i = 9; i < 12;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "~(%d)", i-7);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i,
                         i-7,
                         str);

        sprintf(str, "*(%d)", i-7);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i,
                         i-7,
                         str);


        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }



    printf("We made it to the end!\n");
}