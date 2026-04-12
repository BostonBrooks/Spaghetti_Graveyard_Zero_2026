#ifndef BBUNITS_H
#define BBUNITS_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbNestedList.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/core/bbMoveable.h"

/** a unit can be drawn to the screen like a drawable, drawn to the map like
 * a map icon, and avoided by other units like an avoidable.
 * units are expected to move around and interact with each other during the
 * course of the game */

/* I don't know how concerned I should be about units being drawn to the map
 * in the right order considering other map icons, but they must be drawn to
 * the screen interleaved with the drawables
 * avoidance should be order agnostic
 */

typedef struct {
    bbDrawable drawable;
    I32 radius;
    bbMapCoords goalPoint;
    bbMoveable_type moveableType;
} bbUnit;

#define bbUnits_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, squares_i, squares_j, sizeof(bbUnit));

#define bbUnitSquare bbDrawableSquare

#define bbUnits bbDrawables

#define bbUnit_isCloser bbDrawable_isCloser
#define bbUnit_getSquareIndex bbDrawables_getSquareIndex

bbFlag bbUnit_new(bbUnit** unit,bbUnits* units, bbGraphicsApp* graphics,bbMapCoords MC);

bbFlag bbUnit_newCat(bbUnit** self,bbUnits* units, bbGraphicsApp* graphics,
                     bbMapCoords MC);

#define bbUnit_setLocation bbDrawable_setLocation
#endif //BBUNITS