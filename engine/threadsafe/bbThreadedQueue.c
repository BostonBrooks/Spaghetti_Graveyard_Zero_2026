#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/logic/bbTerminal.h"

#define head_tail(queue){bbAssert((queue->head < 0)==(queue->tail < 0),\
    "head = %d, tail = %d\n", queue->head, queue->tail)}

bbFlag bbThreadedQueue_init(bbThreadedQueue* queue, bbVPool* pool, I32 sizeOf, I32 num, I32 offset_of)
{

    if (pool == NULL)
    {
        bbVPool_newThreaded(&queue->pool, sizeOf, num);
    }
    else
    {
        queue->pool = (bbVPool*)pool;
    }


    queue->head = -1;
    queue->tail = -1;
    queue->offset_of = offset_of;


    int flag = pthread_mutex_init(&queue->mutex, NULL);

    bbAssert(flag == 0, "pthread_mutex_init failed\n");

    flag = pthread_mutex_init(&queue->empty, NULL);
    bbAssert(flag == 0, "pthread_mutex_init failed\n");
    flag = pthread_cond_init(&queue->empty_cond, NULL);

    bbAssert(flag == 0, "pthread_cond_init failed\n");
    return bbSuccess;
}


bbFlag bbThreadedQueue_alloc(bbThreadedQueue* queue, void** element)
{

    void* element1;
    bbVPool_alloc(queue->pool, (void**)&element1);

     bbListElement_Handle* list_element = (element1 + queue->offset_of);

    list_element->prev = queue->pool->null;
    list_element->next = queue->pool->null;

    *element = element1;

    return bbSuccess;
}

bbFlag bbThreadedQueue_free(bbThreadedQueue* queue, void** element)
{
    bbVPool_free(queue->pool, (void*)*element);
    *element = NULL;

    return bbSuccess;
}

bbFlag bbThreadedQueue_pushL(bbThreadedQueue* queue, void* element)
{


    bbMutexLock(&queue->mutex);
    head_tail(queue)

    bbFlag flag;
    bbListElement_Handle* list_element = element + queue->offset_of;

    bbAssert(
        bbVPool_handleIsEqual(queue->pool, queue->pool->null, list_element->prev)
        && bbVPool_handleIsEqual(queue->pool, queue->pool->null, list_element->next),
        "Tried to push element already in a queue\n"
    );
    bbHandle handle_element;
    flag = bbVPool_reverseLookup(queue->pool, element, &handle_element);
    head_tail(queue)
    //list empty
    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");

        queue->head = handle_element.u64;
        queue->tail = handle_element.u64;

        //I guess we're using null for endpoints of lists, IE not a circular list
        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;

        bbMutexUnlock(&queue->mutex);


        pthread_cond_signal(&queue->empty_cond);
        pthread_mutex_unlock(&queue->empty);
        return bbSuccess;
    }


    void* head;
    bbHandle head_handle;

    bbAssert( queue->head >= 0, "list not empty but has null head\n");
    head_handle.u64 = queue->head;


    bbAssert( queue->tail != -1, "list not empty but has null tail\n");
    bbVPool_lookup(queue->pool, &head, head_handle);
    bbAssert( queue->tail != -1, "list not empty but has null tail\n");

    bbListElement_Handle* head_listElement = (head + queue->offset_of);

    list_element->prev = queue->pool->null;
    list_element->next = head_handle;

    head_listElement->prev = handle_element;
    queue->head = (I32)handle_element.u64;

    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}

