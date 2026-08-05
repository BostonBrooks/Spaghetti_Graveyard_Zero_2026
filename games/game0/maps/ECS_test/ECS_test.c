#include "engine/ECS/ECS.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbConstants.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbTerminal.h"
#include "engine/test_string/bbTestString.h"
#include "engine/ECS/ECS_instructions.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"
#include "virtual_instructions/instructions.h"

thread_local char* thread;
thread_local bool debug_off = {0};
U64 test_time = 0;
bbHandle null_handle = {0};
char test_string[KEY_LENGTH];

bbCore core;
bbServerEntities server_entities;



typedef struct
{
    char key[KEY_LENGTH];
} test_struct;

int main(void)
{
    thread = "MAIN";
    bbDebug("Hello World!\n");

    strcpy(test_string, "Hello World!");


    bbCore_init(&core);

    core.instruction_functions = calloc(sizeof(bbInstruction_fn*), 16);
    core.instruction_functions[bbInstruction_spawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_spawnEmptyEntity_fn;
    core.instruction_functions[bbInstruction_unspawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_unspawnEmptyEntity_fn;
    core.instruction_functions[bbInstruction_entity_setComponent-bbInstruction_numTypes] = bbInstruction_entity_setComponent_fn;
    core.instruction_functions[bbInstruction_entity_unsetComponent-bbInstruction_numTypes] = bbInstruction_entity_unsetComponent_fn;
    core.instruction_functions[bbInstruction_setServerEntity-bbInstruction_numTypes] = bbInstruction_setServerEntity_fn;
    core.instruction_functions[bbInstruction_unsetServerEntity-bbInstruction_numTypes] = bbInstruction_unsetServerEntity_fn;

    core.instruction_functions[bbInstruction_spawnServerEntity-bbInstruction_numTypes] = bbInstruction_spawnServerEntity_fn;
    core.instruction_functions[bbInstruction_unspawnServerEntity-bbInstruction_numTypes] = bbInstruction_unspawnServerEntity_fn;

    bbECS EntityComponentSystem;
    bbECS_init(&EntityComponentSystem, bbECS_numSystems);

    core.ECS = &EntityComponentSystem;

    bbServerEntities_init(&server_entities,core.ECS);

    bbCoreInput_setTime(&core, 1, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 1;

    bbCoreInput_setTime(&core, 2, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 2;

    bbCoreInput_setTime(&core, 3, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 3;


    bbCS_setString(&core, "bish", bbInstructionSource_input, no_handle);
    bbCS_setString(&core, "bash", bbInstructionSource_internal,no_handle);
    bbCS_setString(&core, "bosh", bbInstructionSource_norewind, no_handle);

    char str[KEY_LENGTH];
    U32 collision = 0;

    for (I32 i = 4; i < 10;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i-3);

        bbAction_setString(&core,
                 0,
                 collision++,
                 i,
                 i-3,
                 str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }

    bbHandle server_handle;
    server_handle.bloated.index = 193;
    server_handle.bloated.collision = 193;
    bbECS_entity* entity;
    bbCoreInput_spawnServerEntity(&core,
                                    "TEST ENTITY",
                                    server_handle,
                                    bbInstructionSource_input,
                                    no_handle);
    bbCore_react(&core);
    //bbCoreSynchronous_spawnEmptyEntity(&core, core.ECS, &entity, "TEST ENTITY", bbInstructionSource_input,no_handle);

    bbVPool_lookup(core.ECS->system.pool,(void**)&entity,core.ECS->list.list.head);
    bbDebug("Entity.key = %s\n", entity->key);

    bbServerEntity* server_entity;
    bbHandle server_handle2;

    bbComponent_mapComponent(core.ECS,bbECS_ECS,(bbComponent*)entity,bbECS_ServerEntities,&server_handle2,(bbComponent**)&server_entity);

    bbECS_entity* entity2;
    bbHandle_mapComponent(core.ECS,bbECS_ServerEntities,server_handle2,bbECS_ECS,NULL,(bbComponent**)&entity2);


    bbDebug("Entity.key = %s\n", entity2->key);




    for (I32 i = 10; i < 15;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i-3);

        bbAction_setString(&core,
                 0,
                 collision++,
                 i,
                 i-3,
                 str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);
    }


    bbVPool* pool;
    test_struct* test_struct_ptr;
    bbHandle test_handle;
    bbVPool_newBloated(&pool, sizeof(test_struct), 100,100,"Test Struct");
    bbVPool_alloc2(pool, (void**)&test_struct_ptr, &test_handle);

    bbVPool_lookup(pool, (void**)&test_struct_ptr, test_handle);
    bbStr_setStr(test_struct_ptr->key, "Test Struct", KEY_LENGTH);

    bbCoreInbox_SetString(&core, "IWHBYD\n");
    bbCore_checkInbox(&core);

    exit(EXIT_SUCCESS);

}
