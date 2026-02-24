/// A virtual pool is a wrapper for different types of concrete pools,
/// providing a single interface, to quickly switch between pool types

#ifndef BB_VIRTUAL_POOL_H
#define BB_VIRTUAL_POOL_H

#include <stdlib.h>

#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef enum
{
    VPoolType_lean,
    VPoolType_bloated
} bbVPool_type;

typedef struct
{
    bbHandle null;
    bbVPool_type type;
    U32 size_of;
    void* pool;

    bbFlag (*delete)(void* pool);
    bbFlag (*clear)(void* pool);
    bbFlag (*alloc_impl)(void* pool, void** address, char* file, int line);
    bbFlag (*free)(void* pool, void* address);
    bbFlag (*lookup)(void* pool, void** address, bbHandle handle);
    bbFlag (*reverse_lookup)(void* pool, void* address, bbHandle* handle);
    bbFlag (*print_header)(void* pool, void* address);
    bool (*handle_is_equal)(void* pool, bbHandle a, bbHandle b);
}bbVPool;


//small wrapper functions, can be inlined
static bbFlag bbVPool_delete(bbVPool* pool)
{
    bbFlag flag = pool->delete(pool->pool);
    free(pool);
    return flag;
}


static bbFlag bbVPool_clear(bbVPool* pool)
{
    return pool->clear(pool->pool);
}

static bbFlag bbVPool_allocImpl(bbVPool* pool, void** address, char* file, int line)
{
    bbAssert(pool!=0x0, "passing null argument\n");

    return pool->alloc_impl(pool->pool, address, file, line);
}
static bbFlag bbVPool_free(bbVPool* pool, void* address)
{
    return pool->free(pool->pool, address);
}
static bbFlag bbVPool_lookup(bbVPool* pool, void** address, bbHandle handle)
{
    //TODO - roll this code into bbLeanPool
//    if(pool->type == VPoolType_lean)
//    {
//        if (handle.ptr == NULL)
//        {
//            bbHere()
//            *address=NULL;
//            return bbNone;
//        }
//    }
    return pool->lookup(pool->pool, address, handle);
}


static bbFlag bbVPool_reverseLookup(bbVPool* pool, void* address, bbHandle* handle)
{
    return pool->reverse_lookup(pool->pool, address, handle);
}
static bbFlag bbVPool_printHeader(bbVPool* pool, void* address)
{
    return pool->print_header(pool->pool, address);
}
static bool bbVPool_handleIsEqual(bbVPool* pool, bbHandle A, bbHandle B)
{
    return pool->handle_is_equal(pool->pool, A, B);
}


#define bbVPool_alloc(pool, address)\
bbVPool_allocImpl(pool, address, __FILE_NAME__, __LINE__);





#endif //BB_VIRTUAL_POOL_H