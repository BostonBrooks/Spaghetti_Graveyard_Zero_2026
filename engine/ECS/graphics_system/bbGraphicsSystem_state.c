


#include "bbGraphicsSystem_data.h"
#include "engine/logic/bbFlag.h"
// typedef struct {
//     bbHandle entity_handle;
//     bbMapCoords MC;
//     bbTime last_state_change;
//     bbTime last_wander_time;
//     U64 random_seed;
//     I32 drawable_state;
// } bbGraphicsComponent_data;
bbFlag bbGraphics_updateState(bbGraphicsComponent_data* new, bbGraphicsComponent_data* old, I32 state, bbTime time) {
    new->entity_handle = old->entity_handle;
    new->s

    return bbSuccess;
}