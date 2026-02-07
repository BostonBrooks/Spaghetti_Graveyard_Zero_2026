/// pool that includes a guard so it can only be modified by one thread at a time

#ifndef BB_THREADED_POOL_H
#define BB_THREADED_POOL_H

#include <pthread.h>
#include <stdalign.h>

#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"

typedef struct
{
    I32 prev;
    I32 next;
} bbThreadedPool_available;

typedef struct
{
    bbHandle null;
    I32 num;
    I32 size_of;
    I32 in_use;
    I32 available_head;
    I32 available_tail;
    pthread_mutex_t mutex;
    //If pool full, thread waits until pool is not full
    pthread_mutex_t pool_full;
    pthread_cond_t pool_full_cond;
    alignas(8) U8 elements[];
} bbThreadedPool;


#define bbThreadedPool_alloc(pool, address)\
bbThreadedPool_allocImpl(pool, address, NULL, 0);

bbFlag bbVPool_newThreaded(bbVPool** pool, I32 size_of, I32 num);
bbFlag bbThreadedPool_new(bbThreadedPool** pool, I32 size_of, I32 num);
bbFlag bbThreadedPool_delete(bbThreadedPool* pool);
bbFlag bbThreadedPool_clear(void* pool);
bbFlag bbThreadedPool_allocImpl(bbThreadedPool* pool, void** address, char* file, int
line);
bbFlag bbThreadedPool_free(bbThreadedPool* pool, void* address);
bbFlag bbThreadedPool_lookup(bbThreadedPool* pool, void** address, bbHandle handle);
bbFlag bbThreadedPool_reverseLookup(void* Pool, void* address, bbHandle*
handle);
bbFlag bbThreadedPool_printHeader(void* Pool, void* address);
bool bbThreadedPool_handleIsEqual(void* USUSED, bbHandle A, bbHandle B);



#endif //BB_THREADED_POOL_H