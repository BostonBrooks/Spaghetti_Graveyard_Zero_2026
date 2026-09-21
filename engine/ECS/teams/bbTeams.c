#include "engine/ECS/teams/bbTeams.h"

#include "engine/logic/bbSystemPool.h"

bbFlag bbTeam_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbTeam_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}

bbFlag bbTeams_init(bbTeams* system, bbECS* ECS) {
    bbVPool_newSystem(&system->system.pool, bbECS_Teams, sizeof(bbTeam),10,1000,"TEAMS");



    system->system.getComponent = bbTeam_getComponent_fn;
    system->system.getHandle = bbTeam_getHandle_fn;
    system->system.delete = NULL;
    system->system.ECS = ECS;


    ECS->systems[bbECS_Teams] = (bbSystem*)system;
}

bbFlag bbCI_spawnTeamSpatialComponent(bbCore* core,
                             bbHandle entity,
                             I32 team,
                             bbInstruction_source source,
                             bbHandle action);