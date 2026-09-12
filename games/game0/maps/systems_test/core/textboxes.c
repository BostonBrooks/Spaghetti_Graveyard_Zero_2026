
#include "engine/data/bbHome.h"
#include "engine/textbox/bbTextbox.h"

bbFlag bbCS_setTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action)
{
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
