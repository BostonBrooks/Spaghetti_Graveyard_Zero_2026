#include <stddef.h>

#include "engine/logic/bbNestedList.h"

#include "engine/logic/bbBloatedPool.h"

bbVPool* bbNestedList_pool;



bbFlag bbNestedList_init(bbNestedList* nestedList) {
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
bbFlag bbNestedList_attach(bbNestedList* nestedList, bbList* list){


    bbFlag flag = bbList_setHead(list, NULL);
    if(flag == bbNone) return bbNone;



    bbNestedListElement* nestedListElement;
    bbVPool_alloc(bbNestedList_pool, (void**) &nestedListElement);
    nestedListElement->list = list;
    nestedListElement->listElement.prev = bbNestedList_pool->null;
    nestedListElement->listElement.next = bbNestedList_pool->null;

    bbList_sortR(&nestedList->list, nestedListElement);



    return bbSuccess;
}


bbFlag bbNestedList_getNext(bbNestedList* nestedList, void** element){


    //bbHere()
    bbNestedListElement* nestedListElement;
    bbFlag flag = bbList_popL(&nestedList->list, (void**) &nestedListElement);
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

    flag = bbList_increment(nestedListElement->list, NULL);
    if(flag == bbSuccess){
        flag = bbList_sortL(&nestedList->list, nestedListElement);
        //bbFlag_print(flag);

    } else {
        flag = bbVPool_free(bbNestedList_pool, nestedListElement);
        //bbFlag_print(flag);

    }

    *element = element1;
    return bbSuccess;
}


bbFlag bbNestedList_map(bbNestedList* list, bbNestedList_mapFunction* fn, void* cl){
    void* element;

    bbList_setHead(&list->list, NULL);
    while(1){
        bbFlag flag = bbNestedList_getNext(list, &element);
        if (flag != bbSuccess)
        {
            //bbFlag_print(flag);
            return flag;
        }
        flag = fn(element, cl);
        //bbFlag_print(flag);
    }
}


I32 compare_list(void* one, void* two){

    bbNestedListElement* nestedListElement1 = one;
    bbNestedListElement* nestedListElement2 = two;


    bbList* list1 = nestedListElement1->list;
    bbList* list2 = nestedListElement2->list;

    void* element1;
    void* element2;

    bbList_getCurrent(list1, &element1);
    bbList_getCurrent(list2, &element2);
    I32 flag = list1->compare(element1, element2);

    return flag;

}

