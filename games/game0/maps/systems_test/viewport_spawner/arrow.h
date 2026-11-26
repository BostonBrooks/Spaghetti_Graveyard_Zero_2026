
#ifndef BB_ARROW_H
#define BB_ARROW_H
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/viewport/bbProjectile_data.h"
#include "engine/viewport/bbViewportApp.h"
bbFlag bbProjectile_spawnArrow(bbViewportApp* viewport_app,
                                bbProjectile_data data,
                                bbHandle entity_handle,
                                bbHandle caster_entity_handle,
                                U64 random_seed) ;

#endif //BB_ARROW_H