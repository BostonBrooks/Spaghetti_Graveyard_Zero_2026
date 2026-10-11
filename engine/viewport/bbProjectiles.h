#ifndef PROJECTILES_H
#define PROJECTILES_H
#include "bbDrawables.h"

 typedef struct {
    bbMapCoords start_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    U64 gravity;
}bbProjectile_data;

typedef struct {
    bbMinimalDrawable md;
    bbProjectile_data data;
} bbProjectile;

#define bbProjectiles_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, bbSystem_Units, squares_i, squares_j, sizeof(Projectile));
#define bbProjectileSquare bbDrawableSquare

#define bbProjectiles bbDrawables

#define bbProjectile_isCloser bbDrawable_isCloser
#define bbProjectile_getSquareIndex bbDrawables_getSquareIndex;


#define bbProjectile_setLocation bbDrawable_setLocation

bbFlag bbProjectiles_update(bbProjectiles* self);
#endif //BB_UNITS_H

#endif