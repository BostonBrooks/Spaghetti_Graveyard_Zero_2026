
#include<unistd.h>
#include "engine/core/bbCore.h"

#include "engine/core/bbCoreInputs.h"

thread_local char* thread;
U64 test_time = 0;

int main (void)
{
    bbCore core;
    bbCore_init(&core);
    bbCore_printInteger(&core, 69, true);
    bbCore_react(&core);
    sleep(5);
    exit(EXIT_SUCCESS);
}
