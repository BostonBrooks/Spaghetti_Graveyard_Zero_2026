
//typedef  bbFlag bbInstruction_fn(bbCore* core, bbInstruction* instruction);

#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "games/game0/maps/systems_test/core/instructions.h"

bbFlag discard_unupdate_moveables_fn(bbCore* core, bbInstruction* undo_instruction)
{
    bbMoveables_snapshot* snapshot;
    bbVPool_lookup(home.ECS.moveables.snapshots, (void**)&snapshot, undo_instruction->snapshot);
    bbVPool_free(home.ECS.moveables.snapshots, snapshot);
    return bbSuccess;
}

bbFlag bbCore_initDiscard(bbCore* core)
{
    I32 max_instructions = 32;//Careful no tomake this too small;
    core->discard_functions = calloc(sizeof(bbInstruction_fn*), max_instructions);

    core->discard_functions[bbInstruction_unupdateMoveables-bbInstruction_numTypes] = discard_unupdate_moveables_fn;
       return bbSuccess;
}