#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbIterator.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

thread_local char* thread;
thread_local bool debug_off;
bbHome home;
U64 test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];
bbMapCoords testGoalPoint;

typedef struct
{
    I32 integer;
    bbListElement_Handle list;
} testStruct;

bbFlag testFunction(bbList* list, void* node, void* cl)
{
    testStruct* test = (testStruct*)node;
    bbDebug("Integer = %d\n", test->integer);
    return bbContinue;
}
int main(void)
{
    debug_off = false;
    bbVPool* pool;
    bbList* list;
    //bbVPool_newSystem(&pool, 193, sizeof(testStruct),10,10,"TEST_STRUCT");
    bbVPool_newBloated(&pool, sizeof(testStruct),10,10,"TEST_STRUCT");
    bbList_new(&list,pool,NULL,offsetof(testStruct,list),NULL);
    testStruct* structure;
    bbHandle handle;

    for (int i = 0; i < 100; i++)
    {
        bbList_alloc2(list, (void**)&structure, &handle);
        structure->integer = i;
        bbList_pushR(list, (void*)&structure);
    }

    bbIterator iterator = bbIterator_new(list);
    bbIterator_mapR(&iterator, testFunction, NULL);
    bbIterator_mapL(&iterator, testFunction, NULL);

    exit(EXIT_SUCCESS);
}
