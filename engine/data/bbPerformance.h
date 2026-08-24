#include "engine/interthread/bbClock.h"

typedef struct
{
    sfClock *clock;
    U64 start_time;
    U64 end_time;

    U64 last_frame;

    float uptime;
    float downtime;
    float framerate;
} bbPerformance;

bbFlag bbPerformance_init(bbPerformance* performance);

bbFlag bbPerformance_start(bbPerformance* performance);
bbFlag bbPerformance_end(bbPerformance* performance);
bbFlag bbPerformance_newFrame(bbPerformance* performance);


