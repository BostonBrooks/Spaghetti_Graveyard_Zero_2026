
#ifndef BB_SEGMENTED_QUEUE_MACROS_H
#define BB_SEGMENTED_QUEUE_MACROS_H


#define DECLARE_SQ_HEADER(BB_CLASS,BB_SIZEOF,BB_SEGMENT_SIZE)\
    \
    typedef struct {\
        U8 data[BB_SEGMENT_SIZE];\
    } CONCAT(BB_CLASS,_dummyStruct);\
    \
    \
    typedef struct\
    {\
        bbDummyStruct** elements;\
        I32 in_use;\
        I32 num_segments;\
\
        I32 start_segment;\
        I32 end_segment;\
\
        I32 start_index;\
        I32 end_index;\
\
    } CONCAT(BB_CLASS,_deque);\
\
bbFlag CONCAT(BB_CLASS,_deque_init(CONCAT(BB_CLASS,_deque)* deque, I32 num_segments);\
bbFlag CONCAT(BB_CLASS,_deque_allocFront(CONCAT(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT(BB_CLASS,_deque_pushFront(CONCAT(BB_CLASS,_deque)* deque, void* UNUSED);\
bbFlag CONCAT(BB_CLASS,_deque_peakFront(CONCAT(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT(BB_CLASS,_deque_popFront(CONCAT(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT(BB_CLASS,_deque_allocBack(CONCAT(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT(BB_CLASS,_deque_pushBack(CONCAT(BB_CLASS,_deque)* deque, void* UNUSED);\
bbFlag CONCAT(BB_CLASS,_deque_peakBack(CONCAT(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT(BB_CLASS,_deque_popBack(CONCAT(BB_CLASS,_deque)* deque, void** element);\


#define DECLARE_SQ_BODY(BB_CLASS,BB_SIZEOF,BB_SEGMENT_SIZE)\
bbFlag bbSegmentedDeque_init(CONCAT(BB_CLASS,_deque)* deque, I32 num_segments)\
{\
    deque->elements = calloc(num_segments, sizeof(bbDummyStruct*));\
    deque->elements[0] = calloc(BB_SEGMENT_SIZE, sizeof(bbDummyStruct));\
    deque->num_segments = num_segments;\
    deque->start_segment = 0;\
    deque->end_segment = 0;\
    deque->start_index = 0;\
    deque->end_index = 0;\
    deque->in_use = 0;\
\
    return bbSuccess;\
}\
\
bbFlag CONCAT(BB_CLASS,_deque_grow)(CONCAT(BB_CLASS,_deque)* deque)\
{\
    bbNotImplemented()\
    }\
\
bbFlag  CONCAT(BB_CLASS,_deque_peakFront)(CONCAT(BB_CLASS,_deque)* deque, void** element)\
{\
    if (deque->in_use == 0)\
    {\
        return bbNone;\
        *element = NULL;\
    }\
    CONCAT(BB_CLASS,_dummyStruct)* segment = deque->elements[deque->end_segment];\
    bbAssert(segment != NULL, "Segment not found\n");\
    if (element != NULL) *element = &segment[deque->end_index];\
    return bbSuccess;\
}\
\
\
bbFlag CONCAT(BB_CLASS,_allocFront)(CONCAT(BB_CLASS,_deque)* deque, void** element)\
{\
    I32 new_index = deque->end_index + 1;\
    I32 new_segment = deque->end_segment;\
    if (new_index >= BB_SEGMENT_SIZE)\
    {\
        new_index = 0;\
        new_segment++;        bbNotImplemented()\
        if (new_segment >= BB_SEGMENT_SIZE)\
        {\
            new_segment = 0;\
        }\
    }\
    bbAssert((new_index != deque->start_index\
                        || new_segment != deque->start_segment)\
                        || deque->in_use == 0,"deque full\n");\
    CONCAT(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, sizeof(bbDummyStruct));\
        deque->elements[new_segment] = segment;\
    }\
    *element = &deque->elements[new_segment][new_index];\
    return bbSuccess;\
}\
\
bbFlag CONCAT(BB_CLASS,_pushFront)(CONCAT(BB_CLASS,_deque)* deque, void* element)\
{\
    I32 new_index = deque->end_index + 1;\
    I32 new_segment = deque->end_segment;\
    if (new_index >= BB_SEGMENT_SIZE)\
    {\
        new_index = 0;\
        new_segment++;        bbNotImplemented()\
\
        if (new_segment >= BB_SEGMENT_SIZE)\
        {\
            new_segment = 0;\
        }\
    }\
    bbAssert((new_index != deque->start_index\
                        || new_segment != deque->start_segment)\
                        || deque->in_use == 0,"deque full\n");\
\
    bbDummyStruct* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, sizeof(CONCAT(BB_CLASS,_dummyStruct)));\
        deque->elements[new_segment] = segment;\
    }\
    deque->end_index = new_index;\
    deque->end_segment = new_segment;\
    deque->in_use++;\
    return bbSuccess;\
}\

#endif