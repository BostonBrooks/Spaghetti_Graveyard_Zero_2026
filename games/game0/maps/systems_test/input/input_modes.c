
#include "engine/data/CSFML.h"

#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbInputMode.h"
#include "games/game0/maps/systems_test/core/core_inbox.h"

///should be called AFTER widgets are spawned, may need to look up widgets in dictionary
bbFlag bbInputModes_populate(bbInputModes* input_modes)
{
    bbInputMode* input_mode = calloc(1, sizeof(bbInputMode));

    bbHandle widget_handle;
    bbDictionary_lookup(home.UI.widgets.dict, "TEXT_INPUT", &widget_handle);
    bbWidget* widget;
    bbVPool_lookup(home.UI.widgets.pool,(void**)&widget,widget_handle);

    input_mode->widget = widget;
    input_mode->click_entity = bbClickEntity_print;
    input_mode->click_map_coords = bbClickMapCoords_print;

    for (I32 i = 0; i < sfKeyCount; i++)
    {
        input_mode->key_actions[i].lowercase = '@';
        input_mode->key_actions[i].uppercase = '#';
        input_mode->key_actions[i].control_key = 0;
        input_mode->key_actions[i].event_code = 0;
        input_mode->key_actions[i].function = bbKeyAction_null;
    }

    //input_mode->key_actions[sfKeyUnknown].function = bbKeyAction_null;


    input_mode->key_actions[sfKeyNumpad0].control_key = 0;
    input_mode->key_actions[sfKeyNumpad0].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad1].control_key = 1;
    input_mode->key_actions[sfKeyNumpad1].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad2].control_key = 2;
    input_mode->key_actions[sfKeyNumpad2].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad3].control_key = 3;
    input_mode->key_actions[sfKeyNumpad3].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad4].control_key = 4;
    input_mode->key_actions[sfKeyNumpad4].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad5].control_key = 5;
    input_mode->key_actions[sfKeyNumpad5].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad6].control_key = 6;
    input_mode->key_actions[sfKeyNumpad6].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad7].control_key = 7;
    input_mode->key_actions[sfKeyNumpad7].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad8].control_key = 8;
    input_mode->key_actions[sfKeyNumpad8].function = bbKeyAction_putCharCtrl;
    input_mode->key_actions[sfKeyNumpad9].control_key = 9;
    input_mode->key_actions[sfKeyNumpad9].function = bbKeyAction_putCharCtrl;




    input_mode->key_actions[sfKeyNumpad0].lowercase = '0';
    input_mode->key_actions[sfKeyNumpad1].lowercase = '1';
    input_mode->key_actions[sfKeyNumpad2].lowercase = '2';
    input_mode->key_actions[sfKeyNumpad3].lowercase = '3';
    input_mode->key_actions[sfKeyNumpad4].lowercase = '4';
    input_mode->key_actions[sfKeyNumpad5].lowercase = '5';
    input_mode->key_actions[sfKeyNumpad6].lowercase = '6';
    input_mode->key_actions[sfKeyNumpad7].lowercase = '7';
    input_mode->key_actions[sfKeyNumpad8].lowercase = '8';
    input_mode->key_actions[sfKeyNumpad9].lowercase = '9';


    input_mode->key_actions[sfKeyNumpad0].uppercase = '0';
    input_mode->key_actions[sfKeyNumpad1].uppercase = '1';
    input_mode->key_actions[sfKeyNumpad2].uppercase = '2';
    input_mode->key_actions[sfKeyNumpad3].uppercase = '3';
    input_mode->key_actions[sfKeyNumpad4].uppercase = '4';
    input_mode->key_actions[sfKeyNumpad5].uppercase = '5';
    input_mode->key_actions[sfKeyNumpad6].uppercase = '6';
    input_mode->key_actions[sfKeyNumpad7].uppercase = '7';
    input_mode->key_actions[sfKeyNumpad8].uppercase = '8';
    input_mode->key_actions[sfKeyNumpad9].uppercase = '9';

    for (I32 i = sfKeyA; i <= sfKeyZ; i++)
    {
        input_mode->key_actions[i].function = bbKeyAction_putChar;
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

    input_mode->key_actions[sfKeyEnter].event_code = 0;
    input_mode->key_actions[sfKeyEnter].function = bbKeyAction_event;


    input_mode->key_actions[sfKeyBackspace].function = bbKeyAction_clearChar;
    input_mode->key_actions[sfKeyPeriod].function = bbKeyAction_clearChar;



    bbInputModes_add(input_modes, input_mode, "TEST_INPUT_MODE");

    return bbSuccess;

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

            bbDebug("you clicked key %c\n", key);
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


bbFlag bbKeyAction_clearChar (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
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
            bbWidget_onCommand(widget,widgets, bbWC_clrStr,
                               handle);
            break;
        }
    }

    return bbSuccess;
}

bbFlag bbKeyAction_putCharCtrl (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    switch (event->type)
    {
        case sfEvtKeyPressed:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys |= mask;
            break;
        }
        case sfEvtKeyReleased:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys &= ~mask;
        }
    }

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
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys |= mask;
        break;
        }
    case sfEvtKeyReleased:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys &= ~mask;
        }
    }
    return bbSuccess;
}
bbFlag bbKeyAction_event (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{

    if (event->type == sfEvtKeyPressed) {
        bbCoreInbox_KeyPress(&home.core.core, action->event_code, input_mode->control_keys);
    }
}

bbFlag bbKeyAction_eventCtrl (struct bbInputMode* input_mode, sfEvent * event, struct bbKeyAction* action )
{
    switch (event->type)
    {
        case sfEvtKeyPressed:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys |= mask;
            break;
        }
        case sfEvtKeyReleased:
        {
            I32 conrol_key = action->control_key;
            U64 mask = 1ULL << conrol_key;
            input_mode->control_keys &= ~mask;
        }
    }

    if (event->type == sfEvtKeyPressed) {
        bbCoreInbox_KeyPress(&home.core.core, action->event_code, input_mode->control_keys);
    }
}
bbFlag bbClickEntity_print(bbInputMode* input_mode, bbHandle entity_handle, bbVPMouseType type)
{
    bbCoreInbox_ClickUnit(&home.core.core, entity_handle, input_mode->control_keys, type);
    bbDebug("clicked entity index = %d, system = %d, generation = %d, type = %d\n",
        entity_handle.system.index, entity_handle.system.system, entity_handle.system.generation, type);
    return bbSuccess;
}
bbFlag bbClickMapCoords_print(bbInputMode* input_mode, bbMapCoords map_coords, bbVPMouseType type)
{
//bbAssert(type != VPMouseLeftDrag, "did we get here?\n");
    bbCoreInbox_ClickMap(&home.core.core, map_coords, input_mode->control_keys, type);
    bbDebug("clicked map coords i = %d, j = %d, k = %d, type = %d\n",
        map_coords.i, map_coords.j, map_coords.k, type);

    return bbSuccess;
}
