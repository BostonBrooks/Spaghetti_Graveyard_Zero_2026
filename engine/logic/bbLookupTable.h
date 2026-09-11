#ifndef BB_LOOKUPTABLE_H
#define BB_LOOKUPTABLE_H
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct
{
    bbHandle index;
    bbHandle value;
} bbLookupTable_element;

typedef struct {
    U16 system;
    U32 level1;
    U32 level2;

    bbLookupTable_element* elements[];
} bbLookupTable;

bbFlag bbLookupTable_new(bbLookupTable** self,
        U16 system,
        I32 level1,
        I32 level2);

bbFlag bbLookupTable_update(bbLookupTable* table,
                         bbHandle index,
                         bbHandle value);

bbFlag bbLookupTable_lookup(bbLookupTable* table,
                         bbHandle index,
                         bbHandle* value);

bbFlag bbLookupTable_remove(bbLookupTable* table,
                            bbHandle index);

#endif // BB_LOOKUPTABLE_H