#ifndef BBSEGMENTEDDEQUE_H
#define BBSEGMENTEDDEQUE_H

/** The segmented deque is a kind of deque that stores elements in chunks, or segments.
 * The implementation has been chosen to match existing operations on the bbList class.
 * Unlike with the list implementation, The order of operations is important.
 * An alloc must be followed by a push.
 * a pop followed by an alloc overwrites the element returned by the pop.
 * it may be safe to access elements inside the deque by direct momory pointers
*/

#define BBSEGMENTEDDEQUE_SIZEOF 128
#define BBSEGMENTEDDEQUE_SEGMENT_SIZE 12
#include "bbFlag.h"
#include "engine/logic/bbIntTypes.h"

typedef struct
{
    U8 data[BBSEGMENTEDDEQUE_SIZEOF];
} bbDummyStruct;

typedef struct
{
    bbDummyStruct** elements;
    I32 num_segments;

    I32 start_segment;
    I32 end_segment;

    I32 start_index;
    I32 end_index;

    I32 in_use;
} bbSegmentedDeque;

bbFlag bbSegmentedDeque_init(bbSegmentedDeque* deque, I32 num_segments);

///return pointer to head+1
bbFlag bbSegmentedDeque_allocFront(bbSegmentedDeque* deque, void** element);

///increment head
bbFlag bbSegmentedDeque_pushFront(bbSegmentedDeque* deque, void* UNUSED);

///Return pointer to head
bbFlag bbSegmentedDeque_peakFront(bbSegmentedDeque* deque, void** element);

///decrement head
bbFlag bbSegmentedDeque_popFront(bbSegmentedDeque* deque, void** element);

///return pointer to tail-1
bbFlag bbSegmentedDeque_allocBack(bbSegmentedDeque* deque, void** element);

///decrement tail
bbFlag bbSegmentedDeque_pushBack(bbSegmentedDeque* deque, void* UNUSED);

///Return pointer to tail
bbFlag bbSegmentedDeque_peakBack(bbSegmentedDeque* deque, void** element);

///increment tail
bbFlag bbSegmentedDeque_popBack(bbSegmentedDeque* deque, void** element);

#endif //BBSEGMENTEDDEQUE_H
