#include "engine/ECS/teams/bbTeams.h"

#include "engine/core/bbInstruction_operations.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/spatial/bbSpatial.h"
#include "engine/ECS/spatial/bbSpatial_query.h"
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
    return bbSuccess;
}

bbFlag bbI_spawnTeamComponent_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnTeamComponent;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)


    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnTeamComponent;
        undo_instruction->source = instruction->source;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        allocRedoInstruction(redo_instruction)
        *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnTeamComponent;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    }
        bbECS* ECS = core->ECS;
        bbTeams* teams = (bbTeams*)ECS->systems[bbECS_Teams];
        bbTeam* component;
        bbHandle component_handle;

        bbVPool_alloc2(teams->system.pool, (void**)&component, &component_handle);

        component->component.entity_handle =  instruction->data.three_handles.handle1;
        component->team = instruction->data.three_handles.handle2.u64;

        bbCS_entity_setComponent(core,
                         ECS,
                          instruction->data.three_handles.handle1,
                         component_handle,
                         bbECS_Teams,
                         bbInstructionSource_internal,
                         no_handle);

        return bbSuccess;
}
bbFlag bbI_unspawnTeamComponent_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotImplemented()

        if (instruction->source == bbInstructionSource_internal)
        {
            //bbVPool_free(core->instruction_pool, (void*)instruction);
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_input)
        {

            popRedoInstruction(redo_instruction,instruction)
            allocActiveInstruction(new_instruction)
            *new_instruction = redo_instruction;
            bbCore_checkMap(&core->map, &redo_instruction, instruction);

            pushActiveInstruction(new_instruction)
            //bbInstruction* redo_instruction;
            //bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
            //bbList_pushL(&core->active_stack, redo_instruction);
            //bbVPool_free(core->instruction_pool, (void*)instruction);
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_action)
        {
            bbAction* redo_action;

            bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
            bbList_sortL(&core->action_queue,(void*)redo_action);
            //bbVPool_free(core->instruction_pool, (void*)instruction);
            return bbSuccess;
        }
}



bbFlag bbCI_spawnTeamComponent(bbCore* core,
                             bbHandle entity_handle,
                             I32 team,
                             bbInstruction_source source,
                             bbHandle action)
{
        allocActiveInstruction(instruction)
    instruction->type = bbI_spawnTeamComponent;
        instruction->source = source;
        instruction->redo_instruction = action;
        instruction->data.three_handles.handle1 = entity_handle;
        instruction->data.three_handles.handle2.u64 = team;
        pushActiveInstruction(instruction)
        return bbSuccess;
}


bbFlag bbCS_spawnTeamComponent(bbCore* core,
                             bbHandle entity_handle,
                             bbTeam** this,
                             I32 team,
                             bbInstruction_source source,
                             bbHandle action)
{

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_spawnTeamComponent;
        instruction->data.three_handles.handle1 = entity_handle;
        instruction->data.three_handles.handle2.u64 = team;
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->type = bbI_unspawnTeamComponent;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->type = bbI_unspawnTeamComponent;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->type = bbI_unspawnTeamComponent;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }
    bbECS* ECS = core->ECS;
    bbTeams* teams = (bbTeams*)ECS->systems[bbECS_Teams];
    bbTeam* component;
    bbHandle component_handle;

    bbVPool_alloc2(teams->system.pool, (void**)&component, &component_handle);

    component->component.entity_handle = entity_handle;
    component->team = team;

    bbCS_entity_setComponent(core,
                     ECS,
                     entity_handle,
                     component_handle,
                     bbECS_Teams,
                     bbInstructionSource_internal,
                     no_handle);


    if (this != NULL) *this = component;

    return bbSuccess;
}
