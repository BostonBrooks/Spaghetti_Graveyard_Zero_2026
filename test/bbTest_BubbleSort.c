#include <stdio.h>

#include "engine/logic/bbArithmetic.h"
#include "engine/logic/bbBubbleSort.h"
#include "engine/logic/bbInterpolate.h"

typedef struct
{
    I32 integer;
    char string[33];
} test_struct;

bbFlag test_compare(void* A, void* B)
{
    test_struct* struct_A = (test_struct*)A;
    test_struct* struct_B = (test_struct*)B;

    if (struct_A->integer > struct_B->integer) return bbSuccess;
    return bbFail;
}

int main (void)
{
    test_struct structs[12];

    printf("unsorted:\n");
    for (I32 i = 0; i < 12; i++)
    {
        I32 num = rand();

        structs[i].integer = num;
        snprintf(structs[i].string, 33, "%d", num);
        printf("%s\n", structs[i].string);
    }


    bbBubbleSort((U8*)structs, 12, sizeof(test_struct), test_compare);

    printf("\nsorted:\n");
    for (I32 i = 0; i < 12; i++)
    {
        printf("%s\n", structs[i].string);
    }

    exit(EXIT_SUCCESS);
}
