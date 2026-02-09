#include "engine/logic/bbList.h"

#include "engine/logic/bbTerminal.h"

#define isEqual(A, B) bbVPool_handleIsEqual(list->pool, A, B)
#define isNULL(A) bbVPool_handleIsEqual(list->pool, A, list->pool->null)

bbFlag bbList_new(bbList** list, bbVPool* pool, void* listPtr, size_t offset_of,
                  I32 (*compare)(void* A, void* B)){

    bbList* List = malloc(sizeof(bbList));
    bbAssert(NULL != list, "malloc failed\n");

    bbFlag flag = bbList_init(List, pool, listPtr, offset_of, compare);

    *list = List;
    return flag;
}

bbFlag bbList_init(bbList* list, bbVPool* pool, void* list_pointer, size_t offset_of,
                   I32 (*compare)(void* A, void* B)){
    list->pool = pool;
    //listPtr is used to attach a bbList to an existing list
    if(list_pointer != NULL){
        list->list_pointer = list_pointer;
    } else {
        list->list_pointer = &list->list;
        list->list.head = pool->null;
        list->list.tail = pool->null;
    }
    list->offset_of = offset_of;
    list->compare = compare;
    list->prev = list->pool->null;
    list->current = list->pool->null;
    list->next = list->pool->null;

    return bbSuccess;
}

bbFlag bbList_pushL(bbList* list, void* element){
    //cases: empty list / 1 element list / other

    bbListElement_Handle* list_element = element + list->offset_of;
    bbHandle handle_element;
    bbVPool_reverseLookup(list->pool, element, &handle_element);

    bbAssert(isNULL(list_element->prev), "already in a list\n");
    bbAssert(isNULL(list_element->next), "already in a list\n");

    //Empty list
    if(isNULL(list->list_pointer->head)){
        bbAssert(isNULL(list->list_pointer->head), "head/tail mismatch\n");
        list_element->prev = handle_element;
        list_element->next = handle_element;
        list->list_pointer->head = handle_element;
        list->list_pointer->tail = handle_element;

        return bbSuccess;
    }

    void* head;
    bbVPool_lookup(list->pool, &head, list->list_pointer->head);
    bbListElement_Handle* head_list_element = head + list->offset_of;

    //One element in list
    if(isEqual(list->list_pointer->head, list->list_pointer->tail)){


        head_list_element->next = handle_element;
        head_list_element->prev = handle_element;
        list_element->next = list->list_pointer->head;
        list_element->prev = list->list_pointer->head;

        list->list_pointer->tail = list->list_pointer->head;
        list->list_pointer->head = handle_element;

        return bbSuccess;

    }
    //More than one element in list
    void* tail;
    bbVPool_lookup(list->pool, &tail, list->list_pointer->tail);
    bbListElement_Handle* tailListElement = tail + list->offset_of;

    tailListElement->next = handle_element;
    head_list_element->prev = handle_element;
    list_element->prev = list->list_pointer->tail;
    list_element->next = list->list_pointer->head;
    list->list_pointer->head = handle_element;

    return bbSuccess;
}

bbFlag bbList_pushR(bbList* list, void* element){
    //cases: empty list / 1 element list / other

    bbListElement_Handle* list_element = element + list->offset_of;
    bbHandle handle_element;
    bbVPool_reverseLookup(list->pool, element, &handle_element);

    bbAssert(isNULL(list_element->prev), "already in a list\n");
    bbAssert(isNULL(list_element->next), "already in a list\n");

    //Empty list
    if(isNULL(list->list_pointer->head)){
        bbAssert(isNULL(list->list_pointer->head), "head/tail mismatch\n");
        list_element->prev = handle_element;
        list_element->next = handle_element;
        list->list_pointer->head = handle_element;
        list->list_pointer->tail = handle_element;

        return bbSuccess;
    }

    void* head;
    bbVPool_lookup(list->pool, &head, list->list_pointer->head);
    bbListElement_Handle* head_list_element = head + list->offset_of;

    //one element in list
    if(isEqual(list->list_pointer->head, list->list_pointer->tail)){


        head_list_element->next = handle_element;
        head_list_element->prev = handle_element;
        list_element->next = list->list_pointer->head;
        list_element->prev = list->list_pointer->head;

        list->list_pointer->tail = handle_element;

        return bbSuccess;

    }
    //More than one element in list
    void* tail;
    bbVPool_lookup(list->pool, &tail, list->list_pointer->tail);
    bbListElement_Handle* tail_list_element = tail + list->offset_of;

    tail_list_element->next = handle_element;
    head_list_element->prev = handle_element;
    list_element->prev = list->list_pointer->tail;
    list_element->next = list->list_pointer->head;
    list->list_pointer->tail = handle_element;

    return bbSuccess;
}

