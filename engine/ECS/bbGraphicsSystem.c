#include "engine/ECS/bbGraphicsSystem.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


bbFlag bbGraphicsSystem_new(bbGraphicsSystem** graphics_system)
{
    bbGraphicsSystem* system = malloc(sizeof(bbGraphicsSystem));

    bbVPool_newBloated(&system->system.pool, sizeof(bbGraphicsComponent), 100, 100, "GRAPHICS SYSTEM");

    *graphics_system = system;
    home.ECS.ECS.systems[bbECS_Graphics] = (bbSystem* )system;
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
    instruction->data.three_handles.handle1 = entity;
    instruction->data.three_handles.handle2 = moveable;
    instruction-
    instruction->ECS = &home.ECS.ECS;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction)
}