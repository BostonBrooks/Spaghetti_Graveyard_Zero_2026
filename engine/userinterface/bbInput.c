#include "engine/logic/bbFlag.h"
#include "engine/data/CSFML.h"
#include "engine/userinterface/bbInput.h"

#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbTerminal.h"

#include "engine/data/bbHome.h"



bbFlag bbInput_init(bbInput* input,
                   sfRenderWindow* window,
                   bbMouse* mouse,
                   bbWidgets* widgets){

    input->mouse = mouse;
    input->widgets = widgets;

    char* keymap;

    keymap = input->keymap_lowercase;
    for (int i = 0; i < sfKeyCount; i++)
    {
        keymap[i] = 0;
    }
    keymap[sfKeyBackspace ] = '\b';

    keymap[sfKeyA] = 'a';
    keymap[sfKeyB] = 'b';
    keymap[sfKeyC] = 'c';
    keymap[sfKeyD] = 'd';
    keymap[sfKeyE] = 'e';
    keymap[sfKeyF] = 'f';
    keymap[sfKeyG] = 'g';
    keymap[sfKeyH] = 'h';
    keymap[sfKeyI] = 'i';
    keymap[sfKeyJ] = 'j';
    keymap[sfKeyK] = 'k';
    keymap[sfKeyL] = 'l';
    keymap[sfKeyM] = 'm';
    keymap[sfKeyN] = 'n';
    keymap[sfKeyO] = 'o';
    keymap[sfKeyP] = 'p';
    keymap[sfKeyQ] = 'q';
    keymap[sfKeyR] = 'r';
    keymap[sfKeyS] = 's';
    keymap[sfKeyT] = 't';
    keymap[sfKeyU] = 'u';
    keymap[sfKeyV] = 'v';
    keymap[sfKeyW] = 'w';
    keymap[sfKeyX] = 'x';
    keymap[sfKeyY] = 'y';
    keymap[sfKeyZ] = 'z';

    keymap[sfKeyNum0] = '0';
    keymap[sfKeyNum1] = '1';
    keymap[sfKeyNum2] = '2';
    keymap[sfKeyNum3] = '3';
    keymap[sfKeyNum4] = '4';
    keymap[sfKeyNum5] = '5';
    keymap[sfKeyNum6] = '6';
    keymap[sfKeyNum7] = '7';
    keymap[sfKeyNum8] = '8';
    keymap[sfKeyNum9] = '9';

    keymap[sfKeyNumpad0] = '0';
    keymap[sfKeyNumpad1] = '1';
    keymap[sfKeyNumpad2] = '2';
    keymap[sfKeyNumpad3] = '3';
    keymap[sfKeyNumpad4] = '4';
    keymap[sfKeyNumpad5] = '5';
    keymap[sfKeyNumpad6] = '6';
    keymap[sfKeyNumpad7] = '7';
    keymap[sfKeyNumpad8] = '8';
    keymap[sfKeyNumpad9] = '9';

    keymap[sfKeyPeriod] = '.';

    keymap = input->keymap_uppercase;
    for (int i = 0; i < sfKeyCount; i++)
    {
        keymap[i] = 0;
    }

    keymap[sfKeyBackspace ] = '\b';

    keymap[sfKeyA] = 'A';
    keymap[sfKeyB] = 'B';
    keymap[sfKeyC] = 'C';
    keymap[sfKeyD] = 'D';
    keymap[sfKeyE] = 'E';
    keymap[sfKeyF] = 'F';
    keymap[sfKeyG] = 'G';
    keymap[sfKeyH] = 'H';
    keymap[sfKeyI] = 'I';
    keymap[sfKeyJ] = 'J';
    keymap[sfKeyK] = 'K';
    keymap[sfKeyL] = 'L';
    keymap[sfKeyM] = 'M';
    keymap[sfKeyN] = 'N';
    keymap[sfKeyO] = 'O';
    keymap[sfKeyP] = 'P';
    keymap[sfKeyQ] = 'Q';
    keymap[sfKeyR] = 'R';
    keymap[sfKeyS] = 'S';
    keymap[sfKeyT] = 'T';
    keymap[sfKeyU] = 'U';
    keymap[sfKeyV] = 'V';
    keymap[sfKeyW] = 'W';
    keymap[sfKeyX] = 'X';
    keymap[sfKeyY] = 'Y';
    keymap[sfKeyZ] = 'Z';

    keymap[sfKeyNum0] = '0';
    keymap[sfKeyNum1] = '1';
    keymap[sfKeyNum2] = '2';
    keymap[sfKeyNum3] = '3';
    keymap[sfKeyNum4] = '4';
    keymap[sfKeyNum5] = '5';
    keymap[sfKeyNum6] = '6';
    keymap[sfKeyNum7] = '7';
    keymap[sfKeyNum8] = '8';
    keymap[sfKeyNum9] = '9';

    keymap[sfKeyNumpad0] = '0';
    keymap[sfKeyNumpad1] = '1';
    keymap[sfKeyNumpad2] = '2';
    keymap[sfKeyNumpad3] = '3';
    keymap[sfKeyNumpad4] = '4';
    keymap[sfKeyNumpad5] = '5';
    keymap[sfKeyNumpad6] = '6';
    keymap[sfKeyNumpad7] = '7';
    keymap[sfKeyNumpad8] = '8';
    keymap[sfKeyNumpad9] = '9';

    keymap[sfKeyPeriod] = '.';

    return bbSuccess;
}

