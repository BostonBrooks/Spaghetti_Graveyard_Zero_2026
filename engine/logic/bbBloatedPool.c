#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbVPool.h"

bbFlag bbVPool_newBloated(bbVPool** Pool, I32 sizeOf, I32 level1, I32 level2){
    bbBloatedPool* BloatedPool;
    bbBloatedPool_new(&BloatedPool, sizeOf, level1, level2);
    bbVPool* pool = malloc(sizeof(bbVPool));
    pool->pool = BloatedPool;
    pool->type = VPoolType_bloated;
    pool->null = BloatedPool->null;
    pool->size_of = BloatedPool->size_of;
    pool->delete = (bbFlag (*)(void* pool)) bbBloatedPool_delete;
    pool->clear = (bbFlag (*)(void* pool)) bbBloatedPool_clear;
    pool->alloc_impl = (bbFlag(*)(void* pool, void** address, char* file, int
    line)) bbBloatedPool_allocImpl;
    pool->free = (bbFlag(*)(void* pool, void* address)) bbBloatedPool_free;
    pool->lookup = (bbFlag (*)(void* pool, void** address, bbHandle
    handle)) bbBloatedPool_lookup;
    pool->reverse_lookup = (bbFlag (*)(void* pool, void* address,
            bbHandle* handle)) bbBloatedPool_reverseLookup;
    pool->print_header = (bbFlag (*)(void *, void *)) bbBloatedPool_printHeader;
    pool->handle_is_equal = (bool (*)(void* USUSED, bbHandle A, bbHandle B)) bbBloatedPool_handleIsEqual;
    *Pool = pool;
    return bbSuccess;
}

