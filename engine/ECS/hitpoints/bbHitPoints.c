#include "engine/ECS/hitpoints/bbHitPoints.h"

#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbIterator.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbHitPoints_init(bbHitPoints* system,bbECS* ECS) {
    bbVPool_newSystem(&system->system.pool,bbECS_Hitpoints,sizeof(bbHitPoint),10,1000,"HITPOINTS");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbHitPoint,list),NULL,bbECS_Hitpoints);

    system->system.getComponent = bbHitPoints_getComponent;
    system->system.getHandle = bbHitPoints_getHandle;
    system->system.delete = bbHitPoints_delete;

    system->system.ECS = ECS;
    ECS->systems[bbECS_Hitpoints] = (bbSystem*)system;
    return bbSuccess;
}
bbFlag bbHitPoints_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbHitPoints_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}
bbFlag bbHitPoints_delete(struct bbSystem* system, bbHandle component_handle)
{
    bbHitPoints* hitpoints = (bbHitPoints*)system;
    bbHitPoint* component;
    bbHitPoints_getComponent(system, (bbComponent**)&component, component_handle);
    bbList_remove(&hitpoints->list,component);
    bbVPool_free(hitpoints->system.pool,component);

    return bbSuccess;
}


bbFlag update_hitpoints_fn(bbList* list, void* node, void* cl) {

    bbHitPoint* hitpoint = (bbHitPoint*)node;

    if (hitpoint->prev_health != hitpoint->current_health) {
        float new_HP = (float)hitpoint->current_health / (float)hitpoint->max_health;
        bbUI_Inbox_SetUnitHP(&home.UI.inbox,hitpoint->component.entity_handle,new_HP);
        //bbDebug("new_HP = %f\n", new_HP);
    }
    hitpoint->prev_health = hitpoint->current_health;
    return bbContinue;
}

bbFlag bbCS_Hitpoints_update(bbCore* core,
                             bbECS* ECS,
                             bbInstruction_source source,
                             bbHandle action) {
    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_Hitpoints_update;

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_Hitpoints_unupdate;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_Hitpoints_unupdate;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_Hitpoints_unupdate;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //do side effects

    bbHitPoints* hitpoints = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbIterator iterator = bbIterator_new(&hitpoints->list);
    bbIterator_mapL(&iterator, update_hitpoints_fn,NULL);

}

bbFlag bbCI_Hitpoints_update(bbCore* core,
                             bbECS* ECS,
                             bbInstruction_source source,
                             bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_Hitpoints_update;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;
}


bbFlag bbI_Hitpoints_update_fn(bbCore* core, bbInstruction* instruction) {

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_Hitpoints_unupdate;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)


    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_Hitpoints_unupdate;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
        *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_Hitpoints_unupdate;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);
        printf("collision = %d ", action->header.collision);
    } //else source == no rewind


    bbHitPoints* hitpoints = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbIterator iterator = bbIterator_new(&hitpoints->list);
    bbIterator_mapL(&iterator, update_hitpoints_fn,NULL);
}
bbFlag bbI_Hitpoints_unupdate_fn(bbCore* core, bbInstruction* instruction) {


    bbHitPoints* hitpoints = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbIterator iterator = bbIterator_new(&hitpoints->list);
    bbIterator_mapL(&iterator, update_hitpoints_fn,NULL);



    if (instruction->source == bbInstructionSource_internal)
    {
        //bbHere()
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        popRedoInstruction(redo_instruction,instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;
        bbCore_checkMap(&core->map, &redo_instruction, instruction);
        pushActiveInstruction(new_instruction)
//bbHere()
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);




        //bbHere()
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");

}

bbFlag bbCS_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbHitPoint** hitpoint,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)

        //set input instruction data
        instruction->type = bbI_Hitpoints_spawn;
        instruction->data.three_handles.handle1 = entity;
        instruction->data.three_handles.handle2.i32x2.x = max_hitpoints;
        instruction->source = source;
        instruction->redo_instruction = action;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = entity;
        pushRedoInstruction(instruction)
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction.u64 = 0;
        //set instruction data
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = entity;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = entity;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }


    bbHitPoints* hitpoints = (bbHitPoints*)ECS->systems[bbECS_Hitpoints];
    bbHitPoint* component;
    bbHandle component_handle;
    bbVPool_alloc2(hitpoints->system.pool,(void**)&component,&component_handle);

    component->max_health = max_hitpoints;
    component->current_health = max_hitpoints;
    component->prev_health = max_hitpoints;
    component->component.entity_handle = entity;

    bbCS_entity_setComponent(core,
                         ECS,
                         entity,
                         component_handle,
                         bbECS_Hitpoints,
                         bbInstructionSource_internal,
                         no_handle);

    bbList_pushL(&hitpoints->list,component);

    if (hitpoint != NULL) *hitpoint = component;


    return bbSuccess;
}

bbFlag bbCI_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_Hitpoints_spawn;
    instruction->data.three_handles.handle1 = entity;
    instruction->data.three_handles.handle2.i32x2.x = max_hitpoints;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
}

