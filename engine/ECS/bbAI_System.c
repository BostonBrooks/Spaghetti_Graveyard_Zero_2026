#include "engine/ECS/bbAI_System.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbPrime.h"

bbFlag bbEntityAIs_init(bbAI_System* system)
{
    bbVPool_newBloated(&system->system.pool, sizeof(bbAI_Component),100,100,"ENTITY AI");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbAI_Component,list_element),NULL);

    bbAI_Functions_init(&system->functions);
    home.ECS.ECS.systems[bbECS_AI] = (bbSystem*)system;
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

bbFlag bbAgent_onCommand(bbAI_Component* component,
                          bbAI_System* system,
                          bbAI_CommandType type,
                          bbAI_CommandData data)
{
    I32 function_index = component->ftable.command;
    if (function_index < 0)
    {

        return bbNone;
    }
    bbAI_Command* func_pointer = system->functions.commands[function_index];
    return func_pointer(component,type,data);
}

// bbFlag bbCoreSynchronous_spawnAIComponent(bbCore* core,
//                                     bbECS_entity* entity,
//                                    bbAI_Component** this,
//                                    bbInstruction_source source)
// {
//
//
// }