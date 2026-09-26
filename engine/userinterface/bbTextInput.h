#ifndef BB_TEXT_INPUT
#define BB_TEXT_INPUT

#include <bits/pthreadtypes.h>

#include "bbWidgets.h"

#define TEXT_INPUT_BUFFER_LENGTH 256
typedef struct
{
    pthread_mutex_t mutex;
    char raw_buffer[TEXT_INPUT_BUFFER_LENGTH];
    char bounded_buffer[TEXT_INPUT_BUFFER_LENGTH];
    I32 rows, cols;
    bbWidget* widget;
    sfText* sf_text;
} bbTextInput;

bbFlag bbTextInput_new(bbTextInput** Text_input, bbWidget* widget);
bbFlag bbTextInput_putChar(bbTextInput* text_input, char c);
bbFlag bbTextInput_putStr(bbTextInput* text_input, char* str);
bbFlag bbTextInput_setStr(bbTextInput* text_input, char* str);

bbFlag bbTextInput_setBounds(bbTextInput* text_input, I32 rows, I32 cols);

#endif //BB_TEXT_INPUT