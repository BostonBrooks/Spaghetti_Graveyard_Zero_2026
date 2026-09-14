
#include "instructions.h"
#include "engine/data/bbHome.h"
#include "engine/textbox/bbTextbox.h"

bbFlag bbCS_setTextbox(bbCore* core,bbHandle* handle, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action)
{

    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,key,&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbHandle message_handle;
    char* message_text;
    bbTextbox_newMessage(textbox,&message_handle,&message_text);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_setTextbox;
        instruction->data.three_handles.handle1 = message_handle;
        instruction->data.three_handles.handle2.u64 = time;

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unsetTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_unsetTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;

        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unsetTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;

        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }

    snprintf(message_text,MESSAGE_LENGTH,"%s",string);
    bbTextbox_setMessage(textbox,message_handle, time);
    bbTextbox_updateBuffer(textbox);

    if (handle!=NULL) *handle = message_handle;
    return bbSuccess;
}


bbFlag bbI_setTextbox_fn(bbCore* core, bbInstruction* instruction) {

    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unsetTextbox;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);

        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->data.three_handles.handle2.u64 = instruction->data.three_handles.handle2.u64;

        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unsetTextbox;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);

        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->data.three_handles.handle2.u64 = instruction->data.three_handles.handle2.u64;

        undo_instruction->source = instruction->source;
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unsetTextbox;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);

        undo_instruction->data.three_handles.handle1 = instruction->data.three_handles.handle1;
        undo_instruction->data.three_handles.handle2.u64 = instruction->data.three_handles.handle2.u64;


        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);

    }


    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,"DIALOGUE",&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbTextbox_setMessage(textbox,
        instruction->data.three_handles.handle1,
        instruction->data.three_handles.handle2.u64);
    bbTextbox_updateBuffer(textbox);

    return bbSuccess;
}
bbFlag bbI_unsetTextbox_fn(bbCore* core, bbInstruction* instruction) {

    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,"DIALOGUE",&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbTextbox_hideMessage(textbox,instruction->data.three_handles.handle1);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);




        return bbSuccess;
    }

    bbNotHere()
}

bbFlag bbCI_setTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action) {
    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,key,&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbHandle message_handle;
    char* message_text;
    bbTextbox_newMessage(textbox,&message_handle,&message_text);

    snprintf(message_text,MESSAGE_LENGTH,"%s",string);

    bbInstruction* instruction;
    bbHandle instruction_handle;
    bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

    //set input instruction data
    instruction->type = bbI_setTextbox;
    instruction->data.three_handles.handle1 = message_handle;
    instruction->data.three_handles.handle2.u64 = time;

    //post instruction to core
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCD_unsetTextbox_fn(bbCore* core, bbInstruction* undo_instruction) {
    bbNotImplemented()
    return bbSuccess;
}


bbFlag bbCI_putTextbox(bbCore* core, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action)
{
    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,key,&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbHandle message_handle;
    char* message_text;
    bbTextbox_newMessage(textbox,&message_handle,&message_text);

    snprintf(message_text,MESSAGE_LENGTH,"%s",string);

    bbInstruction* instruction;
    bbHandle instruction_handle;
    bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

    //set input instruction data
    instruction->type = bbI_putTextbox;
    instruction->data.three_handles.handle1 = message_handle;
    instruction->data.three_handles.handle2.u64 = time;

    //post instruction to core
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
bbFlag bbCS_putTextbox(bbCore* core,bbHandle* handle, char* string, char* key, U64 time, bbInstruction_source source, bbHandle action)
{

    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,key,&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbHandle message_handle;
    char* message_text;
    bbTextbox_newMessage(textbox,&message_handle,&message_text);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_putTextbox;
        instruction->data.three_handles.handle1 = message_handle;
        instruction->data.three_handles.handle2.u64 = time;

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unputTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_unputTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;

        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unputTextbox;
        undo_instruction->data.three_handles.handle1 = message_handle;
        undo_instruction->data.three_handles.handle2.u64 = time;

        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }

    snprintf(message_text,MESSAGE_LENGTH,"%s",string);
    bbTextbox_putMessage(textbox,message_handle, time);
    bbTextbox_updateBuffer(textbox);

    if (handle!=NULL) *handle = message_handle;
    return bbSuccess;
}


bbFlag bbI_putTextbox_fn(bbCore* core, bbInstruction* instruction) {

    bbTextbox* textbox;
    bbHandle textbox_handle;
    //bbDictionary_lookup(home.textbox_system.dict,"DIALOGUE",&textbox_handle);
    textbox = home.textbox_app.textboxes[bbTextbox_Dialogue];

    bbTextbox_putMessage(textbox,
        instruction->data.three_handles.handle1,
        instruction->data.three_handles.handle2.u64);
    bbTextbox_updateBuffer(textbox);
    bbNotImplemented() //rollback
    return bbSuccess;
}
bbFlag bbI_unputTextbox_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
    return bbSuccess;
}

bbFlag bbCD_unputTextbox_fn(bbCore* core, bbInstruction* undo_instruction) {
    bbNotImplemented()
    return bbSuccess;
}