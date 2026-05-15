
#ifndef BB_AGENT_FUNCTION_ENUMS_H
#define BB_AGENT_FUNCTION_ENUMS_H
#include "bbAgents2.h"
#include "engine/logic/bbHandle.h"
#include "engine/userinterface/bbWidgetFunctions.h"


typedef enum
{
    AgentUpdate,
    AgentCommand
} bbAgentFunctionType;

typedef enum bbAgentCommandType
{
    bbAC_unused
} bbAgentCommandType;

#endif //BB_AGENT_FUNCTION_ENUMS_H

#ifndef BB_AGENT_FUNCTIONS_H
#define BB_AGENT_FUNCTIONS_H

#include "engine/agents/bbAgents.h"

typedef bbFlag bbAgent_Update(bbAgent2* agent);
typedef bbFlag bbAgent_Command (bbAgent2* agent,
                                   bbAgentCommandType type,
                                   bbHandle data);

typedef struct
{
    bbAgent_Update** updates;
    bbDictionary* update_dict;
    I32 updates_available;

    bbAgent_Command** commands;
    bbDictionary* command_dict;
    I32 commands_available;
} bbAgentFunctions;

bbFlag bbAgentFunctions_init(bbAgentFunctions* self);

bbFlag bbAgentFunctions_populate(bbAgentFunctions* self);

bbFlag bbAgentFunctions_add(bbAgentFunctions* functions,
                              bbAgentFunctionType function_type,
                              void* function_pointer,
                              char* key );

bbFlag bbAgentFunctions_getFunction(void** function,
                                    bbAgentFunctions* functions,
                                    bbAgentFunctionType function_type,
                                    char* key);

I32 bbAgentFunctions_getInt(bbAgentFunctions* functions,
                             bbAgentFunctionType function_type,
                             char* key);


bbFlag bbAgent2_update(bbAgent2* agent);
#endif