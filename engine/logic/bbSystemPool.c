
#include <stddef.h>
#include <stdlib.h>

#include "engine/logic/bbSystemPool.h"

#include "bbString.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbArithmetic.h"


//Elements available to be allocated are stored in a non-circular list

bbFlag bbSystemPool_handleIsEqual(bbSystemPool* UNUSED, bbHandle A, bbHandle B){
	if (A.system.system != B.system.system) return bbHandleError_System;
	if (A.system.index != B.system.index) return bbIndexMismatch;
	if (A.system.generation != B.system.generation) return bbGenerationMismatch;
	return  bbSuccess;
};

bool bbSystemPool_handleIsNULL(bbHandle handle){
	return (handle.system.generation == 0);
};

#define IS_NULL(A) bbSystemPool_handleIsNULL(A)

bbFlag bbSystemPool_print (bbSystemPool* pool);

bbFlag bbSystemPool_getHeader(bbSystemPool_Header** header, void* address){
	size_t offset = offsetof(bbSystemPool_Header, user_data);
	*header = address - offset;
	return bbSuccess;
}

bbFlag bbVPool_newSystem(bbVPool** Pool,
		U16 system,
		I32 size_of,
		I32 level1,
		I32 level2,
		char* key){
    bbSystemPool* SystemPool;
    bbSystemPool_new(&SystemPool,system, size_of, level1, level2, key);
    bbVPool* pool = malloc(sizeof(bbVPool));
	bbStr_setStr(pool->pool_name,key,KEY_LENGTH);
    pool->pool = SystemPool;
	pool->system = system;
    pool->type = VPoolType_bloated;
    pool->null = SystemPool->null;
    pool->size_of = SystemPool->size_of;
    pool->delete = (bbFlag (*)(void* pool)) bbSystemPool_delete;
    pool->clear = (bbFlag (*)(void* pool)) bbSystemPool_clear;
    pool->alloc_impl = (bbFlag(*)(void* pool, void** address, bbHandle* handle, char* file, int
    line)) bbSystemPool_allocImpl;
    pool->free = (bbFlag(*)(void* pool, void* address)) bbSystemPool_free;
    pool->lookup = (bbFlag (*)(void* pool, void** address, bbHandle
    handle)) bbSystemPool_lookup;
    pool->reverse_lookup = (bbFlag (*)(void* pool, void* address,
            bbHandle* handle)) bbSystemPool_reverseLookup;
    pool->handle_is_equal = (bbFlag (*)(void* USUSED, bbHandle A, bbHandle B)) bbSystemPool_handleIsEqual;
	pool->alloc_from_handle = (bbFlag (*)(void* pool, void** address, bbHandle handle, char* file, int
	line)) bbSystemPool_allocFromHandle;
	*Pool = pool;
    return bbSuccess;
}
bbFlag bbSystemPool_expand(bbSystemPool* pool);
bbFlag bbSystemPool_new(bbSystemPool** Pool, U16 system, I32 sizeOf, I32 level1, I32
level2, char* key){
    //We might get errors if leve1, level2 are too small
    if (level1 < 1) level1 = 1;
    if (level2 < 5) level2 = 5;

    bbSystemPool* pool = malloc(sizeof(bbSystemPool) + level1 * sizeof
            (void*));

    I32 size = bbArith_roundUp(sizeOf, 8);
    pool->null.bloated.index = 0;
    pool->null.bloated.collision = 0;

	pool->system = system;
	bbStr_putStr(pool->pool_name,key, KEY_LENGTH);
    pool->size_of = size;
    pool->level1 = level1;
    pool->level2 = level2;
    pool->available.head = pool->null;
    pool->available.tail = pool->null;
    for(I32 i = 0; i < level1; i++){
        pool->elements[i] = NULL;
    }


	bbSystemPool_expand(pool);
    *Pool = pool;
    return bbSuccess;
}

bbFlag bbSystemPool_delete(bbSystemPool* pool){
    for(I32 i = 0; i < pool->level1; i++){
        free(pool->elements[i]);
    }
    free(pool);
    return bbSuccess;
}

bbFlag bbSystemPool_clear(bbSystemPool* pool){
    for(I32 i = 0; i < pool->level1; i++){
        free(pool->elements[i]);
        pool->elements[i] = NULL;
    }
    pool->available.head = pool->null;
    pool->available.tail = pool->null;
    return bbSuccess;
}

