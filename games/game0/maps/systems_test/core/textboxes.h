#ifndef BB_TEXTBOXES_H
#define BB_TEXTBOXES_H


bbFlag bbCS_setTextbox(bbCore* core,bbHandle* handle, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action);
bbFlag bbCI_setTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action);

bbFlag bbI_setTextbox_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unsetTextbox_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCD_unsetTextbox_fn(bbCore* core, bbInstruction* undo_instruction);

bbFlag bbCS_putTextbox(bbCore* core,bbHandle* handle, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action);
bbFlag bbCI_putTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action);

bbFlag bbI_putTextbox_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unputTextbox_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCD_unputTextbox_fn(bbCore* core, bbInstruction* undo_instruction);

#endif //BB_TEXTBOXES_H