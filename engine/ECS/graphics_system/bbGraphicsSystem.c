
#include "engine/ECS/graphics_system/bbGraphicsSystem.h"

#include "engine/core/bbInstruction_operations.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle);
bbFlag bbGraphicsSystem_deleteComponent_fn(struct bbSystem* system, bbHandle component_handle)
{
    bbGraphicsSystem* GraphicsSystem = (bbGraphicsSystem*)system;
    bbGraphicsComponent* component;
    bbGraphicsSystem_getComponent_fn(system, (bbComponent**)&component, component_handle);

    bbUI_Inbox_DeleteUnit(GraphicsSystem->inbox,component->component.entity_handle,no_handle);
    bbVPool_free(GraphicsSystem->system.pool,component);

    return bbSuccess;
}

bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS, bbUI_Inbox* inbox)
{
    bbVPool_newBloated(&graphics_system->system.pool, sizeof(bbGraphicsComponent), 1000, 10, "GRAPHICS SYSTEM");

    graphics_system->system.getComponent = bbGraphicsSystem_getComponent_fn;
    graphics_system->system.getHandle = bbGraphicsSystem_getHandle_fn;
    graphics_system->system.delete = bbGraphicsSystem_deleteComponent_fn;
    graphics_system->system.ECS = ECS;
    graphics_system->inbox = inbox;

    ECS->systems[bbECS_Graphics] = (bbSystem* )graphics_system;

    graphics_system->spawn_functions = calloc(193, sizeof(bbGraphics_spawnFunction*));
    graphics_system->spawn_function_count = 0;
    graphics_system->max_function_count = 193;

    bbDictionary_new(&graphics_system->spawn_dict,193);
    return bbSuccess;
}

bbFlag bbGraphics_spawnFunction_add(bbGraphicsSystem* graphics_system,
    bbGraphics_spawnFunction* function, char* key) {
    I32 index = graphics_system->spawn_function_count++;
    bbAssert(index < graphics_system->max_function_count, "out of allocated space\n");
    graphics_system->spawn_functions[index] = function;
    bbHandle function_handle; function_handle.u64 = index;
    bbDictionary_add(graphics_system->spawn_dict,key, function_handle);
    return bbSuccess;

}

bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool,(void**)component,component_handle);
}

bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool,(void*)component,component_handle);
}

bbFlag bbCI_spawnGraphicsComponent(  bbCore* core,
                                          char* type,
                                          bbGraphicsComponent_data* data,
                                          bbInstruction_source source,
                                          bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_spawnGraphicsComponent;

    bbECS* ECS = core->ECS;
    bbGraphicsSystem* system = (bbGraphicsSystem*)ECS->systems[bbECS_Graphics];

    bbHandle function_handle;
    bbDictionary_lookup(system->spawn_dict,type, &function_handle);

    instruction->snapshot = function_handle;
    instruction->data.graphics = *data;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbCS_spawnGraphicsComponent( bbCore* core,
                                          bbGraphicsComponent** this,
                                          char* type,
                                          bbGraphicsComponent_data* data,
                                          bbInstruction_source source,
                                          bbHandle action){

    bbECS* ECS = core->ECS;
    bbGraphicsSystem* system = (bbGraphicsSystem*)ECS->systems[bbECS_Graphics];
    bbGraphicsComponent* component;
    bbHandle component_handle;
    bbHandle function_handle;
    bbDictionary_lookup(system->spawn_dict,type, &function_handle);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_spawnGraphicsComponent;
        instruction->snapshot = function_handle;
        instruction->data.graphics = *data;
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = instruction->data.graphics.entity_handle;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = data->entity_handle;
        undo_instruction->source = source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = data->entity_handle;
        undo_instruction->source = source;
        undo_instruction->redo_instruction = action;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    bbVPool_alloc2(system->system.pool, (void**)&component, &component_handle);

    component->data = *data;

    bbCS_entity_setComponent(core,
                 ECS,
                 data->entity_handle,
                 component_handle,
                 bbECS_Graphics,
                 bbInstructionSource_internal,
                 no_handle);



    bbGraphics_spawnFunction* function = system->spawn_functions[function_handle.u64];
    function(system,*data);

    if (this != NULL) *this = component;

    return bbSuccess;
}

bbFlag bbI_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction) {



    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = instruction->data.graphics.entity_handle;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)


    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = instruction->data.graphics.entity_handle;
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
        undo_instruction->type = bbI_unspawnGraphicsComponent;
        undo_instruction->data.graphics.entity_handle = instruction->data.graphics.entity_handle;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);
        printf("collision = %d ", action->header.collision);
    } //else source == no rewind

    bbECS* ECS = core->ECS;
    bbGraphicsSystem* system = (bbGraphicsSystem*)ECS->systems[bbECS_Graphics];
    bbGraphicsComponent* component;
    bbHandle component_handle;

    bbVPool_alloc2(system->system.pool, (void**)&component, &component_handle);

    component->data = instruction->data.graphics;

    bbCS_entity_setComponent(core,
                 ECS,
                 instruction->data.graphics.entity_handle,
                 component_handle,
                 bbECS_Graphics,
                 bbInstructionSource_internal,
                 no_handle);

    bbGraphics_spawnFunction* function = system->spawn_functions[instruction->snapshot.u64];
    function(system,instruction->data.graphics);

    return bbSuccess;
}
bbFlag bbI_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction) {
    //bbHere()

    bbHandle component_handle;
    bbGraphicsComponent* component;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,instruction->data.graphics.entity_handle,
        bbECS_Graphics,&component_handle,(bbComponent**)&component);

    bbHandle_deleteComponent(core->ECS->systems[bbECS_Graphics], component_handle);

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