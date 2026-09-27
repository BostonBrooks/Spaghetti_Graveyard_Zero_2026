

#include <stdlib.h>

#include "action_request.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "games/game0/maps/systems_test/core/instructions.h"
#include "games/game0/maps/systems_test/core/textboxes.h"
#include "AI_system/ai_instructions.h"
#include "AI_system/player_send_goalpoint.h"
#include "games/game0/maps/systems_test/core/player_goalpoint.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/ECS/graphics_system/bbGraphicsSystem.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
#include "engine/ECS/players/bbPlayers.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/ECS/teams/bbTeams.h"
#include "entity_spawner/live_spawn.h"
#include "moveables/moveables.h"

bbFlag bbCore_initVInstructions(bbCore* core)
{
    I32 max_instructions = bbVInstruction_numTypes - bbInstruction_numTypes;//Careful no tomake this too small;
    core->instruction_functions = calloc( max_instructions, sizeof(bbInstruction_fn*));
    core->instruction_functions[bbI_ECS_spawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_spawnEmptyEntity_fn;
    core->instruction_functions[bbI_ECS_unspawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_unspawnEmptyEntity_fn;
    core->instruction_functions[bbI_ECS_entity_setComponent-bbInstruction_numTypes] = bbInstruction_entity_setComponent_fn;
    core->instruction_functions[bbI_ECS_entity_unsetComponent-bbInstruction_numTypes] = bbInstruction_entity_unsetComponent_fn;
    core->instruction_functions[bbI_ECS_setServerEntity-bbInstruction_numTypes] = bbInstruction_setServerEntity_fn;
    core->instruction_functions[bbI_ECS_unsetServerEntity-bbInstruction_numTypes] = bbInstruction_unsetServerEntity_fn;
    core->instruction_functions[bbInstruction_testClick3-bbInstruction_numTypes] = bbInstruction_testClick3_fn;
    core->instruction_functions[bbInstruction_testClick4-bbInstruction_numTypes] = bbInstruction_testClick4_fn;
    core->instruction_functions[bbI_ECS_moveable_setState-bbInstruction_numTypes] = bbI_Moveable_setState_fn;
    core->instruction_functions[bbI_ECS_moveable_unsetState-bbInstruction_numTypes] = bbI_Moveable_unsetState_fn;
    core->instruction_functions[bbI_ECS_moveable_setDead-bbInstruction_numTypes] = bbI_Moveable_setDead_fn;
    core->instruction_functions[bbI_ECS_moveable_unsetDead-bbInstruction_numTypes] = bbI_Moveable_unsetDead_fn;
    core->instruction_functions[bbI_ECS_entity_deleteEntity-bbInstruction_numTypes] = bbInstruction_entity_deleteEntity_fn;
    core->instruction_functions[bbI_ECS_entity_undeleteEntity-bbInstruction_numTypes] = bbInstruction_entity_undeleteEntity_fn;
    core->instruction_functions[bbI_spawnTeamComponent-bbInstruction_numTypes] = bbI_spawnTeamComponent_fn;
    core->instruction_functions[bbI_unspawnTeamComponent-bbInstruction_numTypes] = bbI_unspawnTeamComponent_fn;

    core->instruction_functions[bbInstruction_spawnServerEntity-bbInstruction_numTypes] = bbInstruction_spawnServerEntity_fn;
    core->instruction_functions[bbInstruction_unspawnServerEntity-bbInstruction_numTypes] = bbInstruction_unspawnServerEntity_fn;
    core->instruction_functions[bbInstruction_netpauseButton-bbInstruction_numTypes] = bbInstruction_netpauseButton_fn;
    core->instruction_functions[bbInstruction_unfreezeButton-bbInstruction_numTypes] = bbInstruction_unfreezeButton_fn;
    core->instruction_functions[bbInstruction_spawnGraphicsComponent-bbInstruction_numTypes] = bbInstruction_spawnDrawable_fn;
    core->instruction_functions[bbInstruction_unspawnGraphicsComponent-bbInstruction_numTypes] = bbInstruction_unspawnDrawable_fn;
    core->instruction_functions[bbInstruction_updateMoveables-bbInstruction_numTypes] = bbInstruction_updateMoveables_fn;
    core->instruction_functions[bbInstruction_unupdateMoveables-bbInstruction_numTypes] = bbInstruction_unupdateMoveables_fn;
    core->instruction_functions[bbInstruction_spawnTestMoveable-bbInstruction_numTypes] = bbInstruction_spawnTestMoveable_fn;
    core->instruction_functions[bbInstruction_unspawnTestMoveable-bbInstruction_numTypes] = bbInstruction_unspawnTestMoveable_fn;
    core->instruction_functions[bbInstruction_updateAI-bbInstruction_numTypes] = bbI_updateAI_fn;
    core->instruction_functions[bbInstruction_unupdateAI-bbInstruction_numTypes] = bbI_unupdateAI_fn;
    core->instruction_functions[bbI_spawnAIComponent2-bbInstruction_numTypes] = bbI_spawnAIComponent2_fn;
    core->instruction_functions[bbI_unspawnAIComponent2-bbInstruction_numTypes] = bbI_unspawnAIComponent2_fn;
    core->instruction_functions[bbInstruction_testClick-bbInstruction_numTypes] = bbInstruction_testClick_fn;
    core->instruction_functions[bbI_live_spawnEntity-bbInstruction_numTypes] = bbI_live_spawnEntity_fn;
    core->instruction_functions[bbI_live_unspawnEntity-bbInstruction_numTypes] = bbI_live_unspawnEntity_fn;
    core->instruction_functions[bbI_AI_setState-bbInstruction_numTypes] = bbI_AI_setState_fn;
    core->instruction_functions[bbI_AI_unsetState-bbInstruction_numTypes] = bbI_AI_unsetState_fn;
    core->instruction_functions[bbI_setTextbox-bbInstruction_numTypes] = bbI_setTextbox_fn;
    core->instruction_functions[bbI_unsetTextbox-bbInstruction_numTypes] = bbI_unsetTextbox_fn;
    core->instruction_functions[bbI_putTextbox-bbInstruction_numTypes] = bbI_putTextbox_fn;
    core->instruction_functions[bbI_unputTextbox-bbInstruction_numTypes] = bbI_unputTextbox_fn;
    core->instruction_functions[bbInstruction_requestAction-bbInstruction_numTypes] = bbInstruction_requestAction_fn;
    core->instruction_functions[bbInstruction_unrequestAction-bbInstruction_numTypes] = bbInstruction_unrequestAction_fn;
    core->instruction_functions[bbI_setPlayerEntity-bbInstruction_numTypes] = bbI_setPlayerEntity_fn;
    core->instruction_functions[bbI_unsetPlayerEntity-bbInstruction_numTypes] = bbI_unsetPlayerEntity_fn;
    core->instruction_functions[bbI_AI_sendGoalpoint-bbInstruction_numTypes] = bbInstruction_sendAIGoalpoint_fn;
    core->instruction_functions[bbI_spawnGraphicsComponent-bbInstruction_numTypes] = bbI_spawnGraphicsComponent_fn;
    core->instruction_functions[bbI_unspawnGraphicsComponent-bbInstruction_numTypes] = bbI_unspawnGraphicsComponent_fn;



       return bbSuccess;
}