#ifndef BBNESTEDLIST_H
#define BBNESTEDLIST_H

#include "engine/logic/bbList.h"
#include "engine/logic/bbTerminal.h"

extern bbVPool* bbNestedList_pool;

typedef struct {
    bbList* list; //pointer to existing list
    bbPool_ListElement listElement; //element of list list
} bbNestedListElement;

typedef struct {
    bbList list;
} bbNestedList;


typedef bbFlag bbNestedList_mapFunction(void* node, void* cl);

bbFlag bbNestedList_init(bbNestedList* nestedList);
bbFlag bbNestedList_attach(bbNestedList* nestedList, bbList* list);
bbFlag bbNestedList_getNext(bbNestedList* nestedList, void** element);
bbFlag bbNestedList_map(bbNestedList* list, bbNestedList_mapFunction* fn, void* cl);


I32 compare_list(void* one, void* two);

#endif // BBNESTEDLIST_H