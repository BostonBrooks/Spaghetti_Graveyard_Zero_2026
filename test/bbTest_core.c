
#include<unistd.h>
#include "engine/core/bbCore.h"

#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/logic/bbString.h"
#include "engine/core/bbLocalMessageInputs.h"

thread_local char* thread;
U64 test_time = 0;

int main (void)
{
    bbCore core;
    bbCore_init(&core);

    /// Functions like bbCore_printInteger should be called from the some thread
    /// that owns the instance of bbCore
    bbCore_printInteger(&core, 69, true);
    bbCore_printString(&core, "69", true);

    bbCore_react(&core);


    bbLocalMessage_PrintString(&core, "sending message from other thread\n");

    bbCore_checkLocalMessages(&core);

    sleep(5);
    exit(EXIT_SUCCESS);
}
