
#include "engine/logic/bbList.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbTerminal.h"


#define isEqual(A, B) bbVPool_handleIsEqual(list->pool, A, B)
#define isNULL(A) bbVPool_handleIsEqual(list->pool, A, list->pool->null)

bbFlag bbList_mapL(bbList* list, bbListFunction* myFunc, void* cl){


    if(isNULL(list->list_pointer->head)){
        bbAssert(isNULL(list->list_pointer->tail), "head/tail\n");
        return bbSuccess;
    }

    bbHandle hande_element = list->list_pointer->head;
    void* element;
    bbListElement_Handle* list_element;
    bbFlag flag;

    while(1){
        bbVPool_lookup(list->pool, &element, hande_element);
        list_element = element + list->offset_of;
        flag = myFunc(list, element, cl);
        switch (flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(hande_element, list->list_pointer->tail))
                return bbSuccess;
            hande_element = list_element->next;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }

    }

}

bbFlag bbList_mapR(bbList* list, bbListFunction* myFunc, void* cl){


    if(isNULL(list->list_pointer->tail)){
        bbAssert(isNULL(list->list_pointer->head), "head/tail\n");
        return bbSuccess;
    }

    bbHandle hande_element = list->list_pointer->tail;
    void* element;
    bbListElement_Handle* list_element;
    bbFlag flag;

    while(1){
        bbVPool_lookup(list->pool, &element, hande_element);
        list_element = element + list->offset_of;
        flag = myFunc(list, element, cl);
        switch (flag){
        case bbBreak:
            return bbBreak;
        case bbContinue:
            if(isEqual(hande_element, list->list_pointer->head))
                return bbSuccess;
            hande_element = list_element->prev;
            break;
        case bbRepeat:
            bbHere();
            break;
        default:
            bbHere();
        }

    }

}


