
#include <stddef.h>
#include <stdlib.h>

#include "engine/logic/bbBloatedPool.h"

#include "bbString.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbArithmetic.h"


//Elements available to be allocated are stored in a non-circular list

bool bbBloatedPool_handleIsEqual(bbBloatedPool* UNUSED, bbHandle A, bbHandle B){
	return(A.bloated.collision == B.bloated.collision
			&& A.bloated.index == B.bloated.index);
};

#define IS_NULL(A) bbBloatedPool_handleIsEqual(NULL, A, pool->null)

bbFlag bbBloatedPool_print (bbBloatedPool* pool);

bbFlag bbBloatedPool_getHeader(bbBloatedPool_Header** header, void* address){
	size_t offset = offsetof(bbBloatedPool_Header, user_data);
	*header = address - offset;
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
//    pool->print_header = (bbFlag (*)(void *, void *)) bbBloatedPool_printHeader;
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
		bbAssert(i < pool->level1, "Pool full\n");
	}

//each element in level 2 has size = size of header + size of user data
	U8* level2 = calloc(pool->level2,
                                sizeof(bbBloatedPool_Header)
                                + pool->size_of * sizeof(U8));

	bbAssert(level2 != NULL, "calloc failed\n");

	U32 j = 0;
	bbBloatedPool_Header* element_A;
	bbBloatedPool_Header* element_B;

	element_A = (bbBloatedPool_Header *)&level2[j * (sizeof(bbBloatedPool_Header) + pool->size_of)];
	element_A->list.prev = pool->null;
	bbBloatedPool_newHandle(pool, i, j, &element_A->self);
	bbBloatedPool_newHandle(pool, i, j+1, &element_A->list.next);

	j++;

	while(j < pool->level2 - 1){
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

bbFlag bbBloatedPool_allocImpl(bbBloatedPool* pool, void** address, char* file, I32 line)
{
	bbAssert(address != NULL, "NULL pointer");

	//If no elements available
	if (IS_NULL(pool->available.head) || IS_NULL(pool->available.tail))
	{
		bbAssert(bbBloatedPool_handleIsEqual(NULL,pool->available.head,pool->available.tail),
			"head/tail mismatch\n");
		bbBloatedPool_expand(pool);
	}

	//If one element available
	if (bbBloatedPool_handleIsEqual(NULL, pool->available.head,
								pool->available.tail))
	{
        bbBloatedPool_Header *element;
		void* element_address;
		bbHandle element_handle = pool->available.head;
		bbBloatedPool_lookup(pool, &element_address, element_handle);
		bbBloatedPool_getHeader(&element, element_address);

		pool->available.head = pool->null;
		pool->available.tail = pool->null;

		//element in use is not in available list
		element->list.prev = pool->null;
		element->list.next = pool->null;


		*address = &element->user_data;
		return bbSuccess;
	}

	//Then, more than one element available

	bbHandle head_handle = pool->available.head;
	void *head_address;
	bbBloatedPool_Header *head_header;
	bbBloatedPool_lookup(pool, &head_address, head_handle);
	bbBloatedPool_getHeader(&head_header, head_address);

	bbHandle next_handle = head_header->list.next;
	void *next_address;
	bbBloatedPool_Header *next_header;
	bbBloatedPool_lookup(pool, &next_address, next_handle);
	bbBloatedPool_getHeader(&next_header, next_address);

	next_header->list.prev = pool->null;
	pool->available.head = next_handle;

	head_header->list.prev = pool->null;
	head_header->list.next = pool->null;
	head_header->in_use = true;
	head_header->line = line;
	bbStr_setStr(head_header->file, file, KEY_LENGTH);

	*address = &head_header->user_data;

	return bbSuccess;
}

bbFlag bbBloatedPool_Handle_incrementCollision(bbHandle* handle){
	U32 collision = handle->bloated.collision;
	collision++;
	if(collision == 0) collision++;
	handle->bloated.collision = collision;
	return bbSuccess;
}


bbFlag bbBloatedPool_free(bbBloatedPool* pool, void* address)
{
	bbBloatedPool_Header* header;
	bbBloatedPool_getHeader(&header, address);
	bbBloatedPool_Handle_incrementCollision(&header->self);

	//return element to empty pool
	if (IS_NULL(pool->available.head) || IS_NULL(pool->available.tail))
	{
		bbAssert(bbBloatedPool_handleIsEqual(NULL,pool->available.head,pool->available.tail ),
			"head/tail mismatch\n");

		pool->available.head = header->self;
		pool->available.tail = header->self;
		header->list.prev = pool->null;
		header->list.next = pool->null;
		header->in_use = false;
		return bbSuccess;

	}
	bbHandle available_handle = pool->available.head;
	bbBloatedPool_Header* available_header;
	void* available_address;

	bbBloatedPool_lookup(pool, &available_address, available_handle);
	bbBloatedPool_getHeader(&available_header, available_address);

	available_header->list.prev = header->self;
	pool->available.head = header->self;
	header->list.prev = pool->null;
	header->list.next = available_header->self;
	available_header->in_use = false;
	return bbSuccess;
}

bbFlag bbBloatedPool_lookupHeader(bbBloatedPool* pool, void** address, bbHandle handle){

	if ( handle.bloated.index == 0 && handle.bloated.collision == 0)
	{
		*address = NULL;
		return bbFail;
	}

	U32 index = handle.bloated.index;
	U32 collision = handle.bloated.collision;
	U32 lvl1index = index / pool->level2;
	bbAssert(lvl1index < pool->level1, "index out of bounds\n");
	U32 lvl2index = index % pool->level2;
	U8* lvl2 = pool->elements[lvl1index];
	bbBloatedPool_Header *element = (bbBloatedPool_Header *)&lvl2[lvl2index * (sizeof(bbBloatedPool_Header) + pool->size_of)];
	bbHandle elementHandle = element->self;
	bbAssert(handle.bloated.collision == elementHandle.bloated.collision,
			 "handle collision\n");

	*address = element;
	return bbSuccess;
}

bbFlag bbBloatedPool_lookup(bbBloatedPool* pool, void** address, bbHandle handle){
	bbBloatedPool_Header* element;
	bbFlag flag = bbBloatedPool_lookupHeader(pool, (void**)&element, handle);

	if (flag == bbSuccess) *address = &element->user_data;
	else *address = NULL;

	return flag;
}

bbFlag bbBloatedPool_reverseLookup(bbBloatedPool* pool, void* address, bbHandle* handle){
	bbAssert(handle != NULL, "handle is NULL\n");
	bbAssert(address != NULL, "address is NULL\n");
	bbAssert(pool != NULL, "pool is NULL\n");
	bbBloatedPool_Header* element;
	bbBloatedPool_getHeader(&element, address);
	*handle = element->self;
	return bbSuccess;
}
