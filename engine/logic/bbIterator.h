#ifndef BB_ITERATOR_H
#define BB_ITERATOR_H
#include "engine/logic/bbList.h"

typedef struct
{
    bbList* list;
    bbHandle handle;
    void* current;

} bbIterator;

bbIterator bbIterator_new(bbList* list);

bbFlag bbIterator_setHead(bbIterator* iterator, bbHandle* handle, void** element);
bbFlag bbIterator_setTail(bbIterator* iterator, bbHandle* handle, void** element);

bbFlag bbIterator_increment(bbIterator* iterator, bbHandle* handle, void** element);
bbFlag bbIterator_decrement(bbIterator* iterator, bbHandle* handle, void** element);

bbFlag bbIterator_mapL(bbIterator* iterator, bbListFunction* myFunc, void* cl);
bbFlag bbIterator_mapR(bbIterator* iterator, bbListFunction* myFunc, void* cl);

#endif //BB_ITERATOR_H