bbFlag bbThreadedQueue_pushR(bbThreadedQueue* queue, void* element)
{
    //what if, as a protection, bbThreadedQueue_pushL sets element to NULL
    bbMutexLock(&queue->mutex);

    bbFlag flag;
    bbListElement_Handle* list_element = element + queue->offset_of;

    bbAssert(
        bbVPool_handleIsEqual(queue->pool, queue->pool->null, list_element->prev)
        && bbVPool_handleIsEqual(queue->pool, queue->pool->null, list_element->next),
        "Tried to push element already in a queue\n"
    );
    bbHandle handle_element;
    flag = bbVPool_reverseLookup(queue->pool, element, &handle_element);

    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");

        queue->head = handle_element.u64;
        queue->tail = handle_element.u64;

        //I guess we're using null for endpoints of lists, IE not a circular list
        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;

        bbMutexUnlock(&queue->mutex);


        pthread_cond_signal(&queue->empty_cond);
        pthread_mutex_unlock(&queue->empty);
        return bbSuccess;
    }

    void* tail;
    bbHandle tail_handle;
    tail_handle.u64 = queue->tail;
    bbVPool_lookup(queue->pool, &tail, tail_handle);
    bbListElement_Handle* tail_list_element = (tail + queue->offset_of);

    list_element->next = queue->pool->null;
    list_element->prev = tail_handle;

    tail_list_element->next = handle_element;
    queue->tail = handle_element.u64;


    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}

//Cases: empty, 1 element, more than 1 element;
bbFlag bbThreadedQueue_popL(bbThreadedQueue* queue, void** Element)
{
    bbMutexLock(&queue->mutex);

    //Case 1: Empty

    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");
        *Element = NULL;
        bbMutexUnlock(&queue->mutex);
        return bbNone;
    }

    //Case 2: One Element

    if (queue->head == queue->tail)
    {
        bbHandle handle;
        handle.u64 = queue->head;
        void* element;
        bbVPool_lookup(queue->pool, &element, handle);
        bbListElement_Handle* listElement = (element + queue->offset_of);
        listElement->prev = queue->pool->null;
        listElement->next = queue->pool->null;
        queue->head = -1;
        queue->tail = -1;

        *Element = element;
        bbMutexUnlock(&queue->mutex);
        return bbSuccess;
    }

    //Case 3: More than one element
    bbHandle head_handle;
    head_handle.u64 = queue->head;
    void* head_element;
    bbVPool_lookup(queue->pool, &head_element, head_handle);
    bbListElement_Handle* head_listElement = (head_element + queue->offset_of);

    void* next_element;
    bbVPool_lookup(queue->pool, &next_element, head_listElement->next);
    bbListElement_Handle* next_listElement = (next_element + queue->offset_of);
    bbHandle next_handle;
    bbVPool_reverseLookup(queue->pool, next_element, &next_handle);

    head_listElement->prev = queue->pool->null;
    head_listElement->next = queue->pool->null;

    next_listElement->prev = queue->pool->null;
    queue->head = next_handle.u64;

    *Element = head_element;
    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}

//Cases: empty, 1 element, more than 1 element;
bbFlag bbThreadedQueue_popR(bbThreadedQueue* queue, void** Element)
{
    bbVPool* pool = queue->pool;
    //   bbDebug("queue = %p, vPool = %p, Pool = %p\n", queue, pool, pool->pool);
    bbMutexLock(&queue->mutex);

    //Case 1: Empty

    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");
        *Element = NULL;
        bbMutexUnlock(&queue->mutex);
        return bbNone;
    }

    //Case 2: One Element

    if (queue->head == queue->tail)
    {
        bbHandle handle;
        handle.u64 = queue->head;
        void* element;
        bbVPool_lookup(queue->pool, &element, handle);
        bbListElement_Handle* listElement = (element + queue->offset_of);
        listElement->prev = queue->pool->null;
        listElement->next = queue->pool->null;
        queue->head = -1;
        queue->tail = -1;

        *Element = element;
        bbMutexUnlock(&queue->mutex);
        return bbSuccess;
    }

    //Case 3: More than one element
    //bbHere()
    bbHandle tail_handle;
    tail_handle.u64 = queue->tail;
    void* tail_element;
    bbVPool_lookup(queue->pool, &tail_element, tail_handle);
    bbListElement_Handle* tail_listElement = (tail_element + queue->offset_of);

    void* prev_element;
    bbVPool_lookup(queue->pool, &prev_element, tail_listElement->prev);
    bbListElement_Handle* prev_listElement = (tail_listElement + queue->offset_of);
    bbHandle prev_handle;
    bbVPool_reverseLookup(queue->pool, prev_element, &prev_handle);

    tail_listElement->prev = queue->pool->null;
    tail_listElement->next = queue->pool->null;


    prev_listElement->next = queue->pool->null;
    queue->tail = prev_handle.u64;

    *Element = tail_element;
    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}

