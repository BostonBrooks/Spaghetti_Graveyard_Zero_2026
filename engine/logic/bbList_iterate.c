#include "engine/logic/bbList.h"
#include "engine/logic/bbFlag.h"


#define isEqual(A, B) bbVPool_handleIsEqual(list->pool, A, B)
#define isNULL(A) bbVPool_handleIsEqual(list->pool, A, list->pool->null)

///point the iterator to the head of the list
bbFlag bbList_setHead(bbList* list, void** element){

    if(isNULL(list->list_pointer->head)) return bbNone;

    list->current = list->list_pointer->head;
    list->prev = list->pool->null;
    list->next = list->pool->null;

    if (element!=NULL) {
        bbVPool_lookup(list->pool, element, list->current);
    }
    return bbSuccess;
}

///point the iterator to the tail of the list
bbFlag bbList_setTail(bbList* list, void** element){

    if(isNULL(list->list_pointer->tail)) return bbNone;

    list->current = list->list_pointer->tail;
    list->prev = list->pool->null;
    list->next = list->pool->null;

    if (element!=NULL) {
        bbVPool_lookup(list->pool, element, list->current);
    }
    return bbSuccess;
}


bbFlag bbList_increment(bbList* list, void** element){

    //default case
    if(!isNULL(list->current)){

        //is current the tail of the list?
        if(isEqual(list->current, list->list_pointer->tail)){
            //cannot increment, what do we do?
            return bbNone;
        }

        void* current;
        bbVPool_lookup(list->pool, &current, list->current);
        bbListElement_Handle* currentList = current + list->offset_of;

        list->current = currentList->next;
        list->prev = list->pool->null;
        list->next = list->pool->null;

        if (element!=NULL) {
            bbVPool_lookup(list->pool, element, list->current);
        }

        return bbSuccess;

    }
    // We must have removed the current element and all elements to the right
    //else isNULL(list->current)
    if(isNULL(list->next)) return bbNone;

    list->current = list->next;
    list->prev = list->pool->null;
    list->next = list->pool->null;

    if (element!=NULL) {
        bbVPool_lookup(list->pool, element, list->current);
    }

    return bbSuccess;
}


bbFlag bbList_decrement(bbList* list, void** element){

    //default case
    if(!isNULL(list->current)){

        //is current the tail of the list?
        if(isEqual(list->current, list->list_pointer->head)){
            //cannot increment, what do we do?
            return bbNone;
        }

        void* current;
        bbVPool_lookup(list->pool, &current, list->current);
        bbListElement_Handle* currentList = current + list->offset_of;

        list->current = currentList->prev;
        list->prev = list->pool->null;
        list->next = list->pool->null;

        if (element!=NULL) {
            bbVPool_lookup(list->pool, element, list->current);
        }

        return bbSuccess;

    }
    //else isNULL(list->current)
    // We must have removed the current element and all elements to the right
    if(isNULL(list->prev)) return bbNone;

    list->current = list->prev;
    list->prev = list->pool->null;
    list->next = list->pool->null;

    if (element!=NULL) {
        bbVPool_lookup(list->pool, element, list->current);
    }

    return bbSuccess;
}

bbFlag bbList_getCurrent(bbList* list, void** element){
    if(isNULL(list->current)){
        *element = NULL;
        return bbNone;
    }

    if (element!=NULL) {
        bbVPool_lookup(list->pool, element, list->current);
    }
    return bbSuccess;
}

bbFlag bbList_setCurrent(bbList* list, void* element){
    bbHandle element_handle;
    bbVPool_reverseLookup(list->pool, element, &element_handle);

    list->current = element_handle;
    list->prev = list->pool->null;
    list->next = list->pool->null;

    return bbSuccess;
}

bbFlag bbList_popCurrent(bbList* list, void** element){
    void* Element;
    bbList_getCurrent(list, &Element);
    bbList_remove(list, Element);

    return bbSuccess;
}