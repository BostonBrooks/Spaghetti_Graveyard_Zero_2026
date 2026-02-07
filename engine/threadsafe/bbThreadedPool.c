#include "engine/threadsafe/bbThreadedPool.h"

#include <string.h>

#include "engine/logic/bbArithmetic.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbVPool.h"

bbFlag bbThreadedPool_lookup_unchecked(bbThreadedPool* pool, void** address, bbHandle handle)
{

    bbAssert(handle.u64 >= 0 && handle.u64 < pool->num, "index %d out of bounds\n", handle.u64);
    I32 offset = handle.u64 * pool->size_of;

    *address = &pool->elements[offset];
    return bbSuccess;
}

bbFlag bbThreadedPool_reverseLookup_unchecked(bbThreadedPool* pool, void* address, bbHandle* handle)
{
    I32 offset = (U8*)address - (U8*)&pool->elements[0];
    I32 index = offset / pool->size_of;
    bbAssert(index >= 0 && index < pool->num, "index %d out of bounds\n", index);
    handle->u64 = index;

    return bbSuccess;
}

bbFlag bbVPool_newThreaded(bbVPool** self, I32 size_of, I32 num)
{
    bbThreadedPool* ThreadedPool;
    bbThreadedPool_new(&ThreadedPool, size_of, num);
    bbVPool* pool = malloc(sizeof(bbVPool));
    bbAssert(pool != NULL, "bad malloc\n");
    pool->pool = ThreadedPool;
    pool->null = ThreadedPool->null;
    pool->size_of = ThreadedPool->size_of;
    pool->delete = (bbFlag (*)(void* pool)) bbThreadedPool_delete;
    pool->clear = bbThreadedPool_clear;
    pool->alloc_impl = (bbFlag(*)(void* pool, void** address, char* file, int
    line)) bbThreadedPool_allocImpl;
    pool->free = (bbFlag(*)(void* pool, void* address)) bbThreadedPool_free;
    pool->lookup = (bbFlag (*)(void* pool, void** address, bbHandle handle))
                    bbThreadedPool_lookup;
    pool->reverse_lookup = bbThreadedPool_reverseLookup;
    //pool->printHeader = bbThreadedPool_printHeader;
    pool->handle_is_equal = bbThreadedPool_handleIsEqual;
    *self = pool;
    return bbSuccess;

}

bbFlag bbThreadedPool_new(bbThreadedPool** self, I32 size_of, I32 num)
{
    bbAssert(num>2, "num to small\n");
    bbAssert(size_of > 0, "sizeOf too small\n");

    I32 size = bbArith_roundUp(size_of, 8);


    bbThreadedPool* pool = malloc(sizeof(bbThreadedPool)+ num * size * sizeof(U8));

    bbAssert(pool != NULL, "bad malloc\n");

    pool->null.u64 = UINT64_MAX;
    pool->size_of = size;
    pool->num = num;
    pool->in_use = 0;

    pool->available_head = 0;
    pool->available_tail = num-1;

    bbHandle handle;
    bbThreadedPool_available* element;

    for (I32 i = 0; i < num; i++)
    {
        handle.u64 = i;


        I32 offset = i * pool->size_of;
        element = (bbThreadedPool_available*)&pool->elements[offset];

        element->prev = i-1;
        element->next = i+1;
    }
    handle.u64 = 0;

    bbThreadedPool_lookup_unchecked(pool, (void*)&element, handle);
    element->prev = -1;
    handle.u64 = num-1;

    bbThreadedPool_lookup_unchecked(pool, (void*)&element, handle);
    element->next = -1;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_mutex_init(&pool->pool_full, NULL);
    pthread_cond_init(&pool->pool_full_cond, NULL);

    *self = pool;


    return bbSuccess;
}

bbFlag bbThreadedPool_delete(bbThreadedPool* pool){

    free(pool);
    return bbSuccess;
}

