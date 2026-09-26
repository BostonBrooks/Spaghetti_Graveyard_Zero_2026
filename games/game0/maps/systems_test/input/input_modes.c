#include <CSFML/Window/Event.h>

#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbInputMode.h"

///should be called AFTER widgets are spawned, may need to look up widgets in dictionary
bbFlag bbInputModes_populate(bbInputModes* input_modes)
{
    bbInputMode* input_mode = calloc(1, sizeof(bbInputMode));

    bbHandle widget_handle;
    bbDictionary_lookup(home.UI.widgets.dict, "TEXT_INPUT", &widget_handle);
    bbWidget* widget;
    bbVPool_lookup(home.UI.widgets.pool,(void**)&widget,widget_handle);

    input_mode->widget = widget;

    for (I32 i = 0; i < sfKeyCount; i++)
    {
        input_mode->key_actions[i].lowercase = '@';
        input_mode->key_actions[i].uppercase = '#';
        input_mode->key_actions[i].control_key = 0;
        input_mode->key_actions[i].function = bbKeyAction_null;
    }
    
    input_mode->key_actions[sfKeyA].lowercase = 'a';
    input_mode->key_actions[sfKeyB].lowercase = 'b';
    input_mode->key_actions[sfKeyC].lowercase = 'c';
    input_mode->key_actions[sfKeyD].lowercase = 'd';
    input_mode->key_actions[sfKeyE].lowercase = 'e';
    input_mode->key_actions[sfKeyF].lowercase = 'f';
    input_mode->key_actions[sfKeyG].lowercase = 'g';
    input_mode->key_actions[sfKeyH].lowercase = 'h';
    input_mode->key_actions[sfKeyI].lowercase = 'i';
    input_mode->key_actions[sfKeyJ].lowercase = 'j';
    input_mode->key_actions[sfKeyK].lowercase = 'k';
    input_mode->key_actions[sfKeyL].lowercase = 'l';
    input_mode->key_actions[sfKeyM].lowercase = 'm';
    input_mode->key_actions[sfKeyN].lowercase = 'n';
    input_mode->key_actions[sfKeyO].lowercase = 'o';
    input_mode->key_actions[sfKeyP].lowercase = 'p';
    input_mode->key_actions[sfKeyQ].lowercase = 'q';
    input_mode->key_actions[sfKeyR].lowercase = 'r';
    input_mode->key_actions[sfKeyS].lowercase = 's';
    input_mode->key_actions[sfKeyT].lowercase = 't';
    input_mode->key_actions[sfKeyU].lowercase = 'u';
    input_mode->key_actions[sfKeyV].lowercase = 'v';
    input_mode->key_actions[sfKeyW].lowercase = 'w';
    input_mode->key_actions[sfKeyX].lowercase = 'x';
    input_mode->key_actions[sfKeyY].lowercase = 'y';
    input_mode->key_actions[sfKeyZ].lowercase = 'z';
    
    input_mode->key_actions[sfKeyA].uppercase = 'A';
    input_mode->key_actions[sfKeyB].uppercase = 'B';
    input_mode->key_actions[sfKeyC].uppercase = 'C';
    input_mode->key_actions[sfKeyD].uppercase = 'D';
    input_mode->key_actions[sfKeyE].uppercase = 'E';
    input_mode->key_actions[sfKeyF].uppercase = 'F';
    input_mode->key_actions[sfKeyG].uppercase = 'G';
    input_mode->key_actions[sfKeyH].uppercase = 'H';
    input_mode->key_actions[sfKeyI].uppercase = 'I';
    input_mode->key_actions[sfKeyJ].uppercase = 'J';
    input_mode->key_actions[sfKeyK].uppercase = 'K';
    input_mode->key_actions[sfKeyL].uppercase = 'L';
    input_mode->key_actions[sfKeyM].uppercase = 'M';
    input_mode->key_actions[sfKeyN].uppercase = 'N';
    input_mode->key_actions[sfKeyO].uppercase = 'O';
    input_mode->key_actions[sfKeyP].uppercase = 'P';
    input_mode->key_actions[sfKeyQ].uppercase = 'Q';
    input_mode->key_actions[sfKeyR].uppercase = 'R';
    input_mode->key_actions[sfKeyS].uppercase = 'S';
    input_mode->key_actions[sfKeyT].uppercase = 'T';
    input_mode->key_actions[sfKeyU].uppercase = 'U';
    input_mode->key_actions[sfKeyV].uppercase = 'V';
    input_mode->key_actions[sfKeyW].uppercase = 'W';
    input_mode->key_actions[sfKeyX].uppercase = 'X';
    input_mode->key_actions[sfKeyY].uppercase = 'Y';
    input_mode->key_actions[sfKeyZ].uppercase = 'Z';


    bbInputModes_add(input_modes, input_mode, "TEST_INPUT_MODE");

}
bbFlag bbKeyAction_null (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    switch (event->type)
    {
    case sfEvtKeyPressed:
        {
            bbWidget* widget = input_mode->widget;
            bbWidgets* widgets = &home.UI.widgets;
            char key = (event->key.shift == sfTrue) ? action->uppercase : action->lowercase;


            bbHandle handle;
            handle.u64 = key;
            bbWidget_onCommand (input_mode->widget,
                                widgets,
                                   bbWC_putChar,
                                   handle);
            fflush(stdout);
            break;
        }
    }

    return bbSuccess;
}

bbFlag bbKeyAction_putChar (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    switch (event->type)
    {
    case sfEvtKeyPressed:
        {
            bbWidget* widget = input_mode->widget;
            bbWidgets* widgets = &home.UI.widgets;
            char key = (event->key.shift == sfTrue) ? action->uppercase : action->lowercase;
            bbHandle handle;
            handle.u64 = key;
            bbWidget_onCommand(widget,widgets, bbWC_putChar,
                               handle);
            break;
        }
    }

    return bbSuccess;
}
bbFlag bbKeyAction_ctrl (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    switch (event->type)
    {
    case sfEvtKeyPressed:
        {
        I32 conrol_key = action->control_key;
            U64 mask = 1u << conrol_key;
            input_mode->control_keys |= mask;
        break;
        }
    case sfEvtKeyReleased:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1u << conrol_key;
            input_mode->control_keys &= ~mask;
        }
    }
    return bbSuccess;
}
bbFlag bbKeyAction_event (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    char key = (event->key.shift == sfTrue) ? action->uppercase : action->lowercase;
    bbCoreInbox_keyPress(&home.core.core, key);
}