#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbTerminal.h"
#include "engine/test_string/bbTestString.h"
#include <time.h>
thread_local char* thread;
thread_local bool debug_off = {0};
U64 test_time = 0;
bbHandle null_handle = {0};
char test_string[KEY_LENGTH];

bbCore core;
int main(void)
{
    thread = "MAIN";
    bbDebug("Hello World!\n");

struct timespec
    strcpy(test_string, "Hello World!");

    bbCore_init(&core);

    //bbHere()
    bbCoreInput_setTime(&core, 1, bbInstructionSource_input, no_handle);
    //bbHere()
    bbCore_react(&core);
    //bbHere()
    test_time = 1;

    bbCoreInput_setTime(&core, 2, bbInstructionSource_input, no_handle);
    //bbHere()
    bbCore_react(&core);
    //bbHere()
    test_time = 2;
    //bbHere()

    bbCoreInput_setTime(&core, 3, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 3;


    bbCS_setString(&core, "bish", bbInstructionSource_input, no_handle);
    //bbHere()
    bbCS_setString(&core, "bash", bbInstructionSource_internal,no_handle);
    //bbHere()
    bbCS_setString(&core, "bosh", bbInstructionSource_norewind, no_handle);
    //bbHere()

    char str[KEY_LENGTH];
    U32 collision = 0;

    for (I32 i = 4; i < 1000;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        //bbHere()
        bbCore_react(&core);

        //bbHere()
        sprintf(str, "(%d)", i-3);

        bbAction_setString(&core,
                 0,
                 collision++,
                 i,
                 i-3,
                 str);

        //bbHere()
        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);

        //bbHere()
        bbCore_react(&core);
        //bbHere()
    }
bbDebug("We made it to the end!\n");
}
