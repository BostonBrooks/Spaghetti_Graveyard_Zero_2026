#include "engine/ECS/moveables/bbMoveables.h"
#ifndef BB_UNITS_H
#define BB_UNITS_H

#include "bbDrawables.h"
#include "bbViewportMouse.h"
#include "engine/viewport/bbViewportMouseFunctions.h"

typedef enum {
    bbUnit_unit,
    bbUnit_projectile
} bbUnit_type;

typedef struct {
    bbMapCoords start_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    float gravity;
    float screen_rotation;
    float screen_angle;
    bbHandle caster;
    bbListElement_Handle list;
}bbProjectile_data;

typedef struct {
    I32 enitity;
    I32 moveable;
    float percent_health;
    bbTime display_health_until;
    //the following are used for interpolation
    bbMapCoords prev_coords;
    bbTime prev_time;
    bbMapCoords prev_goalpoint;
    bbMapCoords next_coords;
    float prev_angle;
    float next_angle;
    bbTime next_time;

    float speed;
    bbMapCoords next_goalpoint;

    bbUnit_mouse mouse;
} bbUnit_data;

typedef struct {
    bbUnit_data unit;
    bbProjectile_data projectile;
} bbUnit_union;

typedef struct bbUnit
{
    //to be draw to the screen
    bbDrawable drawable;
    bbHandle entity_handle;
    bbUnit_type type;
    bbUnit_union u;

} bbUnit;

#define bbUnits_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, bbSystem_Units, squares_i, squares_j, sizeof(bbUnit));

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