///Bloated pool implements features and checks that might slow down execution

#ifndef BB_BLOATEDPOOL_H
#define BB_BLOATEDPOOL_H

#include "engine/data/bbConstants.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbIntTypes.h"


typedef struct
{
    char pool_name[KEY_LENGTH];
    bbHandle null;
    U32 level1;
    U32 level2;
    U32 size_of;
    bbList_Handle available;
    void* elements[];
} bbBloatedPool;

typedef struct
{
    bbHandle self;
    bbListElement_Handle list;
    bool in_use;
    I32 line;
    char file[KEY_LENGTH];
    alignas(8) U8 user_data[];
} bbBloatedPool_Header;


bbFlag bbVPool_newBloated(bbVPool** pool, I32 size_of, I32 level1, I32 level2, char* key);
bbFlag bbBloatedPool_new(
        bbBloatedPool** pool, I32 size_of, I32 level1, I32 level2, char* key);
bbFlag bbBloatedPool_delete(bbBloatedPool* pool);
bbFlag bbBloatedPool_clear(bbBloatedPool* pool);
bbFlag bbBloatedPool_allocImpl(bbBloatedPool* pool, void** address, bbHandle* handle, char* file, I32 line);
bbFlag bbBloatedPool_free(bbBloatedPool* pool, void* address);

bbFlag bbBloatedPool_lookup(bbBloatedPool* pool, void** address, bbHandle handle);

///returns bbFail if element not in use
bbFlag bbBloatedPool_lookup2(bbBloatedPool* pool, void** address, bbHandle handle);
bbFlag bbBloatedPool_reverseLookup(bbBloatedPool* pool, void* address, bbHandle* handle);
bbFlag bbBloatedPool_printHeader(bbBloatedPool* pool, void* address);
bool bbBloatedPool_handleIsEqual(bbBloatedPool* USUSED, bbHandle A, bbHandle B);

//Get the header used to track allocation of elements in pool
bbFlag bbBloatedPool_lookupHeader(bbBloatedPool* pool, void** address, bbHandle handle);
bbFlag bbBloatedPool_allocFromHandle(bbBloatedPool* pool, void** address, bbHandle handle, char* file, I32 line);


#define bbBloatedPool_alloc(pool, address)\
bbBloatedPool_allocImpl(pool, address,  NULL, __FILE_NAME__, __LINE__);

#define bbBloatedPool_alloc2(pool, address, handle)\
bbBloatedPool_allocImpl(pool, address,  handle, __FILE_NAME__, __LINE__);
#endif //BB_BLOATEDPOOL_H