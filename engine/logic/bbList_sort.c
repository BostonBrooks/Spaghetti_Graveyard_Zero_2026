#include "engine/logic/bbList.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"
#include "engine/logic/bbFlag.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>


#define isEqual(A, B) bbVPool_handleIsEqual(list->pool, A, B)
#define isNULL(A) bbVPool_handleIsEqual(list->pool, A, list->pool->null)

I32 getLength(bbList* list, bbList_Handle temp_list){
    if(isNULL(temp_list.head)){
        bbAssert(isNULL(temp_list.tail), "head / tail mismatch\n");
        return 0;
    }

    if (isEqual(temp_list.head, temp_list.tail)){
        return 1;
    }
    I32 len = 2;
    bbHandle handle_element = temp_list.head;
    void* element;
    bbVPool_lookup(list->pool, &element, handle_element);
    bbListElement_Handle* list_element = element + list->offset_of;

    while (!isEqual(list_element->next, temp_list.tail)){
        handle_element = list_element->next;
        bbVPool_lookup(list->pool, &element, handle_element);
        list_element = element + list->offset_of;
        len++;
    }
    return len;
}


bbFlag popL (bbList* list, void** element, bbList_Handle* temp_list){
	//cases: empty, 1 element, more than 1 element;
	if (isNULL(temp_list->head)){
		bbAssert(isNULL(temp_list->tail), "head / tail mismatch");
		if (element != NULL){
			*element = NULL;
		}
		return bbNone;
	}

	void* head;
	bbVPool_lookup(list->pool, &head, temp_list->head);
	bbListElement_Handle* head_list_element = head + list->offset_of;

	if(isEqual(temp_list->head, temp_list->tail)){
		bbAssert(isEqual(temp_list->head, head_list_element->prev), "\n");
		bbAssert(isEqual(temp_list->head, head_list_element->next), "\n");

		temp_list->head = list->pool->null;
		temp_list->tail = list->pool->null;
		head_list_element->prev = list->pool->null;
		head_list_element->next = list->pool->null;
		if (element != NULL){
			*element = head;

		}
		return bbSuccess;
	}

	void* prev;
	bbVPool_lookup(list->pool, &prev, head_list_element->prev);
	bbListElement_Handle* prev_list_element = prev + list->offset_of;

	void* next;
	bbVPool_lookup(list->pool, &next, head_list_element->next);
	bbListElement_Handle* next_list_element = next + list->offset_of;

	prev_list_element->next = head_list_element->next;
	next_list_element->prev = head_list_element->prev;
	temp_list->head = head_list_element->next;

	head_list_element->prev = list->pool->null;
	head_list_element->next = list->pool->null;

	if (element != NULL){
		*element = head;

	}
	return bbSuccess;
}


bbFlag pushR(bbList* list, void* element, bbList_Handle* temp_list){
	//cases: empty list / 1 element list / other

	bbListElement_Handle* list_element = element + list->offset_of;
	bbHandle handleElement;
	bbVPool_reverseLookup(list->pool, element, &handleElement);

	bbAssert(isNULL(list_element->prev), "already in a list\n");
	bbAssert(isNULL(list_element->next), "already in a list\n");

	//set element to only member of list
	if(isNULL(temp_list->head)){
		bbAssert(isNULL(temp_list->head), "head/tail mismatch\n");
		list_element->prev = handleElement;
		list_element->next = handleElement;
		temp_list->head = handleElement;
		temp_list->tail = handleElement;

		return bbSuccess;
	}

	void* head;
	bbVPool_lookup(list->pool, &head, temp_list->head);
	bbListElement_Handle* head_list_element = head + list->offset_of;

	//set element to tail, tail to head
	if(isEqual(temp_list->head, temp_list->tail)){


		head_list_element->next = handleElement;
		head_list_element->prev = handleElement;
		list_element->next = temp_list->head;
		list_element->prev = temp_list->head;

		temp_list->head = temp_list->tail;
		temp_list->tail = handleElement;

		return bbSuccess;

	}
	//insert between head and tail and update tail
	void* tail;
	bbVPool_lookup(list->pool, &tail, temp_list->tail);
	bbListElement_Handle* tail_list_element = tail + list->offset_of;

	tail_list_element->next = handleElement;
	head_list_element->prev = handleElement;
	list_element->prev = temp_list->tail;
	list_element->next = temp_list->head;
	temp_list->tail = handleElement;

	return bbSuccess;
}