bbFlag bbList_popL(bbList* list, void** element){
    //cases: empty, 1 element, more than 1 element;

    //List Empty
    if (isNULL(list->list_pointer->head)){
        bbAssert(isNULL(list->list_pointer->tail), "head / tail mismatch");
        if (element != NULL){
            *element = NULL;
        }
        return bbNone;
    }

    void* head;
    bbVPool_lookup(list->pool, &head, list->list_pointer->head);
    bbListElement_Handle* head_list_element = head + list->offset_of;


    //One element
    if(isEqual(list->list_pointer->head, list->list_pointer->tail)){
        bbAssert(isEqual(list->list_pointer->head, head_list_element->prev), "\n");
        bbAssert(isEqual(list->list_pointer->head, head_list_element->next), "\n");

        list->list_pointer->head = list->pool->null;
        list->list_pointer->tail = list->pool->null;
        head_list_element->prev = list->pool->null;
        head_list_element->next = list->pool->null;
        if (element != NULL){
            *element = head;
        }
        return bbSuccess;
    }

    //More than one element
    void* prev;
    bbVPool_lookup(list->pool, &prev, head_list_element->prev);
    bbListElement_Handle* prev_list_element = prev + list->offset_of;

    void* next;
    bbVPool_lookup(list->pool, &next, head_list_element->next);
    bbListElement_Handle* next_list_element = next + list->offset_of;

    prev_list_element->next = head_list_element->next;
    next_list_element->prev = head_list_element->prev;
    list->list_pointer->head = head_list_element->next;

    head_list_element->prev = list->pool->null;
    head_list_element->next = list->pool->null;

    if (element != NULL){
        *element = head;
    }
    return bbSuccess;
}


bbFlag bbList_popR(bbList* list, void** element){

    //List empty
    if (isNULL(list->list_pointer->head) || isNULL(list->list_pointer->tail)){
        bbAssert(isNULL(list->list_pointer->head) && isNULL(list->list_pointer->head), "head / tail mismatch");
        if (element != NULL){
            *element = NULL;
        }
        return bbNone;
    }

    void* tail;
    bbVPool_lookup(list->pool, &tail, list->list_pointer->tail);
    bbListElement_Handle* tail_list_element = tail + list->offset_of;

    //One element
    if(isEqual(list->list_pointer->head, list->list_pointer->tail)){
        bbAssert(isEqual(list->list_pointer->tail, tail_list_element->prev), "\n");
        bbAssert(isEqual(list->list_pointer->tail, tail_list_element->next), "\n");

        list->list_pointer->head = list->pool->null;
        list->list_pointer->tail = list->pool->null;
        tail_list_element->prev = list->pool->null;
        tail_list_element->next = list->pool->null;
        if (element != NULL){
            *element = tail;
        }

        tail_list_element->prev = list->pool->null;
        tail_list_element->next = list->pool->null;
        return bbSuccess;
    }

    //More than one element
    void* prev;
    bbVPool_lookup(list->pool, &prev, tail_list_element->prev);
    bbListElement_Handle* prev_list_element = prev + list->offset_of;

    void* next;
    bbVPool_lookup(list->pool, &next, tail_list_element->next);
    bbListElement_Handle* next_list_element = next + list->offset_of;

    prev_list_element->next = tail_list_element->next;
    next_list_element->prev = tail_list_element->prev;
    list->list_pointer->tail = tail_list_element->prev;

    tail_list_element->prev = list->pool->null;
    tail_list_element->next = list->pool->null;

    if (element != NULL){
        *element = tail;
    }
    return bbSuccess;
}

