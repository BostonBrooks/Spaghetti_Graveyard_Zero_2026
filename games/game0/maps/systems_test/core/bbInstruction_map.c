#include "engine/core/bbInstruction_map.h"


#include "engine/core/bbInstruction.h"
#include  "games/game0/maps/systems_test/core/instructions.h"
bbFlag bbCore_initMap(bbInstructionMap* map)
{
    map->forward = calloc(bbVInstruction_numTypes, sizeof(I32));
    map->rollback = calloc(bbVInstruction_numTypes, sizeof(I32));

    for (I32 i = 0; i < bbVInstruction_numTypes; i++) {
        map->forward[i] = WRONG_FUNCTION_TYPE;
        map->rollback[i] = WRONG_FUNCTION_TYPE;
    }

    map->forward[bbInstruction_unsetString] = bbInstruction_setString;
    map->rollback[bbInstruction_setString] = bbInstruction_unsetString;

    map->forward[bbI_unsetString] = bbI_setString;
    map->rollback[bbI_setString] = bbI_unsetString;

    map->forward[bbI_undoNothing] = bbI_doNothing;
    map->rollback[bbI_doNothing] = bbI_undoNothing;

    map->forward[bbInstruction_uncheckActions] = bbInstruction_checkActions;
    map->rollback[bbInstruction_checkActions] = bbInstruction_uncheckActions;

    map->forward[bbInstruction_unsetTime] = bbInstruction_setTime;
    map->rollback[bbInstruction_setTime] = bbInstruction_unsetTime;

    map->forward[bbI_ECS_unspawnEmptyEntity] = bbI_ECS_spawnEmptyEntity;
    map->rollback[bbI_ECS_spawnEmptyEntity] = bbI_ECS_unspawnEmptyEntity;

    map->forward[bbI_ECS_entity_unsetComponent] = bbI_ECS_entity_setComponent;
    map->rollback[bbI_ECS_entity_setComponent] = bbI_ECS_entity_unsetComponent;

    map->forward[bbI_ECS_unsetServerEntity] = bbI_ECS_setServerEntity;
    map->rollback[bbI_ECS_setServerEntity] = bbI_ECS_unsetServerEntity;

    map->forward[bbI_ECS_moveable_unsetState] = bbI_ECS_moveable_setState;
    map->rollback[bbI_ECS_moveable_setState] = bbI_ECS_moveable_unsetState;

    map->forward[bbI_ECS_moveable_unsetDead] = bbI_ECS_moveable_setDead;
    map->rollback[bbI_ECS_moveable_setDead] = bbI_ECS_moveable_unsetDead;

    map->forward[bbI_ECS_entity_undeleteEntity] = bbI_ECS_entity_deleteEntity;
    map->rollback[bbI_ECS_entity_deleteEntity] = bbI_ECS_entity_undeleteEntity;

    map->forward[bbI_ECS_spatial_unspawn] = bbI_ECS_spatial_spawn;
    map->rollback[bbI_ECS_spatial_spawn] = bbI_ECS_spatial_unspawn;

    map->forward[bbI_unsetPlayerEntity] = bbI_setPlayerEntity;
    map->rollback[bbI_setPlayerEntity] = bbI_unsetPlayerEntity;

    map->forward[bbI_unsetTeam] = bbI_setTeam;
    map->rollback[bbI_setTeam] = bbI_unsetTeam;

    //map->forward[] = ;
    map->rollback[bbInstruction_testClick4] = NO_INVERSE_FUNCTION;

    map->forward[bbInstruction_unspawnGraphicsComponent] = bbInstruction_spawnGraphicsComponent;
    map->rollback[bbInstruction_spawnGraphicsComponent] = bbInstruction_unspawnGraphicsComponent;

    map->forward[bbInstruction_unspawnTestMoveable] = bbInstruction_spawnTestMoveable;
    map->rollback[bbInstruction_spawnTestMoveable] = bbInstruction_unspawnTestMoveable;

    map->forward[bbInstruction_unupdateMoveables] = bbInstruction_updateMoveables;
    map->rollback[bbInstruction_updateMoveables] = bbInstruction_unupdateMoveables;

    map->forward[bbI_unspawnAIComponent] = bbI_spawnAIComponent;
    map->rollback[bbI_spawnAIComponent] = bbI_unspawnAIComponent;


    map->forward[bbI_unsetPlayerEntity] = bbI_setPlayerEntity;
    map->rollback[bbI_setPlayerEntity] = bbI_unsetPlayerEntity;

    map->forward[bbI_unsetTeam] = bbI_setTeam;
    map->rollback[bbI_setTeam] = bbI_unsetTeam;


    //map->forward[] = ;
    map->rollback[bbInstruction_testClick4] = NO_INVERSE_FUNCTION;

    map->forward[bbInstruction_unspawnGraphicsComponent] = bbInstruction_spawnGraphicsComponent;
    map->rollback[bbInstruction_spawnGraphicsComponent] = bbInstruction_unspawnGraphicsComponent;


    map->forward[bbInstruction_unspawnTestMoveable] = bbInstruction_spawnTestMoveable;
    map->rollback[bbInstruction_spawnTestMoveable] = bbInstruction_unspawnTestMoveable;

    map->forward[bbInstruction_unupdateMoveables] = bbInstruction_updateMoveables;
    map->rollback[bbInstruction_updateMoveables] = bbInstruction_unupdateMoveables;


    map->forward[bbI_unspawnAIComponent] = bbI_spawnAIComponent;
    map->rollback[bbI_spawnAIComponent] = bbI_unspawnAIComponent;

    map->forward[bbI_unspawnAIComponent2] = bbI_spawnAIComponent2;
    map->rollback[bbI_spawnAIComponent2] = bbI_unspawnAIComponent2;

    map->forward[bbInstruction_unspawnServerEntity] = bbInstruction_spawnServerEntity;
    map->rollback[bbInstruction_spawnServerEntity] = bbInstruction_unspawnServerEntity;

    //map->forward[] = ;
    map->rollback[bbInstruction_netpauseButton] = NO_INVERSE_FUNCTION;

    //map->forward[] = ;
    map->rollback[bbInstruction_unfreezeButton] = NO_INVERSE_FUNCTION;

    map->forward[bbInstruction_unupdateAI] = bbInstruction_updateAI;
    map->rollback[bbInstruction_updateAI] = bbInstruction_unupdateAI;

    //map->forward[] = ;
    map->rollback[bbInstruction_testClick] = NO_INVERSE_FUNCTION;

    map->forward[bbI_live_unspawnEntity] = bbI_live_spawnEntity;
    map->rollback[bbI_live_spawnEntity] = bbI_live_unspawnEntity;

    map->forward[bbI_AI_unsetState] = bbI_AI_setState;
    map->rollback[bbI_AI_setState] = bbI_AI_unsetState;

    map->forward[bbI_unsetTextbox] = bbI_setTextbox;
    map->rollback[bbI_setTextbox] = bbI_unsetTextbox;

    map->forward[bbI_unputTextbox] = bbI_putTextbox;
    map->rollback[bbI_putTextbox] = bbI_unputTextbox;

    map->forward[bbInstruction_unrequestAction] = bbInstruction_requestAction;
    map->rollback[bbInstruction_requestAction] = bbInstruction_unrequestAction;

    map->forward[bbI_unspawnTeamComponent] = bbI_spawnTeamComponent;
    map->rollback[bbI_spawnTeamComponent] = bbI_unspawnTeamComponent;

    map->rollback[bbI_AI_sendGoalpoint] = NO_INVERSE_FUNCTION;
}



bbFlag bbCore_checkMap(bbInstructionMap* map, void* forward, void* rollback) {


    bbInstruction* forward_i = (bbInstruction*) forward;
    bbInstruction* rollback_i = (bbInstruction*) rollback;

    I32 forward_back = map->rollback[forward_i->type];
    I32 backward_forward = map->forward[rollback_i->type];

    bbAssert(forward_back == rollback_i->type, "instruction type mismatch");

    return bbSuccess;
}