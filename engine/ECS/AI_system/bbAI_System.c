

#include "bbAI_System.h"

#include "core/core_inputs.h"
#include "core/instructions.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbPrime.h"


bbFlag bbAI_System_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbAI_System_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

bbFlag bbAI_System_init(bbAI_System* system,bbECS* ECS)
{
    bbVPool_newBloated(&system->system.pool, sizeof(bbAI_Component),10,1000,"ENTITY AI");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbAI_Component,list_element),NULL);

    system->system.getComponent = bbAI_System_getComponent_fn;
    system->system.getHandle = bbAI_System_getHandle_fn;

    bbAI_Functions_init(&system->functions);
    ECS->systems[bbECS_AI] = (bbSystem*)system;
    return bbSuccess;
}

bbFlag bbAI_Functions_init(bbAI_Functions* functions)
{
    I32 magic_number = nextPrime(ai_update_function_count);

    functions->updates = calloc(ai_update_function_count,sizeof(bbAI_Update*));
    bbDictionary_new(&functions->update_dict,magic_number);
    functions->updates_available = 0;

    magic_number = nextPrime(ai_command_function_count);

    functions->commands = calloc(ai_command_function_count,sizeof(bbAI_Command*));
    bbDictionary_new(&functions->command_dict,magic_number);
    functions->commands_available = 0;

    return bbSuccess;
}

bbFlag bbAI_Functions_add(bbAI_Functions* functions,
                              bbAI_FunctionType function_type,
                              void* function_pointer,
                              char* key )
{
    U32 available;
    bbHandle handle;
    switch (function_type)
    {
    case AI_Update:
        available = functions->updates_available++;
        bbAssert(available < ai_update_function_count, "out of bounds error\n");

        functions->updates[available] = function_pointer;
        handle.u64 = available;

        bbDictionary_add(functions->update_dict, key, handle);

        return bbSuccess;

    case AI_Command:
        available = functions->commands_available++;
        bbAssert(available < ai_command_function_count, "out of bounds error\n");

        functions->commands[available] = function_pointer;
        handle.u64 = available;

        bbDictionary_add(functions->command_dict, key, handle);

        return bbSuccess;

    default:
        bbAssert(0, "bad widget function type\n");
        return bbFail;
    }

    return bbSuccess;
}

I32 bbAI_Functions_getInt(bbAI_Functions* functions,
                             bbAI_FunctionType function_type,
                             char* key)
{
    bbDictionary* dict;
    switch (function_type){
    case AI_Update:
        dict = functions->update_dict;
        break;

    case AI_Command:
        dict = functions->command_dict;
        break;
    }

    bbHandle handle;
    bbFlag flag = bbDictionary_lookup(dict,key,&handle);
    if (flag == bbSuccess) return handle.u64;
    return -1;
}

bbFlag bbAI_Functions_getFunction(void** function,
                                    bbAI_Functions* functions,
                                    bbAI_FunctionType function_type,
                                    char* key)
{
    bbHandle handle;

    switch (function_type)
    {
    case AI_Update:
        bbDictionary_lookup(functions->update_dict,key,&handle);
        *function = functions->updates[handle.u64];
        return bbSuccess;

    case AI_Command:
        bbDictionary_lookup(functions->command_dict,key,&handle);
        *function = functions->commands[handle.u64];
        return bbSuccess;
    }
    *function = NULL;
    return bbNone;
}

bbFlag bbAI_Component_update(bbAI_Component* component, bbAI_System* system)
{
    I32 function_index = component->ftable.update;
    if (function_index < 0) return bbNone;
    bbAI_Update* function = system->functions.updates[function_index];
    return function(component);
}

bbFlag updateAI_list_fn(bbList* list, void* node, void* cl)
{
    bbHandle handle;
    bbVPool_reverseLookup(list->pool, node, &handle);
    //bbCoreInput_updateAgent(&home.core.core, handle, bbInstructionSource_input,
    //                        no_handle);

    bbAI_Component_update(node,&home.ECS.AI_system);

    return bbContinue;
}

bbFlag bbAI_System_update(bbAI_System* system)
{
    bbList_mapL(&system->list,updateAI_list_fn, NULL);
}


bbFlag bbAI_onCommand(bbAI_Component* component,
                          bbAI_System* system,
                          bbAI_CommandType type,
                          bbAI_CommandData data,
                          bool is_action)
{
    I32 function_index = component->ftable.command;
    if (function_index < 0)
    {

        return bbNone;
    }
    bbAI_Command* func_pointer = system->functions.commands[function_index];
    return func_pointer(component,type,data,is_action);
}


bbFlag bbCI_spawnAIComponent(bbCore* core,
                             bbHandle entity,
                             bbInstruction_source source,
                             bbHandle action)
{

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);
    instruction->type = bbI_spawnAIComponent;
    instruction->data.three_handles.handle1 = entity;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbI_spawnAIComponent_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
        undo_instruction->source = instruction->source;
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } //else source == no rewind


    bbAI_Component* component;
    bbHandle component_handle;

    bbList_alloc2(&home.ECS.AI_system.list,(void**)&component, &component_handle);
    component->ftable.command = 0;
    component->ftable.update = 0;
    component->state = 0;
    component->component.entity_handle = instruction->data.three_handles.handle1;

    bbCS_entity_setComponent(core,
                             home.ECS.ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);

    bbList_pushL(&home.ECS.AI_system.list,component);
    return bbSuccess;
}

bbFlag bbI_unspawnAIComponent_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
}


bbFlag bbCS_spawnAIComponent(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbAI_Component** this,
                             bbInstruction_source source,
                             bbHandle action)
{
    //Post undo instruction

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

        //set input instruction data
        instruction->type = bbI_spawnAIComponent;
        instruction->data.three_handles.handle1 = entity;
        instruction->source = source;
        instruction->redo_instruction = action;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;
        undo_instruction->redo_instruction.u64 = 0;
        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unspawnAIComponent;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //take action

    bbAI_Component* component;
    bbHandle component_handle;

    bbAI_System* AI_System = (bbAI_System*)ECS->systems[bbECS_AI];
    bbList_alloc2(&AI_System->list,(void**)&component, &component_handle);
    component->ftable.command = 0;
    component->ftable.update = 1;
    component->state = 0;
    component->component.entity_handle = entity;

    bbCS_entity_setComponent(core,
                             ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);

    bbList_pushL(&AI_System->list,component);

    if (this != NULL) *this = component;


    return bbSuccess;

}


bbFlag bbAI_System_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbAI_System_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}