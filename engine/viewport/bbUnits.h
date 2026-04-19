#include "bbDrawables.h"

typedef struct
{
    //to be draw to the screen
    bbDrawable drawable;

    //the following are used for interpolation
    bbMilliCoords prev_coords;
    U64 prev_time;
    bbMilliCoords prev_goalpoint;

    bbMilliCoords next_coords;
    U64 next_time;
    bbMilliCoords next_goalpoint;


} bbUnit;

#define bbUnits_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, squares_i, squares_j, sizeof(bbUnit));

#define bbUnitSquare bbDrawableSquare

#define bbUnits bbDrawables

#define bbUnit_isCloser bbDrawable_isCloser
#define bbUnit_getSquareIndex bbDrawables_getSquareIndex

bbFlag bbUnit_newCat(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
                     bbMapCoords MC, I32 index);

bbFlag bbUnit_newSkeleton(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
bbMapCoords MC, I32 index);

#define bbUnit_setLocation bbDrawable_setLocation

bbFlag bbUnits_consumeBuffer(bbUnits* units, bbHandle* unit_array, bbMoveables_snapshot* snapshot);