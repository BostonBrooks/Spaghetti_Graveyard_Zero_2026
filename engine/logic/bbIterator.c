#include "engine/logic/bbIterator.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbFlag.h"


bbIterator bbIterator_new(bbList* list)
{
    bbIterator iterator;
    iterator.list = list;
    bbIterator_setHead(&iterator, NULL,NULL);

    return iterator;
}

bbFlag bbIterator_setHead(bbIterator* iterator, bbHandle* handle, void** element)
{

    bbHandle head_handle = iterator->list->list_pointer->head;

    void* head_element;
    bbFlag flag = bbVPool_lookup(iterator->list->pool,&head_element,head_handle);
    //this will catch NULL handle
    if (flag != bbSuccess)
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return flag;
    }

    iterator->current = head_element;
    iterator->handle = head_handle;

    if (handle != NULL) *handle = head_handle;
    if (element != NULL) *element = head_element;

    return bbSuccess;

}
bbFlag bbIterator_setTail(bbIterator* iterator, bbHandle* handle, void** element)
{
    bbHandle tail_handle = iterator->list->list_pointer->tail;

    void* tail_element;
    bbFlag flag = bbVPool_lookup(iterator->list->pool,&tail_element,tail_handle);
    //this will catch NULL handle
    if (flag != bbSuccess)
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return flag;
    }

    iterator->current = tail_element;
    iterator->handle = tail_handle;

    if (handle != NULL) *handle = tail_handle;
    if (element != NULL) *element = tail_element;

    return bbSuccess;
}

bbFlag bbIterator_increment(bbIterator* iterator, bbHandle* handle, void** element)
{
    bbAssert(iterator->current != NULL, "Iterator has no current element\n");


    if (bbSuccess == bbVPool_handleIsEqual(iterator->list->pool,iterator->handle,iterator->list->list.tail))
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return bbTail;
    }

    bbListElement_Handle* currentList = iterator->current + iterator->list->offset_of;
    bbHandle next_handle = currentList->next;

    void* next_element;

    //Will catch null handle
    bbFlag flag = bbVPool_lookup(iterator->list->pool,&next_element,next_handle);
    if (flag != bbSuccess)
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return flag;
    }

    iterator->current = next_element;
    iterator->handle = next_handle;

    if (handle != NULL) *handle = next_handle;
    if (element != NULL) *element = next_element;
    return bbSuccess;
}

bbFlag bbIterator_decrement(bbIterator* iterator, bbHandle* handle, void** element)
{
    bbAssert(iterator->current != NULL, "Iterator has no current element\n");


    if (bbSuccess == bbVPool_handleIsEqual(iterator->list->pool,iterator->handle,iterator->list->list.head))
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return bbHead;
    }

    bbListElement_Handle* currentList = iterator->current + iterator->list->offset_of;
    bbHandle prev_handle = currentList->prev;

    void* prev_element;

    //Will catch null handle
    bbFlag flag = bbVPool_lookup(iterator->list->pool,&prev_element,prev_handle);
    if (flag != bbSuccess)
    {
        if (handle != NULL) *handle = iterator->list->pool->null;
        if (element != NULL) *element = NULL;
        return flag;
    }

    iterator->current = prev_element;
    iterator->handle = prev_handle;

    if (handle != NULL) *handle = prev_handle;
    if (element != NULL) *element = prev_element;
    return bbSuccess;
}

//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
bbFlag bbIterator_mapL(bbIterator* iterator, bbListFunction* myFunc, void* cl)
{
    bbAssert(iterator->current != NULL, "Iterator has no current element\n");

    bbHandle handle;
    void* element;

    bbFlag flag =  bbIterator_setHead(iterator, &handle, &element);
    if (flag != bbSuccess) return flag;
    while (1)
    {
        flag = myFunc(iterator->list,element, cl);
        switch (flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            bbFlag flag2 = bbIterator_increment(iterator, &handle, &element);
            if (flag2 != bbSuccess) return flag2;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }
    }
}


bbFlag bbIterator_mapR(bbIterator* iterator, bbListFunction* myFunc, void* cl)
{
    bbAssert(iterator->current != NULL, "Iterator has no current element\n");

    bbHandle handle;
    void* element;

    bbFlag flag =  bbIterator_setTail(iterator, &handle, &element);
    if (flag != bbSuccess) return flag;
    while (1)
    {
        flag = myFunc(iterator->list,element, cl);
        switch (flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            bbFlag flag2 = bbIterator_decrement(iterator, &handle, &element);
            if (flag2 != bbSuccess) return flag2;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }
    }
}