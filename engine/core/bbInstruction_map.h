#ifndef INSTRUCTION_MAP
#define INSTRUCTION_MAP

#define NO_INVERSE_FUNCTION -1
#define WRONG_FUNCTION_TYPE -2
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"


typedef struct {
    I32* forward;
    I32* rollback;
} bbInstructionMap;

bbFlag bbCore_initMap(bbInstructionMap* map);

bbFlag bbCore_checkMap(bbInstructionMap* map, void* forward, void* rollback);

#endif