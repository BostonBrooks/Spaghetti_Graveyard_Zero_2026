
#include<unistd.h>
#include "engine/core/bbCore.h"

#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/logic/bbString.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

thread_local char* thread;
U64 test_time = 0;
bbHome home;
bbHandle no_handle = {0};


int main (void)
{

    //bbCore core;
    bbCore_init(&home.core.core);

    bbStr_putStr(home.core.quote, "Abera Kedabera",KEY_LENGTH);
    home.core.core.simulation_time = 117;
    home.core.test_int = 69;

    for (I32 i = 0; i < 8;i++)
    {
        char temp_string[KEY_LENGTH];
        sprintf(temp_string,"i = %d",i);
        bbHandle handle;
        handle.u64 = 0;
        bbCoreInput_setQuote(&home.core.core, temp_string, bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);


        bbCoreInput_setTime(&home.core.core, i, bbInstructionSource_input, no_handle);
        bbCore_react(&home.core.core);

    }


    bbCoreInput_setTestInt(&home.core.core, 7, bbInstructionSource_input, no_handle);
    bbCore_react(&home.core.core);

    bbCore_rewind(&home.core.core);
    bbCore_react(&home.core.core);
}