bbFlag bbSystemPool_newHandle(bbSystemPool* Pool, U32 lvl1index, U32
lvl2index, bbHandle* Handle){
    U32 index = lvl1index * Pool->level2 + lvl2index;
    bbHandle handle;
    handle.system.index = index;
	handle.system.system = Pool->system;
    handle.system.generation = 1;
    *Handle = handle;
    return bbSuccess;
}

bbFlag bbSystemPool_expand(bbSystemPool* pool){
	bbAssert(IS_NULL(pool->available.head)
			 && IS_NULL(pool->available.tail),
			 "expanding non-empty pool");
	U32 i = 0;
	while (pool->elements[i] != NULL){
		i++;
		bbAssert(i < pool->level1, "%s pool full\n", pool->pool_name);
	}

//each element in level 2 has size = size of header + size of user data
	U8* level2 = calloc(pool->level2,
                                sizeof(bbSystemPool_Header)
                                + pool->size_of * sizeof(U8));

	bbAssert(level2 != NULL, "calloc failed\n");

	U32 j = 0;
	bbSystemPool_Header* element_A;
	bbSystemPool_Header* element_B;

	element_A = (bbSystemPool_Header *)&level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
	element_A->list.prev = pool->null;
	bbSystemPool_newHandle(pool, i, j, &element_A->self);
	bbSystemPool_newHandle(pool, i, j+1, &element_A->list.next);
	element_A->in_use = false;
	j++;

	while(j < pool->level2 - 1){
		element_B = (bbSystemPool_Header *) &level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
		element_B->list.prev = element_A->self;
		element_B->self = element_A->list.next;
		bbSystemPool_newHandle(pool, i, j+1, &element_B->list.next);
		element_A->in_use = false;
		element_A = element_B;

		j++;
	}
	element_B->in_use = false;
	element_B = (bbSystemPool_Header *) &level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
	element_B->list.prev = element_A->self;
	//TODO the next line is a guess
	element_B->self = element_A->list.next;
	element_B->list.next = pool->null;
	element_A = (bbSystemPool_Header *)&level2[0 * (sizeof(bbSystemPool_Header) + pool->size_of)];

	pool->elements[i] = level2;
	pool->available.head = element_A->self;
	pool->available.tail = element_B->self;



	return bbSuccess;
}

bbFlag bbSystemPool_expandHandle(bbSystemPool* pool, bbHandle handle){

	U32 i = handle.bloated.index / pool->level2;
	while (pool->elements[i] != NULL){
		i++;
		bbAssert(i < pool->level1, "%s pool full\n", pool->pool_name);
	}

	//each element in level 2 has size = size of header + size of user data
	U8* level2 = calloc(pool->level2,
								sizeof(bbSystemPool_Header)
								+ pool->size_of * sizeof(U8));

	bbAssert(level2 != NULL, "calloc failed\n");

	U32 j = 0;
	bbSystemPool_Header* element_A;
	bbSystemPool_Header* element_B;

	if (IS_NULL(pool->available.tail))
	{
		element_A = (bbSystemPool_Header *)&level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
		element_A->list.prev = pool->null;
		bbSystemPool_newHandle(pool, i, j, &element_A->self);
		bbSystemPool_newHandle(pool, i, j+1, &element_A->list.next);
		element_A->in_use = false;

		pool->available.head = element_A->self;
		j++;
	} else
	{
		bbSystemPool_lookupHeader(pool,(void**)&element_A,pool->available.tail);
		bbSystemPool_newHandle(pool, i, j, &element_A->list.next);
	}

	while(j < pool->level2 - 1){
		element_B = (bbSystemPool_Header *) &level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
		element_B->list.prev = element_A->self;
		element_B->self = element_A->list.next;
		bbSystemPool_newHandle(pool, i, j+1, &element_B->list.next);
		element_A->in_use = false;
		element_A = element_B;

		j++;
	}
	element_A->in_use = false;
	element_B = (bbSystemPool_Header *) &level2[j * (sizeof(bbSystemPool_Header) + pool->size_of)];
	element_B->list.prev = element_A->self;
	element_B->self = element_A->list.next;
	element_B->list.next = pool->null;
	element_A = (bbSystemPool_Header *)&level2[0 * (sizeof(bbSystemPool_Header) + pool->size_of)];


	pool->elements[i] = level2;
	pool->available.tail = element_B->self;



	return bbSuccess;
}


