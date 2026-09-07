#include "engine/logic/bbLookupTable.h"

#include <stdlib.h>

#include "bbTerminal.h"

bbFlag bbLookupTable_new(bbLookupTable** self,
                         U16 system,
                         I32 level1,
                         I32 level2)
{
    bbLookupTable* table
    = (bbLookupTable*)malloc(sizeof(bbLookupTable)+level1*sizeof(bbLookupTable_element*));

    table->system = system;
    table->level1 = level1;
    table->level2 = level2;
    for (I32 i = 0; i < level1; i++)
    {
        table->elements[i] = NULL;
    }
    *self = table;
    return bbSuccess;
}

bbFlag bbLookupTable_expand(bbLookupTable* table,
                            bbHandle handle)
{
    if (handle.system.system != table->system) return bbHandleError_System;

    U32 index = handle.system.index;

    U32 level1 = handle.system.index / table->level2;

    if (table->elements[level1] != NULL) return bbFail;

    bbLookupTable_element* level2 = calloc(table->level2,sizeof(bbLookupTable_element));
    bbAssert(level2 != NULL, "calloc failed\n");

    U16 system = table->system;
    for (U32 i = 0; i < table->level2; i++)
    {
        level2->index.system.system = system;
    }

    table->elements[level1] = level2;

    return bbSuccess;
}

bbFlag bbLookupTable_update(bbLookupTable* table,
                         bbHandle index,
                         bbHandle value)
{
    if (index.system.system != table->system) return bbHandleError_System;

    U32 handle_index = value.system.index;
    U32 level1_index = handle_index / table->level2;
    U32 level2_index = handle_index % table->level2;


    bbLookupTable_element* level2 = table->elements[level1_index];

    if (level2 == NULL)
    {
        bbFlag flag = bbLookupTable_expand(table, index);
        if (flag != bbSuccess) return flag;
        level2 = table->elements[level1_index];
    }


    bbLookupTable_element* element = &level2[level2_index];


    if (value.system.generation != 0)
    {
        element->index = index;
        element->value = value;
        return bbSuccess;
    }
        //set left and right generation values to null
        element->index.system.generation = 0;
        element->value.u64 = 0;
        return bbSuccess;

}

bbFlag bbLookupTable_lookup(bbLookupTable* table,
                         bbHandle index,
                         bbHandle* value)
{
    if (index.system.system != table->system) return bbHandleError_System;

    U32 handle_index = index.system.index;
    U32 level1_index = handle_index / table->level2;
    U32 level2_index = handle_index % table->level2;


    bbLookupTable_element* level2 = table->elements[level1_index];

    if (level2 == NULL) return bbNone;

    bbLookupTable_element element = level2[level2_index];

    bbAssert (element.index.system.system == table->system, "memory corruption?\n");

    if (element.index.system.generation == 0) return bbHandleError_Generation;
    if (element.value.system.generation == 0) return bbHandleError_NULL;

    if (element.index.system.generation != index.system.generation) return bbHandleError_Stale;

    *value = element.value;

    return bbSuccess;

}