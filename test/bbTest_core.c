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

    bbStr_putStr(test_string, "Abera Kedabera",KEY_LENGTH);

    U32 collision = 0;

    char str[KEY_LENGTH];


    //game is running normally
    for (I32 i = 1; i < 9;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
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

    bbLocalMessage_SetString(&core, "\"I made this world for you\"");
    bbCore_checkLocalMessages(&core);

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

    bbLocalMessage_SetString(&core, "\"The size of a marble\"");
    bbCore_checkLocalMessages(&core);

    // Game continues to run normally
    for (I32 i = 9; i < 12;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
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

    for (I32 i = 12; i < 24;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        bbCore_react(&core);

        sprintf(str, "(%d)", i-10);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i-10,
                         i-10,
                         str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }

    bbLocalMessage_SetString(&core, "\"I made this world for you\"");
    bbCore_checkLocalMessages(&core);


    printf("We made it to the end!\n");
}