bbFlag bbSystemPool_allocImpl(bbSystemPool* pool, void** address, bbHandle* handle, char* file, I32 line)
{

	//If no elements available
	if (IS_NULL(pool->available.head) || IS_NULL(pool->available.tail))
	{
		bbAssert(bbSuccess == bbSystemPool_handleIsEqual(NULL,pool->available.head,pool->available.tail),
			"head/tail mismatch\n");
		bbSystemPool_expand(pool);
	}

	//If one element available
	if (bbSuccess == bbSystemPool_handleIsEqual(NULL, pool->available.head,
								pool->available.tail))
	{
        bbSystemPool_Header *element;
		void* element_address;
		bbHandle element_handle = pool->available.head;
		bbSystemPool_lookup(pool, &element_address, element_handle);
		bbSystemPool_getHeader(&element, element_address);

		pool->available.head = pool->null;
		pool->available.tail = pool->null;

		//element in use is not in available list
		element->list.prev = pool->null;
		element->list.next = pool->null;

		element->in_use = true;

		if (address != NULL) *address = &element->user_data;
		if (handle != NULL) *handle = element_handle;
		return bbSuccess;
	}

	//Then, more than one element available

	bbHandle head_handle = pool->available.head;
	void *head_address;
	bbSystemPool_Header *head_header;
	bbSystemPool_lookup(pool, &head_address, head_handle);
	bbSystemPool_getHeader(&head_header, head_address);

	bbHandle next_handle = head_header->list.next;
	void *next_address;
	bbSystemPool_Header *next_header;
	bbSystemPool_lookup(pool, &next_address, next_handle);
	bbSystemPool_getHeader(&next_header, next_address);

	next_header->list.prev = pool->null;
	pool->available.head = next_handle;

	head_header->list.prev = pool->null;
	head_header->list.next = pool->null;
	head_header->in_use = true;
	head_header->line = line;
	bbStr_setStr(head_header->file, file, KEY_LENGTH);

	if (address != NULL) *address = &head_header->user_data;
	if (handle != NULL) *handle = head_handle;

	return bbSuccess;
}

bbFlag bbSystemPool_Handle_incrementCollision(bbHandle* handle){
	U16 collision = handle->system.generation;
	collision++;
	if(collision == 0) collision++;
	handle->system.generation = collision;
	return bbSuccess;
}


bbFlag bbSystemPool_free(bbSystemPool* pool, void* address)
{
	bbSystemPool_Header* header;
	bbSystemPool_getHeader(&header, address);
	bbSystemPool_Handle_incrementCollision(&header->self);
	header->in_use = false;
	//return element to empty pool
	if (IS_NULL(pool->available.head) || IS_NULL(pool->available.tail))
	{
		bbAssert(bbSuccess == bbSystemPool_handleIsEqual(NULL,pool->available.head,pool->available.tail ),
			"head/tail mismatch\n");

		pool->available.head = header->self;
		pool->available.tail = header->self;
		header->list.prev = pool->null;
		header->list.next = pool->null;

		return bbSuccess;

	}
	bbHandle available_handle = pool->available.head;
	bbSystemPool_Header* available_header;
	void* available_address;

	bbSystemPool_lookup(pool, &available_address, available_handle);
	bbSystemPool_getHeader(&available_header, available_address);

	available_header->list.prev = header->self;
	pool->available.head = header->self;
	header->list.prev = pool->null;
	header->list.next = available_header->self;
	return bbSuccess;
}

bbFlag bbSystemPool_lookupHeader(bbSystemPool* pool, void** address, bbHandle handle){

	if ( handle.system.generation == 0)
	{
		*address = NULL;
		return bbHandleError_NULL;
	}

	if ( handle.system.system != pool->system)
	{
		*address = NULL;
		return bbHandleError_System;
	}

	U32 index = handle.system.index;
	U32 lvl1index = index / pool->level2;
	bbAssert(lvl1index < pool->level1, "index out of bounds\n");
	U32 lvl2index = index % pool->level2;
	U8* lvl2 = pool->elements[lvl1index];
	bbSystemPool_Header *element = (bbSystemPool_Header *)&lvl2[lvl2index * (sizeof(bbSystemPool_Header) + pool->size_of)];
	bbHandle elementHandle = element->self;

	if ( handle.system.generation != elementHandle.system.generation)
	{
		*address = NULL;
		return bbHandleError_Generation;
	}

	*address = element;
	return bbSuccess;
}