bbFlag bbInput_poll(bbInput* input, sfRenderWindow* window){
    sfEvent event;
    sfBool flag;

    //TODO code up mouse
    //input->mouse->right_changed = false;
    //input->mouse->left_changed = false;
    while (1) {
        flag = sfRenderWindow_pollEvent(window, &event);
        if (flag == sfFalse) return bbContinue;
        switch (event.type)
        {
        case sfEvtClosed: return bbBreak;

        case sfEvtMouseMoved:
        case sfEvtMouseButtonPressed:
        case sfEvtMouseButtonReleased:{

            //bbMouse_Event(input->mouse, &event);
            break;
        }

        case sfEvtKeyPressed:{

            bbWidgets* widgets = input->widgets;

                ///find whatever textbox is selected
            bbWidget* widget = home.UI.widgets.selected_textbox;

            sfKeyCode keyCode = event.key.code;
            unsigned char key;
                if (event.key.shift == sfTrue)
                {
                    key = input->keymap_uppercase[keyCode];
                    if (key != 0)
                    {
                        bbHandle handle;
                        handle.u64 = key;
                        bbWidget_onCommand(widget,widgets, bbWC_putChar,
                                           handle);
                    }
                } else {
                    key = input->keymap_lowercase[keyCode];
                    if (key != 0)
                    {
                        bbHandle handle;
                        handle.u64 = key;
                        bbWidget_onCommand(widget,widgets, bbWC_putChar,
                                           handle);
                    }
                }
                if (key == 0)
                {
                    if (keyCode == sfKeyEscape) exit(EXIT_SUCCESS);
                }
                //special character
            /*TODO code up mouse
            if (key == 0)
            {
                if (keyCode == sfKeyReturn)
                {

                    bbHandle handle;
                    handle.u64 = '\n';

                    bbSpell_receiveStr(home.private.UI.spells.currentSpell, &home.private.UI.spells, widgets->command->typeData.textBox.string);
                    char sendstring[] = "";
                    handle.ptr = sendstring;
                    bbWidget_onCommand(widgets->command, widgets,bbWC_setStr,
                                       handle);
                }
                if (keyCode == sfKeyTilde)
                {
                    bbCore_rewind(home.shared.core);
                    bbCore_clearFuture(home.shared.core);
                }
                if (keyCode == sfKeyLeft)
                {
                    home.shared.playmode = PlayMode_rewind;
                }
                if (keyCode == sfKeyDown)
                {
                    home.shared.playmode = PlayMode_pause;
                }
                if (keyCode == sfKeyRight)
                {
                    home.shared.playmode = PlayMode_play;
                }
            }*/

            break;
        }
        case sfEvtKeyReleased:
            sfKeyCode keyCode = event.key.code;
            if (keyCode == sfKeyEscape)
            {

            }
        case sfEvtTextEntered:
            break;
        default:{
            bbDebug("input not recognised\n");
        }

        }

    }
}