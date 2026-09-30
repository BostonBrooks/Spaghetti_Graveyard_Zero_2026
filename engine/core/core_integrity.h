#ifndef INSTRUCTION_MAP
#define INSTRUCTION_MAP

#define NO_INVERSE_FUNCTION -1
#define WRONG_FUNCTION_TYPE -2

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"

//#define bbCore_checkIntegrity(NAME) bbCore_checkIntegrity_fn(NAME);
//#define bbCore_quickCheck(NAME) bbCore_quickCheck_fn(NAME);


#define bbCore_checkIntegrity(NAME)
#define bbCore_quickCheck(NAME)


typedef struct bbCore bbCore;
typedef struct {
    I32* forward;
    I32* rollback;
} bbInstructionMap;

bbFlag bbCore_initMap(bbInstructionMap* map);

///Check if the forward instruction matches the corresponding rollback instruction
bbFlag bbCore_checkMap(bbInstructionMap* map, void* forward, void* rollback);
bbFlag bbCore_checkIntegrity_fn(bbCore* core);

bbFlag bbCore_quickCheck_fn(bbCore* core);


#endif