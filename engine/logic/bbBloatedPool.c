
#include <stddef.h>
#include <stdlib.h>

#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbArithmetic.h"

bool bbBloatedPool_handleIsEqual(bbBloatedPool* UNUSED, bbHandle A, bbHandle B){
	return(A.bloated.collision == B.bloated.collision
			&& A.bloated.index == B.bloated.index);
};

#define IS_NULL(A) bbBloatedPool_handleIsEqual(NULL, A, pool->null)

bbFlag bbBloatedPool_print (bbBloatedPool* pool);

bbFlag bbBloatedPool_getHeader(bbBloatedPool_Header** header, void* address){
	size_t offset = offsetof(bbBloatedPool_Header, user_data);
	*header = address - offset;


	bbBloatedPool_Header* hdr= address - offset;
	hdr->file[1] = 'z';
	return bbSuccess;
}

bbFlag bbVPool_newBloated(bbVPool** Pool, I32 sizeOf, I32 level1, I32 level2){
    bbBloatedPool* BloatedPool;
    bbBloatedPool_new(&BloatedPool, sizeOf, level1, level2);
    bbVPool* pool = malloc(sizeof(bbVPool));
    pool->pool = BloatedPool;
    pool->type = VPoolType_bloated;
    pool->null = BloatedPool->null;
    pool->size_of = BloatedPool->size_of;
    pool->delete = (bbFlag (*)(void* pool)) bbBloatedPool_delete;
    pool->clear = (bbFlag (*)(void* pool)) bbBloatedPool_clear;
    pool->alloc_impl = (bbFlag(*)(void* pool, void** address, char* file, int
    line)) bbBloatedPool_allocImpl;
    pool->free = (bbFlag(*)(void* pool, void* address)) bbBloatedPool_free;
    pool->lookup = (bbFlag (*)(void* pool, void** address, bbHandle
    handle)) bbBloatedPool_lookup;
    pool->reverse_lookup = (bbFlag (*)(void* pool, void* address,
            bbHandle* handle)) bbBloatedPool_reverseLookup;
    pool->print_header = (bbFlag (*)(void *, void *)) bbBloatedPool_printHeader;
    pool->handle_is_equal = (bool (*)(void* USUSED, bbHandle A, bbHandle B)) bbBloatedPool_handleIsEqual;
    *Pool = pool;
    return bbSuccess;
}

bbFlag bbBloatedPool_new(bbBloatedPool** Pool, I32 sizeOf, I32 level1, I32
level2){
    //We might get errors if leve1, level2 are too small
    if (level1 < 1) level1 = 1;
    if (level2 < 5) level2 = 5;

    bbBloatedPool* pool = malloc(sizeof(bbBloatedPool) + level1 * sizeof
            (void*));
    I32 size = bbArith_roundUp(sizeOf, 8);
    pool->null.bloated.index = 0;
    pool->null.bloated.collision = 0;

    pool->size_of = size;
    pool->level1 = level1;
    pool->level2 = level2;
    pool->available.head = pool->null;
    pool->available.tail = pool->null;
    for(I32 i = 0; i < level1; i++){
        pool->elements[i] = NULL;
    }
    *Pool = pool;
    return bbSuccess;
}

bbFlag bbBloatedPool_delete(bbBloatedPool* pool){
    for(I32 i = 0; i < pool->level1; i++){
        free(pool->elements[i]);
    }
    free(pool);
    return bbSuccess;
}

bbFlag bbBloatedPool_clear(bbBloatedPool* pool){
    for(I32 i = 0; i < pool->level1; i++){
        free(pool->elements[i]);
        pool->elements[i] = NULL;
    }
    pool->available.head = pool->null;
    pool->available.tail = pool->null;
    return bbSuccess;
}

bbFlag bbBloatedPool_newHandle(bbBloatedPool* Pool, U32 lvl1index, U32
lvl2index, bbHandle* Handle){
    U32 index = lvl1index * Pool->level2 + lvl2index;
    U32 randint = rand();
    if (randint == 0) randint++;
    U32 collision = randint;
    //bbDebug("collision = %d\n", collision);
    bbHandle handle;
    handle.bloated.index = index;
    handle.bloated.collision = collision;
    *Handle = handle;
    return bbSuccess;
}

bbFlag bbBloatedPool_expand(bbBloatedPool* pool){
	bbAssert(IS_NULL(pool->available.head)
			 && IS_NULL(pool->available.tail),
			 "expanding non-empty pool");
	U32 i = 0;
	while (pool->elements[i] != NULL){
		i++;
		bbAssert(i<pool->level1, "Pool full\n");
	}

    int* array = calloc(23, sizeof (int));



	U8* level2 = calloc(pool->level2,
                                sizeof(bbBloatedPool_Header)
                                + pool->size_of * sizeof(U8)); //why is
                                // this?

	bbAssert(level2 != NULL, "calloc failed\n");

	U32 j = 0;
	bbBloatedPool_Header* element_A;
	bbBloatedPool_Header* element_B;

	element_A = (bbBloatedPool_Header *)&level2[j * (sizeof(bbBloatedPool_Header) + pool->size_of)];
	element_A->list.prev = pool->null;
	bbBloatedPool_newHandle(pool, i, j, &element_A->self);
	bbBloatedPool_newHandle(pool, i, j+1, &element_A->list.next);

	j++;

	while(j<pool->level2 - 1){
		element_B = (bbBloatedPool_Header *) &level2[j * (sizeof(bbBloatedPool_Header) + pool->size_of)];
		element_B->list.prev = element_A->self;
		element_B->self = element_A->list.next;
		bbBloatedPool_newHandle(pool, i, j+1, &element_B->list.next);
		element_A = element_B;

		j++;
	}

	element_B = (bbBloatedPool_Header *) &level2[j * (sizeof(bbBloatedPool_Header) + pool->size_of)];
	element_B->list.prev = element_A->self;
	//TODO the next line is a guess
	element_B->self = element_A->list.next;
	element_B->list.next = pool->null;
	element_A = (bbBloatedPool_Header *)&level2[0 * (sizeof(bbBloatedPool_Header) + pool->size_of)];


	pool->elements[i] = level2;
	pool->available.head = element_A->self;
	pool->available.tail = element_B->self;



	return bbSuccess;
}

//TODO - the rest of these functions