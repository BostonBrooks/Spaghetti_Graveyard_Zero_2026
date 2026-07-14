#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/ECS/ECS.h"

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
    test_time = 1;
    bbHandle entity_handle;

    bbCoreInput_spawnEntity(&core, bbInstructionSource_input, null_handle);
    bbCore_react(&core);

    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, ECS.list.list_pointer->head);

    bbDebug("entity has components %064llb\n", entity->has_component);


    exit(EXIT_SUCCESS);

}