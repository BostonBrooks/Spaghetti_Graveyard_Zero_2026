#ifndef BB_THREADED_QUEUE_H
#define BB_THREADED_QUEUE_H

#include <pthread.h>
#include "engine/logic/bbVPool.h"

typedef struct
{
    // bbVPool can be any type of pool, with the same interface.
    // We may want different variations on bbThreadedPool.
    bbVPool* pool;
    pthread_mutex_t mutex;
    // head and tail should be bbPool_handle
    I32 head;
    I32 tail;
    I32 offset_of;

    pthread_mutex_t empty;
    pthread_cond_t empty_cond;

} bbThreadedQueue;

///Init in place, create new pool if pool == NULL
bbFlag bbThreadedQueue_init(bbThreadedQueue* queue, bbVPool* pool, I32 sizeOf, I32 num, I32 offset_of);

///malloc then init
bbFlag bbThreadedQueue_new(bbThreadedQueue** queue, bbVPool* pool, I32 sizeOf, I32 num, I32 offset_of);
///delete queue without deleting it's pool
bbFlag bbThreadedQueue_delete(bbThreadedQueue* queue);
///delete queue and it's pool
bbFlag bbThreadedQueue_deletePool(bbThreadedQueue* queue);

bbFlag bbThreadedQueue_alloc(bbThreadedQueue* queue, void** element);
bbFlag bbThreadedQueue_free(bbThreadedQueue* queue, void** element);

///signal emtpyCond, unlock empty. Does order between empty and mutex matter?
bbFlag bbThreadedQueue_pushL(bbThreadedQueue* queue, void* element);
bbFlag bbThreadedQueue_pushR(bbThreadedQueue* queue, void* element);

///if emtpy return None
bbFlag bbThreadedQueue_popL(bbThreadedQueue* queue, void** element);
bbFlag bbThreadedQueue_popR(bbThreadedQueue* queue, void** element);

///if empty pthread_cond_wait(...);
bbFlag bbThreadedQueue_popL_block(bbThreadedQueue* queue, void** element);
bbFlag bbThreadedQueue_popR_block(bbThreadedQueue* queue, void** element);

#endif //BB_THREADED_QUEUE_H