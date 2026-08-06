#include "engine/ECS/moveables/bbMoveables.h"
#ifndef BB_UNITS_H
#define BB_UNITS_H

#include "bbDrawables.h"

typedef struct
{
    //to be draw to the screen
    bbDrawable drawable;

    I32 enitity;
    I32 moveable;
    float percent_health;
    U64 display_health_until;
    //the following are used for interpolation
    bbMapCoords prev_coords;
    U64 prev_time;
    bbMapCoords prev_goalpoint;

    bbMapCoords next_coords;
    U64 next_time;
    bbMapCoords next_goalpoint;


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

bbFlag bbUnits_consumeBuffer(bbUnits* units, bbVPool* entity_units, bbMoveables_snapshot* snapshot);

//bbFlag bbUnit_newSkelly(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
//bbMapCoords MC, I32 index);

#endif //BB_UNITS_H