bbFlag bbList_peakL(bbList* list, void** element)
{
    if (isNULL(list->list_pointer->head))
    {
        *element = NULL;
        return bbNone;
    }
    void* head;
    bbVPool_lookup(list->pool, &head, list->list_pointer->head);
    *element = head;
    return bbSuccess;
}

bbFlag bbList_peakR(bbList* list, void** element)
{
    if (isNULL(list->list_pointer->tail))
    {
        *element = NULL;
        return bbNone;
    }
    void* head;
    bbVPool_lookup(list->pool, &head, list->list_pointer->tail);
    *element = head;
    return bbSuccess;
}

I32 bbList_getLength(bbList* list){
    if(isNULL(list->list_pointer->head) || isNULL(list->list_pointer->tail)){
        bbAssert(isNULL(list->list_pointer->tail) && isNULL(list->list_pointer->tail),
            "head / tail mismatch\n");
        return 0;
    }

    if (isEqual(list->list_pointer->head, list->list_pointer->tail)){
        return 1;
    }
    I32 len = 2;
    bbHandle handle_element = list->list_pointer->head;
    void* element;
    bbVPool_lookup(list->pool, &element, handle_element);
    bbListElement_Handle* list_element = element + list->offset_of;

    while (!isEqual(list_element->next, list->list_pointer->tail)){
        handle_element = list_element->next;
        bbVPool_lookup(list->pool, &element, handle_element);
        list_element = element + list->offset_of;
        len++;
    }
    return len;
}

bbFlag bbList_insertAfter(bbList* list, void* Node, void* Key){

    if (Key == NULL){
        bbList_pushR(list, Node);
        return bbSuccess;
    }

    bbHandle NodeHandle;
    bbVPool_reverseLookup(list->pool, Node, &NodeHandle);
    bbListElement_Handle* NodeList = Node + list->offset_of;

    bbHandle KeyHandle;
    bbVPool_reverseLookup(list->pool, Key, &KeyHandle);
    bbListElement_Handle* KeyList = Key + list->offset_of;

    bbHandle NextHandle = KeyList->next;
    void* Next;
    bbVPool_lookup(list->pool, &Next, NextHandle);
    bbListElement_Handle* NextList = Next + list->offset_of;

    KeyList->next = NodeHandle;
    NodeList->prev = KeyHandle;
    NodeList->next = NextHandle;
    NextList->prev = NodeHandle;

    if (isEqual(KeyHandle, list->list_pointer->tail)){
        list->list_pointer->tail = NodeHandle;
    }

    return bbSuccess;
}

bbFlag bbList_insertBefore(bbList* list, void* Node, void* Key){

    if (Key == NULL){
        bbList_pushL(list, Node);
        return bbSuccess;
    }
    bbHandle node_handle;
    bbVPool_reverseLookup(list->pool, Node, &node_handle);
    bbListElement_Handle* node_list = Node + list->offset_of;

    bbHandle key_handle;
    bbVPool_reverseLookup(list->pool, Key, &key_handle);
    bbListElement_Handle* key_list = Key + list->offset_of;

    bbHandle prev_handle = key_list->prev;
    void* prev;
    bbVPool_lookup(list->pool, &prev, prev_handle);
    bbListElement_Handle* prev_list = prev + list->offset_of;

    prev_list->next = node_handle;
    node_list->prev = prev_handle;
    node_list->next = key_handle;
    key_list->prev = node_handle;

    if(isEqual(key_handle, list->list_pointer->head)){
        list->list_pointer->head = node_handle;
    }

    return bbSuccess;
}

