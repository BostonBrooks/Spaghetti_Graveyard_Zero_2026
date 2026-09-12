#include "engine/textbox/bbTextbox.h"

pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;

U64 test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];


int main(void)
{
    bbTextbox_system system;
    bbTextbox_systemInit(&system);
    bbTextbox* textbox;
    bbTextbox_new(&textbox,&system);

    char* message_text;
    bbHandle message_handle1;

    bbTextbox_newMessage(textbox, &message_handle1, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"Any way, any day, but you've got it now!\n",message_handle1);
    bbTextbox_putMessage(textbox,message_handle1,37);
    bbTextbox_updateBuffer(textbox);
    bbDebug("textbox buffer:\n%s\n", &textbox->buffer[textbox->buffer_start]);

    bbHandle message_handle2;
    bbTextbox_newMessage(textbox, &message_handle2, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"Tell me - all of your secrets\n",message_handle2);
    bbTextbox_putMessage(textbox,message_handle2,38);
    bbTextbox_updateBuffer(textbox);
    bbDebug("textbox buffer:\n%s\n", &textbox->buffer[textbox->buffer_start]);

    bbTextbox_hideMessage(textbox,message_handle2);
    bbTextbox_putMessage(textbox,message_handle2,30);
    bbTextbox_updateBuffer(textbox);
    bbDebug("textbox buffer:\n%s\n", &textbox->buffer[textbox->buffer_start]);

    bbTextbox_deleteMessage(textbox,message_handle2);
    bbTextbox_deleteMessage(textbox,message_handle1);


    bbHandle message_handle3;

    bbTextbox_newMessage(textbox, &message_handle3, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"There they are standing in a row\n");
    bbTextbox_putMessage(textbox,message_handle3,40);

    bbTextbox_newMessage(textbox, &message_handle3, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"I've got a lovely bunch of coconuts\n");
    bbTextbox_putMessage(textbox,message_handle3,39);


    bbTextbox_newMessage(textbox, &message_handle3, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"big ones small ones some as big as your head\n");
    bbTextbox_putMessage(textbox,message_handle3,41);


    bbTextbox_updateBuffer(textbox);
    bbDebug("textbox buffer:\n%s\n", &textbox->buffer[textbox->buffer_start]);

    char out_buffer[128];

    bbTextbox_copyBuffer(textbox,out_buffer,3,12,128);

    bbDebug("out_box buffer:\n%s\n", out_buffer);


    bbTextbox_newMessage(textbox, &message_handle3, &message_text);
    snprintf(message_text,MESSAGE_LENGTH,"tom bom bombadil, tom bombadillo\n\n\n");
    bbTextbox_putMessage(textbox,message_handle3,41);

    bbTextbox_updatecopyBuffer(textbox,out_buffer,3,12,128);

    bbDebug("out_box buffer:\n%s\n", out_buffer);

    printf("We made it to the end!\n");
    return bbSuccess;
}
