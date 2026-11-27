#ifndef BB_PROFILING_H
#define BB_PROFILING_H
#include <SFML/System/Types.h>
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"

typedef struct {

    sfClock * clock;
    U64 instructions_executed;
    U64 instructions_undone;

    U64 actions_received;
    U64 actions_executed;

    U64 rollback_count;
    U64 rollback_depth_total;
    U64 rollback_depth_max;

} bbProfilingData;

bbFlag bbProfilingData_print(bbProfilingData * data);
bbFlag bbProfilingData_init(bbProfilingData * data);

#define BB_ENABLE_PROFILING
#ifdef BB_ENABLE_PROFILING

extern bbProfilingData bbProfiling;

#define BB_PROFILE_INIT() bbProfilingData_init(&bbProfiling)
#define BB_PROFILE_INC(field) (++bbProfiling.field)
#define BB_PROFILE_PRINT() bbProfilingData_print(&bbProfiling)


#else

#define BB_PROFILE_INC(field) ((void)0)
#define BB_PROFILE_PRINT() (bbFail)
#define BB_PROFILE_INIT() (bbFail)

#endif


#endif