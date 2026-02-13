///bbInstruction is used to tell the core what to do

#ifndef BB_INSTRUCTION_H
#define BB_INSTRUCTION_H

#include "engine/logic/bbHandle.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbCore.h"
#include "engine/data/bbConstants.h"

typedef enum
{
    bbInstruction_printInteger,
    bbInstruction_unprintInteger,

    bbInstruction_printString,
    bbInstruction_unprintString,

    bbInstruction_unfreezeButton,
    bbInstruction_netsendButton,

} bbInstruction_type;

typedef struct
{
    I32 integer;
} bbInstructionData_integer;

typedef struct
{
    char string[KEY_LENGTH];
} bbInstructionData_string;

typedef union
{
    bbInstructionData_integer integer;
    bbInstructionData_string string;
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


bbFlag bbInstruction_printString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unprintString_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction);

#endif // BB_INSTRUCTION_H