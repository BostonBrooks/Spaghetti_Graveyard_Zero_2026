#ifndef BBNESTEDLISTR_H
#define BBNESTEDLISTR_H

#include "engine/logic/bbList.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbNestedList.h"


bbFlag bbNestedListR_init(bbNestedList* nestedList);
bbFlag bbNestedListR_attach(bbNestedList* nestedList, bbList* list);
bbFlag bbNestedListR_getNext(bbNestedList* nestedList, void** element);
bbFlag bbNestedListR_map(bbNestedList* list, bbNestedList_mapFunction* fn, void* cl);


I32 compare_list(void* one, void* two);

#endif // BBNESTEDLIST_H