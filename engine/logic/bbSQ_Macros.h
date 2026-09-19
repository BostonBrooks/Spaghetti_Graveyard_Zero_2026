
#ifndef BB_SEGMENTED_QUEUE_MACROS_H
#define BB_SEGMENTED_QUEUE_MACROS_H

//I cant remember why I decided to do this many years ago
#define CONCAT1(x,y) x##y
#define CONCAT2(x,y) CONCAT1(x,y)

///Declare headers for segmented deque
#define DECLARE_SQ_HEADER(BB_CLASS,BB_SIZEOF,BB_SEGMENT_SIZE)\
    \
    typedef struct {\
        U8 data[BB_SEGMENT_SIZE];\
    } CONCAT2(BB_CLASS,_dummyStruct);\
    \
    \
    typedef struct\
    {\
        CONCAT2(BB_CLASS,_dummyStruct)** elements;\
        I32 in_use;\
        I32 num_segments;\
\
        I32 start_segment;\
        I32 end_segment;\
\
        I32 start_index;\
        I32 end_index;\
\
    } CONCAT2(BB_CLASS,_deque);\
\
bbFlag CONCAT2(BB_CLASS,_deque_init)(CONCAT2(BB_CLASS,_deque)* deque, I32 num_segments);\
bbFlag CONCAT2(BB_CLASS,_deque_allocFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT2(BB_CLASS,_deque_pushFront)(CONCAT2(BB_CLASS,_deque)* deque, void* UNUSED);\
bbFlag CONCAT2(BB_CLASS,_deque_peakFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT2(BB_CLASS,_deque_popFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT2(BB_CLASS,_deque_allocBack)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT2(BB_CLASS,_deque_pushBack)(CONCAT2(BB_CLASS,_deque)* deque, void* UNUSED);\
bbFlag CONCAT2(BB_CLASS,_deque_peakBack)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\
bbFlag CONCAT2(BB_CLASS,_deque_popBack)(CONCAT2(BB_CLASS,_deque)* deque, void** element);\

///Define functions for segmented queue
#define DECLARE_SQ_BODY(BB_CLASS,BB_SIZEOF,BB_SEGMENT_SIZE)\
bbFlag CONCAT2(BB_CLASS,_deque_init)(CONCAT2(BB_CLASS,_deque)* deque, I32 num_segments)\
{\
    deque->elements = calloc(num_segments, sizeof(CONCAT2(BB_CLASS,_dummyStruct)*));\
    deque->elements[0] = calloc(BB_SEGMENT_SIZE, sizeof(CONCAT2(BB_CLASS,_dummyStruct)));\
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
bbFlag CONCAT2(BB_CLASS,_deque_grow)(CONCAT2(BB_CLASS,_deque)* deque)\
{\
    bbNotImplemented()\
    }\
\
\
bbFlag CONCAT2(BB_CLASS,_deque_allocFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element)\
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
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, sizeof(bbDummyStruct));\
        deque->elements[new_segment] = segment;\
    }\
    *element = &deque->elements[new_segment][new_index];\
    return bbSuccess;\
}\
bbFlag CONCAT2(BB_CLASS,_deque_pushFront)(CONCAT2(BB_CLASS,_deque)* deque, void* UNUSED)\
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
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, sizeof(CONCAT2(BB_CLASS,_dummyStruct)));\
        deque->elements[new_segment] = segment;\
    }\
    deque->end_index = new_index;\
    deque->end_segment = new_segment;\
    deque->in_use++;\
    return bbSuccess;\
}\
bbFlag  CONCAT2(BB_CLASS,_deque_peakFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element)\
{\
    if (deque->in_use == 0)\
    {\
        return bbNone;\
        *element = NULL;\
    }\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[deque->end_segment];\
    bbAssert(segment != NULL, "Segment not found\n");\
    if (element != NULL) *element = &segment[deque->end_index];\
    return bbSuccess;\
}\
\
\
\
bbFlag CONCAT2(BB_CLASS,_deque_popFront)(CONCAT2(BB_CLASS,_deque)* deque, void** element)\
{\
    if (deque->in_use == 0)\
    {\
        return bbNone;\
        if (element != NULL) *element = NULL;\
    }\
    I32 new_index = deque->end_index - 1;\
    I32 new_segment = deque->end_segment;\
    if (new_index < 0)\
    {\
        new_index = BB_SEGMENT_SIZE - 1;\
        new_segment--;      bbNotImplemented()\
        if (new_segment <0)\
        {\
            new_segment = deque->num_segments - 1;\
        }\
    }\
    deque->end_index = new_index;\
    deque->end_segment = new_segment;\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    bbAssert(segment != NULL, "Segment not found\n");\
    if (element != NULL) *element = &segment[new_index];\
    deque->in_use--;\
    return bbSuccess;\
}\
\
bbFlag CONCAT2(BB_CLASS,_deque_allocBack)(CONCAT2(BB_CLASS,_deque)* deque, void** element)\
{\
    I32 new_index = deque->start_index - 1;\
    I32 new_segment = deque->start_segment;\
    if (new_index < 0)\
    {\
        new_index = BB_SEGMENT_SIZE - 1;\
        new_segment--;      bbNotImplemented()\
        if (new_segment <0)\
        {\
            new_segment = deque->num_segments - 1;\
        }\
    }\
    bbAssert((new_index != deque->start_index\
                        || new_segment != deque->start_segment)\
                        || deque->in_use == 0,"deque full\n");\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, CONCAT2(BB_CLASS,_dummyStruct));\
        deque->elements[new_segment] = segment;\
    }\
    *element = &deque->elements[new_segment][new_index];\
    return bbSuccess;\
}\
\
\
bbFlag CONCAT2(BB_CLASS,_deque_pushBack)(CONCAT2(BB_CLASS,_deque)* deque, void* UNUSED)\
{\
    I32 new_index = deque->start_index - 1;\
    I32 new_segment = deque->start_segment;\
    if (new_index < 0)\
    {\
        new_index = BB_SEGMENT_SIZE - 1;\
        new_segment--;      bbNotImplemented()\
        if (new_segment <0)\
        {\
            new_segment = deque->num_segments - 1;\
        }\
    }\
    bbAssert((new_index != deque->start_index\
                        || new_segment != deque->start_segment)\
                        || deque->in_use == 0,"deque full\n");\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    if (segment == NULL)\
    {\
        segment = calloc(BB_SEGMENT_SIZE, sizeof(CONCAT2(BB_CLASS,_dummyStruct)));\
        deque->elements[new_segment] = segment;\
    }\
    deque->start_index = new_index;\
    deque->start_segment = new_segment;\
    deque->in_use++;\
    return bbSuccess;\
}\
\
bbFlag CONCAT2(BB_CLASS,_deque_peakBack)(CONCAT2(BB_CLASS,_deque)* deque, void** element)\
{\
    if (deque->in_use == 0)\
    {\
        return bbNone;\
        *element = NULL;\
    }\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[deque->start_segment];\
    bbAssert(segment != NULL, "Segment not found\n");\
    if (element != NULL) *element = &segment[deque->start_index];\
    return bbSuccess;\
}\
\
\
bbFlag bbSegmentedDeque_popBack(bbSegmentedDeque* deque, void** element)\
{\
    if (deque->in_use == 0)\
    {\
        return bbNone;\
        if (element != NULL) *element = NULL;\
    }\
    I32 new_index = deque->start_index + 1;\
    I32 new_segment = deque->start_segment;\
    if (new_index >= BB_SEGMENT_SIZE)\
    {\
        new_index = 0;\
        new_segment++;        bbNotImplemented()\
        if (new_segment >= BB_SEGMENT_SIZE)\
        {\
            new_segment = 0;\
        }\
    }\
    deque->start_index = new_index;\
    deque->start_segment = new_segment;\
    CONCAT2(BB_CLASS,_dummyStruct)* segment = deque->elements[new_segment];\
    bbAssert(segment != NULL, "Segment not found\n");\
    if (element != NULL) *element = &segment[new_index];\
    deque->in_use--;\
    return bbSuccess;\
}\


*/
#endif