bbFlag bbList_remove(bbList* list, void* element){

    bbHandle element_handle;
    bbListElement_Handle* element_list = element + list->offset_of;
    bbVPool_reverseLookup(list->pool, element, &element_handle);

	//Since it's a circular list, if an element points to itself, it's the only element in the list.
    if (isEqual(element_handle, element_list->next)){

        bbAssert(isEqual(element_handle, element_list->prev)
                 && isEqual(element_handle, list->list_pointer->head)
                 && isEqual(element_handle, list->list_pointer->tail),
                 "all things being equal\n");
        element_list->next = list->pool->null;
        element_list->prev = list->pool->null;
        list->list_pointer->head = list->pool->null;
        list->list_pointer->tail = list->pool->null;

        list->prev = list->pool->null;
        list->current = list->pool->null;
        list->next = list->pool->null;

        return bbSuccess;

    }

	//The element before points to the element after and the element after points to the element before
    bbHandle prev_handle = element_list->prev;
    void* prev;
    bbVPool_lookup(list->pool, &prev, prev_handle);
    bbListElement_Handle* prev_list = prev + list->offset_of;

    bbHandle next_handle = element_list->next;
    void* next;
    bbVPool_lookup(list->pool, &next, next_handle);
    bbListElement_Handle* next_list = next + list->offset_of;

    next_list->prev = prev_handle;
    prev_list->next = next_handle;

	//An element not in a list points to null
    element_list->next = list->pool->null;
    element_list->prev = list->pool->null;


//remove an element from a list that's being iterated
    if(isEqual(element_handle, list->current)){
        list->prev = prev_handle;
        list->current = list->pool->null;
        list->next = next_handle;
    } else if (isEqual(element_handle, list->prev)){
        list->prev = prev_handle;
    } else if (isEqual(element_handle, list->next)){
        list->next = next_handle;
    }

	//is element first or last element in the list
    if(isEqual(list->list_pointer->head, element_handle)){
        list->list_pointer->head = next_handle;
        list->prev = list->pool->null;
    } else if(isEqual(list->list_pointer->tail, element_handle)){
        list->list_pointer->tail = prev_handle;
        list->next = list->pool->null;
    }

    return bbSuccess;
}

bbFlag bbList_isEmpty(bbList* list){
    if (isNULL(list->list_pointer->head)) return 1;
    return 0;
}


bbFlag bbList_alloc(bbList* list, void** element)
{
    void* new_element;
    bbFlag flag = bbVPool_alloc(list->pool, &new_element);
    if (flag == bbSuccess)
    {
        bbListElement_Handle* element_list = new_element + list->offset_of;
        element_list->next = list->pool->null;
        element_list->prev = list->pool->null;
        *element = new_element;
        return bbSuccess;

    }
    *element = NULL;
    return flag;
}


bbFlag bbList_checkIntegrity(bbList* list)
{
    bbAssert (bbVPool_handleIsEqual(list->pool, list->list.head, list->pool->null)
              == bbVPool_handleIsEqual(list->pool, list->list.tail, list->pool->null),
                  "head/tail mismatch\n");

    if (bbVPool_handleIsEqual(list->pool, list->list.head, list->pool->null)) return bbEmpty;

    void* element;
    bbListElement_Handle* list_element;
    bbHandle element_handle = list->list.head;
    bbVPool_lookup(list->pool, &element, element_handle);

    void* first_element = element;

    I32 i = 0;
    while (1)
    {
        list_element = element + list->offset_of;
        element_handle = list_element->next;
        if (bbVPool_handleIsEqual(list->pool, element_handle, list->list.head)) break;
        bbFlag flag = bbVPool_lookup(list->pool, (void**)&element,element_handle);

        if (element == first_element)
        {
            bbHere()
            break;
        }

        if (i%60 == 0) bbDebug("i = %d\n", i);
        i++;
    }



    element_handle = list->list.tail;
    bbVPool_lookup(list->pool, &element, element_handle);

    first_element = element;

    i = 0;
    while (1)
    {
        list_element = element + list->offset_of;
        element_handle = list_element->prev;
        if (bbVPool_handleIsEqual(list->pool, element_handle, list->list.tail)) break;
        bbFlag flag = bbVPool_lookup(list->pool, (void**)&element,element_handle);

        if (element == first_element)
        {
            bbHere()
            break;
        }
        if (i%60 == 0) bbDebug("i = %d\n", i);
        i++;

    }
    return bbSuccess;
}