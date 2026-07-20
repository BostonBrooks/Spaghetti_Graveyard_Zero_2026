#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbDictionary.h"


U64 test_time = 0;
thread_local char* thread;
char test_string[KEY_LENGTH];

typedef struct
{
    char key[KEY_LENGTH];
} test_struct;

int main(void)
{
    bbVPool *pool1, *pool2;
    test_struct *test1, *test2;
    bbHandle handle;
    bbVPool_newBloated(&pool1, sizeof(test_struct), 100,100, "Test Struct");
    bbVPool_newBloated(&pool2, sizeof(test_struct), 100,100, "Test Struct");

    handle.bloated.index = 1005;
    handle.bloated.collision = 193;

    bbVPool_allocFromHandle(pool2,(void**)&test2,handle);

    bbBloatedPool_printHeader(pool2->pool, test2);

    for (I32 i = 0; i < 1000; i++)
    {
        bbDebug("i = %d\n", i);
        bbVPool_alloc2(pool1, (void**)&test1,&handle);
        bbHere()
        bbVPool_allocFromHandle(pool2,(void**)&test2,handle);

        bbVPool_lookup(pool2,(void**)&test2,handle);
    }

}
