

#include "engine/logic/bbNestedList.h"

#include "bbLeanPool.h"

bbVPool* bbNestedList_pool;



bbFlag bbNestedList_init(bbNestedList* nestedList) {
    if (bbNestedList_pool == NULL){
        bbVPool_newLean(&bbNestedList_pool, sizeof(bbNestedListElement), 1000);
    }
    bbList_init(&nestedList->list,
                bbNestedList_pool,
                NULL,
                offsetof(bbNestedListElement, listElement),
                compare_list);


    return Success;
}




bbFlag bbNestedList_attach(bbNestedList* nestedList, bbList* list){


    bbFlag flag = bbList_setHead(list, NULL);

    if(flag == None) return None;
    bbNestedListElement* nestedListElement;
    bbVPool_alloc(bbNestedList_pool, (void**) &nestedListElement);
    nestedListElement->list = list;
    nestedListElement->listElement.prev = bbNestedList_pool->null;
    nestedListElement->listElement.next = bbNestedList_pool->null;

    bbList_sortR(&nestedList->list, nestedListElement);



    return Success;
}


bbFlag bbNestedList_getNext(bbNestedList* nestedList, void** element){
    bbNestedListElement* nestedListElement;
    bbFlag flag = bbList_popL(&nestedList->list, (void**) &nestedListElement);
    if(flag != Success) return flag;


    void* element1;
    flag = bbList_getCurrent(nestedListElement->list, &element1);
    if(flag != Success) return flag;

    flag = bbList_increment(nestedListElement->list, NULL);
    if(flag == Success){
        bbList_sortL(&nestedList->list, nestedListElement);
    } else {
        bbVPool_free(bbNestedList_pool, nestedListElement);
    }

    *element = element1;
    return Success;
}


bbFlag bbNestedList_map(bbNestedList* list, bbNestedList_mapFunction* fn, void* cl){
    void* element;

    while(1){
        bbFlag flag = bbNestedList_getNext(list, &element);
        if (flag != Success) break;
        fn(element, cl);
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

