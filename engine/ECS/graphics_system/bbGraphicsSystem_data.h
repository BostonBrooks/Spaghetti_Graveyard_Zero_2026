#ifndef GRAPHICS_DATA
#define GRAPHICS_DATA

typedef struct {
    bbMapCoords start_point;
    bbMapCoords end_point;
    bbTime start_time;
    bbTime end_time;
    bbHandle caster_entity_handle;
}bbGraphicsComponent_dataProjectile ;


typedef struct {
    U16 drawable_state;
    bbTime last_state_change;
    bbTime last_wander_time;
}bbGraphicsComponent_dataUnit ;

typedef union {
    bbGraphicsComponent_dataUnit unit;
    bbGraphicsComponent_dataProjectile projectile;
} bbGraphicsComponent_dataUnion;

typedef struct {
    bbHandle entity_handle;
    bbMapCoords MC;
    U64 random_seed;
    U16 drawable_type;

    bbGraphicsComponent_dataUnion u;

} bbGraphicsComponent_data;

#endif