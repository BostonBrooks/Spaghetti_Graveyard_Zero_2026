
#ifndef BB_INSTRUCTION_H
#define BB_INSTRUCTION_H
#include "engine/core/bbCore.h"
#include "engine/data/bbData.h"
#include "engine/logic/bbHandle.h"

typedef enum
{
    bbInstruction_setString,
    bbInstruction_unsetString,


    bbInstruction_checkActions,
    bbInstruction_uncheckActions,

    bbInstruction_setTime,
    bbInstruction_unsetTime
} bbInstruction_type;


typedef enum
{
    ///The instruction was added from inside the core
    bbInstructionSource_internal,
    ///The instruction was added in the MAIN thread
    bbInstructionSource_input,
    ///The instruction was added through the action queue
    bbInstructionSource_action,
} bbInstruction_source;


typedef union
{
    char string[KEY_LENGTH];
    U64 unsigned_long;
} bbInstruction_data;

typedef struct
{
    bbInstruction_type type;
    bbInstruction_data data;
    U64 act_time;
    bbListElement_Handle list_element;
    bbHandle redo_instruction;
    bbInstruction_source source;
} bbInstruction;


bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_setString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetString_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetTime_fn(bbCore* core, bbInstruction* instruction);

#endif //BB_INSTRUCTION_H