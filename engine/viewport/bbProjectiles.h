#ifndef PROJECTILES_H
#define PROJECTILES_H
#include "bbDrawables.h"
#include "bbViewportApp.h"

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

    ///Note: this is not the same offset as in bbDrawable, will need separate machinery to work;
    struct bbRenderUnitGroup* group;
} bbProjectile;

#define bbProjectiles_new(self, squares_i, squares_j)\
bbDrawables_newImpl(self, bbSystem_Projectiles, squares_i, squares_j, sizeof(bbProjectile));
#define bbProjectileSquare bbDrawableSquare

#define bbProjectiles bbDrawables

#define bbProjectile_isCloser bbDrawable_isCloser
#define bbProjectile_getSquareIndex bbDrawables_getSquareIndex;


#define bbProjectile_setLocation bbDrawable_setLocation

bbFlag bbProjectiles_update(bbProjectiles* self);

bbFlag bbViewportSpawnSphere(bbViewportApp* viewport_app,
                             bbMapCoords MC,
                             bbHandle entity_handle,
                             bbHandle random_seed);

#endif