bbFlag bbThreadedQueue_popR_block(bbThreadedQueue* queue, void** Element)
{
    bbVPool* pool = queue->pool;
    //   bbDebug("queue = %p, vPool = %p, Pool = %p\n", queue, pool, pool->pool);
    bbMutexLock(&queue->mutex);

    //Case 1: Empty

    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");
        bbMutexUnlock(&queue->mutex);

        //bbDebug("Threaded Queue Empty\n");
        pthread_cond_wait(&queue->empty_cond, &queue->empty);

        bbMutexLock(&queue->mutex);
    }

    //Case 2: One Element

    if (queue->head == queue->tail)
    {
        bbAssert(queue->head == queue->tail, "how did we get here?\n");
        bbHandle handle;
        handle.u64 = queue->head;
        void* element;
        bbVPool_lookup(queue->pool, &element, handle);
        bbListElement_Handle* list_element = (element + queue->offset_of);
        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;
        queue->head = -1;
        queue->tail = -1;

        *Element = element;
        bbMutexUnlock(&queue->mutex);
        return bbSuccess;
    }

    //Case 3: More than one element
    //bbHere()
    bbHandle tail_handle;
    tail_handle.u64 = queue->tail;
    void* tail_element;
    bbVPool_lookup(queue->pool, &tail_element, tail_handle);
    bbListElement_Handle* tail_list_element = (tail_element + queue->offset_of);

    void* prev_element;
    bbVPool_lookup(queue->pool, &prev_element, tail_list_element->prev);
    bbListElement_Handle* prev_listElement = (tail_list_element + queue->offset_of);
    bbHandle prev_handle;
    bbVPool_reverseLookup(queue->pool, prev_element, &prev_handle);

    tail_list_element->prev = queue->pool->null;
    tail_list_element->next = queue->pool->null;

    prev_listElement->next = queue->pool->null;
    queue->tail = prev_handle.u64;

    *Element = tail_element;
    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}

//Cases: empty, 1 element, more than 1 element;
bbFlag bbThreadedQueue_popL_block(bbThreadedQueue* queue, void** Element)
{
    bbMutexLock(&queue->mutex);

    //Case 1: Empty


    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch");
        bbMutexUnlock(&queue->mutex);
        //bbDebug("Threaded Queue Empty\n");
        pthread_cond_wait(&queue->empty_cond, &queue->empty);

        bbMutexLock(&queue->mutex);
    }

    //Case 2: One Element

    if (queue->head == queue->tail)
    {
        bbHandle handle;
        handle.u64 = queue->head;
        void* element;
        bbVPool_lookup(queue->pool, &element, handle);
        bbListElement_Handle* list_element = (element + queue->offset_of);
        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;
        queue->head = -1;
        queue->tail = -1;

        *Element = element;
        bbMutexUnlock(&queue->mutex);
        return bbSuccess;
    }

    //Case 3: More than one element
    bbHandle head_handle;
    head_handle.u64 = queue->head;
    void* head_element;
    bbVPool_lookup(queue->pool, &head_element, head_handle);
    bbListElement_Handle* head_list_element = (head_element + queue->offset_of);

    void* next_element;
    bbVPool_lookup(queue->pool, &next_element, head_list_element->next);
    bbListElement_Handle* next_list_element = (next_element + queue->offset_of);
    bbHandle next_handle;
    bbVPool_reverseLookup(queue->pool, next_element, &next_handle);

    head_list_element->prev = queue->pool->null;
    head_list_element->next = queue->pool->null;

    next_list_element->prev = queue->pool->null;
    queue->head = next_handle.u64;

    *Element = head_element;
    bbMutexUnlock(&queue->mutex);
    return bbSuccess;
}