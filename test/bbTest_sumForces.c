#include <math.h>
#include <stdio.h>

#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbArithmetic.h"
#include <stdio.h>

#include "engine/data/bbHome.h"
#include "engine/logic/bbHandle.h"

pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;
bbHome home;

U64 test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];

bbMapCoords testGoalPoint;

int main (void)
{
for (I64 delta_i = 0; delta_i < MILLS_PER_TILE*4; delta_i+= MILLS_PER_TILE/4)
{
    I64 delta_j = 0;
    I64 distance = bbArith64_sqrt(delta_i * delta_i + delta_j * delta_j);



    I64 gap = distance-MILLS_PER_TILE;




    double distanceReduced = (distance - 1.l * MILLS_PER_TILE) / 10000.l;
    double distanceReduced2 = distance / 100000.l;

    if (distanceReduced == 0.0 || distanceReduced2 == 0.0) continue;

    bbMilliCoords mC;
    mC.i = ((delta_i) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.j = ((delta_j) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.k = 0;

    mC.i = (double)MILLS_PER_TILE/4.f*atan((double)mC.i/((double)MILLS_PER_TILE/4.f));
    mC.j = (double)MILLS_PER_TILE/4.f*atan((double)mC.j/((double)MILLS_PER_TILE/4.f));


    double force = sqrt(mC.i * mC.i + mC.j * mC.j);


    printf("%lld,%f\n", gap, force);
}
}
