
#include "engine/data/bbHome.h"
#include "engine/textbox/bbTextbox.h"

bbFlag bbCS_setTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action)
{
    bbNotImplemented(); //spawn undo instruction

    bbTextbox* textbox;
    bbHandle textbox_handle;
    bbDictionary_lookup(home.textbox_system.dict,key,&textbox_handle);
    textbox = textbox_handle.ptr;

    bbHandle message_handle;
    char* message_text;
    bbTextbox_newMessage(textbox,&message_handle,&message_text);

    snprintf(message_text,MESSAGE_LENGTH,"%s",string);
    bbTextbox_setMessage(textbox,message_handle, time);
    bbTextbox_updateBuffer(textbox);

    return bbSuccess;
}


bbFlag bbI_setTextbox_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
    return bbSuccess;
}
bbFlag bbI_unsetTextbox_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
    return bbSuccess;
}

bbFlag bbCI_setTextbox(bbCore* core, char* string, char* key, bbInstruction_source source, bbHandle action) {
    bbNotImplemented()
    return bbSuccess;
}

bbFlag bbCD_unsetTextbox_fn(bbCore* core, bbInstruction* undo_instruction) {
    bbNotImplemented()
    return bbSuccess;
}