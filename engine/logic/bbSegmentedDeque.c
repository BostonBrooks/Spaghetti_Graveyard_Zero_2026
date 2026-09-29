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
    if (deque->in_use == 0)
    {
        return bbNone;
        *element = NULL;
    }
    bbDummyStruct* segment = deque->elements[deque->end_segment];
    bbAssert(segment != NULL, "Segment not found\n");
    if (element != NULL) *element = &segment[deque->end_index];
    return bbSuccess;
}



bbFlag bbSegmentedDeque_allocFront(bbSegmentedDeque* deque, void** element)
{




    I32 new_index = deque->end_index + 1;
    I32 new_segment = deque->end_segment;
    if (new_index >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
    {
        new_index = 0;
        new_segment++;        bbNotImplemented()

        if (new_segment >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
        {
            new_segment = 0;
        }
    }


    bbAssert((new_index != deque->start_index
                        || new_segment != deque->start_segment)
                        || deque->in_use == 0,"deque full\n");

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
        new_segment++;        bbNotImplemented()

        if (new_segment >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
        {
            new_segment = 0;
        }
    }

    bbAssert((new_index != deque->start_index
                        || new_segment != deque->start_segment)
                        || deque->in_use == 0,"deque full\n");

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
    if (deque->in_use == 0)
    {
        return bbNone;
        if (element != NULL) *element = NULL;
    }

    I32 new_index = deque->end_index - 1;
    I32 new_segment = deque->end_segment;



    if (new_index < 0)
    {
        new_index = BBSEGMENTEDDEQUE_SEGMENT_SIZE - 1;
        new_segment--;      bbNotImplemented()
        if (new_segment <0)
        {
            new_segment = deque->num_segments - 1;
        }
    }

    deque->end_index = new_index;
    deque->end_segment = new_segment;

    bbDummyStruct* segment = deque->elements[new_segment];

    bbAssert(segment != NULL, "Segment not found\n");
        if (element != NULL) *element = &segment[new_index];
    deque->in_use--;
    return bbSuccess;

}



bbFlag bbSegmentedDeque_allocBack(bbSegmentedDeque* deque, void** element)
{




    I32 new_index = deque->start_index - 1;
    I32 new_segment = deque->start_segment;

    if (new_index < 0)
    {
        new_index = BBSEGMENTEDDEQUE_SEGMENT_SIZE - 1;
        new_segment--;      bbNotImplemented()
        if (new_segment <0)
        {
            new_segment = deque->num_segments - 1;
        }
    }

    bbAssert((new_index != deque->start_index
                        || new_segment != deque->start_segment)
                        || deque->in_use == 0,"deque full\n");

    bbDummyStruct* segment = deque->elements[new_segment];
    if (segment == NULL)
    {
        segment = calloc(BBSEGMENTEDDEQUE_SEGMENT_SIZE, sizeof(bbDummyStruct));
        deque->elements[new_segment] = segment;
    }

    *element = &deque->elements[new_segment][new_index];

    return bbSuccess;

}

bbFlag bbSegmentedDeque_pushBack(bbSegmentedDeque* deque, void* element)
{
    I32 new_index = deque->start_index - 1;
    I32 new_segment = deque->start_segment;

    if (new_index < 0)
    {
        new_index = BBSEGMENTEDDEQUE_SEGMENT_SIZE - 1;
        new_segment--;      bbNotImplemented()
        if (new_segment <0)
        {
            new_segment = deque->num_segments - 1;
        }
    }

    bbAssert((new_index != deque->start_index
                        || new_segment != deque->start_segment)
                        || deque->in_use == 0,"deque full\n");

    bbDummyStruct* segment = deque->elements[new_segment];
    if (segment == NULL)
    {
        segment = calloc(BBSEGMENTEDDEQUE_SEGMENT_SIZE, sizeof(bbDummyStruct));
        deque->elements[new_segment] = segment;
    }

    deque->start_index = new_index;
    deque->start_segment = new_segment;

    deque->in_use++;
    return bbSuccess;

}

bbFlag bbSegmentedDeque_peakBack(bbSegmentedDeque* deque, void** element)
{

    if (deque->in_use == 0)
    {
        return bbNone;
        *element = NULL;
    }
    bbDummyStruct* segment = deque->elements[deque->start_segment];

    bbAssert(segment != NULL, "Segment not found\n");

    if (element != NULL) *element = &segment[deque->start_index];

    return bbSuccess;
}


bbFlag bbSegmentedDeque_popBack(bbSegmentedDeque* deque, void** element)
{
    if (deque->in_use == 0)
    {
        return bbNone;
        if (element != NULL) *element = NULL;
    }
    I32 new_index = deque->start_index + 1;
    I32 new_segment = deque->start_segment;

    if (new_index >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
    {
        new_index = 0;
        new_segment++;        bbNotImplemented()

        if (new_segment >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
        {
            new_segment = 0;
        }
    }




    deque->start_index = new_index;
    deque->start_segment = new_segment;

    bbDummyStruct* segment = deque->elements[new_segment];

    bbAssert(segment != NULL, "Segment not found\n");
        if (element != NULL) *element = &segment[new_index];
    deque->in_use--;
    return bbSuccess;

}


//typedef bbFlag bbSegmentedDequeFunction(bbSegmentedDeque* deque, void* node, void* cl);

bbFlag bbSegmentedDeque_mapL(bbSegmentedDeque* deque, bbSegmentedDequeFunction* myFunc, void* cl) {
    I32 index = deque->start_index;
    I32 segment_int = deque->start_segment;
    bbDummyStruct* segment = deque->elements[segment_int];
    bbDummyStruct* element = &segment[index];
    while (1) {
        while (1) {
            myFunc(deque, element,cl);
            if (segment_int == deque->end_segment || segment_int == deque->end_segment) goto Label_Name;
            index++;
            if (index >= BBSEGMENTEDDEQUE_SEGMENT_SIZE)
            {
                index = 0;
                break;
            }
        }
        segment_int++;
        if (segment_int >= deque->num_segments) segment_int++;
    }
    Label_Name:

    return bbSuccess;
}