bbList_Handle merge(bbList* list, bbList_Handle* A, bbList_Handle* B){
	if (isNULL(A->head) && isNULL(B->head)  ){
		bbList_Handle C;
		C.head = list->pool->null;
		C.tail = list->pool->null;
		return C;
	}
	if (isNULL(A->head)) return *B;
	if (isNULL(B->head)) return *A;

	//actually merge the lists:

	bbList_Handle C;
	C.head = list->pool->null;
	C.tail = list->pool->null;


	void *elementA;
	popL(list, &elementA, A);


	void *elementB;
    popL(list, &elementB, B);

	bbFlag flag;

	while(1) {
		if (list->compare(elementA, elementB)) {
			pushR(list, elementA, &C);
			flag = popL(list, &elementA, A);
			if (flag != bbSuccess) goto RemainingB;
		} else {
			pushR(list, elementB, &C);
			flag = popL(list, &elementB, B);
			if (flag != bbSuccess) goto RemainingA;
		}
	}
	//add remaining elements fram A to C
	RemainingA:
	while(1) {
	    pushR(list, elementA, &C);
	    flag = popL(list, &elementA, A);
		if (flag != bbSuccess) return C;
    }


	RemainingB:
	while(1) {
		pushR(list, elementB, &C);
		flag = popL(list, &elementB, B);
		if (flag != bbSuccess) return C;
	}


	return C;
}

///if the list has an element N, starting at 1, return that element,
///else return the nearest element ie the head or the tail

bbFlag getNth (bbList* list, bbList_Handle temp_list, void** element, I32 N){

    bbHandle handle = temp_list.head;
    void* element1;
    bbVPool_lookup(list->pool, &element1, handle);
    bbListElement_Handle* list_element = element1 + list->offset_of;

    if (N <= 1){
        *element = element1;
        return bbHead;
    }

    for(I32 i = 1; i < N; i++){
        handle = list_element->next;
        bbVPool_lookup(list->pool, &element1, handle);
        if(isEqual(handle,temp_list.tail)) {
            *element = element1;
            return bbTail;
        };
        list_element = element1 + list->offset_of;

    }

    *element = element1;

    return bbSuccess;

}

