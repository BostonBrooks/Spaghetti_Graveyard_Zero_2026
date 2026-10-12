#include "engine/viewport/bbProjectiles.h"

#include "engine/data/bbHome.h"

typedef struct {
    bbTime current_time;
} projectile_update_cl;

float projectile_height (float start_time,
                       float end_time,
                       float start_height,
                       float end_height,
                       float gravity,
                       float current_time) {
    float dt = end_time - start_time;

    if (dt <= 0.0f) return end_height;
    float u = (current_time - start_time) / dt;
    if (u<=0.0f) return start_height;
    if (u>=1.0f) return end_height;
    return start_height + (end_height - start_height) * u + gravity * dt * dt * (u*u-u);
}


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


    bbMapCoords position
   = bbMapCoords_interpolate(unit->u.projectile.start_coords, unit->u.projectile.end_coords, start_time,
       current_time, end_time);

    position.k = projectile_height (start_time, end_time,  start_coords.k, end_coords.k, -0.1,current_time);

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