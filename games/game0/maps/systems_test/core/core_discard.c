
//typedef  bbFlag bbInstruction_fn(bbCore* core, bbInstruction* instruction);

#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "games/game0/maps/systems_test/core/instructions.h"
///discard data needed to restore entity
bbFlag discard_entity_undeleteEntity_fn(bbCore* core, bbInstruction* undo_instruction)
{

    bbHandle entity_handle = undo_instruction->data.three_handles.handle1;


    bbUI_Inbox_DeleteUnit(&home.UI.inbox, entity_handle, no_handle);

    bbHandle moveable_handle;
    bbMoveable* moveable;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_Moveables,&moveable_handle,(bbComponent**)&moveable);
    bbHandle_deleteComponent((bbSystem*)&home.ECS.moveables, moveable_handle);


    bbHandle spatial_handle;
    bbFlag flag = bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_Spatial,&spatial_handle,NULL);

    bbAssert(flag == bbSuccess, "bad handle lookup\n")

    bbHandle_deleteComponent((bbSystem*)&home.ECS.spatial, spatial_handle);

    bbHandle_deleteComponent(&home.ECS.ECS->system, entity_handle);



}

///discards moveable from pool when the instruction to undo setting its state to dead expires
bbFlag discard_Moveable_unsetDead_fn(bbCore* core, bbInstruction* undo_instruction)
{
    bbHandle moveable_handle = undo_instruction->data.moveable_state.handle;
    bbMoveable* moveable;

    bbHandle_getComponent((bbSystem*)&home.ECS.moveables,(bbComponent**)&moveable,moveable_handle);
    moveable->type=bbMoveableType_Unused;

    //bbDebug("moveable_handle = %d, %d\n", moveable_handle.system.index, moveable->type);
    return bbSuccess;
}

bbFlag discard_unupdate_moveables_fn(bbCore* core, bbInstruction* undo_instruction)
{
    bbMoveables_snapshot* snapshot;
    bbVPool_lookup(home.ECS.moveables.snapshots, (void**)&snapshot, undo_instruction->snapshot);
    bbVPool_free(home.ECS.moveables.snapshots, snapshot);
    return bbSuccess;
}

bbFlag bbCore_initDiscard(bbCore* core)
{
    I32 max_instructions = bbVInstruction_numTypes;
    core->discard_functions = calloc(sizeof(bbInstruction_fn*), max_instructions);

    core->discard_functions[bbInstruction_unupdateMoveables-bbInstruction_numTypes] = discard_unupdate_moveables_fn;
    core->discard_functions[bbInstruction_entity_undeleteEntity-bbInstruction_numTypes] =discard_entity_undeleteEntity_fn;
    core->discard_functions[bbI_moveable_unsetDead-bbInstruction_numTypes] = discard_Moveable_unsetDead_fn;
       return bbSuccess;
}