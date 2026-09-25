#include <stddef.h>

#include "engine/logic/bbNestedList.h"
#include "engine/logic/bbNestedListR.h"

#include "engine/logic/bbBloatedPool.h"


///nested list R is identical to nested list, but everythign is sorted in the reverse order
bbFlag bbNestedListR_init(bbNestedList* nestedList) {
    if (bbNestedList_pool == NULL){
        bbVPool_newBloated(&bbNestedList_pool, sizeof(bbNestedListElement), 1000, 10, "bbNestedListElement");
    }
    bbList_init(&nestedList->list,
                bbNestedList_pool,
                NULL,
                offsetof(bbNestedListElement, listElement),
                compare_list,
                80);


    return bbSuccess;
}



///If the list being inserted is empty, dont add to nested list
bbFlag bbNestedListR_attach(bbNestedList* nestedList, bbList* list){


    bbFlag flag = bbList_setTail(list, NULL);
    if(flag == bbNone) return bbNone;



    bbNestedListElement* nestedListElement;
    bbVPool_alloc(bbNestedList_pool, (void**) &nestedListElement);
    nestedListElement->list = list;
    nestedListElement->listElement.prev = bbNestedList_pool->null;
    nestedListElement->listElement.next = bbNestedList_pool->null;

    bbList_sortL(&nestedList->list, nestedListElement);



    return bbSuccess;
}


bbFlag bbNestedListR_getPrev(bbNestedList* nestedList, void** element){


    //bbHere()
    bbNestedListElement* nestedListElement;
    bbFlag flag = bbList_popR(&nestedList->list, (void**) &nestedListElement);
    if(flag != bbSuccess)
    {
        //bbFlag_print(flag);
        return flag;
    }

    void* element1;
    flag = bbList_getCurrent(nestedListElement->list, &element1);
    if(flag != bbSuccess)
    {
        //bbFlag_print(flag);
        return flag;
    }

    flag = bbList_decrement(nestedListElement->list, NULL);
    if(flag == bbSuccess){
        flag = bbList_sortR(&nestedList->list, nestedListElement);
        //bbFlag_print(flag);

    } else {
        flag = bbVPool_free(bbNestedList_pool, nestedListElement);
        //bbFlag_print(flag);

    }

    *element = element1;
    return bbSuccess;
}


bbFlag bbNestedListR_map(bbNestedList* list, bbNestedList_mapFunction* fn, void* cl){
    void* element;

    bbList_setTail(&list->list, NULL);
    while(1){
        bbFlag flag = bbNestedListR_getPrev(list, &element);
        if (flag != bbSuccess)
        {
            //bbFlag_print(flag);
            return flag;
        }
        flag = fn(element, cl);

        if (flag == bbBreak) break;
        //bbFlag_print(flag);
    }
}
