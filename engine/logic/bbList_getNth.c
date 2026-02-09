
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"

bbFlag bbList_getNth(bbList* list, void** element, I32 N)
{
    int index = 0;
    void* nth_element;
    bbListElement_Handle* list_element;

    bbHandle handle = list->list.head;
    bbVPool_lookup(list->pool, (void**)&nth_element,handle);

    //If we get to the end, i suspect we will go back to the start and keep looking
    for (I32 i = 0; i < N; i++)
    {
        list_element = nth_element + list->offset_of;
        handle = list_element->next;
        bbVPool_lookup(list->pool, (void**)&nth_element,handle);
    }

    *element = nth_element;
    return bbSuccess;
}
