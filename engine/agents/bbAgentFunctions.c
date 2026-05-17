#include "engine/agents/bbAgentFunctions.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbPrime.h"

bbFlag bbAgentFunctions_init(bbAgentFunctions* functions)
{
    I32 magic_number = nextPrime(256);

    functions->updates = calloc(magic_number,sizeof(bbAgent_Update*));
    bbDictionary_new(&functions->update_dict,magic_number);
    functions->updates_available = 0;

    functions->commands = calloc(magic_number,sizeof(bbAgent_Command*));
    bbDictionary_new(&functions->command_dict,magic_number);
    functions->commands_available = 0;

    return bbSuccess;
}

bbFlag bbAgentFunctions_add(bbAgentFunctions* functions, bbAgentFunctionType function_type, void* function_pointer, char* key )
{

    U32 available;
    bbHandle handle;
    I32 magic_number = nextPrime(256);
    switch (function_type)
    {
    case AgentUpdate:
        available = functions->updates_available++;
        bbAssert(available < magic_number, "out of bounds error\n");

        functions->updates[available] = function_pointer;
        handle.u64 = available;

        bbDictionary_add(functions->update_dict, key, handle);

        return bbSuccess;

    case AgentCommand:
        available = functions->commands_available++;
        bbAssert(available < magic_number, "out of bounds error\n");

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

I32 bbAgentFunctions_getInt(bbAgentFunctions* functions,
                             bbAgentFunctionType function_type, char* key){
    bbDictionary* dict;
    switch (function_type){
    case AgentUpdate:
        dict = functions->update_dict;
        break;

    case AgentCommand:
        dict = functions->command_dict;
        break;
    }

    bbHandle handle;
    bbFlag flag = bbDictionary_lookup(dict,key,&handle);
    if (flag == bbSuccess) return handle.u64;
    return -1;
}

bbFlag bbAgentFunctions_getFunction(void** function, bbAgentFunctions* functions,
                                     bbAgentFunctionType function_type, char* key){
    bbHandle handle;

    switch (function_type)
    {
    case AgentUpdate:
        bbDictionary_lookup(functions->update_dict,key,&handle);
        *function = functions->updates[handle.u64];
        return bbSuccess;

    case AgentCommand:
        bbDictionary_lookup(functions->command_dict,key,&handle);
        *function = functions->commands[handle.u64];
        return bbSuccess;
    }
    *function = NULL;
    return bbNone;
}


bbFlag bbAgent2_update(bbAgent2* agent)
{
    I32 function_index = agent->ftable.update;
    if (function_index < 0) return bbNone;
    bbAgent_Update* function = home.agents_app.functions.updates[function_index];
    return function(agent);
}

bbFlag bbAgent2_onCommand(bbAgent2* agent,
                          bbAgents2* agents,
                          bbAgentCommandType type,
                          bbHandle data)
{
    I32 function_index = agent->ftable.command;
    if (function_index < 0)
    {
        bbHere()
        return bbNone;
    }
    bbFlag (*func_pointer) (bbAgent2*,bbAgentCommandType,bbHandle);
    func_pointer = home.agents_app.functions.commands[function_index];
    return func_pointer(agent,type,data);
}

