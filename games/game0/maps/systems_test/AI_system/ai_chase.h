#include "engine/logic/bbIntTypes.h"

bbFlag bbAI_Update_Chase2(bbAI_Component* component) {
    debug_off = true;
    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbHandle attacker_entity_handle;
    bbECS_entity* attacker_entity;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &attacker_entity_handle,
                             (bbComponent**)&attacker_entity);



    bbMoveable* target_moveable;
    bbHandle target_moveable_handle;


    bbPlayers* players = &home.ECS.players;
    bbHandle target_entity_handle = home.ECS.ECS->system.pool->null;
    //debug_off = false;
    bbFlag flag = bbTeams_findNearestTarget(&home.core.core, home.ECS.ECS, attacker_entity_handle , &target_entity_handle, POINTS_PER_SQUARE);


    if (bbSuccess != bbVPool_handleIsNULL(home.ECS.ECS->system.pool,target_entity_handle)) {

        bbHere()
        return bbSuccess;
        //debug_off = true;
    }
        //debug_off = true;
    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          target_entity_handle, bbECS_Moveables,
                          &target_moveable_handle, (bbComponent**)&target_moveable);


    U64 distance_squared = (moveable->position.i - target_moveable->position.i)
        * (moveable->position.i - target_moveable->position.i)
        + (moveable->position.j - target_moveable->position.j)
        * (moveable->position.j - target_moveable->position.j);

    debug_off = false;

    U64 distance = bbArith64_sqrt2(distance_squared);
    bbDebug("distance: %llu\n", distance);

    debug_off = true;

    return bbSuccess;
}