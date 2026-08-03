#include "engine/ECS/bbGraphicsSystem.h"

#include "core/instructions.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS)
{
    bbVPool_newBloated(&graphics_system->system.pool, sizeof(bbGraphicsComponent), 100, 100, "GRAPHICS SYSTEM");
    ECS->systems[bbECS_Graphics] = (bbSystem* )graphics_system;
    return bbSuccess;
}

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle entity,
                                   bbHandle moveable,
                                   bbInstruction_source source,
                                   bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnGraphicsComponent;
    instruction->data.agent_MC.handle1 = entity;
    instruction->data.agent_MC.handle2 = moveable;
    instruction->data.agent_MC.coords = MC;

    instruction->ECS = &home.ECS.ECS;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);

    return  bbSuccess;
}


bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction)
{
    bbUI_Inbox_NewBanana(&home.UI.inbox, instruction->data.agent_MC.coords,
        instruction->data.agent_MC.handle1,
        instruction->data.agent_MC.handle2);

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
    undo_instruction->source = instruction->source;

    undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
    undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;


    //printf("+time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()


    return bbSuccess;
}

bbFlag bbInstruction_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction)
{
    bbUI_Inbox_DeleteUnit(&home.UI.inbox, instruction->data.agent_MC.handle1,
        instruction->data.agent_MC.handle2);

    return bbSuccess;
}

bbFlag bbCoreSynchronous_spawnGraphicsComponent(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle entity,
                                   bbHandle moveable,
                                   bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not yet implemented")
    bbHere()
    bbUI_Inbox_NewBanana(&home.UI.inbox,
                         MC,
                         entity,
                         moveable);


}