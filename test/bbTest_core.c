
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

int main (void)
{
    bbCore core;
    bbCore_init(&core);

    bbStr_putStr(home.core.quote, "Abera Kedabera",KEY_LENGTH);
    /// Functions like bbCore_printInteger should be called from the some thread
    /// that owns the instance of bbCore
    for (I32 i = 0; i < 8;i++)
    {
        char temp_string[KEY_LENGTH];
        sprintf(temp_string,"i = %d",i);
        bbHandle handle;
        handle.u64 = 0;
        bbCoreInput_setQuote(&core, temp_string, bbInstructionSource_input, handle);
        bbCore_react(&core);
    }


    bbCore_rewind(&core);
    bbCore_react(&core);

    sleep(5);
    exit(EXIT_SUCCESS);
}