bbFlag bbI_Hitpoints_spawn_fn(bbCore* core, bbInstruction* instruction) {
    if (instruction->source == bbInstructionSource_internal)
    {

        allocUndoInstruction(undo_instruction)
        undo_instruction->type =
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_unspawn;
        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    } //else source == no rewind

    I32 max_hitpoints = instruction->data.three_handles.handle2.i32x2.x;
    bbECS* ECS = core->ECS;
    bbHandle entity = instruction->data.three_handles.handle1;
    bbHitPoints* hitpoints = (bbHitPoints*)ECS->systems[bbECS_Hitpoints];
    bbHitPoint* component;
    bbHandle component_handle;
    bbVPool_alloc2(hitpoints->system.pool,(void**)&component,&component_handle);

    component->max_health = max_hitpoints;
    component->current_health = max_hitpoints;
    component->prev_health = max_hitpoints;
    component->component.entity_handle = entity;

    bbCS_entity_setComponent(core,
                         ECS,
                         entity,
                         component_handle,
                         bbECS_Hitpoints,
                         bbInstructionSource_internal,
                         no_handle);

    bbList_pushL(&hitpoints->list,component);
}
bbFlag bbI_Hitpoints_unspawn_fn(bbCore* core, bbInstruction* instruction) {
    {
        bbHandle entity_handle = instruction->data.three_handles.handle1;
        bbHandle component_handle;
        bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Hitpoints,&component_handle,NULL);
        bbHandle_deleteComponent(core->ECS->systems[bbECS_Hitpoints], component_handle);

        if (instruction->source == bbInstructionSource_internal)
        {
            //bbHere()
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_input)
        {
            popRedoInstruction(redo_instruction,instruction)
            allocActiveInstruction(new_instruction)
            *new_instruction = redo_instruction;
            bbCore_checkMap(&core->map, &redo_instruction, instruction);
            pushActiveInstruction(new_instruction)
    //bbHere()
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_action)
        {
            bbAction* redo_action;

            bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
            bbList_sortL(&core->action_queue,(void*)redo_action);

            //bbHere()
            return bbSuccess;
        }
        bbAssert(0==1, "We should not get here\n");
    }
}

bbFlag bbCS_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_Hitpoints_damage;

        instruction->data.damage_agent.agent = entity;
        instruction->data.damage_agent.hitpoints = hitpoints;
        instruction->data.damage_agent.damage_type = damageType; //TODO damage calculation must be reversible
        pushRedoInstruction(instruction)
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;

        undo_instruction->data.damage_agent.agent = entity;
        undo_instruction->data.damage_agent.hitpoints = hitpoints;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction

        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;

        undo_instruction->data.damage_agent.agent = entity;
        undo_instruction->data.damage_agent.hitpoints = hitpoints;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->source = source;

        //set instruction data
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;

        undo_instruction->data.damage_agent.agent = entity;
        undo_instruction->data.damage_agent.hitpoints = hitpoints;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }


    bbHitPoints* hit_points = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbHandle entity_handle = entity;
    I32 damage = hitpoints;

    bbHitPoint* hitpoint;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Hitpoints,NULL,(bbComponent**)&hitpoint);

    bbWarning(hitpoint!=NULL,"Damaging an entity with no hitpoint compnent\n");
    if (hitpoint != NULL) {
        hitpoint->current_health -= damage;
    }

}

bbFlag bbCI_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_Hitpoints_damage;

    instruction->data.damage_agent.agent = entity;
    instruction->data.damage_agent.hitpoints = hitpoints;
    instruction->data.damage_agent.damage_type = damageType;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbI_Hitpoints_damage_fn(bbCore* core, bbInstruction* instruction) {


    bbHitPoints* hit_points = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbHandle entity_handle = instruction->data.damage_agent.agent;
    I32 damage = instruction->data.damage_agent.hitpoints;
    I32 damageType = instruction->data.damage_agent.damage_type;

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;
        undo_instruction->data.damage_agent.agent = entity_handle;
        undo_instruction->data.damage_agent.hitpoints = damage;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;
        undo_instruction->data.damage_agent.agent = entity_handle;
        undo_instruction->data.damage_agent.hitpoints = damage;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_Hitpoints_undamage;
        undo_instruction->data.damage_agent.agent = entity_handle;
        undo_instruction->data.damage_agent.hitpoints = damage;
        undo_instruction->data.damage_agent.damage_type = damageType;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

    } //else source == no rewind


    bbHitPoint* hitpoint;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Hitpoints,NULL,(bbComponent**)&hitpoint);

    bbWarning(hitpoint!=NULL,"Damaging an entity with no hitpoint compnent\n");
    if (hitpoint != NULL) {
        hitpoint->current_health -= damage;
    }

    return bbSuccess;
}
bbFlag bbI_Hitpoints_undamage_fn(bbCore* core, bbInstruction* instruction) {
    bbHitPoints* hit_points = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbHandle entity_handle = instruction->data.damage_agent.agent;
    I32 damage = instruction->data.damage_agent.hitpoints;
    I32 damageType = instruction->data.damage_agent.damage_type;
    bbHitPoint* hitpoint;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Hitpoints,NULL,(bbComponent**)&hitpoint);

    bbWarning(hitpoint!=NULL,"Damaging an entity with no hitpoint compnent\n");
    if (hitpoint != NULL) {
        hitpoint->current_health += damage;
    }
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
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        return bbSuccess;
    }

}