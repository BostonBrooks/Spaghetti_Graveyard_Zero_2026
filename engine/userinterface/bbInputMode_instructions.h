#ifndef BB_INPUTMODE_I_H
#define BB_INPUTMODE_I_H
#include "engine/logic/bbFlag.h"


bbFlag bbCI_bbInputModes_set(bbCore* core,bbInputModes* input_modes, char* string, bbInstruction_source source, bbHandle action);
bbFlag bbCS_bbInputModes_set(bbCore* core,bbInputModes* input_modes, char* string, bbInstruction_source source, bbHandle action);

bbFlag bbI_bbInputModes_set_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_bbInputModes_unset_fn(bbCore* core, bbInstruction* instruction);

#endif //BB_INPUTMODE_I_H