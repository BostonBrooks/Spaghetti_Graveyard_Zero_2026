#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/ECS.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"
//#include "engine/network/bbNetwork.h"
//#include "engine/network/bbNetworkApp.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/test_string/bbTestString.h"

bbCore core;
bbECS ECS;
bbAI_System AI_system;
bbNetwork network;
bbNetworkTime* network_time;
U64 test_time = 0;
thread_local char* thread;
char test_string[KEY_LENGTH];
bbHandle null_handle;
bbHome home;


pthread_barrier_t barrier1;

thread_local bool debug_off;

bbMapCoords testGoalPoint;



typedef struct
{
    char key[KEY_LENGTH];
} test_struct;

int main(void)
{
    null_handle.u64 = 0;
    bbCore_init(&core);
    bbECS_init(&ECS);
    bbAI_System_init(&AI_system,&ECS);
    debug_off = false;

    bbNetworkApp_init(&network);
    network_time = (bbNetworkTime*)network.extra_data;

    char address[64] = "127.0.0.1";
    char port[64] = "1701";
    bbNetworkApp_connect(&network, address, port);

    bbCoreInput_setTime(&core, 1, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 1;

    bbCoreInput_setTime(&core, 2, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 2;

    bbCoreInput_setTime(&core, 3, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 3;

    bbMapCoords MC = {0};

    bbCoreInput_spawnTestEntity(&core,&ECS,MC, no_handle, bbInstructionSource_input, null_handle);

    bbCore_react(&core);

    bbCoreInput_setTime(&core, 4, bbInstructionSource_input, no_handle);
    bbCore_react(&core);
    test_time = 4;

    bbCore_rewindUntil(&core, 2);
    bbCore_react(&core);

    bbHandle head_handle = ECS.list.list_pointer->head;
    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, head_handle);


    bbDebug("Entity has key: %s\n", entity->key);

    bbDebug("entity has components ");
    print_binary_8(entity->has_component);

    bbCS_spawnAIComponent(&core,
                          &ECS,
                          head_handle,
                          NULL,
                          bbInstructionSource_norewind,
                          no_handle);

    U32 collision = 0;
    char str[KEY_LENGTH];

    for (I32 i = 5; i < 10;i++)
    {
        bbCoreInput_setTime(&core, i, bbInstructionSource_input, no_handle);
        test_time = core.actual_time = i;
        bbCore_react(&core);

        sprintf(str, "(%d)", i);

        bbAction_setString(&core,
                         0,
                         collision++,
                         i,
                         i-3,
                         str);

        bbCoreInput_checkActions(&core,i,bbInstructionSource_input, no_handle);
        bbCore_react(&core);

        sprintf(str, "~(%d)", i);

        bbCI_setString(&core, str,bbInstructionSource_input, no_handle);
        bbCore_react(&core);

        bbCS_setString(&core, "bish", bbInstructionSource_input, no_handle);
        bbCS_setString(&core, "bash", bbInstructionSource_internal,no_handle);
        bbCS_setString(&core, "bosh", bbInstructionSource_norewind, no_handle);
    }


    bbVPool* pool;
    test_struct* test_struct_ptr;
    bbHandle test_handle;
    bbVPool_newBloated(&pool, sizeof(test_struct), 100,100,"Test Struct");
    bbVPool_alloc2(pool, (void**)&test_struct_ptr, &test_handle);

    bbVPool_lookup(pool, (void**)&test_struct_ptr, test_handle);
    bbStr_setStr(test_struct_ptr->key, "Test Struct", KEY_LENGTH);


    bbCoreInbox_SetString(&core, "IWHBYD");

    bbCore_checkInbox(&core);
    exit(EXIT_SUCCESS);
}