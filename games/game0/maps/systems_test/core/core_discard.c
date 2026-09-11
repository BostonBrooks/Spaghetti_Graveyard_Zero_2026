
//typedef  bbFlag bbInstruction_fn(bbCore* core, bbInstruction* instruction);

#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "games/game0/maps/systems_test/core/instructions.h"
///discard data needed to restore entity
bbFlag discard_entity_undeleteEntity_fn(bbCore* core, bbInstruction* undo_instruction)
{

    bbHandle entity_handle = undo_instruction->data.three_handles.handle1;



    bbHandle moveable_handle;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_Moveables,&moveable_handle,NULL);
    bbHandle_deleteComponent((bbSystem*)&home.ECS.moveables, moveable_handle);


    bbHandle spatial_handle;
    bbFlag flag = bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_Spatial,&spatial_handle,NULL);

    bbAssert(flag == bbSuccess, "bad handle lookup\n")

    bbHandle_deleteComponent((bbSystem*)&home.ECS.spatial, spatial_handle);

    bbUI_Inbox_DeleteUnit(&home.UI.inbox, entity_handle, moveable_handle);

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
       return bbSuccess;
}