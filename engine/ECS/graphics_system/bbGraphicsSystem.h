#ifndef BB_GRAPHICS_SYSTEM_H
#define BB_GRAPHICS_SYSTEM_H


#include "engine/ECS/bbECS.h"
#include "engine/logic/bbDictionary.h"

typedef struct bbGraphicsSystem bbGraphicsSystem;



typedef struct {
    bbHandle entity_handle;
    bbMapCoords MC;
    bbTime last_state_change;
    bbTime last_wander_time;
    U64 random_seed;
    I32 drawable_state;
} bbGraphicsComponent_data;

typedef struct
{
    bbComponent component;

    bbGraphicsComponent_data data;
} bbGraphicsComponent;

typedef bbFlag bbGraphics_spawnFunction(bbGraphicsSystem* system,
                                   bbGraphicsComponent_data data);


typedef struct bbGraphicsSystem
{
    bbSystem system;

    bbGraphics_spawnFunction** spawn_functions;
    I32 spawn_function_count;
    I32 max_function_count;
    bbDictionary* spawn_dict;
} bbGraphicsSystem;

bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS);
bbFlag bbGraphicsSystem_populate(bbGraphicsSystem* graphics_system);

bbFlag bbGraphics_spawnFunction_add(bbGraphicsSystem* graphics_system,
    bbGraphics_spawnFunction* function, char* key);

bbFlag bbCI_spawnGraphicsComponent( bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          I32 drawable_state,
                                          U64 random_seed,
                                          bbHandle entity,
                                          bbInstruction_source source,
                                          bbHandle action);

bbFlag bbCS_spawnGraphicsComponent( bbCore* core,
                                          bbGraphicsComponent** this,
                                          char* type,
                                          bbGraphicsComponent_data* data,
                                          bbInstruction_source source,
                                          bbHandle action);

bbFlag bbI_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);




bbFlag bbCoreInput_spawnDrawable(bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          I32 drawable_state,
                                          bbHandle entity,
                                          bbHandle moveable,
                                          bbInstruction_source source,
                                          bbHandle action);

bbFlag bbCoreSynchronous_spawnDrawable(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle entity,
                                   bbHandle moveable,
                                   bbInstruction_source source);

bbFlag bbInstruction_spawnDrawable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnDrawable_fn(bbCore* core, bbInstruction* instruction);




#endif //BB_GRAPHICS_SYSTEM_H