/// take the first num elements and put in A, the rest go in C
bbFlag split(bbList* list, bbList_Handle A, bbList_Handle* B, bbList_Handle* C, I32 num){


    if (isNULL(A.head)){
        bbAssert(isNULL(A.tail), "split empty list?\n");


        B->head = list->pool->null;
        B->tail = list->pool->null;

        C->head = list->pool->null;
        C->tail = list->pool->null;
    }
	if (num <= 0) {
		*C = A;
		B->head = list->pool->null;
		B->tail = list->pool->null;

		return bbSuccess;
	}

	if (num == 1){

        if(isEqual(A.head, A.tail)){
            *B = A;
            C->head = list->pool->null;
            C->tail = list->pool->null;

            return bbSuccess;
        }

		void *BHead, *BTail, *CHead, *CTail;
		bbListElement_Handle *BHead_list, *BTail_list, *CHead_list, *CTail_list;
		bbHandle BHead_handle, BTail_handle, CHead_handle, CTail_handle;

		BHead_handle = BTail_handle = A.head;

		bbVPool_lookup(list->pool, &BHead, BHead_handle);
		BTail = BHead;
		BHead_list = BTail_list = BHead + list->offset_of;

		CHead_handle = BTail_list->next;
		bbVPool_lookup(list->pool, &CHead, CHead_handle);
		CHead_list = CHead + list->offset_of;

		CTail_handle = BHead_list->prev;
		bbVPool_lookup(list->pool, &CTail, CTail_handle);
		CTail_list = CTail + list->offset_of;

		BHead_list->prev = BTail_handle;
		BTail_list->next = BHead_handle;
		CHead_list->prev = CTail_handle;
		CTail_list->next = CHead_handle;

		B->head = BHead_handle;
		B->tail = BTail_handle;
		C->head = CHead_handle;
		C->tail = CTail_handle;

		return bbSuccess;

	}

    if (num == 2){
        if(isEqual(A.head, A.tail)){
            *B = A;
            C->head = list->pool->null;
            C->tail = list->pool->null;

            return bbSuccess;
        }

        bbHandle AHeadHandle = A.head;
        void* AHead;
        bbVPool_lookup(list->pool, &AHead, AHeadHandle);
        bbListElement_Handle* AHeadList = AHead + list->offset_of;

        bbHandle A2Handle = AHeadList->next;

        //list has only 2 elements
        if (isEqual(A2Handle, A.tail)){
            *B = A;
            C->head = list->pool->null;
            C->tail = list->pool->null;

            return bbSuccess;
        }

        void* A2;
        bbVPool_lookup(list->pool, &A2, A2Handle);
        bbListElement_Handle* A2List = A2 + list->offset_of;

        //B contains AHead, A2,
        //C contains A3 to A.Tail

        bbHandle A3Handle = A2List->next;
        void* A3;
        bbVPool_lookup(list->pool, &A3, A3Handle);
        bbListElement_Handle* A3List = A3 + list->offset_of;

        B->head = A.head;
        B->tail = A2Handle;
        AHeadList->prev = A2Handle;
        A2List->next = AHeadHandle;

        if(isEqual(A3Handle, A.tail)){
            C->head = A3Handle;
            C->tail = A3Handle;
            A3List->prev = A3Handle;
            A3List->next = A3Handle;

            return bbSuccess;
        }

        bbHandle ATailHandle = A.tail;
        void* ATail;
        bbVPool_lookup(list->pool, &ATail, ATailHandle);
        bbListElement_Handle* ATailList = ATail + list->offset_of;

        C->head = A3Handle;
        C->tail = A.tail;
        A3List->prev = A.tail;
        ATailList->next = A3Handle;

        return bbSuccess;

    }

    //if (num == 3)
    {

        void* Nth;
        bbFlag flag = getNth(list, A, &Nth, num);

        if (flag == bbTail){
            B->head = A.head;
            B->tail = A.tail;
            C->head = list->pool->null;
            C->tail = list->pool->null;

            return bbSuccess;
        }

        bbHandle NthHandle;
        bbVPool_reverseLookup(list->pool, Nth, &NthHandle);
        bbListElement_Handle* NthList = Nth + list->offset_of;

        bbHandle AHeadHandle = A.head;
        void* AHead;
        bbVPool_lookup(list->pool, &AHead, AHeadHandle);
        bbListElement_Handle* AHeadList = AHead + list->offset_of;

        bbHandle NthNextHandle = NthList->next;

        B->head = A.head;
        B->tail = NthHandle;
        NthList->next = A.head;
        AHeadList->prev = NthHandle;

        void* NthNext;
        bbVPool_lookup(list->pool, &NthNext, NthNextHandle);
        bbListElement_Handle* NthNextList = NthNext + list->offset_of;

        if(isEqual(NthNextHandle, A.tail)){
            // one element in A;


            C->head = NthNextHandle;
            C->tail = NthNextHandle;
            NthNextList->prev = NthNextHandle;
            NthNextList->next = NthNextHandle;

            return bbSuccess;

        }

        bbHandle ATailHandle = A.tail;
        void* ATail;
        bbVPool_lookup(list->pool, &ATail, ATailHandle);
        bbListElement_Handle* ATailList = ATail + list->offset_of;

        C->head = NthNextHandle;
        C->tail = ATailHandle;
        NthNextList->prev = ATailHandle;
        ATailList->next = NthNextHandle;

        return bbSuccess;
    }
}

