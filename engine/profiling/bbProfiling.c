
#include "bbProfiling.h"

#include <stddef.h>
#include <SFML/System/Clock.h>

#include "engine/logic/bbTerminal.h"


bbFlag bbProfilingData_init(bbProfilingData * data) {

    *data = (bbProfilingData){0};

    data->clock = sfClock_create();


    return bbSuccess;
}

bbFlag bbProfilingData_print(bbProfilingData * data) {

    double elapsed = -1;
    if (data->clock != NULL) {
        sfTime time = sfClock_getElapsedTime(data->clock);
        I64 microseconds = sfTime_asMicroseconds(time);
        elapsed = microseconds / 1000000.0;
    }
    bbDebug("\nInstructions spawned: %lu\n"
            "Instructions executed: %lu\n"
            "Instructions undone: %lu\n"
            "Actions received: %lu\n"
            "Actions executed: %lu\n"
            "Elapsed time: %f seconds\n",
            data->instructions_executed - data->instructions_undone,
            data->instructions_executed,
            data->instructions_undone,
            data->actions_received,
            data->actions_executed,
            elapsed);

    return bbSuccess;
}

#ifdef BB_ENABLE_PROFILING
bbProfilingData bbProfiling = (bbProfilingData){0};
#endif