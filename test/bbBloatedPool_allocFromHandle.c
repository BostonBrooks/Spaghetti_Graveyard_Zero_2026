#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"


thread_local char* thread;
U64 test_time = 0;

typedef struct
{
    char str [KEY_LENGTH];
} a_struct;

int main(void)
{
    bbBloatedPool* pool;
    bbBloatedPool_new(&pool, sizeof(a_struct), 1000, 1000, "a_struct");



    a_struct *a,*b;

    bbHandle handle;
    handle.bloated.index = 7;
    handle.bloated.collision = 193;

    bbBloatedPool_alloc(pool, (void**)&b);
    bbBloatedPool_allocFromHandle(pool,(void**)&a, handle);

    bbStr_setStr(a->str, "test",KEY_LENGTH);

    bbDebug("a.str = %s\n",a->str);

    bbBloatedPool_printHeader(pool, a);

    return 0;
}