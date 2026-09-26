
#include <pthread.h>
#include <stdlib.h>
#include "engine/userinterface/bbTextInput.h"
#include "bbWidgets.h"
#include "engine/logic/bbString.h"

bbFlag bbTextInput_new(bbTextInput** Text_input, bbWidget* widget)
{
    bbTextInput* text_input = calloc(1, sizeof(bbTextInput));
    pthread_mutex_init(&text_input->mutex, NULL);
    text_input->rows = 3;
    text_input->cols = 24;
    widget->extra_data = text_input;


    if (Text_input!=NULL) *Text_input = text_input;
}

bbFlag bbTextInput_putChar(bbTextInput* text_input, char c)
{
    bbMutexLock(&text_input->mutex);
    bbFlag flag = bbStr_putChar(text_input->raw_buffer,c,TEXT_INPUT_BUFFER_LENGTH);
    bbStr_copyBounds(text_input->bounded_buffer,text_input->raw_buffer, text_input->cols,text_input->rows,TEXT_INPUT_BUFFER_LENGTH);
    bbMutexUnlock(&text_input->mutex);
    return flag;
}


bbFlag bbTextInput_putStr(bbTextInput* text_input, char* str)
{
    bbMutexLock(&text_input->mutex);
    bbFlag flag = bbStr_putStr(text_input->raw_buffer,str,TEXT_INPUT_BUFFER_LENGTH);
    bbStr_copyBounds(text_input->bounded_buffer,text_input->raw_buffer, text_input->cols,text_input->rows,TEXT_INPUT_BUFFER_LENGTH);
    bbMutexUnlock(&text_input->mutex);
    return flag;
}


bbFlag bbTextInput_setStr(bbTextInput* text_input, char* str)
{
    bbMutexLock(&text_input->mutex);
    bbFlag flag = bbStr_setStr(text_input->raw_buffer,str,TEXT_INPUT_BUFFER_LENGTH);
    bbStr_copyBounds(text_input->bounded_buffer,text_input->raw_buffer, text_input->cols,text_input->rows,TEXT_INPUT_BUFFER_LENGTH);
    bbMutexUnlock(&text_input->mutex);
    return flag;
}


bbFlag bbTextInput_setBounds(bbTextInput* text_input, I32 rows, I32 cols)
{
    bbMutexLock(&text_input->mutex);
    text_input->rows = rows;
    text_input->cols = cols;
    bbStr_copyBounds(text_input->bounded_buffer,text_input->raw_buffer, text_input->cols,text_input->rows,TEXT_INPUT_BUFFER_LENGTH);
    bbMutexUnlock(&text_input->mutex);
    return bbSuccess;
}