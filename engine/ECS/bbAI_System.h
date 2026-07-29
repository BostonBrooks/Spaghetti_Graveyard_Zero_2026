 /**   A Unit is something that moves around the map on the graphics thread.
Movables calculate the movement of units.
EntityAIs control the behaviour of units at a higher level.

use bbCoreSynchronous functions to set data in a way that is "core-safe"
*/

#ifndef BB_AISYSTEM_H
#define BB_AISYSTEM_H
#include "engine/ECS/bbMoveables.h"
#include "engine/ECS/ECS.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbIntTypes.h"

typedef enum
{
    bbAIState_Idle,
    bbAIState_Follow
} bbAI_state;

typedef struct
{
    I32 update;
    I32 command;
} bbAI_FunctionTable;

typedef struct
{
    bbHandle ECS_entity_handle;
    bbListElement_Handle list_element;
    bbAI_FunctionTable ftable;
    bbAI_state state;
} bbAI_Component;

typedef enum
{
    AI_Update,
    AI_Command,
} bbAI_FunctionType;

typedef enum
{
    bbAI_setGoalPoint,
    bbAI_mapClick,
    bbAI_damageAgent,
    bbAI_killAgent
} bbAI_CommandType;

typedef struct
{
    bbMoveable_type type;
    bbMapCoords goal_point;
    I32 movable;
} bbAI_CommandData;

typedef bbFlag bbAI_Update(bbAI_Component* component);
typedef bbFlag bbAI_Command(bbAI_Component* component,
                                   bbAI_CommandType type,
                                   bbAI_CommandData data);


extern I32 ai_update_function_count;
extern I32 ai_command_function_count;

typedef struct
{
    bbAI_Update** updates;
    bbDictionary* update_dict;
    I32 updates_available;

    bbAI_Command** commands;
    bbDictionary* command_dict;
    I32 commands_available;
} bbAI_Functions;

typedef struct
{
    bbSystem system;
    bbList list;
    bbAI_Functions functions;
} bbAI_System;

bbFlag bbAI_System_init(bbAI_System* system);
bbFlag bbAI_System_update(bbAI_System* system);

bbFlag bbAI_Functions_init(bbAI_Functions* self);
bbFlag bbAI_Functions_populate(bbAI_Functions* self);


bbFlag bbAI_Functions_add(bbAI_Functions* functions,
                              bbAI_FunctionType function_type,
                              void* function_pointer,
                              char* key );

bbFlag bbAI_Functions_getFunction(void** function,
                                    bbAI_Functions* functions,
                                    bbAI_FunctionType function_type,
                                    char* key);

I32 bbAI_Functions_getInt(bbAI_Functions* functions,
                             bbAI_FunctionType function_type,
                             char* key);

bbFlag bbAI_Component_update(bbAI_Component* component, bbAI_System* system);


bbFlag bbAgent_onCommand(bbAI_Component* component,
                          bbAI_System* system,
                          bbAI_CommandType type,
                          bbAI_CommandData data);

bbFlag bbCS_spawnAIComponent(bbCore* core,
                             bbHandle entity,
                             bbAI_Component** this,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_spawnAIComponent(bbCore* core,
                             bbHandle entity,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_spawnAIComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnAIComponent_fn(bbCore* core, bbInstruction* instruction);
#endif //BB_AISYSTEM_H