bbFlag sort(bbList* list, bbList_Handle* temp_list, I32 len){
//printf("sort: len = %d\n", len);
    if (len == 1){
        return bbSuccess;
    }

    if (len == 2){
        void *head, *tail;
        bbVPool_lookup(list->pool, &head, temp_list->head);
        bbVPool_lookup(list->pool, &tail, temp_list->tail);

        if(list->compare(head, tail)) return bbSuccess;


        bbHandle tmp = temp_list->head;
        temp_list->head = temp_list->tail;
        temp_list->tail = tmp;
        return bbSuccess;


    }

    bbList_Handle B,C;
    I32 lenB = len/2;
    I32 lenC = len - lenB;

    split(list, *temp_list, &B, &C, lenB);



//printf("lenB = %d, len(B) = %d, lenC = %d, len(C) = %d\n",
//       lenB, getLength(list, B),
//       lenC, getLength(list, C));

    sort(list, &B, lenB);
    sort(list, &C, lenC);

    bbList_Handle tmp;

    tmp = merge(list, &B, &C);

    *temp_list = tmp;
    return bbNone;
}
bbFlag bbList_sort(bbList* list) {
    I32 len = bbList_getLength(list);
    bbList_Handle temp_list = *list->list_pointer;
    sort(list, &temp_list, len);
    *list->list_pointer = temp_list;
    return bbSuccess;
}

bbFlag bbList_sortL(bbList* list, void* element){

	bbVPool* pool = list->pool;
	//bbVPool_handleValid(pool, list->list_pointer->head);
	//bbVPool_handleValid(pool, list->list_pointer->tail);



    bbHandle element_handle;
    bbVPool_reverseLookup(list->pool, element, &element_handle);
    bbListElement_Handle* elementList = element + list->offset_of;

    bbAssert(isNULL(elementList->prev), "element already in a list\n");
    bbAssert(isNULL(elementList->next), "element already in a list\n");

	//this should deal with empty list
    if(isNULL(list->list_pointer->head) || isNULL(list->list_pointer->tail)){
        bbAssert(isNULL(list->list_pointer->head) && isNULL(list->list_pointer->tail), "head / tail mismatch\n");
        bbList_pushL(list, element);
        return bbSuccess;
    }

    bbHandle next_handle = list->list_pointer->head;
    void *next;
    bbListElement_Handle *next_list;
    while(1) {

        bbFlag flag = bbVPool_lookup(list->pool, &next, next_handle);

        next_list = next + list->offset_of;

    	bbVPool* pool = list->pool;

    	//bbVPool_elementInBounds(pool, element);

    	if (next == NULL)
    	{
    		bbList_pushR(list, element);
    		return bbSuccess;
    	}

    	//bbVPool_elementInBounds(pool, next);
        if (list->compare(element, next)) {
            bbList_insertBefore(list, element, next);
            return bbSuccess;
        }
        if (isEqual(next_handle,list->list_pointer->tail)) {
            bbList_pushR(list, element);
            return bbSuccess;
        }
        next_handle = next_list->next;

    }



}
bbFlag bbList_sortR(bbList* list, void* element) {
    bbHandle element_handle;
    bbVPool_reverseLookup(list->pool, element, &element_handle);
    bbListElement_Handle *element_list = element + list->offset_of;

    bbAssert(isNULL(element_list->prev), "element already in a list\n");
    bbAssert(isNULL(element_list->next), "element already in a list\n");

    if(isNULL(list->list_pointer->head)){
        bbAssert(isNULL(list->list_pointer->tail), "head / tail mismatch\n");
        bbList_pushR(list, element);
        return bbSuccess;
    }

    bbHandle prev_handle = list->list_pointer->tail;
    void *prev;
    bbListElement_Handle *prev_list;

    while(1) {
        bbVPool_lookup(list->pool, &prev, prev_handle);
        prev_list = prev + list->offset_of;

    	if (prev == NULL)
    	{
    		bbList_pushL(list, element);
    		return bbSuccess;
    	}

        if (!list->compare(element, prev)) {
            bbList_insertAfter(list, element, prev);
            return bbSuccess;
        }
        if (isEqual(prev_handle,list->list_pointer->head)) {
            bbList_pushL(list, element);
            return bbSuccess;
        }
        prev_handle = prev_list->prev;
    }
}

