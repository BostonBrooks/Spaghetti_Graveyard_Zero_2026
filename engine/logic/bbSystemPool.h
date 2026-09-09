#ifndef BB_SYSTEMPOOL_H
#define BB_SYSTEMPOOL_H

#include "engine/logic/bbVPool.h"
#include "engine/logic/bbFlag.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbIntTypes.h"


typedef struct
{
    char pool_name[KEY_LENGTH];
    U16 system;
    bbHandle null;
    U32 level1;
    U32 level2;
    U32 size_of;
    bbList_Handle available;
    void* elements[];
} bbSystemPool;

typedef struct
{
    bbHandle self;
    bbListElement_Handle list;
    bool in_use;
    I32 line;
    char file[KEY_LENGTH];
    alignas(8) U8 user_data[];
} bbSystemPool_Header;



bbFlag bbVPool_newSystem(bbVPool** pool,
        U16 system,
        I32 size_of,
        I32 level1,
        I32 level2,
        char* key);

bbFlag bbSystemPool_new(
        bbSystemPool** pool,
        U16 system,
        I32 size_of,
        I32 level1,
        I32 level2,
        char* key);

/* Functions provided to bbVPool
bbFlag (*delete)(void* pool);
bbFlag (*clear)(void* pool);
bbFlag (*alloc_impl)(void* pool, void** address, bbHandle* handle, char* file, int line);
bbFlag (*free)(void* pool, void* address);
bbFlag (*lookup)(void* pool, void** address, bbHandle handle);
bbFlag (*reverse_lookup)(void* pool, void* address, bbHandle* handle);
bbFlag (*print_header)(void* pool, void* address);
bool (*handle_is_equal)(void* pool, bbHandle a, bbHandle b);
bbFlag (*alloc_from_handle)(void* pool, void** address, bbHandle handle, char* file, I32 line);
*/
///Delete Pool
bbFlag bbSystemPool_delete(bbSystemPool* pool);

///Clear Pool
bbFlag bbSystemPool_clear(bbSystemPool* pool);

///Allocate an element then return address and handle by reference
bbFlag bbSystemPool_allocImpl(bbSystemPool* pool, void** address, bbHandle* handle, char* file, int line);

///De-allocate an element
bbFlag bbSystemPool_free(bbSystemPool* pool, void* address);

///Lookup an element from handle
bbFlag bbSystemPool_lookup(bbSystemPool* pool, void** address, bbHandle handle);

///Lookup a handle from element
bbFlag bbSystemPool_reverseLookup(bbSystemPool* pool, void* address, bbHandle* handle);

///Print element meta-data
bbFlag bbSystemPool_printHeader(bbSystemPool* pool, void* address);

///return true if handles are equal. return different flag for system/index/generation mismatch
bbFlag bbSystemPool_handleIsEqual(bbSystemPool* UNUSED, bbHandle a, bbHandle b);

///return bbSuccess if handle is not NULL
bbFlag bbSystemPool_handleIsNULL(bbVPool* pool, bbHandle A);

///Get the header used to track allocation of elements in pool
bbFlag bbSystemPool_lookupHeader(bbSystemPool* pool, void** address, bbHandle handle);

///Allocate an element with the given handle.
bbFlag bbSystemPool_allocFromHandle(bbSystemPool* pool, void** address, bbHandle handle, char* file, I32 line);




#endif // BB_SYSTEMPOOL_H