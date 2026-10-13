#ifndef GRAPHICS_DATA
#define GRAPHICS_DATA

typedef struct {
    bbHandle entity_handle;
    bbMapCoords MC;
    bbTime last_state_change;
    bbTime last_wander_time;
    U64 random_seed;
    U16 drawable_type;
    U16 drawable_state;

    //TODO put these into a union
    bbMapCoords start_point;
    bbMapCoords end_point;
    bbTime start_time;
    bbTime end_time;
} bbGraphicsComponent_data;

#endif