bbFlag bbList_sortElement(bbList* list, void* element){
    bbHandle element_handle;
    bbVPool_reverseLookup(list->pool, element, &element_handle);
    bbListElement_Handle *element_list = element + list->offset_of;

    bbAssert(!isNULL(list->list_pointer->head), "list should not ne empty\n");
    bbAssert(!isNULL(list->list_pointer->tail), "list should not ne empty\n");

    //Is this the only element in the list?
    if (isEqual(element_handle, list->list_pointer->head )
        && isEqual(element_handle, list->list_pointer->tail ) ){
        return bbSuccess;
    }

    //Is this the head element?
    if (isEqual(element_handle, list->list_pointer->head )){


        bbHandle next_handle = element_list->next;
        void* next;
        bbVPool_lookup(list->pool, &next, next_handle);
        bbListElement_Handle* nextList = next + list->offset_of;

        //Is the element already sorted?
        if(list->compare(element, next)) return bbSuccess;

        //remove element from list
        bbHandle tail_handle = list->list_pointer->tail;
        void* tail;
        bbVPool_lookup(list->pool, &tail, tail_handle);
        bbListElement_Handle* tailList = tail + list->offset_of;

        nextList->prev = tail_handle;
        tailList->next = next_handle;
        element_list->prev = list->pool->null;
        element_list->next = list->pool->null;

        list->list_pointer->head = next_handle;

        return bbList_sortL(list, element);
    }

    //Is this the tail element?
    if (isEqual(element_handle, list->list_pointer->tail )){

        bbHandle prev_handle = element_list->prev;
        void* prev;
        bbVPool_lookup(list->pool, &prev, prev_handle);
        bbListElement_Handle* prevList = prev + list->offset_of;

        //Is the element already sorted?
        if(list->compare(prev, element)) return bbSuccess;

        //remove element from list
        bbHandle head_handle = list->list_pointer->head;
        void* head;
        bbVPool_lookup(list->pool, &head, head_handle);
        bbListElement_Handle* headList = head + list->offset_of;

        prevList->next = head_handle;
        headList->prev = prev_handle;
        element_list->prev = list->pool->null;
        element_list->next = list->pool->null;

        list->list_pointer->tail = prev_handle;

        return bbList_sortR(list, element);
    }


    bbHandle prev_handle = element_list->prev;
    void* prev;
    bbVPool_lookup(list->pool, &prev, prev_handle);
    bbListElement_Handle* prev_list = prev + list->offset_of;

    bbHandle next_handle = element_list->next;
    void* next;
    bbVPool_lookup(list->pool, &next, next_handle);
    bbListElement_Handle* next_list = next + list->offset_of;

    //Is the element already sorted?
    if (list->compare(prev, element) && list->compare(element, next))
        return bbSuccess;


    next_list->prev = prev_handle;
    prev_list->next = next_handle;
    element_list->prev = list->pool->null;
    element_list->next = list->pool->null;

    //move element left
    if (!list->compare(prev, element)){

        while(1) {
            bbVPool_lookup(list->pool, &prev, prev_handle);
            prev_list = prev + list->offset_of;

            if (list->compare(prev, element)) {
                bbList_insertAfter(list, element, prev);
                return bbSuccess;
            }
            if (isEqual(prev_handle,list->list_pointer->head)) {
                bbList_pushL(list, element);
                return bbSuccess;
            }
            prev_handle = prev_list->prev;
        }

        //move element left

        return bbSuccess;
    }

    //if (!list->compare(element, next)){
        //move element right
    while(1) {

        bbVPool_lookup(list->pool, &next, next_handle);
        next_list = next + list->offset_of;

        if (list->compare(element, next)) {
            bbList_insertBefore(list, element, next);
            return bbSuccess;
        }
        if (isEqual(next_handle,list->list_pointer->tail)) {
            bbList_pushR(list, element);
            return bbSuccess;
        }
        next_handle = next_list->next;
    }

}