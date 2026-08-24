#include "engine/data/bbPerformance.h"


bbFlag bbPerformance_init(bbPerformance* performance)
{
    performance->clock = sfClock_create();

    performance->start_time = 0;
    performance->end_time = 0;

    performance->last_frame = 0;

    return bbSuccess;
}


bbFlag bbPerformance_newFrame(bbPerformance* performance)
{
    sfTime new_frame = sfClock_getElapsedTime(performance->clock);
    U64 new_frame_time = sfTime_asMicroseconds(new_frame);

    float time_elsapsed = new_frame_time - performance->last_frame;

    performance->framerate = 1000000.f / time_elsapsed;

    performance->last_frame = new_frame_time;

    return bbSuccess;
}


bbFlag bbPerformance_start(bbPerformance* performance)
{

    sfTime new_time = sfClock_getElapsedTime(performance->clock);
    U64 new_start_time = sfTime_asMicroseconds(new_time);

    float time_elsapsed = new_start_time - performance->end_time;

    performance->start_time = new_start_time;
    performance->downtime = time_elsapsed/100000.f;

    return bbSuccess;

}
bbFlag bbPerformance_end(bbPerformance* performance)
{

    sfTime new_time = sfClock_getElapsedTime(performance->clock);
    U64 new_end_time = sfTime_asMicroseconds(new_time);

    float time_elsapsed = new_end_time - performance->start_time;

    performance->end_time = new_end_time;
    performance->uptime = time_elsapsed/100000.f;

    return bbSuccess;

}