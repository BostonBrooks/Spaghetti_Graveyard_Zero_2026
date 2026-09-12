
#include "engine/textbox/bbTextbox.h"

typedef struct
{
    bbTextbox* textbox;
    sfText* sf_text;
    char text[MESSAGE_BUFFER_LENGTH];
} bbWidget_textboxData;
