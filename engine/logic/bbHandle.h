/// Handle is used to address elements of pool, hold value in key-value pairs in dictionary, etc

#ifndef BB_HANDLE_H
#define BB_HANDLE_H

#include "engine/logic/bbIntTypes.h"

//may want to do a 16:48 split to allow 2^48 indices
typedef struct {
    U32 collision;
    U32 index;
} bbBloatedPool_Handle;

typedef union {
    void* ptr;
    bbBloatedPool_Handle bloated;
    U64 u64;
    I32x2 i32x2;
} bbHandle;

typedef struct
{
    bbHandle head;
    bbHandle tail;
} bbList_Handle;

typedef struct
{
    bbHandle prev;
    bbHandle next;
} bbListElement_Handle;

static const bbHandle no_handle = {0};
#endif // BB_HANDLE_H