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

///Return bbSuccess if the filter passes. return bbContinue to continue to the next node.
bbFlag bbFilter_canAttack_fn(bbList* list, void* node, void* cl)
{
    //bbHere()

    bbFilter_canAttack_cl* filter_cl = cl;
    bbSpatial_Component* component = node;
    bbFlag flag;
    bbTeam *attacker;
    flag = bbHandle_mapComponent(filter_cl->ECS, bbECS_ECS,filter_cl->attacker_entity,bbECS_Teams,NULL,(bbComponent**)&attacker);
    if (flag == bbNone) {
        //bbHere()
        return bbContinue;
    }
    if (attacker == NULL) {
        //bbHere()
        return bbContinue;
    }

    bbTeam *target;
    flag = bbComponent_mapComponent(filter_cl->ECS, bbECS_Spatial,(bbComponent*)component,bbECS_Teams,NULL,(bbComponent**)&target);
    if (flag == bbNone) {
        //bbHere()
        return bbContinue;
    }
    if (target == NULL) {
        //bbFlag_print(flag)
        //bbHere()
        return bbContinue;
    }

    //bbDebug("attacker team = %d, target team = %d\n",attacker->team, target->team);
    if (attacker->team != target->team) {
       // bbDebug("WE FOUND ONE!\n")
        return(bbSuccess);
    }

    {
        //bbHere()
        return bbContinue;
    }

}
typedef struct
{
    bbECS* ECS;
    bbHandle nearest_entity;
    bbHandle attacker_entity;
    U32 nearest_distance;
} bbFilter_findNearest_cl;

///Find the nearest target that the entity can attack
bbFlag bbListFunction_findNearest_fn(bbList* list, void* node, void* cl)
{
    bbFilter_findNearest_cl* filter_cl = cl;
    bbSpatial_Component* component = node;

    //bbHere()

    if (bbHandleError_NULL == bbVPool_handleIsNULL(filter_cl->ECS->systems[bbECS_ECS]->pool,filter_cl->nearest_entity)){
         filter_cl->nearest_entity = component->component.entity_handle;

        // bbDebug("index = %d, system = %d, generation = %d\n",
        //     component->component.entity_handle.system.index,
        //     component->component.entity_handle.system.system,
        //     component->component.entity_handle.system.generation);

        bbMapCoords target_coords = component->map_coords;
        bbSpatial_Component* attacker_spatial;

        bbHandle_mapComponent(filter_cl->ECS,
                              bbECS_ECS,
                              filter_cl->attacker_entity,
                              bbECS_Spatial,
                              NULL,
                              (bbComponent**)&attacker_spatial);

        bbMapCoords attacker_coords = attacker_spatial->map_coords;

        I64 delta_i = attacker_coords.i - target_coords.j;
        I64 delta_j = attacker_coords.j - target_coords.j;
        I64 dist_squared = delta_i * delta_i + delta_j * delta_j;
        I64 nearest_distance = filter_cl->nearest_distance;

        filter_cl->nearest_distance = bbArith64_sqrt2(dist_squared);

        return bbContinue;
    }
    bbMapCoords target_coords = component->map_coords;
    bbSpatial_Component* attacker_spatial;

    bbHandle_mapComponent(filter_cl->ECS,
                          bbECS_ECS,
                          filter_cl->attacker_entity,
                          bbECS_Spatial,
                          NULL,
                          (bbComponent**)&attacker_spatial);

    bbMapCoords attacker_coords = attacker_spatial->map_coords;

    I64 delta_i = attacker_coords.i - target_coords.j;
    I64 delta_j = attacker_coords.j - target_coords.j;
    I64 dist_squared = delta_i * delta_i + delta_j * delta_j;
    I64 nearest_distance = filter_cl->nearest_distance;

    I64 new_distance = bbArith64_sqrt2(dist_squared);
        //bbDebug("new distance = %ld\n", new_distance);

    if (dist_squared < nearest_distance*nearest_distance)
    {
        filter_cl->nearest_distance = new_distance;
        filter_cl->nearest_entity = component->component.entity_handle;
        return bbContinue;
        //bbDebug("new nearest index = %d\n",component->component.entity_handle.system.index)
    }
    if (dist_squared > nearest_distance*nearest_distance)return bbContinue;

    if (delta_i < 0)
    {
        filter_cl->nearest_distance = new_distance;
        filter_cl->nearest_entity = component->component.entity_handle;
        return bbContinue;
    }
    if (delta_i > 0) return bbContinue;

    if (delta_j < 0)
    {
        filter_cl->nearest_distance = new_distance;
        filter_cl->nearest_entity = component->component.entity_handle;
        return bbContinue;
    }
    if (delta_j > 0) return bbContinue;

    bbNotImplemented()





    return bbContinue;
}



