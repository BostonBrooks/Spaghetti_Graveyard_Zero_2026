#ifndef  INSTRUCTIONS_H
#define  INSTRUCTIONS_H
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS_instructions.h"


typedef enum
{
    bbInstruction_testClick3 = bbInstruction_numECS_Instructions,
    bbInstruction_spawnGraphicsComponent,
    bbInstruction_unspawnGraphicsComponent,
    bbInstruction_spawnTestMoveable,
    bbInstruction_unspawnTestMoveable,
    bbInstruction_updateMoveables,
    bbInstruction_unupdateMoveables,
    bbI_spawnAIComponent,
    bbI_unspawnAIComponent,
    bbI_spawnAIComponent2,
    bbI_unspawnAIComponent2,
    bbInstruction_spawnServerEntity,
    bbInstruction_unspawnServerEntity,
    bbInstruction_netpauseButton,
    bbInstruction_unfreezeButton,
    bbInstruction_updateAI,
    bbInstruction_unupdateAI,
    bbInstruction_testClick,
    bbI_live_spawnEntity,
    bbI_live_unspawnEntity,
    bbVInstruction_numTypes
}bbVInstruction_type;

bbFlag bbCoreInput_spawnServerEntity(bbCore* core,
                                    char* key,
                                    bbHandle server_entity,
                                    bbInstruction_source source,
                                    bbHandle action);

bbFlag bbInstruction_unspawnServerEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_spawnServerEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_testClick_fn(bbCore* core, bbInstruction* instruction);
/*
bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateMovables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMovables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_setGoalMovable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalMovable_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateMovables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMovables_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateAgentsSquare_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgentsSquare_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateAgentSquare_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgentSquare_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateAgents_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgents_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_commandAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_spawnUnitOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_spawnUnitIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unspawnUnit_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_damageAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_undamageAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_spawnAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unspawnAgent_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_commandAgentMapClick_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_deleteEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_undeleteEntity_fn(bbCore* core, bbInstruction* instruction);
*/
#endif// INSTRUCTIONS_H