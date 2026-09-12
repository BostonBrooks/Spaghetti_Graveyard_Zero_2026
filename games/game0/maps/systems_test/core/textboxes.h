
bbFlag bbI_setTextbox_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unsetTextbox_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCI_setTextbox(bbCore* core, char* string, char* key, bbInstruction_source source, bbHandle action);
bbFlag bbCS_setTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action);

bbFlag bbCD_unsetTextbox_fn(bbCore* core, bbInstruction* undo_instruction);