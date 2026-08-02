#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbTerminal.h"
#include "engine/test_string/bbTestString.h"

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

    strcpy(test_string, "Hello World!");


    bbCore_init(&core);

    bbCoreInput_setTime(&core, 1, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 1;

    bbCoreInput_setTime(&core, 2, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 2;

    bbCoreInput_setTime(&core, 3, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 3;


    bbCS_setString(&core, "bish", bbInstructionSource_input, no_handle);
    bbCS_setString(&core, "bash", bbInstructionSource_internal,no_handle);
    bbCS_setString(&core, "bosh", bbInstructionSource_norewind, no_handle);

    char str[KEY_LENGTH];
    U32 collision = 0;

    for (I32 i = 4; i < 10;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i-3);

        bbAction_setString(&core,
                 0,
                 collision++,
                 i,
                 i-3,
                 str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }

}
