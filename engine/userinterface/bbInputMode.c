#include "engine/userinterface/bbInputMode.h"

#include "engine/logic/bbDictionary.h"

bbFlag bbInputModes_init(bbInputModes* input_modes)
{
    input_modes->modes_available = 0;
    bbDictionary_new(&input_modes->dict,MAX_INPUT_MODES);
    return bbSuccess;

}


bbFlag bbInputModes_add(bbInputModes* input_modes, bbInputMode* input_mode, char* key)
{
    I32 available = input_modes->modes_available++;
    bbAssert(available < MAX_INPUT_MODES);

    input_modes->modes[available] = input_mode;
    bbHandle mode_handle;
    mode_handle.ptr = input_mode;
    bbDictionary_add(input_modes->dict,key,mode_handle);

    return bbSuccess;
}


bbFlag bbInputModes_set(bbInputModes* input_modes, char* key)
{

    bbHandle mode_handle;
    bbFlag flag = bbDictionary_lookup(input_modes->dict,key,&mode_handle);
    bbAssert(flag = bbSuccess, "input node %s not found\n", key);

    input_modes->current_mode = mode_handle.ptr;

    return bbSuccess;
}



bbFlag bbInputModes_event(bbInputModes* input_modes, sfEvent* event)
{
    bbInputMode* input_mode = input_modes->current_mode;
    bbKeyAction* action = input_mode[event->key.code];

    return action->function(input_mode,event,action);
}