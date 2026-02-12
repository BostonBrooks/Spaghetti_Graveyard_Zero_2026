///bbInstruction is used to tell the core what to do

#ifndef BB_INSTRUCTION_H
#define BB_INSTRUCTION_H

#include "engine/logic/bbHandle.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbCore.h"

typedef enum
{
    bbInstruction_printInteger,
    bbInstruction_unprintInteger,

} bbInstruction_type;

typedef struct
{
    I32 integer;
} bbInstruction_printInteger_data;

typedef union
{
    bbInstruction_printInteger_data integer;
} bbInstruction_data;

typedef struct
{
    bbInstruction_type type;
    bbInstruction_data data;
    bbListElement_Handle list_element;
    bbHandle redo;
    bool is_input;
} bbInstruction;

bbFlag bbInstruction_printInteger_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unprintInteger_fn(bbCore* core, bbInstruction* instruction);

#endif // BB_INSTRUCTION_H