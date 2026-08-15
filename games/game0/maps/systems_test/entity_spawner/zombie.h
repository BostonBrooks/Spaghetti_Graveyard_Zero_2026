#include "engine/ECS/ECS.h"
#include "../../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbFlag.h"
#include "moveables/moveables.h"

bbFlag bbSF_addGraphics_zombie(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);


bbFlag bbSF_addGraphics_zombie(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbHandle moveable = entity->components[bbECS_Moveables];
    //
    // bbCoreSynchronous_spawnGraphicsComponent(&home.core.core,
    //                                args.position,
    //                                handle,
    //                                moveable,
    //                                bbInstructionSource_norewind);

    bbCoreInput_spawnGraphicsComponent(&home.core.core,
                                    "ZOMBIE",
                                   args.position,
                                   handle,
                                   moveable,
                                   bbInstructionSource_norewind,
                                   no_handle);

    return bbSuccess;
}

bbFlag bbSF_addAI_player(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{

    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");


    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbAI_Component* this;




    bbCS_spawnAIComponent2(&home.core.core,
                          home.core.core.ECS,
                          handle,
                          0,1,
                          &this,
                          source,
                          no_handle);

    return bbSuccess;
}