bbFlag bbThreadedPool_allocImpl(bbThreadedPool* pool, void** address, char* file, int line)
{
    bbMutexLock(&pool->mutex);

    if (pool->in_use >= pool->num)
    {
        //assert available list empty
        bbMutexUnlock(&pool->mutex);

        //there is a bug when mutex is unlocked in between the following lines
        pthread_cond_wait(&pool->pool_full_cond, &pool->pool_full);
        bbMutexLock(&pool->mutex);
    }

    pool->in_use++;

    bbHandle handle;
    bbThreadedPool_available* element;
    handle.u64 = pool->available_head;

    bbThreadedPool_lookup_unchecked(pool, (void*)&element, handle);


    if (element->next == -1) //last element
    {
        pool->available_head = -1;
        pool->available_tail = -1;

        memset(element, 0, pool->size_of);
        *address = element;
        bbMutexUnlock(&pool->mutex);
        return bbSuccess;
    }

    bbThreadedPool_available* next_element;
    handle.u64 = element->next;
    bbThreadedPool_lookup_unchecked(pool, (void*)&next_element, handle);
    next_element->prev = -1;
    pool->available_head = element->next;
    memset(element, 0, pool->size_of);
    *address = element;
    bbMutexUnlock(&pool->mutex);
    return bbSuccess;
}

bbFlag bbThreadedPool_free(bbThreadedPool* pool, void* address)
{
    bbMutexLock(&pool->mutex);
    pool->in_use--;
    pthread_cond_signal(&pool->pool_full_cond);

    //pool full, reserve empty
    if (pool->available_head == -1)
    {
        bbAssert(pool->available_tail == -1, "head/tail mismatch\n");
        bbHandle handle;

        bbThreadedPool_reverseLookup_unchecked(pool, address, &handle);
        pool->available_head = handle.u64;
        pool->available_tail = handle.u64;

        bbThreadedPool_available* element = address;
        element->prev = -1;
        element->next = -1;


        bbMutexUnlock(&pool->pool_full);
        bbMutexUnlock(&pool->mutex);
        return bbSuccess;
    }



    bbThreadedPool_available* next_element;
    bbHandle next_handle;
    next_handle.u64 = pool->available_head;

    bbThreadedPool_lookup_unchecked(pool, (void**)&next_element, next_handle);

    bbHandle handle;

    bbThreadedPool_reverseLookup_unchecked(pool, address, &handle);
    bbThreadedPool_available* element = address;

    next_element->prev = handle.u64;
    pool->available_head = handle.u64;
    element->next = next_handle.u64;
    element->prev = -1;

    //the following line may not be necessary because the pool is not full

    bbMutexUnlock(&pool->pool_full);
    bbMutexUnlock(&pool->mutex);
    return bbSuccess;
}

bbFlag bbThreadedPool_lookup(bbThreadedPool* pool, void** address, bbHandle handle)
{

    void** element;
    bbThreadedPool_lookup_unchecked(pool, (void**)&element, handle);
    *address = element;

    return bbSuccess;
}

bbFlag bbThreadedPool_reverseLookup(void* Pool, void* address, bbHandle* Handle)
{
    bbThreadedPool* pool = Pool;
    bbHandle handle;
    bbThreadedPool_reverseLookup_unchecked(pool, (void*)address, &handle);
    *Handle = handle;
    return bbSuccess;
}

bool bbThreadedPool_handleIsEqual(void* USUSED, bbHandle A, bbHandle B)
{
    return (A.u64 == B.u64);
}

bbFlag bbThreadedPool_clear(void* Pool)
{
    bbThreadedPool* pool = Pool;
    bbMutexLock(&pool->mutex);
    bbThreadedPool_available* element;
    pool->in_use = 0;
    for (I32 i = 0; i < pool->num; i++)
    {
        element = (bbThreadedPool_available*)&pool->elements[i];
        element->prev = i-1;
        element->next = i+1;
    }
    element = (bbThreadedPool_available*)&pool->elements[0];
    element->prev = -1;
    element = (bbThreadedPool_available*)&pool->elements[pool->num-1];
    element->next = -1;

    pool->available_head = 0;
    pool->available_tail = pool->num - 1;


    bbMutexUnlock(&pool->mutex);
    return bbSuccess;
}
