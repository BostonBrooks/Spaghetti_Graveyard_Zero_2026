#include "engine/logic/bbList.h"
#include "engine/logic/bbString.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbList_sort.c"

thread_local char* thread;

typedef struct {
	I32 i;
	bbListElement_Handle listElement;
} testStruct;

I32 compare (void* A, void* B){
	testStruct* aStruct = A;
	testStruct* bStruct = B;
	return aStruct->i < bStruct->i;
}

int main(void){

	bbVPool* pool;
	bbVPool_newBloated(&pool, sizeof (testStruct),64, 64, "testStruct");

	testStruct* aStruct;

	bbList listA;
	bbList_init(&listA, pool, NULL, offsetof(testStruct, listElement),compare);

	printf("input:\n");

	for (I32 i = 0; i < 32; i ++) {
		I32 len = bbList_getLength(&listA);
		bbVPool_alloc(pool, (void**)&aStruct);
		aStruct->i = rand()%100;
		aStruct->listElement.prev = pool->null;
		aStruct->listElement.next = pool->null;
		bbList_pushR(&listA, aStruct);


	}


	printf("processing:\n");

    bbList_sort(&listA);
    printf("list A sorted:\n");

	while(bbSuccess == bbList_popL(&listA, (void**)&aStruct)){

		printf("%d\n", aStruct->i);
		bbVPool_free(pool, aStruct);
	}

    exit(EXIT_SUCCESS);
}