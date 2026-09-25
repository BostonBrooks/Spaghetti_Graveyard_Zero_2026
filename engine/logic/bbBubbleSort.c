#include "engine/logic/bbBubbleSort.h"

#include <string.h>


static bbFlag swap(void* A, void* B, I32 length)
{
    U8* array_A = (U8*)A;
    U8* array_B = (U8*)B;
    for (I32 i = 0; i < length; i++)
    {
        U8 temp = array_A[i];
        array_A[i] = array_B[i];
        array_B[i] = temp;
    }
}

bbFlag bbBubbleSort(U8* array, I32 count, I32 element_size, bbArrayCompare* compare)
{
    bool swapped;
    for (I32 i = 0; i < count-1; i++)
    {
        swapped = false;
        for (I32 j = 0; j < count - i-1; j++)
        {
            void* A = &array[j*element_size];
            void* B = &array[(j+1)*element_size];
            if (bbSuccess == compare(A, B))
            {
                swap(A,B,element_size);
                swapped = true;
            }
        }

        if (!swapped) break;
    }

    return bbSuccess;
}
