#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/ECS(old)/ECS(old).h"

bbCore core;
bbECS ECS;
U64 test_time = 0;
thread_local char* thread;
char test_string[KEY_LENGTH];
bbHandle null_handle;

int main(void)
{
    null_handle.u64 = 0;
    bbCore_init(&core);
    bbECS_init(&ECS);


    bbCoreInput_setTime(&core, 1, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 1;

    bbCoreInput_setTime(&core, 2, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 2;

    bbCoreInput_setTime(&core, 3, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 3;




    bbCoreInput_spawnEntity(&core, bbInstructionSource_input, null_handle);
    bbCore_react(&core);

    bbHandle entity_handle = ECS.list.list_pointer->head;

    bbCoreInput_entity_setComponent(&core, &ECS, entity_handle, no_handle,bbECS_Moveables,bbInstructionSource_input, no_handle);
    bbCoreInput_entity_setComponent(&core, &ECS, entity_handle, no_handle,bbECS_Unit,bbInstructionSource_input, no_handle);
    bbCoreInput_entity_setComponent(&core, &ECS, entity_handle, no_handle,bbECS_AI,bbInstructionSource_input, no_handle);
    bbCore_react(&core);

    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, ECS.list.list_pointer->head);

    bbDebug("entity has components ");
    print_binary_8(entity->has_component);

    bbCoreInput_setTime(&core, 10, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 10;

    bbCore_rewindUntil(&core, 2);
    bbCore_react(&core);

    bbDebug("entity has components ");
    print_binary_8(entity->has_component);


    exit(EXIT_SUCCESS);

}