bbFlag bbSystemPool_lookup(bbSystemPool* pool, void** address, bbHandle handle){
	bbSystemPool_Header* element;
	bbFlag flag = bbSystemPool_lookupHeader(pool, (void**)&element, handle);

	if (flag == bbSuccess) *address = &element->user_data;
	else
	{
		*address = NULL;
	}
	return flag;
}

bbFlag bbSystemPool_lookup2(bbSystemPool* pool, void** address, bbHandle handle){
	bbSystemPool_Header* element;
	bbFlag flag = bbSystemPool_lookupHeader(pool, (void**)&element, handle);

	if (flag == bbSuccess) *address = &element->user_data;
	else *address = NULL;

	return flag;
}

bbFlag bbSystemPool_reverseLookup(bbSystemPool* pool, void* address, bbHandle* handle){
	bbAssert(handle != NULL, "handle is NULL\n");
	bbAssert(address != NULL, "address is NULL\n");
	bbAssert(pool != NULL, "pool is NULL\n");
	bbSystemPool_Header* element;
	bbSystemPool_getHeader(&element, address);
	*handle = element->self;
	return bbSuccess;
}


bbFlag bbSystemPool_allocFromHandle(bbSystemPool* pool, void** address, bbHandle handle, char* file, I32 line){


	if ( handle.system.system != pool->system)
	{
		*address = NULL;
		return bbHandleError_System;
	}

	U32 index = handle.system.index;
	U32 collision = handle.system.generation;
	U32 lvl1index = index / pool->level2;
	bbAssert(lvl1index < pool->level1, "index out of bounds\n");
	U32 lvl2index = index % pool->level2;
	U8* lvl2 = pool->elements[lvl1index];
	if(lvl2 == NULL)  bbSystemPool_expandHandle(pool, handle);
	lvl2 = pool->elements[lvl1index];
	bbSystemPool_Header *element = (bbSystemPool_Header *)&lvl2[lvl2index * (sizeof(bbSystemPool_Header) + pool->size_of)];

	bbAssert(element->in_use != true, "alloc from handle - in use\n, pool name: %s\n", pool->pool_name);


	bbHandle prev_handle = element->list.prev;
	bbHandle next_handle = element->list.next;

	bbHandle head_handle = pool->available.head;
	bbHandle tail_handle = pool->available.tail;

	bbHandle old_handle = element->self;

	bool is_head = bbSuccess == bbSystemPool_handleIsEqual(pool, head_handle, old_handle);
	bool is_tail = bbSuccess == bbSystemPool_handleIsEqual(pool, tail_handle, old_handle);

	element->in_use = true;
	element->self.system.generation = collision;

	if (is_head && is_tail)
	{
		pool->available.head = pool->null;
		pool->available.tail = pool->null;
	} else if (is_head)
	{
		pool->available.head = next_handle;
		bbSystemPool_Header* next_header;
		bbSystemPool_lookupHeader(pool, (void**)&next_header, next_handle);
		next_header->list.prev = pool->null;

	} else if (is_tail)
	{
		pool->available.tail = prev_handle;
		bbSystemPool_Header* prev_header;
		bbSystemPool_lookupHeader(pool, (void**)&prev_header, prev_handle);
		prev_header->list.next = pool->null;
	} else
	{

		bbSystemPool_Header* next_header;
		bbSystemPool_lookupHeader(pool, (void**)&next_header, next_handle);

		bbSystemPool_Header* prev_header;
		bbSystemPool_lookupHeader(pool, (void**)&prev_header, prev_handle);

		next_header->list.prev = prev_header->self;
		prev_header->list.next = next_header->self;
	}

	element->list.prev = pool->null;
	element->list.next = pool->null;
	element->in_use = true;
	element->line = line;
	bbStr_setStr(element->file, file, KEY_LENGTH);

	if (address!=NULL) *address = element->user_data;



	return bbSuccess;
}

bbFlag bbSystemPool_printHeader(bbSystemPool* pool, void* address)
{
	bbSystemPool_Header* element = address - offsetof(bbSystemPool_Header, user_data);
//TODO print self.system
	bbDebug("self.index = %d, self.generation = %d\n"
		"list.prev.index = %d, list.prev.generation = %d\n"
		"list.next.index = %d, list.next.generation = %d\n"
		"in use = %d\n"
		"line  = %d\n"
		"file = %s\n",
		element->self.system.index,
		element->self.system.generation,
		element->list.prev.system.index,
		element->list.prev.system.generation,
		element->list.next.system.index,
		element->list.next.system.generation,
		element->in_use,
		element->line,
		element->file);

	return bbSuccess;
}