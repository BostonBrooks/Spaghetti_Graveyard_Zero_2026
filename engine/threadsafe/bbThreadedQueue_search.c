#include "engine/threadsafe/bbThreadedQueue_search.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbThreadedQueue_removeUnchecked(bbThreadedQueue* queue, void* element) {

    bbHandle element_handle;
    bbListElement_Handle* list_element = element + queue->offset_of;
    bbVPool_reverseLookup(queue->pool, element, &element_handle);
    I32 element_int = element_handle.u64;

    //Case 1: only element
    if (queue->head == queue->tail) {
        bbAssert(queue->head == element_int, "element not in list\n");

        bbAssert(
            bbVPool_handleIsEqual(queue->pool, list_element->prev, queue->pool->null) &&
            bbVPool_handleIsEqual(queue->pool, list_element->next, queue->pool->null),
            "weird bbPool_ListElement\n"
        );
        queue->head = -1;
        queue->tail = -1;

        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;
        return bbSuccess;
    }
    //Case 2: first element
    if (queue->head == element_int) {
        bbHandle next_handle = list_element->next;
        void* next_element;
        bbVPool_lookup(queue->pool, &next_element, next_handle);
        bbListElement_Handle* next_list_element = next_element + queue->offset_of;
        next_list_element->prev = queue->pool->null;
        queue->head = next_handle.u64;

        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;
        return bbSuccess;
    }
    //Case 3: last element
    if (queue->tail == element_int) {
        bbHandle prev_handle = list_element->prev;
        void* prev_element;
        bbVPool_lookup(queue->pool, &prev_element, prev_handle);
        bbListElement_Handle* prev_list_element = prev_element + queue->offset_of;
        prev_list_element->next = queue->pool->null;
        queue->tail = prev_handle.u64;

        list_element->prev = queue->pool->null;
        list_element->next = queue->pool->null;
        return bbSuccess;
    }
    //case 4: middle element
    bbHandle prev_handle = list_element->prev;
    bbHandle next_handle = list_element->next;

    void* prevElement;
    void* nextElement;

    bbVPool_lookup(queue->pool, &prevElement, prev_handle);
    bbVPool_lookup(queue->pool, &nextElement, next_handle);

    bbListElement_Handle* prev_list_element = prevElement + queue->offset_of;
    bbListElement_Handle* next_list_element = nextElement + queue->offset_of;

    prev_list_element->next = next_handle;
    next_list_element->prev = prev_handle;

    list_element->prev = queue->pool->null;
    list_element->next = queue->pool->null;

    return bbSuccess;
}

bbFlag bbThreadedQueue_search(bbThreadedQueue* queue, void** Element, bbCallback* callback)
{
    bbMutexLock(&queue->mutex);

    //Case 1: Empty
    if (queue->head == -1 || queue->tail == -1)
    {
        bbAssert(queue->head == -1 && queue->tail == -1, "head/tail mismatch\n");
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
        bbHandle handle2;
        handle2.ptr = element;

        bbFlag flag = bbCallback_execute(callback, handle2);

        if (flag == bbSuccess)
        {
            bbListElement_Handle* list_element = (element + queue->offset_of);
            list_element->prev = queue->pool->null;
            list_element->next = queue->pool->null;
            queue->head = -1;
            queue->tail = -1;

            *Element = element;
            bbMutexUnlock(&queue->mutex);
            return bbSuccess;
        }

        *Element = NULL;
        bbMutexUnlock(&queue->mutex);
        return bbNone;
    }

    //Case 3: More than one element

    I32 element_int = queue->head;
    bbHandle element_handle;
    void* element;
    bbListElement_Handle* list_element;
    bbFlag flag;
    bbHandle address_handle;


    element_handle.u64 = element_int;
    bbVPool_lookup(queue->pool, &element, element_handle);

    while (1)
    {

        address_handle.ptr = element;
        bbFlag flag = bbCallback_execute(callback, address_handle);

        if (flag == bbSuccess)
        {
            bbThreadedQueue_removeUnchecked(queue, element);
            *Element = element;
            bbMutexUnlock(&queue->mutex);
            return bbSuccess;

        }

        //update elementInt and return None if we reach the end

        bbListElement_Handle* list_element = (element + queue->offset_of);
        bbHandle next_handle = list_element->next;
        if (bbVPool_handleIsEqual(queue->pool, next_handle, queue->pool->null)) {
            return bbNone;
        }
        bbVPool_lookup(queue->pool, &element, next_handle);
    }

}