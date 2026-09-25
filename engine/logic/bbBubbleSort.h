#ifndef BB_ARRAYSORT_H
#define BB_ARRAYSORT_H

#include "bbFlag.h"
#include "bbIntTypes.h"
typedef bbFlag bbArrayCompare(void* A, void* B);

bbFlag bbBubbleSort(U8* array, I32 count, I32 element_size, bbArrayCompare* compare);


#endif