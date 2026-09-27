#ifndef GRAPHICS_DATA
#define GRAPHICS_DATA

typedef struct {
    bbHandle entity_handle;
    bbMapCoords MC;
    bbTime last_state_change;
    bbTime last_wander_time;
    U64 random_seed;
    I32 drawable_state;
} bbGraphicsComponent_data;

#endif