bbFlag bbTeams_findNearestTarget(bbCore* core, bbECS* ECS, bbHandle attacker_entity , bbHandle* target_entity, U32 max_distance)
{

    // typedef struct
    // {
    //     bbECS* ECS;
    //     bbHandle nearest_entity;
    //     bbHandle attacker_entity;
    //     U32 nearest_distance;
    // } bbFilter_findNearest_cl;

    bbFilter_findNearest_cl nearest_cl;

    nearest_cl.ECS = ECS;
    nearest_cl.nearest_entity = ECS->system.pool->null;
    nearest_cl.attacker_entity = attacker_entity;
    nearest_cl.nearest_distance = max_distance+193;

    // typedef struct
    // {
    //     bbECS* ECS;
    //     bbHandle attacker_entity;
    // } bbFilter_canAttack_cl;

    bbFilter_canAttack_cl can_attack_cl;
    can_attack_cl.attacker_entity = attacker_entity;
    can_attack_cl.ECS = ECS;

    // typedef struct
    // {
    //     bbMapCoords coords;
    //     U32 radius;
    //     bbListFunction* function;
    //     void* cl;
    //
    //     bbFilterFunction* filters[MAX_FILTER_FUNCTIONS];
    //     void* cls[MAX_FILTER_FUNCTIONS];
    //
    // }bbSpatialFilters;

    bbSpatial_Component* attacker_spatial;
    bbHandle_mapComponent(ECS,bbECS_ECS,attacker_entity,bbECS_Spatial,NULL,(bbComponent**)&attacker_spatial);


    bbSpatialFilters filters_cl;
    filters_cl.coords = attacker_spatial->map_coords;


    filters_cl.radius = max_distance;

    filters_cl.function = bbListFunction_findNearest_fn;
    filters_cl.cl = &nearest_cl;

    for (I32 i=0; i<MAX_FILTER_FUNCTIONS; i++)
    {
        filters_cl.filters[i] = NULL;
        filters_cl.cls[i] = NULL;
    }

    filters_cl.filters[0] = bbFilter_canAttack_fn;
    filters_cl.cls[0] = &can_attack_cl;

    //bbDebug("radius = %llu\n",filters_cl.radius);
    bbSpatial_mapRadiusFilter((bbSpatial*)ECS->systems[bbECS_Spatial],&filters_cl);

    //bbDebug("nearest distance = %u\n", nearest_cl.nearest_distance);

    if (nearest_cl.nearest_entity.u64 == ECS->system.pool->null.u64) {
        //bbHere()
        return bbFail;
    }
    if (nearest_cl.nearest_distance == max_distance+193) {
        //bbHere()
        return bbFail;
    }
    if (nearest_cl.nearest_entity.system.system != bbECS_ECS) {
        //bbHere()
        return bbFail;
    }

    //bbDebug("distance = %d, max_distance = %d\n",nearest_cl.nearest_distance,max_distance);
    *target_entity = nearest_cl.nearest_entity;
    return bbSuccess;
}