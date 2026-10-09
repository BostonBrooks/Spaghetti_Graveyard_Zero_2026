
#define isEqual(A, B) (bbSuccess == bbVPool_handleIsEqual(list->pool, A, B))
#define isNULL(A) (bbSuccess != bbVPool_handleIsNULL(list->pool, A))
#include <stddef.h>

#include "bbFlag.h"
#include "bbList.h"
///Start from the left, keep searching until compare returns true
bbFlag bbList_searchL(bbList* list,I32 (*compare)(void* A, void* B), void* element_in, void** element_out)
{

    bbVPool* pool = list->pool;

    if(isNULL(list->list_pointer->head) || isNULL(list->list_pointer->tail))
    {
        *element_out = NULL;
        return bbFail;
    }

    bbHandle next_handle = list->list_pointer->head;
    void *next;
    bbListElement_Handle *next_list;

    while (1)
    {
        bbFlag flag = bbVPool_lookup(list->pool, &next, next_handle);

        next_list = next + list->offset_of;

        if (compare(element_in, next))
        {
            *element_out = next;
            return bbSuccess;
        }

        next_handle = next_list->next;

        if(isNULL(next_handle) || isEqual(next_handle,list->list_pointer->tail))
        {
            *element_out = NULL;
            return bbFail;
        }


    }
}
///Start from the right, keep searching until compare returns true
bbFlag bbList_searchR(bbList* list,I32 (*compare)(void* A, void* B), void* element_in, void** element_out)
{

    bbVPool* pool = list->pool;

    if(isNULL(list->list_pointer->head) || isNULL(list->list_pointer->tail))
    {
        *element_out = NULL;
        return bbFail;
    }

    bbHandle prev_handle = list->list_pointer->tail;
    void *prev;
    bbListElement_Handle *prev_list;

    while (1)
    {
        bbFlag flag = bbVPool_lookup(list->pool, &prev, prev_handle);

        prev_list = prev + list->offset_of;

        if (compare(element_in, prev))
        {
            *element_out = prev;
            return bbSuccess;
        }

        prev_handle = prev_list->prev;

        if(isNULL(prev_handle) || isEqual(prev_handle,list->list_pointer->head))
        {
            *element_out = NULL;
            return bbFail;
        }
    }

}