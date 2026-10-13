#include "engine/viewport/bbProjectiles.h"

#include "engine/data/bbHome.h"
#include "engine/geometry/bbViewportCoords.h"

typedef struct {
    bbTime current_time;
} projectile_update_cl;


///typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
bbFlag projectile_update(bbList* list, void* node, void* cl)
{
    bbUnit* unit = (bbUnit*)node;
    projectile_update_cl* closure = ( projectile_update_cl*)cl;

    bbTime current_time = closure->current_time;

    bbTime start_time = unit->u.projectile.start_time;
    bbTime end_time = unit->u.projectile.end_time;
    bbMapCoords start_coords = unit->u.projectile.start_coords;
    bbMapCoords end_coords = unit->u.projectile.end_coords;
    float gravity = unit->u.projectile.gravity;

    if (current_time < start_time) return bbContinue;
    if (current_time > end_time) return bbContinue;

    bbMapCoords position = bbMapCoords_interpolateGravity(
        start_coords,
        end_coords,
        1000,
        start_time,
        current_time,
        end_time);

    bbFloat3D velocity = bbMapCoords_getVelocity(
        start_coords,
        end_coords,
        1000,
        start_time,
        current_time,
        end_time);

    float rotation = bbVelocity_getRotation(velocity);



    unit->u.projectile.screen_rotation = rotation;
    float screen_angle = bbVelocity_getAngle(velocity);
    unit->u.projectile.screen_angle = screen_angle;
    bbDebug("theta = %f\n", rotation);

    bbUnit_setLocation((bbDrawable*)unit, home.viewport_app.units,position);

    return bbContinue;

}


bbFlag bbProjectiles_update(bbViewportApp *app)
{
    projectile_update_cl cl;
    cl.current_time = home.UI.clock2_handle.map_tick;
    bbList_mapL(&app->projectiles,projectile_update,&cl);
    return bbSuccess;
}