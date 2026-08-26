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

int main(void)
{
    for (I64 delta_i = 0; delta_i < MILLS_PER_TILE * 4; delta_i +=
         MILLS_PER_TILE / 8)
    {
        I64 delta_j = 0;
        I64 distance = bbArith64_sqrt(delta_i * delta_i + delta_j * delta_j);


        I64 gap = distance - MILLS_PER_TILE;


        I64 force = MILLS_PER_TILE / 4 * (M_PI_2 - atan(gap * 64 / MILLS_PER_TILE));


        printf("%lld,%lld\n", gap, force);
    }
}
