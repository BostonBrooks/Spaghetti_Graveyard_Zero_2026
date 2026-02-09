/// bbList is the Swiss army knife of doubly linked lists

// implements circular doubly linked list.
// uses head/tail in bbList_Handle
// head.prev = tail, tail.next = head

#ifndef BB_LIST_H
#define BB_LIST_H


#include <stdlib.h>
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbFlag.h"


typedef struct
{
    bbVPool *pool;
    ///points to head/tail of list
    bbList_Handle* list_pointer;
    ///supply own head/tail of none given
    bbList_Handle list;

    /// offset of  bbListElement_Handle in list element
    size_t offset_of;

    I32 (*compare)(void* A, void* B);

    bbHandle current;
    bbHandle prev;
    bbHandle next;
} bbList;

///The third element of this function is used to share head/tail with another bbList
bbFlag bbList_init(bbList* list, bbVPool* pool, void* list_pointer, size_t offset_of,
I32 (*compare)(void* A, void* B));
bbFlag bbList_new(bbList** list, bbVPool* pool, void* list_pointer, size_t offset_of,
                   I32 (*compare)(void* A, void* B));

bbFlag bbList_pushL(bbList* list, void* element);
bbFlag bbList_pushR(bbList* list, void* element);

bbFlag bbList_popL(bbList* list, void** element);
bbFlag bbList_popR(bbList* list, void** element);

bbFlag bbList_sortL(bbList* list, void* element);
bbFlag bbList_sortR(bbList* list, void* element);

bbFlag bbList_peakL(bbList* list, void** element);
bbFlag bbList_peakR(bbList* list, void** element);


I32 bbList_getLength(bbList* list);
bbFlag bbList_sort(bbList* list);

///point the iterator to the head of the list
bbFlag bbList_setHead(bbList* list, void** element);
///point the iterator to the tail of the list
bbFlag bbList_setTail(bbList* list, void** element);

/// move the iterator to the right and return pointer by reference
bbFlag bbList_increment(bbList* list, void** element);
/// move the iterator to the left and return pointer by reference
bbFlag bbList_decrement(bbList* list, void** element);

/// return by reference, pointer to current location of iterator
bbFlag bbList_getCurrent(bbList* list, void** element);
/// point the iterator at element;
bbFlag bbList_setCurrent(bbList* list, void* element);

/// take the element and sort back into the list,
/// starting from it's current location
/// if element == current, this could be problematic, bbWarning
bbFlag bbList_sortElement(bbList* list, void* element);

// remove current object pointed to by iterator from list and return by ref
// store current.prev and current.next
//bbFlag bbList_popCurrent(bbList* list, void** element);

/// remove object from list
bbFlag bbList_remove(bbList* list, void* element);

bbFlag bbList_insertAfter(bbList* list, void* Node, void* Key);
bbFlag bbList_insertBefore(bbList* list, void* Node, void* Key);


typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
/// map function to list, going left-to-right
bbFlag bbList_mapL(bbList* list, bbListFunction* myFunc, void* cl);

/// map function to list, going right-to-left
bbFlag bbList_mapR(bbList* list, bbListFunction* myFunc, void* cl);

bbFlag bbList_isEmpty(bbList* list);

bbFlag bbList_alloc(bbList* list, void** element);

bbFlag bbList_getNth(bbList* list, void** element, I32 N);

bbFlag bbList_checkIntegrity(bbList* list);

#endif //BB_LIST_H