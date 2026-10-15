#include "engine/ECS/projectiles/bbProjectileMovement.h"

#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbIterator.h"
#include "engine/logic/bbSystemPool.h"

bbFlag bbProj_getComponent_fn(struct bbSystem *system, bbComponent **component, bbHandle component_handle);

bbFlag bbProj_getHandle_fn(struct bbSystem *system, bbComponent *component, bbHandle *component_handle);

bbFlag bbProj_deleteComponent_fn(struct bbSystem *system, bbHandle component_handle);

bbFlag bbProj_getComponent_fn(struct bbSystem *system, bbComponent **component, bbHandle component_handle) {
    return bbVPool_lookup(system->pool, (void **) component, component_handle);
}

bbFlag bbProj_getHandle_fn(struct bbSystem *system, bbComponent *component, bbHandle *component_handle) {
    return bbVPool_reverseLookup(system->pool, (void *) component, component_handle);
}


bbFlag bbProjectileMovement_init(bbProjectileMovement *system, bbECS *ECS, bbTime current_time)
{
    bbVPool_newSystem(&system->system.pool, bbECS_Projectiles, sizeof(bbProjectileMovement_component), 10, 1000, "ENTITY AI");
    bbList_init(&system->list, system->system.pool,NULL,offsetof(bbProjectileMovement_component, list),NULL, bbECS_Projectiles);

    system->system.getComponent = bbProj_getComponent_fn;
    system->system.getHandle = bbProj_getHandle_fn;
    system->system.delete = bbProj_deleteComponent_fn;

    system->last_update = current_time;
    system->system.ECS = ECS;
    ECS->systems[bbECS_Projectiles] = (bbSystem *) system;
    return bbSuccess;
}

typedef struct
{
    bbTime time;
}interpolate_projectile_cl;

static bbFlag interpolate_projectile_fn(bbList* list, void* node, void* cl) {

    bbProjectileMovement_component* component = node;
    interpolate_projectile_cl* closure = cl;
    bbMapCoords start_coords = component->data.start_coords;
    bbMapCoords end_coords = component->data.end_coords;
    I64 gravity = component->data.gravity;
    bbTime start_time = component->data.start_time;
    bbTime end_time = component->data.end_time;
    bbTime current_time = closure->time;

    if (current_time < start_time) {
        component->data.current_coords = start_coords;
        return bbContinue;
    }
    if (current_time > end_time) {
        component->data.current_coords = end_coords;
        return bbContinue;
    }

    //We dont have to worry if start_coords == end_coords
    bbMapCoords new_coords = bbMapCoords_interpolateGravity(
                              start_coords,
                              end_coords ,
                              gravity,
                              start_time,
                              current_time,
                              end_time);

    component->data.current_coords = new_coords;

    return bbContinue;
}
bbFlag bbProjectileMovement_interpolate(bbProjectileMovement* system, bbECS* ECS, bbTime current_time ) {

    bbIterator iterator = bbIterator_new(&system->list);
    interpolate_projectile_cl closure;
    closure.time = current_time;
    bbIterator_mapL(&iterator,interpolate_projectile_fn,&closure);
    system->last_update = current_time;
    return bbSuccess;
}

bbFlag bbProj_deleteComponent_fn(struct bbSystem *system, bbHandle component_handle) {
    bbProjectileMovement *proj_system = (bbProjectileMovement *) system;
    bbProjectileMovement_component *component;
    bbProj_getComponent_fn(system, (bbComponent **) &component, component_handle);

    bbList_remove(&proj_system->list, component);
    bbVPool_free(proj_system->system.pool, component);

    return bbSuccess;
}

bbFlag bbCI_updateProjectileMovement(bbCore *core, bbTime time,
                              bbInstruction_source source, bbHandle action) {
    allocActiveInstruction(instruction)

    instruction->type = bbI_updateProjectileMovement;
    instruction->data.three_handles.handle1.i64 = time;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbI_updateProjectileMovement_fn(bbCore *core, bbInstruction *instruction) {
    bbProjectileMovement *system = (bbProjectileMovement *) core->ECS->systems[bbECS_Projectiles];
    if (instruction->source == bbInstructionSource_internal) {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unupdateProjectileMovement;

        undo_instruction->data.three_handles.handle1.i64 = system->last_update;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    } else if (instruction->source == bbInstructionSource_input) {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unupdateProjectileMovement;
        undo_instruction->data.three_handles.handle1.i64 = system->last_update;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
        *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle) redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    } else if (instruction->source == bbInstructionSource_action) {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unupdateProjectileMovement;
        undo_instruction->data.three_handles.handle1.i64 = system->last_update;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle) instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    } //else source == no rewind

    bbProjectileMovement_interpolate(system, core->ECS, instruction->data.three_handles.handle1.i64 );
    system->last_update = instruction->data.three_handles.handle1.i64;
    return bbSuccess;
}

bbFlag bbI_unupdateProjectileMovement_fn(bbCore *core, bbInstruction *instruction) {



    bbProjectileMovement *system = (bbProjectileMovement *) core->ECS->systems[bbECS_Projectiles];
    bbProjectileMovement_interpolate(system, core->ECS, instruction->data.three_handles.handle1.i64 );
    system->last_update = instruction->data.three_handles.handle1.i64;

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

bbFlag bbCI_spawnProjectileMovement(bbCore *core,
                            bbHandle entity,
                            bbProjectileMovement_data *data,
                            bbInstruction_source source,
                            bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_spawnProjectileMovement;
    instruction->data.projectile = *data;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
}

bbFlag bbCS_spawnProjectileMovement(bbCore *core,
                            bbHandle entity,
                            bbProjectileMovement **new_projectile,
                            bbProjectileMovement_data *data,
                            bbInstruction_source source,
                            bbHandle action) {
    bbNotImplemented()
}


bbFlag bbI_spawnProjectileMovement_fn(bbCore *core, bbInstruction *instruction) {
    bbNotImplemented() //rollback

    bbProjectileMovement *system = (bbProjectileMovement *) core->ECS->systems[bbECS_Projectiles];
    bbProjectileMovement_component* component;
    bbHandle component_handle;
    bbList_alloc2(&system->list,(void**)&component, &component_handle);

    component->data = instruction->data.projectile;

    bbCS_entity_setComponent(core,
                         home.ECS.ECS,
                         component->component.entity_handle,
                         component_handle,
                         bbECS_Projectiles,
                         bbInstructionSource_internal,
                         no_handle);

    bbList_pushL(&home.ECS.projectiles.list,component);


}

bbFlag bbI_unspawnProjectileMovement_fn(bbCore *core, bbInstruction *instruction) {
    bbNotImplemented()
}
