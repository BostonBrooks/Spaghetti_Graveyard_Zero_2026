/**
- init spawner                                             function call
                                                           bbFlag bbEntitySpawner_init(bbEntitySpawner* entity_spawner);

- populate spawner functions                               function call
                                                           bbFlag bbEntitySpawner_populate(bbEntitySpawner* entity_spawner);

- open spawner file and go line by line                    function call
                                                           bbFlag bbEntitySpawner_spawnFile(bbEntitySpawner* entity_spawner, char* file_name);

  - parse each line and call spawn function                virtual function call
                                                           typedef bbFlag bbParseFunction(void* spawner, char* line);


    - spawn empty entity                                   bbCoreSynchronous with bbInstructionSource_norewind
                                                           bbFlag bbCoreSynchronous_spawnEmptyEntity(...)

    - spawn components                                     bbCoreSynchronous with bbInstructionSource_norewind
                                                           bbFlag bbCoreSynchronous_spawnComponentI(I32 function_index);

      - spawn one component                                virtual function call with bbInstructionSource_norewind
                                                           typedef bbFlag bbSpawnFunction(...)

-begin play

-spawn entity as part of an instruction                    bbCoreInput with unknown bbInstructionSource

   - spawn empty entity and create undo instruction        bbCoreSynchronous with bbInstructionSource_internal
                                                           bbFlag bbCoreSynchronous_spawnEmptyEntity(...)

   - spawn components                                      bbCoreSynchronous with bbInstructionSource_internal
                                                           bbFlag bbCoreSynchronous_spawnComponentI(I32 function_index);

      - spawn one component and create undo instruction    virtual function call with bbInstructionSource_internal
                                                           typedef bbFlag bbSpawnFunction(...)

I am using bbCoreSynchronous instructions to spawn the empty entity
these only work if called with bbInstructionSource_internal, or
bbInstructionSource_norewind,because there is no way to pass data from the redo
instruction to subsequent instructions.

in a bbCoreSynchronous instruction, the undo instruction must be posted before
calling other bbCoreSynchronous instructions

the file loaded by the spawner will be called spawner.csv

The format of one row of spawner.csv is determined by the parser. Here is an
example set of columns

PARSER, TYPE, POSITION X, Y, Z, GOALPOINT X, Y, Z, SERVER HANDLE INDEX, COLLISION, SPAWN FUNCTIONS...

It is important that the spawn functions are called in the correct order.
The spawn function for the Moveable component must come before the spawn function
for the Graphics component
**/




#ifndef BB_ENTITYSPAWNER_H
#define BB_ENTITYSPAWNER_H

#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"


///read one line of input file and spawn one entity
typedef bbFlag bbParseFunction(void* spawner, char* line);

///spawn one or more components, can be called recursively. If
///bbInstructionSource_internal, add rewind instruction.
typedef bbFlag bbSpawnFunction(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);

typedef bbFlag bbLiveSpawnFunction(void* spawner,
                                  bbSpawnFunctionArgs args,
                                  bbInstruction_source source);

typedef struct
{
    bbParseFunction** parse_functions;
    I32 parse_function_count;
    bbDictionary* parse_dict;

    bbSpawnFunction** spawn_functions;
    I32 spawn_function_count;
    bbDictionary* spawn_dict;

    bbLiveSpawnFunction** live_spawn_functions;
    I32 live_spawn_function_count;
    bbDictionary* live_spawn_dict;
} bbEntitySpawner;

extern I32 parse_function_count;
extern I32 spawn_function_count;
extern I32 live_spawn_function_count;

///Prepare spawner for use in program
bbFlag bbEntitySpawner_init(bbEntitySpawner* entity_spawner);

///Load one function into vtable
bbFlag bbParseFunction_add(bbEntitySpawner* spawner, bbParseFunction* parse_function, char* key );
bbFlag bbSpawnFunction_add(bbEntitySpawner* spawner, bbSpawnFunction* spawn_function, char* key );
bbFlag bbLiveSpawnFunction_add(bbEntitySpawner* spawner, bbLiveSpawnFunction* live_spawn_function, char* key );


///Load functions specific to this map
bbFlag bbEntitySpawner_populate(bbEntitySpawner* entity_spawner);
///Read file and spawn entities, before game starts
bbFlag bbEntitySpawner_spawnFile(bbEntitySpawner* entity_spawner, char* file_name);

///Take input and spawn one entity
bbFlag bbCoreSynchronous_spawnEntity(bbEntitySpawner* entity_spawner,
                                     bbECS_entity** entity,
                                     I32 type,
                                     bbMapCoords position,
                                     bbMapCoords goalpoint,
                                     bbHandle server_handle);

#endif //BB_ENTITYSPAWNER_H