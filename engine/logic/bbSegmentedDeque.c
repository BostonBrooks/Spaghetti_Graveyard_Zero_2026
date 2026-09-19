#include "engine/logic/bbSegmentedDeque.h"

#include <stdlib.h>

#include "bbTerminal.h"


bbFlag bbSegmentedDeque_init(bbSegmentedDeque* deque, I32 num_segments)
{
    deque->elements = calloc(num_segments, sizeof(bbDummyStruct*));
    deque->elements[0] = calloc(BBSEGMENTEDDEQUE_SEGMENT_SIZE, sizeof(bbDummyStruct));
    deque->num_segments = num_segments;
    deque->start_segment = 0;
    deque->end_segment = 0;
    deque->start_index = 0;
    deque->end_index = 0;
    deque->in_use = 0;

    return bbSuccess;
}

///expand the deque containing segments
bbFlag bbSegmentedDeque_grow(bbSegmentedDeque* deque)
{
bbNotImplemented()
}


bbFlag bbSegmentedDeque_peakFront(bbSegmentedDeque* deque, void** element)
{
    bbDummyStruct* segment = deque->elements[deque->end_segment];

    bbAssert(segment != NULL, "Segment not found\n");

    *element = &segment[deque->end_index];

    return bbSuccess;
}



bbFlag bbSegmentedDeque_allocFront(bbSegmentedDeque* deque, void** element)
{

    I32 new_index = deque->end_index + 1;
    I32 new_segment = deque->end_segment;
    if (new_index >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
    {
        new_index = 0;
        new_segment++;        bbNotImplemented() //wrap around, grow


    }

    bbDummyStruct* segment = deque->elements[new_segment];
    if (segment == NULL)
    {
        segment = calloc(BBSEGMENTEDDEQUE_SEGMENT_SIZE, sizeof(bbDummyStruct));
        deque->elements[new_segment] = segment;
    }

    *element = &deque->elements[new_segment][new_index];

    return bbSuccess;

}

bbFlag bbSegmentedDeque_pushFront(bbSegmentedDeque* deque, void* element)
{
    I32 new_index = deque->end_index + 1;
    I32 new_segment = deque->end_segment;
    if (new_index >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
    {
        new_index = 0;
        new_segment++;        bbNotImplemented() //wrap around, grow
        bbAssert(new_segment != deque->start_segment, "implement bbSegmentedDeque_grow")
    }

    bbDummyStruct* segment = deque->elements[new_segment];
    if (segment == NULL)
    {
        segment = calloc(BBSEGMENTEDDEQUE_SEGMENT_SIZE, sizeof(bbDummyStruct));
        deque->elements[new_segment] = segment;
    }

    deque->end_index = new_index;
    deque->end_segment = new_segment;

    deque->in_use++;
    return bbSuccess;

}

bbFlag bbSegmentedDeque_popFront(bbSegmentedDeque* deque, void** element)
{
    I32 new_index = deque->end_index - 1;
    I32 new_segment = deque->end_segment;
    if (new_index < 0)
    {
        new_index = BBSEGMENTEDDEQUE_SIZEOF - 1;
        new_segment--;      bbNotImplemented() //wrap around.
    }

    deque->end_index = new_index;
    deque->end_segment = new_segment;

    *element = &deque->elements[new_segment][new_index];

    deque->in_use--;
    return bbSuccess;

}
